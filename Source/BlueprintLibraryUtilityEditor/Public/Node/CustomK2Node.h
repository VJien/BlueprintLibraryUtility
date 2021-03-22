// CustomK2Node.h
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Kismet2/BlueprintEditorUtils.h"

#include "CustomK2Node.generated.h"


UENUM(BlueprintType)
enum class ETypeName2 :uint8
{
	None=0,
	Bool,
	Byte,
	Class,
	SoftClass,
	Int,
	Int64,
	Float,
	Name,
	Delegate,
	Object,
	Interface,
	SoftObject,
	String,
	Text,
	Struct,
	Enum
};

UCLASS(MinimalAPI)
class  UCustomK2Node : public UK2Node
{
	GENERATED_BODY()
	virtual FText GetTooltipText()const override
	{
		return FText::FromString("This is my custom k2 node");
	}

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType)const override
	{
		return FText::FromString(TEXT("CustomK2Node"));
	}
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegister)const override
	{
		UClass* ActionKey = GetClass();
		if (ActionRegister.IsOpenForRegistration(ActionKey))
		{
			UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
			check(NodeSpawner);
			ActionRegister.AddBlueprintAction(ActionKey, NodeSpawner);
		}
	}
	virtual FText GetMenuCategory()const {
		return FText::FromString(TEXT("UCustomK2Node"));
	}

	virtual void ExpandNode(FKismetCompilerContext & CompilerContext, UEdGraph * SourceGraph)override {
		Super::ExpandNode(CompilerContext, SourceGraph);

		UEdGraphPin* ExecPin = GetExecPin();
		UEdGraphPin* ThenPin = GetThenPin();
		if (ExecPin && ThenPin) {

			// create a CallFunction node
			FName MyFunctionName = GET_FUNCTION_NAME_CHECKED(UCustomK2Node, SayHello);

			UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
			CallFuncNode->FunctionReference.SetExternalMember(MyFunctionName, UCustomK2Node::StaticClass());
			CallFuncNode->AllocateDefaultPins();

			// move pins
			CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(CallFuncNode->GetExecPin()));
			CompilerContext.MovePinLinksToIntermediate(*ThenPin, *(CallFuncNode->GetThenPin()));
		}

		// break any links to the expanded node
		BreakAllNodeLinks();
	}

	virtual void AllocateDefaultPins()override
	{
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

		UEnum* const MethodEnum = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("ETypeName2"), true);
		UEdGraphPin *const EnumPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, MethodEnum, TEXT("TypeName"));
		if (CurrentName.IsEmpty())
		{
			EnumPin->DefaultValue = MethodEnum->GetNameStringByValue(static_cast<int>(ETypeName2::None));
			CurrentName = EnumPin->DefaultValue;
		}
		else
		{
			EnumPin->DefaultValue = MethodEnum->GetNameStringByValue(static_cast<int>(MethodEnum->GetValueByNameString(CurrentName)));
			CreatePin(EGPD_Input, FName(*CurrentName), *CurrentName);
		}

		Super::AllocateDefaultPins();

	}

	virtual void PinDefaultValueChanged(UEdGraphPin* ChangedPin)override
	{
		if (ChangedPin != nullptr && ChangedPin->PinName == TypeName && ChangedPin->Direction == EGPD_Input)
		{
			OnTypePinChanged();
		}
	}

	void OnTypePinChanged()
	{
		Modify();

		UEdGraphPin* Pin = FindPinChecked(TEXT("TypeName"));

		if (Pin && Pin->Direction == EGPD_Input)
		{

			UEnum* const MethodEnum = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("ETypeName2"), true);

			FString EnumParamName = Pin->GetDefaultAsString();
			if (EnumParamName.Equals(TEXT("None"))) return;

			FString EnumValueHasDoubleColon = FString::Printf(TEXT("%s::%s"), *MethodEnum->GetName(), *EnumParamName);
			int64 EnumValue = MethodEnum->GetValueByNameString(EnumValueHasDoubleColon);

			if (CurrentName.Equals(EnumParamName.ToLower())) return;

			UEdGraph* Graph = GetGraph();
			{
				UEdGraphPin* OldParamPin = FindPin(CurrentName);

				if (OldParamPin)
				{
					RemovePin(OldParamPin);
					Graph->NotifyGraphChanged();
				}
			}

			CreatePin(EGPD_Input, FName(*EnumParamName.ToLower()), *EnumParamName);
			CurrentName = EnumParamName.ToLower();

			Graph->NotifyGraphChanged();

			FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
		}
	}

	UEdGraphPin* GetThenPin() const
	{
		UEdGraphPin* Pin = FindPin(UEdGraphSchema_K2::PN_Then);
		check(Pin == nullptr || Pin->Direction == EGPD_Output); // If pin exists, it must be output
		return Pin;
	}

	UFUNCTION(BlueprintCallable)
	static void SayHello()
	{
		UE_LOG(LogTemp, Log, TEXT("Hello"));
	}

	static FName TypeName;

private:
	UPROPERTY()
	FString CurrentName;
};




