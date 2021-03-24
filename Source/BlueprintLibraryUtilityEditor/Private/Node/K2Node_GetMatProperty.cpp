// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/K2Node_GetMatProperty.h"
#include "Kismet/KismetStringLibrary.h"
#include "IO/Types/IOTypes.h"
#include "IO/Lib/Flib_IO.h"
#include "Utility/Flib_Utilities.h"



#define LOCTEXT_NAMESPACE "K2Node_GetMatProperty"


 UScriptStruct* UK2Node_GetMatProperty::VectorStruct = nullptr;
 UScriptStruct* UK2Node_GetMatProperty::ColorStruct = nullptr;

 FName UK2Node_GetMatProperty:: FuntionName_Int = FName(TEXT("HandleData_Int"));
 FName UK2Node_GetMatProperty:: FuntionName_Float = FName(TEXT("HandleData_Float"));
 FName UK2Node_GetMatProperty:: FuntionName_String = FName(TEXT("HandleData_String"));
 FName UK2Node_GetMatProperty:: FuntionName_Vector = FName(TEXT("HandleData_Vector"));
 FName UK2Node_GetMatProperty:: FuntionName_Color = FName(TEXT("HandleData_Color"));

 FName UK2Node_GetMatProperty:: PinName_File = FName(TEXT("FilePath"));
 FName UK2Node_GetMatProperty:: PinName_Key = FName(TEXT("Key"));
 FName UK2Node_GetMatProperty:: PinName_Return = FName(TEXT("ReturnValue"));
 FName UK2Node_GetMatProperty:: PinName_Idx = FName(TEXT("Index"));
 FName UK2Node_GetMatProperty:: PinName_T = FName(TEXT("Type"));
 FName UK2Node_GetMatProperty:: PinName_N = FName(TEXT("N"));


EReturnType GetReturnTypeByKeyType(EMatPropertyKeyType key)
{
	switch (key)
	{
	case EMatPropertyKeyType::NAME:
		return EReturnType::String;
		break;
	case EMatPropertyKeyType::COLOR_DIFFUSE:
		return EReturnType ::Color;
		break;
	case EMatPropertyKeyType::COLOR_SPECULAR:
		return EReturnType ::Color;
		break;
	case EMatPropertyKeyType::COLOR_AMBIENT:
		return EReturnType::Color;
		break;
	case EMatPropertyKeyType::COLOR_EMISSIVE:
		return EReturnType::Color;
		break;
	case EMatPropertyKeyType::COLOR_TRANSPARENT:
		return EReturnType::Color;
		break;
	case EMatPropertyKeyType::COLOR_REFLECTIVE:
		return EReturnType::Color;
		break;
	case EMatPropertyKeyType::REFLECTIVITY:
		return EReturnType::Float;
		break;
	case EMatPropertyKeyType::WIREFRAME:
		return EReturnType::Int;
		break;
	case EMatPropertyKeyType::TWOSIDED:
		return EReturnType::Int;
		break;
	case EMatPropertyKeyType::SHADING_MODEL:
		return EReturnType::Int;
		break;
	case EMatPropertyKeyType::BLEND_FUNC:
		return EReturnType::Int;
		break;
	case EMatPropertyKeyType::OPACITY:
		return EReturnType::Float;
		break;
	case EMatPropertyKeyType::SHININESS:
		return EReturnType::Float;
		break;
	case EMatPropertyKeyType::SHININESS_STRENGTH:
		return EReturnType::Float;
		break;
	case EMatPropertyKeyType::REFRACTI:
		return EReturnType::Float;
		break;
	case EMatPropertyKeyType::TEXTURE:
		return EReturnType::String;
		break;
	case EMatPropertyKeyType::TEXBLEND:
		return EReturnType::Float;
		break;
	case EMatPropertyKeyType::TEXOP:
		return EReturnType::Int;
		break;
	case EMatPropertyKeyType::MAPPING:
		return EReturnType::Int;
		break;
	case EMatPropertyKeyType::UVWSRC:
		return EReturnType::Int;
		break;
	case EMatPropertyKeyType::MAPPINGMODE_U:
		return EReturnType::Int;
		break;
	case EMatPropertyKeyType::MAPPINGMODE_V:
		return EReturnType::Int;
		break;
	case EMatPropertyKeyType::TEXMAP_AXIS:
		return EReturnType::Vector;
		break;
	case EMatPropertyKeyType::TEXFLAGS:
		return EReturnType::Int;
		break;
	}
	return EReturnType::String;
}



