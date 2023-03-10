// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Navigation/PathFollowingComponent.h"

#include "MoveProxy.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FMoveSimpleDelegate, TEnumAsByte<EPathFollowingResult::Type>, MovementResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveMulDlg, TEnumAsByte<EPathFollowingResult::Type>, MovementResult);

class UAIAsyncTaskBlueprintProxy;

UCLASS(Blueprintable,BlueprintType)
class BLUEPRINTLIBRARYUTILITY_API UMoveProxy : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		UAIAsyncTaskBlueprintProxy* Proxy;

	UFUNCTION(BlueprintCallable, Category = "Tomb | MovementEx | Proxy")
		bool Proxy_AIMoveToDelegate(APawn* Pawn, FVector Des, AActor* Target, const FMoveSimpleDelegate& MoveEndEvent, float AcceptanceRadius = 5.0, bool StopOnOverlap = false);
	UFUNCTION(BlueprintCallable, Category = "Tomb | MovementEx | Proxy")
		UAIAsyncTaskBlueprintProxy* Proxy_AIMoveTo(APawn* Pawn, FVector Des, AActor* Target, float AcceptanceRadius = 5.0, bool StopOnOverlap = false);
	UFUNCTION()
		void MoveEnd(EPathFollowingResult::Type Result);


	UPROPERTY()
		FMoveSimpleDelegate OnMoveEnd;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FMoveMulDlg OnSuccess;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FMoveMulDlg OnFail;
};
