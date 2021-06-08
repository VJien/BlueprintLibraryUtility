// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "RotationAndMoveProxy.generated.h"

/**
 * 
 */

 /**
  * Comment
  */



DECLARE_DYNAMIC_DELEGATE(FActorMoveAndRotateDlg);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorMoveAndRotateMulDlg);

UCLASS(Blueprintable, BlueprintType)
class BLUEPRINTLIBRARYUTILITY_API UMoveAndRotateProxy : public UObject, public FTickableGameObject
{
	GENERATED_BODY()


public:

	/*       FTickableGameObject          */
	virtual void Tick(float DeltaTime)override;
	virtual bool IsTickable()const override { return bIsInit; };
	virtual TStatId GetStatId() const override { return Super::GetStatID(); };
	/*       FTickableGameObject         */

	UFUNCTION(BlueprintCallable, Category = "MovementEx | Proxy", meta = (AdvancedDisplay = 5))
		bool ProxyMoveAndRotation(AActor* Target, FVector Destination, bool bUseOffset,  float MoveSpeed,  float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f, float RotateSpeed = 90.f, bool bRotatorIsAdditive = false);
	UFUNCTION(BlueprintCallable, Category = "MovementEx | Proxy", meta = (AdvancedDisplay = 5))
		bool ProxyMoveAndRotationDelegate(AActor* Target, const FActorMoveAndRotateDlg& EndEvent,  FVector Destination, bool bUseOffset, float MoveSpeed,  float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f,  float RotateSpeed = 90, bool bRotatorIsAdditive = false);


	bool Init();
	void Uninit();

	UPROPERTY()
		FActorMoveAndRotateDlg OnEnd;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FActorMoveAndRotateMulDlg OnSuccess;



	AActor* m_Actor;
	bool bIsInit = false;

	FVector tarDestination, currLocation;

	float currYaw = 0.f;
	float currPitch = 0.f;
	float currRoll = 0.f;

	float tarYaw = 0.f;
	float tarPitch = 0.f;
	float tarRoll = 0.f;

	float fMoveSpeed, fRotSpeed;

};