FString TypeToString(EReturnType type)
{

	return BLU::EnumToString<EReturnType>(TEXT("EReturnType"), type, true);
}





FName UK2Node_GetMatProperty::GetFunctionName(EReturnType type)
{
	switch (type)
	{
	case EReturnType::Int:
		return   UK2Node_GetMatProperty::FuntionName_Int;     //FName(TEXT("HandleData_Int")); 
		break;
	case EReturnType::Float:
		return  UK2Node_GetMatProperty::FuntionName_Float;//FName(TEXT("HandleData_Float"));
		break;
	case EReturnType::String:
		return UK2Node_GetMatProperty::FuntionName_String; //FName(TEXT("HandleData_String"));
		break;
	case EReturnType::Vector:
		return UK2Node_GetMatProperty::FuntionName_Vector;//FName(TEXT("HandleData_Vector"));
		break;
	case EReturnType::Color:
		return UK2Node_GetMatProperty::FuntionName_Color; //FName(TEXT("HandleData_Color"));
		break;
	}
	return NAME_None;
}


UK2Node_GetMatProperty::UK2Node_GetMatProperty(const FObjectInitializer& ObjectInitializer) 
:Super(ObjectInitializer)
{
	FunctionName = FuntionName_String;
	if (VectorStruct == nullptr)
	{
		VectorStruct = TBaseStructure<FVector>::Get();
		ColorStruct = TBaseStructure<FLinearColor>::Get();
	}
}

void UK2Node_GetMatProperty::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin == FindPin(UK2Node_GetMatProperty::PinName_Key))
	{
		//UE_LOG(LogTemp, Log, TEXT("111111"));
	}
}

void UK2Node_GetMatProperty::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* ThenPin = GetThenPin();
	if (ExecPin && ThenPin) {

		UFunction* Function = UFlib_IO::StaticClass()->FindFunctionByName(FunctionName);
		if (Function == NULL)
		{
			CompilerContext.MessageLog.Error(*LOCTEXT("InvalidFunctionName", "BaseAsyncTask: Type not supported or not initialized. @@").ToString(), this);
			return;
		}

		UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);

		CallFuncNode->SetFromFunction(Function);
		CallFuncNode->AllocateDefaultPins();

		// move pins
		CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(CallFuncNode->GetExecPin()));
		CompilerContext.MovePinLinksToIntermediate(*ThenPin, *(CallFuncNode->GetThenPin()));
		CompilerContext.MovePinLinksToIntermediate(*FindPin(UK2Node_GetMatProperty::PinName_File), *(CallFuncNode->FindPin(UK2Node_GetMatProperty::PinName_File)));
		CompilerContext.MovePinLinksToIntermediate(*FindPin(UK2Node_GetMatProperty::PinName_Key), *(CallFuncNode->FindPin(UK2Node_GetMatProperty::PinName_Key)));
		CompilerContext.MovePinLinksToIntermediate(*FindPin(UK2Node_GetMatProperty::PinName_Return), *(CallFuncNode->FindPin(UK2Node_GetMatProperty::PinName_Return)));
		UEdGraphPin* tPin = FindPin(UK2Node_GetMatProperty::PinName_T);
		if (tPin && !tPin->bHidden)
		{
			CompilerContext.MovePinLinksToIntermediate(*FindPin(UK2Node_GetMatProperty::PinName_T), *(CallFuncNode->FindPin(UK2Node_GetMatProperty::PinName_T)));
		}
		UEdGraphPin* nPin = FindPin(UK2Node_GetMatProperty::PinName_N);
		if (nPin && !nPin->bHidden)
		{
			CompilerContext.MovePinLinksToIntermediate(*FindPin(UK2Node_GetMatProperty::PinName_N), *(CallFuncNode->FindPin(UK2Node_GetMatProperty::PinName_N)));
		}
		
	}

	// break any links to the expanded node
	BreakAllNodeLinks();
}





