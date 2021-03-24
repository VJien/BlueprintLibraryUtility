// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "IO/Types/IOTypes.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "K2Node_GetMatProperty.generated.h"






UCLASS(MinimalAPI)
class  UK2Node_GetMatProperty : public UK2Node
{
	GENERATED_UCLASS_BODY()

public:
	/*virtual bool IsNodePure() const override { return true; }
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override{return true;};*/
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin)override;

	virtual FText GetTooltipText()const override
	{
		return FText::FromString("Load Mat Property from File");
	}

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType)const override
	{
		return FText::FromString(TEXT("LoadMaterialProperty"));
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
		return FText::FromString(TEXT("BlueprintLibraryUtility|IO"));
	}
	UEdGraphPin* GetThenPin() const
	{
		UEdGraphPin* Pin = FindPin(UEdGraphSchema_K2::PN_Then);
		check(Pin == nullptr || Pin->Direction == EGPD_Output); // If pin exists, it must be output
		return Pin;
	}
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)override ;
	
	

	virtual void AllocateDefaultPins()override;


	virtual void PinDefaultValueChanged(UEdGraphPin* ChangedPin)override;

	void OnTypePinChanged();
	void OnTextureTypePinChanged();




	UFUNCTION()
		FName GetFunctionName(EReturnType type);
	UFUNCTION()
		void RefreshReturnPin(EReturnType newType);
	UFUNCTION()
		void RefreshExtraPin(EMatPropertyKeyType key);
		
private:
		UPROPERTY()
			FName FunctionName;
		UPROPERTY()
			FString CurrentKeyName;
		UPROPERTY()
			FString CurrentTextureTypeName;
		UPROPERTY()
			FString CurrentOutName;

protected:
		static FName FuntionName_Int ;
		static FName FuntionName_Float ;
		static FName FuntionName_String ;
		static FName FuntionName_Vector ;
		static FName FuntionName_Color ;

		static FName PinName_File ;
		static FName PinName_ZUP ;
		static FName PinName_Key ;
		static FName PinName_Return ;
		static FName PinName_Idx ;
		static FName PinName_T ;
		static FName PinName_N ;

		static  UScriptStruct* VectorStruct;
		static  UScriptStruct* ColorStruct;
			
};
