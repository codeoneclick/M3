// Copyright serhii serhiiv 2017. All rights reserved.

#include "HMMeshComponent.h"
#include "Engine.h"
#include "DynamicMeshBuilder.h"
#include "PhysicsEngine/BodySetup.h"

const int32 UHMMeshComponent::NumSubdivisions = 6;

class FHMVBO : public FVertexBuffer
{
public:

	TArray<FDynamicMeshVertex> Vertices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		void* Buffer = nullptr;
		VertexBufferRHI = RHICreateAndLockVertexBuffer(Vertices.Num() * sizeof(FDynamicMeshVertex), BUF_Static, CreateInfo, Buffer);

		FMemory::Memcpy(Buffer, Vertices.GetData(), Vertices.Num() * sizeof(FDynamicMeshVertex));
		RHIUnlockVertexBuffer(VertexBufferRHI);
	}
};

class FHMIBO : public FIndexBuffer
{
public:

	TArray<int32> Indices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		void* Buffer = nullptr;
		IndexBufferRHI = RHICreateAndLockIndexBuffer(sizeof(int32), Indices.Num() * sizeof(int32), BUF_Static, CreateInfo, Buffer);
		
		FMemory::Memcpy(Buffer, Indices.GetData(), Indices.Num() * sizeof(int32));
		RHIUnlockIndexBuffer(IndexBufferRHI);
	}
};

class FHMVertexFactory : public FLocalVertexFactory
{
public:

	void Init_RenderThread(const FHMVBO* VertexBuffer)
	{
		check(IsInRenderingThread());

		FDataType Declaration;
		Declaration.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Position, VET_Float3);
		Declaration.TextureCoordinates.Add(
			FVertexStreamComponent(VertexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate), sizeof(FDynamicMeshVertex), VET_Float2)
		);
		Declaration.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentX, VET_PackedNormal);
		Declaration.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentZ, VET_PackedNormal);
		Declaration.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Color, VET_Color);

		SetData(Declaration);
	}

	void Init(const FHMVBO* VertexBuffer)
	{
		if (IsInRenderingThread())
		{
			Init_RenderThread(VertexBuffer);
		}
		else
		{
			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
				InitHMVertexFactory,
				FHMVertexFactory*, VertexFactory, this,
				const FHMVBO*, VertexBuffer, VertexBuffer,
				{
					VertexFactory->Init_RenderThread(VertexBuffer);
				});
		}
	}
};

class FHMMeshSceneProxy : public FPrimitiveSceneProxy
{
public:

	/*FHMMeshSceneProxy(UHMMeshComponent* Component) :
		FPrimitiveSceneProxy(Component),
		MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
	{
		for (int32 TriIdx = 0; TriIdx<Component->Triangles.Num(); TriIdx++)
		{
			FHMMeshTriangle& Triangle = Component->Triangles[TriIdx];

			const FVector Edge01 = (Triangle.Vertices[1].Position - Triangle.Vertices[0].Position);
			const FVector Edge02 = (Triangle.Vertices[2].Position - Triangle.Vertices[0].Position);

			const FVector TangentX = Edge01.GetSafeNormal();
			const FVector TangentZ = (Edge02 ^ Edge01).GetSafeNormal();
			const FVector TangentY = (TangentX ^ TangentZ).GetSafeNormal();

			FDynamicMeshVertex Vertex;
			int32 VIndex = 0;
			for (int32 VertexInTriangleIndex = 0; VertexInTriangleIndex < 3; ++VertexInTriangleIndex)
			{
				Vertex.Position = Triangle.Vertices[VertexInTriangleIndex].Position;
				Vertex.Color = Triangle.Vertices[VertexInTriangleIndex].Color;
				Vertex.SetTangents(TangentX, TangentY, TangentZ);
				Vertex.TextureCoordinate.Set(Triangle.Vertices[VertexInTriangleIndex].U, Triangle.Vertices[VertexInTriangleIndex].V);
				VIndex = VertexBuffer.Vertices.Add(Vertex);
				IndexBuffer.Indices.Add(VIndex);
			}
		}

		VertexFactory.Init(&VertexBuffer);

		BeginInitResource(&VertexBuffer);
		BeginInitResource(&IndexBuffer);
		BeginInitResource(&VertexFactory);

		Material = Component->GetMaterial(0);
		if (Material == nullptr)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}
	}

	virtual ~FHMMeshSceneProxy()
	{
		VertexBuffer.ReleaseResource();
		IndexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_CustomMeshSceneProxy_GetDynamicMeshElements);

		const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

		auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy(IsSelected()) : NULL,
			FLinearColor(0, 0.5f, 1.f)
		);

		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);

		FMaterialRenderProxy* MaterialProxy = NULL;
		if (bWireframe)
		{
			MaterialProxy = WireframeMaterialInstance;
		}
		else
		{
			MaterialProxy = Material->GetRenderProxy(IsSelected());
		}

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				const FSceneView* View = Views[ViewIndex];

				FMeshBatch& Mesh = Collector.AllocateMesh();
				FMeshBatchElement& BatchElement = Mesh.Elements[0];
				BatchElement.IndexBuffer = &IndexBuffer;
				Mesh.bWireframe = bWireframe;
				Mesh.VertexFactory = &VertexFactory;
				Mesh.MaterialRenderProxy = MaterialProxy;
				BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, UseEditorDepthTest());
				BatchElement.FirstIndex = 0;
				BatchElement.NumPrimitives = IndexBuffer.Indices.Num() / 3;
				BatchElement.MinVertexIndex = 0;
				BatchElement.MaxVertexIndex = VertexBuffer.Vertices.Num() - 1;
				Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
				Mesh.Type = PT_TriangleList;
				Mesh.DepthPriorityGroup = SDPG_World;
				Mesh.bCanApplyViewModeOverrides = false;
				Collector.AddMesh(ViewIndex, Mesh);
			}
		}
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bDynamicRelevance = true;
		Result.bRenderInMainPass = ShouldRenderInMainPass();
		Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
		Result.bRenderCustomDepth = ShouldRenderCustomDepth();
		MaterialRelevance.SetPrimitiveViewRelevance(Result);
		return Result;
	}

	virtual bool CanBeOccluded() const override
	{
		return !MaterialRelevance.bDisableDepthTest;
	}

	virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

private:

	UMaterialInterface* Material;
	FHMVBO VertexBuffer;
	FHMIBO IndexBuffer;
	FHMVertexFactory VertexFactory;

	FMaterialRelevance MaterialRelevance;*/
};

