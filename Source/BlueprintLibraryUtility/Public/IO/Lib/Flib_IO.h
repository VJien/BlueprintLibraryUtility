// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralMeshComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IO/Types/IOTypes.h"
#include "Flib_IO.generated.h"

class UTextureRenderTarget2D;





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
	/** Load Texture2D */
	UFUNCTION(BlueprintPure, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = "image png jpg jpeg bmp bitmap ico icon exr icns"))
		static UTexture2D* LoadTexture2DFromFile(const FString& FilePath, bool& IsValid, int32& Width, int32& Height);

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
		static bool ReadFile(const FString FilePath, FString& ReturnString);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool WriteFile(const FString FilePath, const FString& FileString);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool DeleteFile(const FString FilePath);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool DeleteFiles(const FString FilePath);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static bool CopyFile(const FString FilePath, const FString ToPath);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static	TArray<FString> OpenWindowsFilesDialog(const FString& Path, const FString& fileName, const FString& SufStr);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static FString OpenWindowsDirectory(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = ""))
		static void OpenWindowsFolder(const FString& AbsolutePath);


#pragma endregion file


#pragma region exe	
	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = "exe"))
		static void OpenExe(const FString Path, const FString Args);
#pragma endregion exe


#pragma region message	
	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO", meta = (Keywords = "msg"))
		static EMsgRetType OpenMessageWindow(EMsgType Type, FText Msg);
#pragma endregion message

};
