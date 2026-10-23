// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDefinition.h"

const UItemFragment* UItemDefinition::FindFragmentByClass(TSubclassOf<UItemFragment> FragmentClass)
{
	if (FragmentClass)
	{
		
		for (const TObjectPtr<UItemFragment>& Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment.Get();
			}
		}
	}
	return  nullptr;
}
