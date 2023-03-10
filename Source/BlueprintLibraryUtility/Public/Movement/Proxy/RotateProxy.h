// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "RotateProxy.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_DELEGATE(FActorSimpleRotateDlg);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorSimpleRotateMulDlg);

UCLASS(Blueprintable, BlueprintType)
class BLUEPRINTLIBRARYUTILITY_API URotateProxy : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	


public:

	/*       FTickableGameObject          */
	virtual void Tick(float DeltaTime)override;
	virtual bool IsTickable()const override { return bIsInit; };
	virtual TStatId GetStatId() const override { return Super::GetStatID(); };
	/*       FTickableGameObject         */

	UFUNCTION(BlueprintCallable, Category = "Tomb | MovementEx | Proxy", meta = (AdvancedDisplay = 5))
		bool ProxyActorRotation(AActor* RotateActor, float roll, float pitch, float yaw, float Speed = 90, bool bIsAdditive = false);
	UFUNCTION(BlueprintCallable, Category = "Tomb | MovementEx | Proxy", meta = (AdvancedDisplay = 5))
		bool ProxyActorRotationDelegate(AActor* RotateActor, float roll, float pitch, float yaw, const FActorSimpleRotateDlg& RotationEndEvent, float Speed = 90, bool bIsAdditive = false);


	bool Init();
	void Uninit();

	UPROPERTY()
		FActorSimpleRotateDlg OnRotateEnd;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FActorSimpleRotateMulDlg OnSuccess;



	AActor* m_RotateActor;
	bool bIsInit = false;
	FRotator currRot;
	//FRotator tarRot;

	float currYaw = 0.f;
	float currPitch = 0.f;
	float currRoll = 0.f;

	float tarYaw = 0.f;
	float tarPitch = 0.f;
	float tarRoll = 0.f;

	float fSpeed;

};
