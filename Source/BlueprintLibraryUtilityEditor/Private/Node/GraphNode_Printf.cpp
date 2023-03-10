// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/GraphNode_Printf.h"
#include "GraphEditorSettings.h"
#include "SGraphNode.h"
#include "Node/K2Node_Print.h"



void SGraphNode_Printf::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	this->GraphNode = InNode;

	this->SetCursor(EMouseCursor::CardinalCross);

	this->UpdateGraphNode();
}

void SGraphNode_Printf::CreateInputSideAddButton(TSharedPtr<SVerticalBox> InputBox)
{
	FText AddPinName = FText::FromString(TEXT("Add Pin"));
	FText RemovePinName = FText::FromString(TEXT("Remove Pin"));
	TSharedRef<SWidget> AddPinButton = AddPinButtonContent(AddPinName, AddPinName);
	TSharedRef<SWidget> RemovePinButton = AddPinButtonContent(RemovePinName, RemovePinName);


	FMargin AddPinPadding = Settings->GetInputPinPadding();
	AddPinPadding.Top += 6.0f;

	InputBox->AddSlot()
		.AutoHeight()
		.VAlign(VAlign_Center)
		.Padding(AddPinPadding)
		[
			AddPinButton
		];
}

FReply SGraphNode_Printf::OnAddPin()
{
	UK2Node_Print* BPNode = CastChecked<UK2Node_Print>(GraphNode);

	const FScopedTransaction Transaction(NSLOCTEXT("Kismet", "AddArgumentPin", "Add Argument Pin"));
	BPNode->Modify();

	BPNode->AddPinToNode();
	FBlueprintEditorUtils::MarkBlueprintAsModified(BPNode->GetBlueprint());

	UpdateGraphNode();
	GraphNode->GetGraph()->NotifyGraphChanged();

	return FReply::Handled();
}
