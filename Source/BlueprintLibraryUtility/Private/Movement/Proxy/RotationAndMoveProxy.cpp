#include "Movement/Proxy/RotationAndMoveProxy.h"
#include "Kismet/KismetMathLibrary.h"



void UMoveAndRotateProxy::Tick(float DeltaTime)
{
	currLocation = UKismetMathLibrary::VInterpTo_Constant(currLocation, tarDestination, DeltaTime, fMoveSpeed);
	currPitch = UKismetMathLibrary::FInterpTo_Constant(currPitch, tarPitch, DeltaTime, fRotSpeed);
	currYaw = UKismetMathLibrary::FInterpTo_Constant(currYaw, tarYaw, DeltaTime, fRotSpeed);
	currRoll = UKismetMathLibrary::FInterpTo_Constant(currRoll, tarRoll, DeltaTime, fRotSpeed);



	m_Actor->SetActorLocationAndRotation(currLocation, FRotator(currPitch, currYaw, currRoll));

	//m_Actor->SetActorRotation(FRotator(currPitch, currYaw, currRoll));

	if (UKismetMathLibrary::NearlyEqual_FloatFloat(currPitch, tarPitch, 0.01f) &&
		UKismetMathLibrary::NearlyEqual_FloatFloat(currYaw, tarYaw, 0.01f) &&
		UKismetMathLibrary::NearlyEqual_FloatFloat(currRoll, tarRoll, 0.01f) &&
		currLocation.Equals(tarDestination, 1.0f))
	{
		OnEnd.ExecuteIfBound();
		OnSuccess.Broadcast();
		Uninit();
		UE_LOG(LogTemp, Log, TEXT("Rotation FinishedFinished"));

	}

}

bool UMoveAndRotateProxy::ProxyMoveAndRotation(AActor* Target, FVector Destination, bool bUseOffset, float MoveSpeed, float roll /*= 0.0f*/, float pitch /*= 0.0f*/, float yaw /*= 0.0f*/, float RotateSpeed /*= 90.f*/, bool bRotatorIsAdditive /*= false*/)
{
	if (Init() && Target && (MoveSpeed > 0 || RotateSpeed > 0))
	{
		m_Actor = Target;
		 
		currPitch = m_Actor->GetActorRotation().Pitch;
		currRoll = m_Actor->GetActorRotation().Roll;
		currYaw = m_Actor->GetActorRotation().Yaw;
		currLocation = m_Actor->GetActorLocation();

		fMoveSpeed = MoveSpeed;
		fRotSpeed = RotateSpeed;

		if (bUseOffset)
		{
			tarDestination = Destination + currLocation;
		}
		else
		{
			tarDestination = Destination;
		}


		if (bRotatorIsAdditive)
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

bool UMoveAndRotateProxy::ProxyMoveAndRotationDelegate(AActor* Target, const FActorMoveAndRotateDlg& EndEvent, FVector Destination, bool bUseOffset, float MoveSpeed, float roll /*= 0.0f*/, float pitch /*= 0.0f*/, float yaw /*= 0.0f*/, float RotateSpeed /*= 90*/, bool bRotatorIsAdditive /*= false*/)
{
	if (Init() && Target && (MoveSpeed > 0 || RotateSpeed > 0))
	{
		m_Actor = Target;
		OnEnd = EndEvent;

		currPitch = m_Actor->GetActorRotation().Pitch;
		currRoll = m_Actor->GetActorRotation().Roll;
		currYaw = m_Actor->GetActorRotation().Yaw;

		fMoveSpeed = MoveSpeed;
		fRotSpeed = RotateSpeed;
		if (bUseOffset)
		{
			tarDestination = Destination + m_Actor->GetActorLocation();
		}
		else
		{
			tarDestination = Destination;
		}


		if (bRotatorIsAdditive)
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

bool UMoveAndRotateProxy::Init()
{
	if (!bIsInit)
	{
		bIsInit = true;
		return true;
	}
	return false;
}

void UMoveAndRotateProxy::Uninit()
{
	bIsInit = false;
}

