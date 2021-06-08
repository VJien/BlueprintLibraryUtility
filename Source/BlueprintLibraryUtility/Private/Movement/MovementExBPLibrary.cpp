// Copyright Epic Games, Inc. All Rights Reserved.

#include "Movement\MovementExBPLibrary.h"
#include "Movement/Proxy/RotationAndMoveProxy.h"


UFlibMovementEx::UFlibMovementEx(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}




bool UFlibMovementEx::V_AIMoveToDelegate(APawn* Pawn, FVector Des, AActor* Target, const FMoveSimpleDelegate& MoveEndEvent, float AcceptanceRadius /*= 5.0*/, bool StopOnOverlap /*= false*/)
{
	if (!Pawn) { return nullptr; };
	UMoveProxy* p = NewObject<UMoveProxy>(Pawn);
	p->Proxy_AIMoveToDelegate(Pawn, Des, Target, MoveEndEvent, AcceptanceRadius, StopOnOverlap);

	return true;
}

UMoveProxy* UFlibMovementEx::V_AIMoveTo(APawn* Pawn, FVector Des, AActor* Target, float AcceptanceRadius /*= 5.0*/, bool StopOnOverlap /*= false*/)
{
	if (!Pawn) { return nullptr; };
	UMoveProxy* p = NewObject<UMoveProxy>(Pawn);
	 p->Proxy_AIMoveTo(Pawn, Des, Target, AcceptanceRadius, StopOnOverlap);
	 return p;
}

bool UFlibMovementEx::V_AIPathMoveToDelegate(APawn* Pawn, TArray<FVector> DesPath, const FPathMoveDelegate& MoveEndEvent, const FPathMoveDelegate2& MoveOneStepEvent, float AcceptanceRadius /*= 5.0*/, bool StopOnOverlap /*= false*/)
{
	if (!Pawn || DesPath.Num()<=0) { return nullptr; };
	UPathMoveProxy* p = NewObject<UPathMoveProxy>(Pawn);
	return p->Proxy_AIPathMoveToDelegate(Pawn, DesPath, MoveEndEvent, MoveOneStepEvent, AcceptanceRadius, StopOnOverlap);
}

UPathMoveProxy* UFlibMovementEx::V_AIPathMoveTo(APawn* Pawn, TArray<FVector> DesPath, float AcceptanceRadius /*= 5.0*/, bool StopOnOverlap /*= false*/)
{
	if (!Pawn || DesPath.Num() <= 0) { return nullptr; };
	UPathMoveProxy* p = NewObject<UPathMoveProxy>(Pawn);
	p->Proxy_AIPathMoveTo(Pawn, DesPath, AcceptanceRadius, StopOnOverlap);
	return p;
}

UActorMoveProxy* UFlibMovementEx::V_ActorSimpleMoveTo(AActor* MoveActor, FVector Des, AActor* Target, float LinearSpeed, float TurnSpeed, float AcceptanceRadius /*= 5.0*/, bool bTurnPitch, bool bTurnYaw, bool bTurnRoll)
{
	if (!MoveActor ) { return nullptr; };
	UActorMoveProxy* p = NewObject<UActorMoveProxy>(MoveActor);
	p->ProxyActorMoveTo(MoveActor,Des,Target,LinearSpeed,TurnSpeed,AcceptanceRadius,bTurnPitch,bTurnYaw,bTurnRoll);
	return p;
}

bool UFlibMovementEx::V_ActorSimpleMoveToDelegate(AActor* MoveActor, FVector Des, AActor* Target, const FActorSimpleMoveDlg& MoveEvent, float LinearSpeed, float TurnSpeed, float AcceptanceRadius /*= 5.0*/, bool bTurnPitch, bool bTurnYaw, bool bTurnRoll)
{
	if (!MoveActor) { return false; };
	UActorMoveProxy* p = NewObject<UActorMoveProxy>(MoveActor);
	p->ProxyActorMoveToDelegate(MoveActor, Des, Target, MoveEvent,LinearSpeed, TurnSpeed, AcceptanceRadius, bTurnPitch, bTurnYaw, bTurnRoll);
	return true;
}


