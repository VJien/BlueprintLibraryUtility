// Fill out your copyright notice in the Description page of Project Settings.


#include "Node\Movement\K2Node_SimpleMoveTo.h"
#include "Movement/MovementExBPLibrary.h"
#include "Movement/Proxy/PathMoveProxy.h"



UK2Node_SimpleMoveTo::UK2Node_SimpleMoveTo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UFlibMovementEx, V_ActorSimpleMoveTo);
	ProxyFactoryClass = UFlibMovementEx::StaticClass();
	ProxyClass = UActorMoveProxy::StaticClass();
}


FText UK2Node_SimpleMoveTo::GetTooltipText() const
{
	return NSLOCTEXT("UK2Node_SimpleMoveTo", "Actor Simple Move To", "Actor Simple Move To");
}

FText UK2Node_SimpleMoveTo::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_SimpleMoveTo", "Actor Simple Move To", "Actor Simple Move To");
}

FText UK2Node_SimpleMoveTo::GetMenuCategory() const
{
	return NSLOCTEXT("UK2Node_SimpleMoveTo", "MovementEx", "MovementEx");
}
