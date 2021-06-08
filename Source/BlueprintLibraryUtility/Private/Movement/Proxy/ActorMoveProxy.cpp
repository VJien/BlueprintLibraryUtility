// Fill out your copyright notice in the Description page of Project Settings.

#include "Movement/Proxy/ActorMoveProxy.h"
#include "Movement/Proxy/PathMoveProxy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"




UActorMoveProxy::~UActorMoveProxy()
{
	UE_LOG(LogTemp, Log, TEXT("ActorMoveProxy GC:  %S"), *UKismetSystemLibrary::GetDisplayName(this));
}

bool UActorMoveProxy::ProxyActorMoveTo(AActor* MoveActor, FVector Des, AActor* Target, float LinearSpeed, float TurnSpeed, float AcceptanceRadius /*= 5.0*/, bool bTurnPitch, bool bTurnYaw, bool bTurnRoll)
{
	if (bIsInit || !MoveActor || UKismetSystemLibrary::K2_IsTimerActiveHandle(this,m_timer))
	{
		return false;
	}
	/*UWorld* wd = MoveActor->GetWorld();
	if (!wd) return false;*/

	//m_Delta = DeltaTime;
	timeSec = 0;
	TurnPitch = bTurnPitch;
	TurnYaw = bTurnYaw;
	TurnRoll = bTurnRoll;
	m_currLoc = m_startLoc = MoveActor->GetActorLocation();
	m_tarLoc = Target ? Target->GetActorLocation() : Des;
	m_TargetActor = Target;
	m_curOffsetLoc = FVector(0);
	m_tarOffsetLoc = m_tarLoc - m_currLoc;
	m_currRot = MoveActor->GetActorRotation();
	m_tarRot = UKismetMathLibrary::Conv_VectorToRotator(m_tarLoc - m_currLoc); 
	m_TurnSpeed = TurnSpeed;
	m_StopDistance = AcceptanceRadius;
	m_MoveActor = MoveActor;
	m_LinearSpeed = LinearSpeed; //* ((m_currLoc-m_tarLoc).Size());

	//wd->GetTimerManager().SetTimer(m_timer,this, &UActorMoveProxy::MoveHandle, m_Delta, true);

	//m_timer = UKismetSystemLibrary::K2_SetTimer(this, "MoveHandle", m_Delta, true);

	bIsInit = true;
	return true;
}

bool UActorMoveProxy::ProxyActorMoveToDelegate(AActor* MoveActor, FVector Des, AActor* Target, const FActorSimpleMoveDlg& MoveEvent ,float LinearSpeed, float TurnSpeed, float AcceptanceRadius /* float DeltaTime */, bool bTurnPitch, bool bTurnYaw, bool bTurnRoll)
{
	if (bIsInit || !MoveActor || UKismetSystemLibrary::K2_IsTimerActiveHandle(this, m_timer))
	{
		return false;
	}
	//m_Delta = DeltaTime;
	timeSec = 0; 
	TurnPitch = bTurnPitch;
	TurnYaw = bTurnYaw;
	TurnRoll = bTurnRoll;
	m_currLoc = m_startLoc = MoveActor->GetActorLocation();
	m_tarLoc = Target ? Target->GetActorLocation() : Des;
	m_TargetActor = Target;
	m_curOffsetLoc = FVector(0);
	m_tarOffsetLoc = m_tarLoc - m_currLoc;
	m_currRot = MoveActor->GetActorRotation();
	m_tarRot = UKismetMathLibrary::Conv_VectorToRotator(m_tarLoc - m_currLoc);
	m_TurnSpeed = TurnSpeed;
	m_StopDistance = AcceptanceRadius;
	m_MoveActor = MoveActor;
	m_LinearSpeed = LinearSpeed; //* ((m_currLoc-m_tarLoc).Size());
	OnMoveEnd = MoveEvent;
	/*
	* use timer 
	*/
	//m_timer = UKismetSystemLibrary::K2_SetTimer(this, "MoveHandle", m_Delta, true);
	bIsInit = true;
	return true;
}

void UActorMoveProxy::MoveEnd(EPathFollowingResult::Type Result)
{
	if (bIsPathMove && m_Path.Num() > 1)
	{
		PathMoveToNextPoint();
	}
	else
	{
		OnMoveEnd.ExecuteIfBound(Result);
		if (Result == EPathFollowingResult::Success)
		{
			OnSuccess.Broadcast(Result);
		}
		else
		{
			OnFail.Broadcast(Result);
		}
		Uninit();
	}
	
}

