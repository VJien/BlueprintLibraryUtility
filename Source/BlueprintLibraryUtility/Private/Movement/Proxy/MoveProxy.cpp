// Fill out your copyright notice in the Description page of Project Settings.


#include "Movement\Proxy/MoveProxy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/AIAsyncTaskBlueprintProxy.h"

bool UMoveProxy::Proxy_AIMoveToDelegate(APawn* Pawn, FVector Des, AActor* Target, const FMoveSimpleDelegate& MoveEndEvent, float AcceptanceRadius /*= 5.0*/, bool StopOnOverlap /*= false*/)
{
	if (!Pawn)
	{
		return false;
	}
	OnMoveEnd = MoveEndEvent;
	Proxy = UAIBlueprintHelperLibrary::CreateMoveToProxyObject(Pawn, Pawn, Des, Target, AcceptanceRadius, StopOnOverlap);
	Proxy->OnFail.AddDynamic(this, &UMoveProxy::MoveEnd);
	Proxy->OnSuccess.AddDynamic(this, &UMoveProxy::MoveEnd);
	return true;
}

UAIAsyncTaskBlueprintProxy* UMoveProxy::Proxy_AIMoveTo(APawn* Pawn, FVector Des, AActor* Target, float AcceptanceRadius /*= 5.0*/, bool StopOnOverlap /*= false*/)
{
	if (!Pawn)
	{
		return nullptr;
	}
	Proxy = UAIBlueprintHelperLibrary::CreateMoveToProxyObject(Pawn, Pawn, Des, Target, AcceptanceRadius, StopOnOverlap);
	Proxy->OnFail.AddDynamic(this, &UMoveProxy::MoveEnd);
	Proxy->OnSuccess.AddDynamic(this, &UMoveProxy::MoveEnd);
	return Proxy;
}

void UMoveProxy::MoveEnd(EPathFollowingResult::Type Result)
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
}
