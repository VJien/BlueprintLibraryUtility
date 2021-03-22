// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Flib_Utilities.generated.h"



namespace BLU
{

template<typename TEnum>
static FString EnumToString(const FString& EnumName, TEnum Enum, bool bShortName)
{
	const UEnum* ep = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (ep)
	{
		if (bShortName)
		{
			FString tempStr = ep->GetNameByValue((int)Enum).ToString();
			TArray<FString> arr = UKismetStringLibrary::ParseIntoArray(tempStr, TEXT("::"));
			if (arr.Num() > 0)
			{
				return arr.Last();
			}
		}

		return ep->GetNameByValue((int)Enum).ToString();


	}
	return TEXT("");
}

template<typename TEnum>
static TEnum StringToEnum(const FString& EnumName, FString EnumValue)
{
	const UEnum* ep = FindObject<UEnum>(ANY_PACKAGE, *EnumName, 1);
	if (!ep)
	{
		return TEnum(0);
	}
	return (TEnum)ep->GetIndexByName(FName(*FString(EnumValue)));
}

}

UCLASS()
class BLUEPRINTLIBRARYUTILITY_API UFlib_Utilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
