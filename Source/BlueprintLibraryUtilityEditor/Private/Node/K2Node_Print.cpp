// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/K2Node_Print.h"

#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "K2Node_MakeArray.h"
#include "Node/GraphNode_Printf.h"
#include "ToolMenu.h"
#include "ToolMenuSection.h"
#include "Utility/Flib_Utilities.h"


#define LOCTEXT_NAMESPACE "K2Node_Print"

FName UK2Node_Print::PN_Color = TEXT("Color");
FName UK2Node_Print::PN_PrintToLog = TEXT("PrintToLog");
FName UK2Node_Print::PN_PrintToSreen = TEXT("PrintToScreen");
FName UK2Node_Print::PN_Duration = TEXT("Duration");



void UK2Node_Print::GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu,Context);

	if (!Context->bIsDebugging)
	{
		if (Context->Pin != FindPin(UK2Node_Print::PN_PrintToSreen) && 
			Context->Pin != FindPin(UK2Node_Print::PN_Color) &&
			Context->Pin != FindPin(UK2Node_Print::PN_PrintToLog) &&
			Context->Pin != FindPin(UK2Node_Print::PN_Duration))
		{
			FToolMenuSection& Section = Menu->AddSection(FName(TEXT("UK2Node_Print")), LOCTEXT("UK2Node_Print","Action"));
			Section.AddMenuEntry(
				"RemovePin",
				LOCTEXT("RemovePin", "Remove pin"),
				LOCTEXT("RemovePinTooltip", "Remove this input pin"),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateUObject(const_cast<UK2Node_Print*>(this), &UK2Node_Print::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin))
				)
			);
		}
	}

}



void UK2Node_Print::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* ThenPin = GetThenPin();
	if (ExecPin && ThenPin) 
	{
		UFunction* Function = UFlib_Utilities::StaticClass()->FindFunctionByName(FName("PrintArray"));
		if (Function == NULL)
		{
			CompilerContext.MessageLog.Error(*LOCTEXT("InvalidFunctionName", "BaseAsyncTask: Type not supported or not initialized. @@").ToString(), this);
			return;
		}

		UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);

		CallFuncNode->SetFromFunction(Function);
		CallFuncNode->AllocateDefaultPins();
		CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(CallFuncNode->GetExecPin()));
		CompilerContext.MovePinLinksToIntermediate(*ThenPin, *(CallFuncNode->GetThenPin()));

		CompilerContext.MovePinLinksToIntermediate(*FindPin(UK2Node_Print::PN_PrintToSreen), *CallFuncNode->FindPin(TEXT("bScreen")));
		CompilerContext.MovePinLinksToIntermediate(*FindPin(UK2Node_Print::PN_PrintToLog), *CallFuncNode->FindPin(TEXT("bLog")));
		CompilerContext.MovePinLinksToIntermediate(*FindPin(UK2Node_Print::PN_Color), *CallFuncNode->FindPin(TEXT("Color")));
		CompilerContext.MovePinLinksToIntermediate(*FindPin(UK2Node_Print::PN_Duration), *CallFuncNode->FindPin(TEXT("Duration")));


		UK2Node_MakeArray* MakeArrayNode = CompilerContext.SpawnIntermediateNode<UK2Node_MakeArray>(this, SourceGraph);
		MakeArrayNode->AllocateDefaultPins();

		UEdGraphPin* ArrayOut = MakeArrayNode->GetOutputPin();
		UEdGraphPin* FuncArgPin = CallFuncNode->FindPinChecked(TEXT("InStrings"));
		ArrayOut->MakeLinkTo(FuncArgPin);

		MakeArrayNode->PinConnectionListChanged(ArrayOut);
		// connect all arg pin to Make Array input
		for (int32 i = 0; i < ArgPinNames.Num(); i++) {

			// Make Array node has one input by default
			if (i > 0)
				MakeArrayNode->AddInputPin();

			// find the input pin on the "Make Array" node by index.
			const FString PinName = FString::Printf(TEXT("[%d]"), i);
			UEdGraphPin* ArrayInputPin = MakeArrayNode->FindPinChecked(PinName);

			// move input word to array 
			UEdGraphPin* MyInputPin = FindPinChecked(ArgPinNames[i], EGPD_Input);
			CompilerContext.MovePinLinksToIntermediate(*MyInputPin, *ArrayInputPin);
		}// end of for
	}

	BreakAllNodeLinks();
}

void UK2Node_Print::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	UEdGraphPin* PinScreen = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, UK2Node_Print::PN_PrintToSreen);
	PinScreen->DefaultValue = "True";
	UEdGraphPin* PinLog = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, UK2Node_Print::PN_PrintToLog);
	PinLog->DefaultValue = "True";
	UScriptStruct* ColorStruct = TBaseStructure<FLinearColor>::Get();
	UEdGraphPin* PinColor = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, ColorStruct, UK2Node_Print::PN_Color);
	FLinearColor color = FLinearColor::Green;
	PinColor->DefaultValue = color.ToString();

	UEdGraphPin* PinDuration = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Float, UK2Node_Print::PN_Duration);
	PinDuration->DefaultValue = "2.0";

	for (const FName& PinName : ArgPinNames) {
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, PinName);
	}
}

void UK2Node_Print::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	Super::PinDefaultValueChanged(ChangedPin);
}


void UK2Node_Print::AddPinToNode()
{
	Modify();

	TMap<FString, FStringFormatArg> FormatArgs = {
			{TEXT("Count"), ArgPinNames.Num()}
	};

	FName NewPinName(*FString::Format(TEXT("{Count}"), FormatArgs));
	ArgPinNames.Add(NewPinName);

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, NewPinName);
}

void UK2Node_Print::RemoveInputPin(UEdGraphPin* Pin)
{
	FScopedTransaction Transaction(FText::FromString("Printf_RemoveInputPin"));
	Modify();

	ArgPinNames.Remove(Pin->GetFName());

	RemovePin(Pin);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}








TSharedPtr<SGraphNode> UK2Node_Print::CreateVisualWidget()
{
	return SNew(SGraphNode_Printf, this);
}

#undef LOCTEXT_NAMESPACE