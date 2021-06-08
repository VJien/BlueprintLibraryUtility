// Fill out your copyright notice in the Description page of Project Settings.


#include "Node\Movement\K2Node_SimpleRotate.h"
#include "Movement/MovementExBPLibrary.h"
#include "Movement/Proxy/RotateProxy.h"


UK2Node_SimpleRotate::UK2Node_SimpleRotate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UFlibMovementEx, V_ActorRotateByTime);
	ProxyFactoryClass = UFlibMovementEx::StaticClass();
	ProxyClass = URotateProxy::StaticClass();
}


FText UK2Node_SimpleRotate::GetTooltipText() const
{
	return NSLOCTEXT("UK2Node_SimpleRotate", "Actor Simple Rotate By Speed", "Actor Simple Rotate By Speed");
}

FText UK2Node_SimpleRotate::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_SimpleRotate", "Actor Simple Rotate By Speed", "Actor Simple Rotate By Speed");
}

FText UK2Node_SimpleRotate::GetMenuCategory() const
{
	return NSLOCTEXT("UK2Node_SimpleRotate", "MovementEx", "MovementEx");
}
