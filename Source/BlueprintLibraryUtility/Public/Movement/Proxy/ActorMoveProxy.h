// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "Tickable.h"
#include "ActorMoveProxy.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FActorSimpleMoveDlg, TEnumAsByte<EPathFollowingResult::Type>, MovementResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorSimpleMoveMulDlg, TEnumAsByte<EPathFollowingResult::Type>, MovementResult);

UCLASS(Blueprintable, BlueprintType)
class BLUEPRINTLIBRARYUTILITY_API UActorMoveProxy : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	UActorMoveProxy() {};
	~UActorMoveProxy();


public:
	/*UFUNCTION(BlueprintCallable, Category = "Tomb | MovementEx | Proxy")
		bool Proxy_ActorMoveToDelegate(AActor* MoveActor, FVector Des, AActor* Target, float TurnSpeed, const FActorSimpleMoveDlg& MoveEndEvent, float AcceptanceRadius = 5.0);*/
	UFUNCTION(BlueprintCallable, Category = "Tomb | MovementEx | Proxy", meta = (AdvancedDisplay = 5))
		bool ProxyActorMoveTo(AActor* MoveActor, FVector Des, AActor* Target, float LinearSpeed = 300, float TurnSpeed = 90, float AcceptanceRadius = 5.0, bool bTurnPitch = true, bool bTurnYaw = true, bool bTurnRoll = true);
	UFUNCTION(BlueprintCallable, Category = "Tomb | MovementEx | Proxy", meta = (AdvancedDisplay = 6))
		bool ProxyActorMoveToDelegate(AActor* MoveActor, FVector Des, AActor* Target, const FActorSimpleMoveDlg& MoveEvent ,float LinearSpeed = 300, float TurnSpeed = 90, float AcceptanceRadius = 5.0, bool bTurnPitch = true, bool bTurnYaw = true, bool bTurnRoll = true);
	UFUNCTION()
		void MoveEnd(EPathFollowingResult::Type Result);

	UFUNCTION(BlueprintCallable, Category = "Tomb | MovementEx | Proxy", meta = (AdvancedDisplay = 5))
		bool ProxyActorNavMoveTo(AActor* MoveActor, FVector Des, AActor* Target, float LinearSpeed = 300, float TurnSpeed = 90, float AcceptanceRadius = 5.0, bool bDebug =  false, bool bTurnPitch = true,bool bTurnYaw = true, bool bTurnRoll = true, bool bKeepLanded = true);

	bool PathMoveToNextPoint();

	UFUNCTION()
		void MoveHandle();
	UFUNCTION()
		void Uninit();
	
	TArray<FVector> FindPathPoints(UObject* Context, const FVector& Start, const FVector& End);


	/*       FTickableGameObject          */
	virtual void Tick(float DeltaTime)override;
	virtual bool IsTickable()const override { return bIsInit; };
	virtual TStatId GetStatId() const override { return Super::GetStatID(); };
	/*       FTickableGameObject         */




	UPROPERTY()
		FActorSimpleMoveDlg OnMoveEnd;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FActorSimpleMoveMulDlg OnSuccess;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FActorSimpleMoveMulDlg OnFail;


protected:
	UPROPERTY()
		float maxHandleTimeSec = 60;
	UPROPERTY()
		float m_Delta = 0.03;
	UPROPERTY()
		float m_TurnSpeed;
	UPROPERTY()
		float m_StopDistance;
	UPROPERTY()
		FVector m_currLoc;
	UPROPERTY()
		FVector m_startLoc;
	UPROPERTY()
		FVector m_tarLoc;
	UPROPERTY()
		FVector m_curOffsetLoc;
	UPROPERTY()
		FVector m_tarOffsetLoc;
	UPROPERTY()
		FRotator m_currRot;
	UPROPERTY()
		FRotator m_tarRot;
	UPROPERTY()
		FTimerHandle m_timer;
	UPROPERTY()
		AActor* m_MoveActor;
	UPROPERTY()
		AActor* m_TargetActor;
	UPROPERTY()
		float m_LinearSpeed;

	UPROPERTY(BlueprintReadOnly)
		TArray<FVector> m_Path;

	UPROPERTY(BlueprintReadWrite)
		float DynamicTargetDetectInterval = 1.0f;


	float DynamicTargetDetectTime;
	bool bIsInit = false;
	float timeSec;
	bool bNeedRot = true;
	bool bIsPathMove = false;
	bool Debug;
	bool TurnRoll = true;
	bool TurnPitch = true;
	bool TurnYaw = true;
	bool KeepLanded = true;

};