UActorMoveProxy* UFlibMovementEx::V_ActorNavMoveTo(AActor* MoveActor, FVector Des, AActor* Target, float LinearSpeed, float TurnSpeed, float AcceptanceRadius /*= 5.0*/, float DynamicTargetDetectInterval /*= 2.0f*/, bool bDebug /*= false*/, bool bTurnPitch, bool bTurnYaw, bool bTurnRoll, bool bKeepLanded)
{
	if (!MoveActor) { return nullptr; };
	UActorMoveProxy* p = NewObject<UActorMoveProxy>(MoveActor);
	p->ProxyActorNavMoveTo(MoveActor, Des, Target, LinearSpeed, TurnSpeed, AcceptanceRadius, bDebug, bTurnPitch, bTurnYaw, bTurnRoll,bKeepLanded);
	return p;
}

URotateProxy* UFlibMovementEx::V_ActorRotateBySpeed(AActor* RotateActor, float roll, float pitch, float yaw, float Speed /*= 90*/, bool bIsAdditive /*= false*/)
{
	if (!RotateActor) { return nullptr; };
	URotateProxy* p = NewObject<URotateProxy>(RotateActor);
	if (p->ProxyActorRotation(RotateActor, roll, pitch, yaw, Speed, bIsAdditive))
	{
		return p;
	}
	return nullptr;	
}

bool UFlibMovementEx::V_ActorRotateBySpeedDelegate(AActor* RotateActor, float roll, float pitch, float yaw, const FActorSimpleRotateDlg& RotationEndEvent, float Speed /*= 90*/, bool bIsAdditive /*= false*/)
{
	if (!RotateActor) { return nullptr; };
	URotateProxy* p = NewObject<URotateProxy>(RotateActor);
	if (p->ProxyActorRotationDelegate(RotateActor, roll, pitch, yaw, RotationEndEvent, Speed, bIsAdditive))
	{
		return true;
	}
	return false;
}

URotateProxy* UFlibMovementEx::V_ActorRotateByTime(AActor* RotateActor, float roll, float pitch, float yaw, float Time /*= 1*/, bool bIsAdditive /*= false*/)
{
	if (!RotateActor || Time <= 0) { return nullptr; };
	FRotator rot = RotateActor->GetActorRotation();
	FVector delta;
	if (bIsAdditive)
	{
		delta.X = roll;
		delta.Y = pitch;
		delta.Z = yaw;
	}
	else
	{
		delta.X = rot.Roll - roll;
		delta.Y = rot.Pitch -pitch;
		delta.Z = rot.Yaw - yaw;
	}
	
	float max = delta.GetAbsMax();
	URotateProxy* p = NewObject<URotateProxy>(RotateActor);
	if (p->ProxyActorRotation(RotateActor, roll, pitch, yaw, max/Time, bIsAdditive))
	{
		return p;
	}
	return nullptr;
}

bool UFlibMovementEx::V_ActorRotateByTimeDelegate(AActor* RotateActor, float roll, float pitch, float yaw, const FActorSimpleRotateDlg& RotationEndEvent, float Time /*= 1*/, bool bIsAdditive /*= false*/)
{
	if (!RotateActor || Time <= 0) { return nullptr; };
	FRotator rot = RotateActor->GetActorRotation();
	FVector delta;
	if (bIsAdditive)
	{
		delta.X = roll;
		delta.Y = pitch;
		delta.Z = yaw;
	}
	else
	{
		delta.X = rot.Roll - roll;
		delta.Y = rot.Pitch - pitch;
		delta.Z = rot.Yaw - yaw;
	}
	float max = delta.GetAbsMax();
	URotateProxy* p = NewObject<URotateProxy>(RotateActor);
	if (p->ProxyActorRotationDelegate(RotateActor, roll, pitch, yaw, RotationEndEvent, max / Time, bIsAdditive))
	{
		return true;
	}
	return false;
}

