// Fill out your copyright notice in the Description page of Project Settings.


#include "Node\Movement\K2Node_AIPathMoveTo.h"
#include "Movement/MovementExBPLibrary.h"
#include "Movement/Proxy/PathMoveProxy.h"


UK2Node_AIPathMoveTo::UK2Node_AIPathMoveTo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UFlibMovementEx, V_AIPathMoveTo);
	ProxyFactoryClass = UFlibMovementEx::StaticClass();
	ProxyClass = UPathMoveProxy::StaticClass();
}


FText UK2Node_AIPathMoveTo::GetTooltipText() const
{
	return NSLOCTEXT("UK2Node_AIPathMoveTo", "move by a path point list", "move by a path point list");
}

FText UK2Node_AIPathMoveTo::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_AIPathMoveTo", "AI Path Move To", "AI Path Move To");
}

FText UK2Node_AIPathMoveTo::GetMenuCategory() const
{
	return NSLOCTEXT("UK2Node_AIPathMoveTo", "MovementEx", "MovementEx");
}
