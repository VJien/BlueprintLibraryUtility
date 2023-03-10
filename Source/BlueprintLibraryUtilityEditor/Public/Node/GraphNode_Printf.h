// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KismetNodes/SGraphNodeK2Base.h"
//#include "GraphNode_Printf.generated.h"

class UK2Node_Print;
class SVerticalBox;

/**
 * 
 */
class BLUEPRINTLIBRARYUTILITYEDITOR_API SGraphNode_Printf : public SGraphNodeK2Base
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_Printf) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UEdGraphNode* InNode);
protected:
	virtual void CreateInputSideAddButton(TSharedPtr<SVerticalBox> InputBox) override;
	virtual FReply OnAddPin() override;
};
