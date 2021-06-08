#include "Node/Movement/K2Node_MoveAndRotate.h"
#include "Movement/MovementExBPLibrary.h"
#include "Movement/Proxy/RotationAndMoveProxy.h"


UK2Node_MoveAndRotate::UK2Node_MoveAndRotate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UFlibMovementEx, V_ActorMoveAndRotateByTime);
	ProxyFactoryClass = UFlibMovementEx::StaticClass();
	ProxyClass = UMoveAndRotateProxy::StaticClass();
}

FText UK2Node_MoveAndRotate::GetTooltipText() const
{
	return NSLOCTEXT("UK2Node_MoveAndRotate", "Actor Move and Rotate By Time", "Actor Move and Rotate By Time");

}

FText UK2Node_MoveAndRotate::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_MoveAndRotate", "Actor Move and Rotate By Time", "Actor Move and Rotate By Time");
}

FText UK2Node_MoveAndRotate::GetMenuCategory() const
{
	return NSLOCTEXT("UK2Node_MoveAndRotate", "MovementEx", "MovementEx");
}