UHMMeshComponent::UHMMeshComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
}


void UHMMeshComponent::SetTriangles(const TArray<FHMMeshTriangle>& aTriangles)
{
	Triangles = aTriangles;

	MarkRenderStateDirty();
	UpdateBounds();
}

void UHMMeshComponent::AddTriangles(const TArray<FHMMeshTriangle>& aTriangles)
{
	Triangles.Append(aTriangles);

	MarkRenderStateDirty();
	UpdateBounds();
}

void UHMMeshComponent::ClearTriangles()
{
	Triangles.Reset();

	MarkRenderStateDirty();
	UpdateBounds();
}

FPrimitiveSceneProxy* UHMMeshComponent::CreateSceneProxy()
{
	FPrimitiveSceneProxy* Proxy = nullptr;
	/*if (Triangles.Num() > 0)
	{
		Proxy = new FHMMeshSceneProxy(this);
	}*/
	return Proxy;
}

int32 UHMMeshComponent::GetNumMaterials() const
{
	return 1;
}

FBoxSphereBounds UHMMeshComponent::CalcBounds(const FTransform & LocalToWorld) const
{
	if (Triangles.Num() > 0)
	{
		FVector BoundsMin = Triangles[0].Vertices[0].Position;
		FVector BoundsMax = Triangles[0].Vertices[0].Position;

		for (int32 TriangleIndex = 0; TriangleIndex < Triangles.Num(); TriangleIndex++)
		{
			for (int32 VertexInTriangleIndex = 0; VertexInTriangleIndex < 3; ++VertexInTriangleIndex)
			{
				BoundsMin.X = BoundsMin.X > Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.X ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.X : BoundsMin.X;
				BoundsMin.Y = BoundsMin.Y > Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Y ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Y : BoundsMin.Y;
				BoundsMin.Z = BoundsMin.Z > Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Z ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Z : BoundsMin.Z;

				BoundsMax.X = BoundsMax.X < Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.X ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.X : BoundsMax.X;
				BoundsMax.Y = BoundsMax.Y < Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Y ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Y : BoundsMax.Y;
				BoundsMax.Z = BoundsMax.Z < Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Z ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Z : BoundsMax.Z;
			}
		}

		FVector Origin = ((BoundsMax - BoundsMin) * .5f) + BoundsMin;
		FVector Delta = BoundsMax - BoundsMin;
		return FBoxSphereBounds(Origin, Delta, Delta.Size()).TransformBy(LocalToWorld);
	}
	else
	{
		return FBoxSphereBounds();
	}
}

void UHMMeshComponent::AddHexTileGeometry(const FVector2D& Location2D, float Size, TArray<struct FHMMeshTriangle>& Triangles)
{
	FVector HexTileCenter = FVector(Location2D.X, Location2D.Y, 0.f);

	TArray<FVector> HexTileVertices;
	TArray<FVector2D> HexTileTexcoords;
	for (float Angle = 0.f; Angle <= FMath::DegreesToRadians(360.f); Angle += (FMath::DegreesToRadians(360.f) / NumSubdivisions))
	{
		HexTileVertices.Add(FVector(Size * cosf(Angle) + Location2D.X, Size * sinf(Angle) + Location2D.Y, 0.f));
		HexTileTexcoords.Add(FVector2D((cosf(Angle) + 1.f) * .5f, (sinf(Angle) + 1.f) * .5f));
	}

	FHMMeshTriangle Triangle;
	Triangle.Vertices.SetNum(3);
	int32 HexTileVerticesIndex = 0;
	for (int32 i = 0; i < NumSubdivisions; ++i)
	{
		Triangle.Vertices[0].Position = HexTileCenter;
		Triangle.Vertices[0].U = .5f;
		Triangle.Vertices[0].V = .5f;
		HexTileVerticesIndex++;
		Triangle.Vertices[1].Position = HexTileVertices[HexTileVerticesIndex];
		Triangle.Vertices[1].U = HexTileTexcoords[HexTileVerticesIndex].X;
		Triangle.Vertices[1].V = HexTileTexcoords[HexTileVerticesIndex].Y;
		if (HexTileVerticesIndex >= HexTileVertices.Num())
		{
			HexTileVerticesIndex = 0;
		}
		HexTileVerticesIndex--;
		Triangle.Vertices[2].Position = HexTileVertices[HexTileVerticesIndex];
		Triangle.Vertices[2].U = HexTileTexcoords[HexTileVerticesIndex].X;
		Triangle.Vertices[2].V = HexTileTexcoords[HexTileVerticesIndex].Y;
		HexTileVerticesIndex++;

		Triangles.Add(Triangle);
	}
}