void UK2Node_GetMatProperty::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	UEnum* const KeyEnum = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EMatPropertyKeyType"), true);
	UEnum* const TypeEnum = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("ETextureType"), true);
	UEdGraphPin* pathPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, UK2Node_GetMatProperty::PinName_File);
	UEdGraphPin*  keyPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, KeyEnum, UK2Node_GetMatProperty::PinName_Key);
	UEdGraphPin* indexPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, UK2Node_GetMatProperty::PinName_Idx);
	UEdGraphPin* typePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, TypeEnum, UK2Node_GetMatProperty::PinName_T);
	UEdGraphPin* numberPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, UK2Node_GetMatProperty::PinName_N);
	
	if (CurrentKeyName.IsEmpty())
	{
		keyPin->DefaultValue = KeyEnum->GetNameStringByValue(static_cast<int>(EMatPropertyKeyType::NAME));
		CurrentKeyName = TypeToString(EReturnType::String);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_String, UK2Node_GetMatProperty::PinName_Return);
		CurrentTextureTypeName = TEXT("None");
		if (typePin && !typePin->bHidden)
		{
			typePin->bHidden = true;
			typePin->BreakAllPinLinks();
		}
		if (numberPin && !numberPin->bHidden)
		{
			numberPin->bHidden = true;
			numberPin->BreakAllPinLinks();
		}
		
	}
	else
	{
		keyPin->DefaultValue = KeyEnum->GetNameStringByValue(static_cast<int>(KeyEnum->GetValueByNameString(CurrentKeyName)));
		typePin->DefaultValue = TypeEnum->GetNameStringByValue(static_cast<int>(KeyEnum->GetValueByNameString(CurrentTextureTypeName)));
		EMatPropertyKeyType keyType = BLU::StringToEnum<EMatPropertyKeyType>(TEXT("EMatPropertyKeyType"), CurrentKeyName);
		EReturnType type =  GetReturnTypeByKeyType(keyType);
		RefreshExtraPin(keyType);
		RefreshReturnPin(type);
		
		
	}

	Super::AllocateDefaultPins();
}

void UK2Node_GetMatProperty::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	if (ChangedPin != nullptr && ChangedPin->PinName == PinName_Key && ChangedPin->Direction == EGPD_Input)
	{
		OnTypePinChanged();
	}
	if (ChangedPin != nullptr && ChangedPin->PinName == PinName_T && ChangedPin->Direction == EGPD_Input)
	{
		OnTextureTypePinChanged();
	}
}

void UK2Node_GetMatProperty::OnTypePinChanged()
{
	Modify();




	UEdGraphPin* keyPin = FindPinChecked(UK2Node_GetMatProperty::PinName_Key);

	if (keyPin && keyPin->Direction == EGPD_Input)
	{
		//获取key的值(string)
		UEnum* const MethodEnum = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EMatPropertyKeyType"), true);
		FString keyValueString = keyPin->GetDefaultAsString();
		if (keyValueString.Equals(TEXT("None"))) return;
		//获取完整名称
		FString keyValueHasDoubleColon = FString::Printf(TEXT("%s::%s"), *MethodEnum->GetName(), *keyValueString);
		int64 keyValue = MethodEnum->GetValueByNameString(keyValueHasDoubleColon);
		//未变化
		if (CurrentKeyName.Equals(keyValueString.ToLower())) return;
		EMatPropertyKeyType keyType = BLU::StringToEnum<EMatPropertyKeyType>(TEXT("EMatPropertyKeyType"), keyValueString);
		RefreshExtraPin(keyType);
		RefreshReturnPin(GetReturnTypeByKeyType(keyType));
		CurrentKeyName = keyValueString.ToLower();

		

		FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
	}
}