bool UActorMoveProxy::ProxyActorNavMoveTo(AActor* MoveActor, FVector Des, AActor* Target, float LinearSpeed, float TurnSpeed, float AcceptanceRadius /*= 5.0*/, bool bDebug, bool bTurnPitch, bool bTurnYaw, bool bTurnRoll, bool bKeepLanded)
{
	if (bIsInit || !MoveActor || UKismetSystemLibrary::K2_IsTimerActiveHandle(this, m_timer))
	{
		return false;
	}
	KeepLanded = bKeepLanded;
	TurnPitch = bTurnPitch;
	TurnYaw = bTurnYaw;
	TurnRoll = bTurnRoll;
	Debug = bDebug;
	bIsPathMove = true;
	timeSec = 0;
	m_MoveActor = MoveActor;
	m_currLoc = m_startLoc = m_MoveActor->GetActorLocation();
	m_Path = FindPathPoints(m_MoveActor, m_currLoc, Target ? Target->GetActorLocation() : Des);
	if (m_Path.Num()<=1)
	{
		UE_LOG(LogTemp, Log, TEXT("Acotr NavMove Path invalid"));
		return false;
	}
	if (Debug)
	{
		for (auto v : m_Path)
		{
			UKismetSystemLibrary::DrawDebugSphere(m_MoveActor, v, 10, 12,FLinearColor::Red,1,2);
		}
		
	}
	m_Path.RemoveAt(0);
	m_tarLoc = m_Path[0];
	if (Debug)
	{
		UKismetSystemLibrary::DrawDebugSphere(m_MoveActor, m_tarLoc, 35, 12, FLinearColor::Green, 1, 2);
	}
	m_TargetActor = Target;
	m_curOffsetLoc = FVector(0);
	m_tarOffsetLoc = m_tarLoc - m_currLoc;
	m_currRot = MoveActor->GetActorRotation();
	m_tarRot = UKismetMathLibrary::Conv_VectorToRotator(m_tarLoc - m_currLoc);
	m_TurnSpeed = TurnSpeed;
	m_StopDistance = AcceptanceRadius;
	m_LinearSpeed = LinearSpeed; 


	bIsInit = true;
	return true;
}

bool UActorMoveProxy::PathMoveToNextPoint()
{
	bIsInit = false;
	FVector lastPoint = m_Path.Last();
	m_currLoc = m_startLoc = m_MoveActor->GetActorLocation();
	m_Path = FindPathPoints(m_MoveActor, m_currLoc, m_TargetActor ? m_TargetActor->GetActorLocation() : lastPoint);
	if (m_Path.Num() <= 1)
	{
		return false;
	}
	if (Debug)
	{
		for (auto v : m_Path)
		{
			UKismetSystemLibrary::DrawDebugSphere(m_MoveActor, v, 10, 12, FLinearColor::Red, 1, 2);
		}
		UE_LOG(LogTemp, Log, TEXT("Path MoveTo Remove PathPoint %s "), *(m_Path[0].ToString()));
	}
	m_Path.RemoveAt(0);
	m_tarLoc = m_Path[0];
	if (Debug)
	{
		UKismetSystemLibrary::DrawDebugSphere(m_MoveActor, m_tarLoc, 35, 12, FLinearColor::Green, 1, 2);
		UE_LOG(LogTemp, Log, TEXT("Path MoveTo move from %s to %s "), *(m_currLoc.ToString()), *(m_tarLoc.ToString()));
	}
	m_curOffsetLoc = FVector(0);
	m_tarOffsetLoc = m_tarLoc - m_currLoc;
	m_currRot = m_MoveActor->GetActorRotation();
	m_tarRot = UKismetMathLibrary::Conv_VectorToRotator(m_tarLoc - m_currLoc);

	bIsInit = true;
	return true;
}


void UActorMoveProxy::MoveHandle()
{
	if (m_MoveActor)
	{
		if (m_TargetActor)
		{
			m_tarOffsetLoc = m_TargetActor->GetActorLocation() - m_startLoc;
			m_tarLoc = m_TargetActor->GetActorLocation();
			
		}
		m_curOffsetLoc = UKismetMathLibrary::VInterpTo_Constant(m_curOffsetLoc, m_tarOffsetLoc, m_Delta, m_LinearSpeed);
		
		m_currLoc = m_startLoc + m_curOffsetLoc;

		if (m_TargetActor)
		{
			FRotator rot = UKismetMathLibrary::Conv_VectorToRotator(m_TargetActor->GetActorLocation() - m_currLoc);
			
			m_tarRot = rot == FRotator::ZeroRotator ? m_tarRot : rot;
		}
		
		bNeedRot = m_TurnSpeed > 0;
		if (bNeedRot)
		{
			m_currRot = UKismetMathLibrary::RInterpTo_Constant(m_currRot, m_tarRot, m_Delta, m_TurnSpeed);
			m_MoveActor->SetActorLocationAndRotation(m_currLoc, m_currRot);
		}
		else
		{
			m_MoveActor->SetActorLocation(m_currLoc);
		}
		
	}
	timeSec += m_Delta;
	
	if (timeSec>= maxHandleTimeSec)
	{
		MoveEnd(EPathFollowingResult::Aborted);
		UE_LOG(LogTemp, Log, TEXT("Actor Move Fail because  out of max time"));
	}
	
	if (UKismetMathLibrary::NearlyEqual_TransformTransform(FTransform(bNeedRot?m_currRot:FRotator(0), m_currLoc, FVector(1)), 
		FTransform(bNeedRot?m_tarRot:FRotator(0), m_tarLoc, FVector(1)), m_StopDistance, 1, 0.01))
	{
		MoveEnd(EPathFollowingResult::Success);
		UE_LOG(LogTemp, Log, TEXT("Actor Move Success"));
	}
	


	
	

}

