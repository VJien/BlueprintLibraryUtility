// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralMeshComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IO/Types/IOTypes.h"

#include "Flib_IO.generated.h"

class UTextureRenderTarget2D;


USTRUCT(BlueprintInternalUseOnly)
struct FDummyStruct {
	GENERATED_USTRUCT_BODY()

};


/**
 * 
 */
UCLASS()
class BLUEPRINTLIBRARYUTILITY_API UFlib_IO : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
#pragma region mesh	
	//load material data for ProceduralMesh create sections
	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO")
		static FLoadedMeshData LoadMeshData(FString filepath, FLoadConfig Config);

	static FMatPropertyRetValue LoadMeshMaterialProperty(const FString& FilePath, EMatPropertyKeyType key, unsigned int matIdx, ETextureType type, int32 number);
#pragma endregion mesh


#pragma region texture	
	/*
		    type = 
			Invalid = -1,
			RGBA =  0,
			BGRA =  1,
			Gray =  2,
	*/
	UFUNCTION(BlueprintPure, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = "image png jpg jpeg bmp bitmap ico icon exr icns"))
		static UTexture2D* LoadTexture2DFromFile(const FString& FilePath, uint8 type,  bool& IsValid, int32& Width, int32& Height);
		/*
		    type = 
			Invalid = -1,
			RGBA =  0,
			BGRA =  1,
			Gray =  2,
		*/
	UFUNCTION(BlueprintPure, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool LoadTextureAsByteFromFile(const FString& FilePath, uint8 type, TArray<uint8>& OutArray);
	UFUNCTION(BlueprintPure, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static TArray<FColor> ByteToColor(const TArray<uint8>& origin);
	UFUNCTION(BlueprintPure, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static UTexture2D* MakeTexture2dFromColor(const int32 SrcWidth, const int32 SrcHeight, const TArray<FColor>& SrcData, const bool UseAlpha);


	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool ExportTextureRenderTarget2D2PNG(UTextureRenderTarget2D* TextureRenderTarget, const FString& FilePath);
#pragma endregion texture




#pragma region sound	
	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static class USoundWave* LoadWaveDataFromFile(const FString& FilePath);
#pragma endregion sound



#pragma region file

	/** Read Or Write Custom Text */
	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool ReadFile(const FString& FilePath, FString& ReturnString);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool RenameFile(const FString& FilePath, const FString& NewName);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool DoesFileExist(const FString& File);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool CreateDirectory(const FString& Directory);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool DoesDirectoryExist(const FString& Directory);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool WriteFile(const FString& FilePath, const FString& FileString);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool DeleteFile(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool MoveFile(const FString& Source, const FString& Dest);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool DeleteFiles(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool CopyFile(const FString& FilePath, const FString& ToPath);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static	TArray<FString> OpenWindowsFilesDialog(const FString& Path, const FString& fileName, const FString& SufStr);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static FString OpenWindowsDirectory(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static void OpenWindowsFolder(const FString& AbsolutePath);


	
#pragma endregion file


#pragma region exe	
	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = "exe"))
		static void OpenExe(const FString& Path, const FString& Args);
#pragma endregion exe


#pragma region message	
	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = "msg"))
		static EMsgRetType OpenMessageWindow(EMsgType Type, FText Msg);
#pragma endregion message


	UFUNCTION(BlueprintPure, CustomThunk, meta = (CustomStructureParam = "StructReference", DisplayName = "Struct to JSON String"), Category = "File|Json")
		static void UStructToJsonObjectString(const int32& StructReference, FString& JSONString);
	static void GenericUStructToJsonObjectString(const UStruct* StructDefinition, const void* Struct, FString& OutJsonString, int64 CheckFlags, 
	int64 SkipFlags);

	DECLARE_FUNCTION(execUStructToJsonObjectString)
	{
		//Stack.Step(Stack.Object, NULL);
		Stack.StepCompiledIn<FStructProperty>(NULL);
		FStructProperty* StructProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
		void* StructPtr = Stack.MostRecentPropertyAddress;

		//Get JsonString reference
		P_GET_PROPERTY_REF(FStrProperty, JSONString);

		P_FINISH;

		P_NATIVE_BEGIN;

		GenericUStructToJsonObjectString(StructProperty->Struct,StructPtr,JSONString,0,0);

		P_NATIVE_END;
	}




	// only handle LoadMaterialsProperty
	UFUNCTION(BlueprintCallable)
		static FString HandleData_String(const FString& FilePath, EMatPropertyKeyType Key, int32 Index, ETextureType Type, int32 N);
	UFUNCTION(BlueprintCallable)
		static int32 HandleData_Int(const FString& FilePath, EMatPropertyKeyType Key, int32 Index, ETextureType Type, int32 N);
	UFUNCTION(BlueprintCallable)
		static float HandleData_Float(const FString& FilePath, EMatPropertyKeyType Key, int32 Index, ETextureType Type, int32 N);
	UFUNCTION(BlueprintCallable)
		static FVector HandleData_Vector(const FString& FilePath, EMatPropertyKeyType Key, int32 Index, ETextureType Type, int32 N);
	UFUNCTION(BlueprintCallable)
		static FLinearColor HandleData_Color(const FString& FilePath, EMatPropertyKeyType Key, int32 Index, ETextureType Type, int32 N);














};
