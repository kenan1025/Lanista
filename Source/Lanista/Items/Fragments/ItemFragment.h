// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemFragment.generated.h"

/** Provides the abstract, instanced base type for item data fragments. */
UCLASS(Blueprintable,Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class LANISTA_API UItemFragment : public UObject
{
	GENERATED_BODY()
	
};
