// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetStringLibrary.h"
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





	UFUNCTION(BlueprintCallable, meta = (CallableWithoutWorldContext = true, WorldContext = "context"), Category = "BlueprintLibraryUtilites")
		static void PrintArray(UObject* context, const TArray<FString>& InStrings, bool bScreen = true, bool bLog = true, FLinearColor Color = FLinearColor::Green, float Duration = 2.0f);


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BlueprintLibraryUtilites")
		static UObject* GetObjBySoftPath(const FSoftObjectPath& softPath);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BlueprintLibraryUtilites")
		static UObject* GetObjRefBySoftPtr(const TSoftObjectPtr<UObject>& softPathPtr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BlueprintLibraryUtilites")
		static UClass* GetClassBySoftPath(const FSoftClassPath& softPath);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BlueprintLibraryUtilites")
		static UClass* GetClassByClassSoftPtr(const TSoftClassPtr<UObject>& softPathPtr);



		//virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)override;

};
