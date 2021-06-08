// Fill out your copyright notice in the Description page of Project Settings.


#include "Node\Movement\K2Node_SimpleNavMoveTo.h"
#include "Movement/MovementExBPLibrary.h"
#include "Movement/Proxy/PathMoveProxy.h"


UK2Node_SimpleNavMoveTo::UK2Node_SimpleNavMoveTo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UFlibMovementEx, V_ActorNavMoveTo);
	ProxyFactoryClass = UFlibMovementEx::StaticClass();
	ProxyClass = UActorMoveProxy::StaticClass();
}


FText UK2Node_SimpleNavMoveTo::GetTooltipText() const
{
	return NSLOCTEXT("UK2Node_SimpleNavMoveTo", "Actor Simple Nav Move To", "Actor Simple Nav Move To");
}

FText UK2Node_SimpleNavMoveTo::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_SimpleNavMoveTo", "Actor Simple Nav Move To", "Actor Simple Nav Move To");
}

FText UK2Node_SimpleNavMoveTo::GetMenuCategory() const
{
	return NSLOCTEXT("UK2Node_SimpleNavMoveTo", "MovementEx", "MovementEx");
}
