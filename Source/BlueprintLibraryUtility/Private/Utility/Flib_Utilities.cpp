// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/Flib_Utilities.h"

void UFlib_Utilities::PrintArray(UObject* context, const TArray<FString>& InStrings, bool bScreen /*= true*/, bool bLog /*= true*/, FLinearColor Color /*= FLinearColor::Green*/, float Duration /*= 2.0f*/)
{
	FString OutString(TEXT(""));
	for (const auto& Word : InStrings)
		OutString += Word;
	UKismetSystemLibrary::PrintString(context, OutString, bScreen, bLog, Color, Duration);
}