void UActorMoveProxy::Uninit()
{
	if (UKismetSystemLibrary::K2_IsTimerActive(this,"MoveHandle"))
	{
		UKismetSystemLibrary::K2_ClearTimer(this, "MoveHandle");
	}
	bIsInit = false;
	m_TargetActor = nullptr;
	m_MoveActor = nullptr;
	RemoveFromRoot();
	UE_LOG(LogTemp, Log, TEXT("Actor Move Uninit"));
	
}



TArray<FVector> UActorMoveProxy::FindPathPoints(UObject* Context, const FVector& Start, const FVector& End)
{
	UNavigationPath* obj = UNavigationSystemV1::FindPathToLocationSynchronously(Context, Start, End);
	if (obj)
	{
		return obj->PathPoints;
	}
	return TArray<FVector>{};
}

void UActorMoveProxy::Tick(float DeltaTime)
{
	if (m_MoveActor)
	{
		if (m_TargetActor && !bIsPathMove)
		{
			m_tarOffsetLoc = m_TargetActor->GetActorLocation() - m_startLoc;
			m_tarLoc = m_TargetActor->GetActorLocation();

		}
		m_curOffsetLoc = UKismetMathLibrary::VInterpTo_Constant(m_curOffsetLoc, m_tarOffsetLoc, DeltaTime, m_LinearSpeed);

		m_currLoc = m_startLoc + m_curOffsetLoc;


	
			
		FRotator rot = UKismetMathLibrary::Conv_VectorToRotator((bIsPathMove?m_tarLoc:(m_TargetActor? m_TargetActor->GetActorLocation(): m_tarLoc)) - m_currLoc);

		rot.Pitch = TurnPitch ? rot.Pitch : m_currRot.Pitch;
		rot.Yaw = TurnYaw ? rot.Yaw : m_currRot.Yaw;
		rot.Roll = TurnRoll ? rot.Roll : m_currRot.Roll;
		m_tarRot = rot == FRotator::ZeroRotator ? m_tarRot : rot;
		if (KeepLanded)
		{
			TArray<AActor*> ignoreList;
			ignoreList.Add(m_MoveActor);
			FHitResult res;
			if (m_MoveActor && UKismetSystemLibrary::LineTraceSingle(m_MoveActor, m_MoveActor->GetActorLocation(), m_MoveActor->GetActorLocation() - FVector(0, 0, 1000), ETraceTypeQuery::TraceTypeQuery1, false,
				ignoreList, Debug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, res, false))
			{
				FVector navPoint;
				if (UNavigationSystemV1::K2_ProjectPointToNavigation(m_MoveActor, res.ImpactPoint, navPoint, nullptr, nullptr))
				{
					ACharacter* c = Cast<ACharacter>(m_MoveActor);
					if (c)
					{
						m_currLoc.Z = navPoint.Z + c->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
					}
					else
					{
						m_currLoc.Z = navPoint.Z;
					}
					
				}
			}

		}

		bNeedRot = m_TurnSpeed > 0;
		if (bNeedRot)
		{
			m_currRot = UKismetMathLibrary::RInterpTo_Constant(m_currRot, m_tarRot, DeltaTime, m_TurnSpeed);
			
			m_MoveActor->SetActorLocationAndRotation(m_currLoc, m_currRot);
		}
		else
		{
			m_MoveActor->SetActorLocation(m_currLoc);
		}


	}
	timeSec += DeltaTime;
	if (bIsPathMove)
	{
		DynamicTargetDetectTime += DeltaTime;
		if (DynamicTargetDetectTime >= DynamicTargetDetectInterval)
		{
			DynamicTargetDetectTime = 0;
			PathMoveToNextPoint();
		}
	}

	if (timeSec >= maxHandleTimeSec)
	{
		MoveEnd(EPathFollowingResult::Aborted);
		UE_LOG(LogTemp, Log, TEXT("Actor Move Fail because  out of max time"));
	}

	if (UKismetMathLibrary::NearlyEqual_TransformTransform(FTransform(bNeedRot ? m_currRot : FRotator(0), m_currLoc, FVector(1)),
		FTransform(bNeedRot ? m_tarRot : FRotator(0), m_tarLoc, FVector(1)), m_StopDistance, 1, 0.01))
	{
		MoveEnd(EPathFollowingResult::Success);
		UE_LOG(LogTemp, Log, TEXT("Actor Move Success"));
	}


	//UE_LOG(LogTemp, Log, TEXT("Actor Move "));
}
