// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "PathMoveProxy.generated.h"



DECLARE_DYNAMIC_DELEGATE_OneParam(FPathMoveDelegate, TEnumAsByte<EPathFollowingResult::Type>, MovementResult);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPathMoveDelegate2, FVector, CurrPathPoint);
//, MovementResult, FVector, CurrPathPoint
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPathMoveMulDlg, TEnumAsByte<EPathFollowingResult::Type>, MovementResult, FVector, CurrPathPoint);

/**
 * 
 */


class UAIAsyncTaskBlueprintProxy;

UCLASS(Blueprintable, BlueprintType)
class BLUEPRINTLIBRARYUTILITY_API UPathMoveProxy : public UObject
{
	GENERATED_BODY()
public:



	//UPROPERTY(BlueprintReadWrite)
	//	UAIAsyncTaskBlueprintProxy* Proxy;

	UFUNCTION(BlueprintCallable, Category = "Tomb | MovementEx | Proxy")
		bool Proxy_AIPathMoveToDelegate(APawn* Pawn, TArray<FVector> Path, const FPathMoveDelegate& MoveEndEvent, const FPathMoveDelegate2& MoveOneStepEvent, float AcceptanceRadius = 5.0, bool StopOnOverlap = false);
	UFUNCTION(BlueprintCallable, Category = "Tomb | MovementEx | Proxy")
		UAIAsyncTaskBlueprintProxy* Proxy_AIPathMoveTo(APawn* Pawn, TArray<FVector> Path,  float AcceptanceRadius = 5.0, bool StopOnOverlap = false);
	UFUNCTION()
		void MoveFail(EPathFollowingResult::Type Result);
	UFUNCTION()
		void MoveSuccess(EPathFollowingResult::Type Result);


	/*
	* 动态代理,用于函数传递
	*/
	UPROPERTY()
		FPathMoveDelegate2 OnMoveOneStep;
	UPROPERTY()
		FPathMoveDelegate OnMoveEnd;

	/*
	* 动态多播代理,用于绑定
	*/
	UPROPERTY(BlueprintAssignable,BlueprintCallable)
		FPathMoveMulDlg	OnSuccess;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FPathMoveMulDlg	OnFail;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FPathMoveMulDlg	OneStep;

private:
	UPROPERTY()
		TArray<FVector> CurrPath;
	UPROPERTY()
		FVector SavedPathPoint;
	UPROPERTY()
		APawn* CurPawn;
	UPROPERTY()
		float fAcceptanceRadius;
	UPROPERTY()
		bool bStopOnOverlap;

	bool bIsInit = false;
};
