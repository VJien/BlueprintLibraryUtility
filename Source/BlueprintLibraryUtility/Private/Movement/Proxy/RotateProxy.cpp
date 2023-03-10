// Fill out your copyright notice in the Description page of Project Settings.


#include "Movement\Proxy/RotateProxy.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"

void URotateProxy::Tick(float DeltaTime)
{
	if (m_RotateActor && bIsInit)
	{
		//UE_LOG(LogTemp, Log, TEXT("Rotation %s"), *currRot.ToString());
		//UE_LOG(LogTemp, Log, TEXT("Rotation %s"), *tarRot.ToString());
		//currRot = UKismetMathLibrary::RInterpTo_Constant(currRot, tarRot, DeltaTime, fSpeed);

		currPitch = UKismetMathLibrary::FInterpTo_Constant(currPitch, tarPitch, DeltaTime, fSpeed);
		currYaw = UKismetMathLibrary::FInterpTo_Constant(currYaw, tarYaw, DeltaTime, fSpeed);
		currRoll = UKismetMathLibrary::FInterpTo_Constant(currRoll, tarRoll, DeltaTime, fSpeed);

		m_RotateActor->SetActorRotation(FRotator(currPitch, currYaw, currRoll));
		
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(currPitch, tarPitch,0.01f) &&
			UKismetMathLibrary::NearlyEqual_FloatFloat(currYaw, tarYaw, 0.01f) &&
			UKismetMathLibrary::NearlyEqual_FloatFloat(currRoll, tarRoll, 0.01f))
		{
			OnRotateEnd.ExecuteIfBound();
			OnSuccess.Broadcast();
			Uninit();
			UE_LOG(LogTemp, Log, TEXT("Rotation FinishedFinished"));
			
		}
		
	}
}

bool URotateProxy::ProxyActorRotation(AActor* RotateActor, float roll, float pitch, float yaw, float Speed /*= 90*/, bool bIsAdditive)
{
	if (Init() && RotateActor && Speed > 0)
	{
		m_RotateActor = RotateActor;
		currRot = m_RotateActor->GetActorRotation();
		currPitch = currRot.Pitch;
		currRoll = currRot.Roll;
		currYaw = currRot.Yaw;

		fSpeed = Speed;	
		if (bIsAdditive)
		{
			tarRoll = roll + currRoll;
			tarPitch = pitch + currPitch;
			tarYaw = yaw + currYaw;
		}
		else
		{
			tarRoll = roll ;
			tarPitch = pitch ;
			tarYaw = yaw;
		}
		return true;
	}
	return false;
}

bool URotateProxy::ProxyActorRotationDelegate(AActor* RotateActor, float roll, float pitch, float yaw, const FActorSimpleRotateDlg& RotationEndEvent, float Speed /*= 90*/, bool bIsAdditive)
{
	if (Init() && RotateActor && Speed > 0)
	{
		m_RotateActor = RotateActor;
		currRot = m_RotateActor->GetActorRotation();
		OnRotateEnd = RotationEndEvent;
		fSpeed = Speed;
		if (bIsAdditive)
		{
			tarRoll = roll + currRoll;
			tarPitch = pitch + currPitch;
			tarYaw = yaw + currYaw;
		}
		else
		{
			tarRoll = roll;
			tarPitch = pitch;
			tarYaw = yaw;
		}
		return true;
	}
	return false;
}

bool URotateProxy::Init()
{
	if (!bIsInit)
	{
		bIsInit = true;
		return true;
	}
	return false;
}

void URotateProxy::Uninit()
{
	bIsInit = false;
}
