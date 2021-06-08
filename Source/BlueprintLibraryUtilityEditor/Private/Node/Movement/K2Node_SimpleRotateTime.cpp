// Fill out your copyright notice in the Description page of Project Settings.


#include "Node\Movement\K2Node_SimpleRotateTime.h"
#include "Movement/MovementExBPLibrary.h"
#include "Movement/MovementExBPLibrary.h"


UK2Node_SimpleRotateTime::UK2Node_SimpleRotateTime(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UFlibMovementEx, V_ActorRotateByTime);
	ProxyFactoryClass = UFlibMovementEx::StaticClass();
	ProxyClass = URotateProxy::StaticClass();
}


FText UK2Node_SimpleRotateTime::GetTooltipText() const
{
	return NSLOCTEXT("UK2Node_SimpleRotateTime", "Actor Simple Rotate By Time", "Actor Simple Rotate By Time");
}

FText UK2Node_SimpleRotateTime::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_SimpleRotateTime", "Actor Simple Rotate By Time", "Actor Simple Rotate By Time");
}

FText UK2Node_SimpleRotateTime::GetMenuCategory() const
{
	return NSLOCTEXT("UK2Node_SimpleRotateTime", "MovementEx", "MovementEx");
}
