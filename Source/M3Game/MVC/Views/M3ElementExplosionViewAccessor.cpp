// Fill out your copyright notice in the Description page of Project Settings.

#include "M3ElementExplosionViewAccessor.h"

UM3ElementExplosionViewAccessor::UM3ElementExplosionViewAccessor() {
}

UM3ElementExplosionViewAccessor::~UM3ElementExplosionViewAccessor() {
}

FVector UM3ElementExplosionViewAccessor::GetExplosionLocation() {
	FVector ParentLocation = View->GetAttachParentActor()->GetActorLocation();
	return FVector(ElementSize.Y * ExplosionRow + ParentLocation.X, ElementSize.X * ExplosionCol + ParentLocation.Y, 0);
}

FVector UM3ElementExplosionViewAccessor::GetExplosionIteration1Location() {
	const auto CurrentLocation = GetCurrentLocation();
	const auto ExplosionLocation = GetExplosionLocation();
	auto Direction = CurrentLocation - ExplosionLocation;
	Direction.Normalize();
	Direction.X *= -1;
	Direction.Y *= -1;
	auto Distance = FVector::Distance(CurrentLocation, ExplosionLocation) * .5f;

	return FVector(CurrentLocation.X + Direction.X * Distance, CurrentLocation.Y + Direction.Y * Distance, 0);
}

FVector UM3ElementExplosionViewAccessor::GetExplosionIteration2Location() {
	const auto CurrentLocation = GetCurrentLocation();
	const auto ExplosionLocation = GetExplosionLocation();
	auto Direction = CurrentLocation - ExplosionLocation;
	Direction.Normalize();
	auto Distance = FVector::Distance(CurrentLocation, ExplosionLocation) * .5f;

	return FVector(CurrentLocation.X + Direction.X * Distance, CurrentLocation.Y + Direction.Y * Distance, 0);
}

void UM3ElementExplosionViewAccessor::Reset() {
	ExplosionCol = -1;
	ExplosionRow = -1;
	IsExplosion = false;
}
