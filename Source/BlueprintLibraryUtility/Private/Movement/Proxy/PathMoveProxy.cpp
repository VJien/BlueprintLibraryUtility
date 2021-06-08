// Fill out your copyright notice in the Description page of Project Settings.
#include "Movement\Proxy/PathMoveProxy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/AIAsyncTaskBlueprintProxy.h"





bool UPathMoveProxy::Proxy_AIPathMoveToDelegate(APawn* Pawn, TArray<FVector> Path, const FPathMoveDelegate& MoveEndEvent, const FPathMoveDelegate2& MoveOneStepEvent, float AcceptanceRadius /*= 5.0*/, bool StopOnOverlap /*= false*/)
{
	if (Pawn == nullptr || Path.Num() <= 0)
	{
		return false;
	}
	if (!bIsInit)
	{
		bIsInit = true;
		CurrPath = Path;
		CurPawn = Pawn;
		fAcceptanceRadius = AcceptanceRadius;
		bStopOnOverlap = StopOnOverlap;
		OnMoveOneStep = MoveOneStepEvent;
		OnMoveEnd = MoveEndEvent;

	}
	UAIAsyncTaskBlueprintProxy* p = UAIBlueprintHelperLibrary::CreateMoveToProxyObject(Pawn, Pawn, Path[0], nullptr, AcceptanceRadius, StopOnOverlap);
	p->OnFail.AddDynamic(this, &UPathMoveProxy::MoveFail);
	p->OnSuccess.AddDynamic(this, &UPathMoveProxy::MoveSuccess);
	return true;
}

UAIAsyncTaskBlueprintProxy* UPathMoveProxy::Proxy_AIPathMoveTo(APawn* Pawn, TArray<FVector> Path, float AcceptanceRadius /*= 5.0*/, bool StopOnOverlap /*= false*/)
{
	
	if (Pawn == nullptr || Path.Num()<=0)
	{
		return nullptr;
	}
	if (!bIsInit)
	{
		bIsInit = true;
		CurrPath = Path;
		CurPawn = Pawn;
		fAcceptanceRadius = AcceptanceRadius;
		bStopOnOverlap = StopOnOverlap;
	}
	UAIAsyncTaskBlueprintProxy* p = UAIBlueprintHelperLibrary::CreateMoveToProxyObject(Pawn, Pawn, Path[0], nullptr, AcceptanceRadius, StopOnOverlap);
	p->OnFail.AddDynamic(this, &UPathMoveProxy::MoveFail);
	p->OnSuccess.AddDynamic(this, &UPathMoveProxy::MoveSuccess);
	return p;
}

void UPathMoveProxy::MoveFail(EPathFollowingResult::Type Result)
{
	OnMoveEnd.ExecuteIfBound(Result);
}

void UPathMoveProxy::MoveSuccess(EPathFollowingResult::Type Result)
{


	SavedPathPoint = CurrPath[0];
	OnMoveOneStep.ExecuteIfBound(SavedPathPoint);
	OneStep.Broadcast(Result, SavedPathPoint);
	if (CurrPath.Num()==1)
	{
		OnMoveEnd.ExecuteIfBound(Result);
		if (Result==EPathFollowingResult::Success)
		{
			OnSuccess.Broadcast(Result, SavedPathPoint);
		}
		else
		{
			OnFail.Broadcast(Result, SavedPathPoint);
		}
	}
	else if(CurrPath.Num() > 1)
	{
		CurrPath.RemoveAt(0);
		Proxy_AIPathMoveTo(CurPawn, CurrPath, fAcceptanceRadius, bStopOnOverlap);
	}
}


