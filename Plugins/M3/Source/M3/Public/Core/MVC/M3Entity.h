// Copyright serhii serhiiv 2018 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3KVProperty.h"

class M3_API M3Entity : public std::enable_shared_from_this<M3Entity> {
public:

	CTTI_CLASS_GUID(M3Entity)

	PROP_STRONG(public, M3Entity, IsAssignedToView, bool, false)

	M3Entity() = default;
	virtual ~M3Entity() = default;
};