void UK2Node_GetMatProperty::OnTextureTypePinChanged()
{
	Modify();
	UEdGraphPin* tPin = FindPinChecked(UK2Node_GetMatProperty::PinName_T);
	if (tPin && tPin->Direction == EGPD_Input)
	{
		UEnum* const MethodEnum = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("ETextureType"), true);
		FString keyValueString = tPin->GetDefaultAsString();
		if (keyValueString.Equals(TEXT("NONE"))) return;
		//获取完整名称
		FString keyValueHasDoubleColon = FString::Printf(TEXT("%s::%s"), *MethodEnum->GetName(), *keyValueString);
		int64 keyValue = MethodEnum->GetValueByNameString(keyValueHasDoubleColon);
		if (CurrentTextureTypeName.Equals(keyValueString.ToLower())) return;
		ETextureType keyType = BLU::StringToEnum<ETextureType>(TEXT("ETextureType"), keyValueString);
		CurrentKeyName = keyValueString.ToLower();
		FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());

	}
}



void UK2Node_GetMatProperty::RefreshReturnPin(EReturnType newType)
{
	UEdGraph* Graph = GetGraph();
	if (Graph)
	{
		UEdGraphPin* oldRetPin = FindPin(UK2Node_GetMatProperty::PinName_Return);
		if (oldRetPin)
		{
			RemovePin(oldRetPin);
			Graph->NotifyGraphChanged();
		}
		Graph->NotifyGraphChanged();
	}
	
	

	switch (newType)
	{
	case EReturnType::Color:
	{
		FunctionName = GetFunctionName(EReturnType::Color);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, ColorStruct, UK2Node_GetMatProperty::PinName_Return);
		break;
	}
	case EReturnType::Vector:
	{
		FunctionName = GetFunctionName(EReturnType::Vector);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, VectorStruct, UK2Node_GetMatProperty::PinName_Return);
		break;
	}
	case EReturnType::Int:
	{
		FunctionName = GetFunctionName(EReturnType::Int);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Int, UK2Node_GetMatProperty::PinName_Return);
		break;
	}
	case EReturnType::Float:
	{
		FunctionName = GetFunctionName(EReturnType::Float);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Float, UK2Node_GetMatProperty::PinName_Return);
		break;
	}
	case EReturnType::String:
	{
		FunctionName = GetFunctionName(EReturnType::String);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_String, UK2Node_GetMatProperty::PinName_Return);
		break;
	}
	default:
		FunctionName = GetFunctionName(EReturnType::String);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_String, UK2Node_GetMatProperty::PinName_Return);
		break;
	}
}

void UK2Node_GetMatProperty::RefreshExtraPin(EMatPropertyKeyType key)
{
	bool bHasExtraPin =  key == EMatPropertyKeyType::TEXTURE || key == EMatPropertyKeyType::TEXBLEND || key == EMatPropertyKeyType::TEXOP ||
		key == EMatPropertyKeyType::MAPPING || key == EMatPropertyKeyType::UVWSRC || key == EMatPropertyKeyType::MAPPINGMODE_U || 
		key == EMatPropertyKeyType::MAPPINGMODE_V || key == EMatPropertyKeyType::TEXMAP_AXIS || key == EMatPropertyKeyType::TEXFLAGS;

		UEdGraphPin* typePin = FindPin(UK2Node_GetMatProperty::PinName_T);
		UEdGraphPin* nPin = FindPin(UK2Node_GetMatProperty::PinName_N);
		if (typePin)
		{
			typePin->bHidden = !bHasExtraPin;
			if (!bHasExtraPin)
			{
				typePin->BreakAllPinLinks();
			}
		}
		if (nPin)
		{
			nPin->bHidden = !bHasExtraPin;
			if (!bHasExtraPin)
			{
				nPin->BreakAllPinLinks();
			}
		}
	
}






#undef LOCTEXT_NAMESPACE