UMoveAndRotateProxy* UFlibMovementEx::V_ActorMoveAndRotateBySpeed(AActor* Target, FVector Destination, bool bUseOffset, float MoveSpeed, float roll /*= 0.0f*/, float pitch /*= 0.0f*/, float yaw /*= 0.0f*/, float RotateSpeed /*= 90.f*/, bool bRotatorIsAdditive /*= false*/)
{
	if (!Target) { return nullptr; };
	UMoveAndRotateProxy* p = NewObject<UMoveAndRotateProxy>(Target);
	if (p->ProxyMoveAndRotation(Target, Destination, bUseOffset, MoveSpeed, roll, pitch, yaw, RotateSpeed, bRotatorIsAdditive))
	{
		return p;
	}
	return nullptr;
}

bool UFlibMovementEx::V_ActorMoveAndRotateBySpeedDelegate(AActor* Target, const FActorMoveAndRotateDlg& EndEvent, FVector Destination, bool bUseOffset, float MoveSpeed, float roll /*= 0.0f*/, float pitch /*= 0.0f*/, float yaw /*= 0.0f*/, float RotateSpeed /*= 90.f*/, bool bRotatorIsAdditive /*= false*/)
{
	if (!Target) { return false; };
	UMoveAndRotateProxy* p = NewObject<UMoveAndRotateProxy>(Target);
	 return p->ProxyMoveAndRotationDelegate(Target, EndEvent, Destination, bUseOffset, MoveSpeed, roll, pitch, yaw, RotateSpeed, bRotatorIsAdditive);


}

UMoveAndRotateProxy* UFlibMovementEx::V_ActorMoveAndRotateByTime(AActor* Target, FVector Destination, bool bUseOffset, float Duration,  float roll /*= 0.0f*/, float pitch /*= 0.0f*/, float yaw /*= 0.0f*/, bool bRotatorIsAdditive /*= false*/)
{
	if (!Target) { return nullptr; };
	FRotator rot = Target->GetActorRotation();
	FVector deltaRot, deltaTranstion;
	deltaTranstion = bUseOffset? Destination : Destination - Target->GetActorLocation();

	if (bRotatorIsAdditive)
	{
		deltaRot.X = roll;
		deltaRot.Y = pitch;
		deltaRot.Z = yaw;
	}
	else
	{
		deltaRot.X = rot.Roll - roll;
		deltaRot.Y = rot.Pitch - pitch;
		deltaRot.Z = rot.Yaw - yaw;
	}

	float rotMax = deltaRot.GetAbsMax();
	float tranMax = deltaTranstion.GetAbsMax();

	UMoveAndRotateProxy* p = NewObject<UMoveAndRotateProxy>(Target);
	if (p->ProxyMoveAndRotation(Target, Destination, bUseOffset, tranMax / Duration, roll, pitch, yaw, rotMax / Duration, bRotatorIsAdditive))
	{
		return p;
	}
	return nullptr;

}

bool UFlibMovementEx::V_ActorMoveAndRotateByTimeDelegate(AActor* Target, const FActorMoveAndRotateDlg& EndEvent, FVector Destination, bool bUseOffset, float Duration, float roll /*= 0.0f*/, float pitch /*= 0.0f*/, float yaw /*= 0.0f*/, bool bRotatorIsAdditive /*= false*/)
{
	if (!Target) { return false; };
	FRotator rot = Target->GetActorRotation();
	FVector deltaRot, deltaTranstion;
	deltaTranstion = bUseOffset ? Destination : Destination - Target->GetActorLocation();

	if (bRotatorIsAdditive)
	{
		deltaRot.X = roll;
		deltaRot.Y = pitch;
		deltaRot.Z = yaw;
	}
	else
	{
		deltaRot.X = rot.Roll - roll;
		deltaRot.Y = rot.Pitch - pitch;
		deltaRot.Z = rot.Yaw - yaw;
	}

	float rotMax = deltaRot.GetAbsMax();
	float tranMax = deltaTranstion.GetAbsMax();

	UMoveAndRotateProxy* p = NewObject<UMoveAndRotateProxy>(Target);
	return p->ProxyMoveAndRotationDelegate(Target, EndEvent, Destination, bUseOffset, tranMax / Duration, roll, pitch, yaw, rotMax / Duration, bRotatorIsAdditive);
	

}

