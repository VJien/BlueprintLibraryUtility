// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/Flib_Utilities.h"

#include "Kismet/KismetSystemLibrary.h"



void UFlib_Utilities::PrintArray(UObject* context, const TArray<FString>& InStrings, bool bScreen /*= true*/, bool bLog /*= true*/, FLinearColor Color /*= FLinearColor::Green*/, float Duration /*= 2.0f*/)
{
	FString OutString(TEXT(""));
	for (const auto& Word : InStrings)
		OutString += Word;
	UKismetSystemLibrary::PrintString(context, OutString, bScreen, bLog, Color, Duration);
}

UObject* UFlib_Utilities::GetObjBySoftPath(const FSoftObjectPath& softPath)
{
	UObject* obj = LoadObject<UObject>(NULL, *softPath.ToString());
	return obj;
}

UObject* UFlib_Utilities::GetObjRefBySoftPtr(const TSoftObjectPtr<UObject>& softPathPtr)
{
	TSoftObjectPtr<UObject> p = TSoftObjectPtr<UObject>(softPathPtr);
	return p.Get();
}

UClass* UFlib_Utilities::GetClassBySoftPath(const FSoftClassPath& softPath)
{
	UClass* c = LoadClass<UObject>(NULL, *softPath.ToString());
	return c;
}

UClass* UFlib_Utilities::GetClassByClassSoftPtr(const TSoftClassPtr<UObject>& softPathPtr)
{
	TSoftClassPtr<UObject> p = TSoftClassPtr<UObject>(softPathPtr);
	return  p.Get();
}

