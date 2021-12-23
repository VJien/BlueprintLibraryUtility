// Fill out your copyright notice in the Description page of Project Settings.
#include "IO/Lib/Flib_IO.h"
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include "IImageWrapper.h"
#include "Engine/Texture2D.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Sound/SoundWave.h"
#include "Misc/FileHelper.h"
#include "TextureResource.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ImageUtils.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "Windows/WindowsPlatformProcess.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "Misc/MessageDialog.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"
#include "Kismet/KismetArrayLibrary.h"





void FindMeshData(const aiScene* scene, aiNode* node, FLoadedMeshData& outLoadedMeshData)
{
	if (scene == nullptr || node == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindMeshData Failed : invalid scene or node!"));
		return;
	}

	for (unsigned i = 0; i < node->mNumMeshes; ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("FindMeshData : %s"), node->mName.C_Str());
		int meshesIdx = *node->mMeshes;
		aiMesh* mesh = scene->mMeshes[meshesIdx];
		FMeshData& meshData = outLoadedMeshData.meshData[meshesIdx];

		//conv aiMatrix4x4 to FTransform
		aiMatrix4x4 trans = node->mTransformation;
		FMatrix matrix;
		matrix.M[0][0] = trans.a1; matrix.M[0][1] = trans.b1; matrix.M[0][2] = trans.c1; matrix.M[0][3] = trans.d1;
		matrix.M[1][0] = trans.a2; matrix.M[1][1] = trans.b2; matrix.M[1][2] = trans.c2; matrix.M[1][3] = trans.d2;
		matrix.M[2][0] = trans.a3; matrix.M[2][1] = trans.b3; matrix.M[2][2] = trans.c3; matrix.M[2][3] = trans.d3;
		matrix.M[3][0] = trans.a4; matrix.M[3][1] = trans.b4; matrix.M[3][2] = trans.c4; matrix.M[3][3] = trans.d4;
		meshData.RelativeTransform = FTransform(matrix);


		//vertex
		for (unsigned j = 0; j < mesh->mNumVertices; ++j)
		{

			FVector vertex = FVector(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
			vertex = meshData.RelativeTransform.TransformFVector4(vertex);
			meshData.Vertices.Push(vertex);

			//normal
			if (mesh->HasNormals())
			{
				FVector normal = FVector(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);
				meshData.Normals.Push(normal);
			}
			else
			{
				meshData.Normals.Push(FVector::ZeroVector);
			}

			//UV
			if (mesh->HasTextureCoords(0))
			{
				FVector2D uv = FVector2D(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
				meshData.UV0.Push(uv);
			}

			//tangent
			if (mesh->HasTangentsAndBitangents())
			{
				FProcMeshTangent tangent = FProcMeshTangent(mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z);
				meshData.Tangents.Push(tangent);
			}

			if (mesh->HasVertexColors(0))
			{
				FLinearColor color = FLinearColor(mesh->mColors[0][j].r, mesh->mColors[0][j].g,
					mesh->mColors[0][j].b, mesh->mColors[0][j].a);
				meshData.VertexColors.Push(color);
			}
		}
	}
}



void FindMeshDataRecursive(const aiScene* scene, aiNode* node, FLoadedMeshData& outLoadedMeshData)
{
	FindMeshData(scene, node, outLoadedMeshData);
	for (uint32 i = 0; i < node->mNumChildren; ++i)
	{
		FindMeshDataRecursive(scene, node->mChildren[i], outLoadedMeshData);
	}
}




FLoadedMeshData UFlib_IO::LoadMeshData(FString filepath, FLoadConfig Config)
{
	FLoadedMeshData out;
	if (filepath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadMesh Failed : invalid path!"));
		return out;
	}

	std::string path = TCHAR_TO_UTF8(*filepath);

	Assimp::Importer Importer;
	auto process = (Config.Triangulate ? aiProcess_Triangulate : 0b0) |
		(Config.MakeLeftHanded ? aiProcess_MakeLeftHanded : 0b0) |
		(Config.CalcTangentSpace ? aiProcess_CalcTangentSpace : 0b0) |
		(Config.GenSmoothNormals ? aiProcess_GenSmoothNormals : 0b0) |
		(Config.OptimizeMeshes ? aiProcess_OptimizeMeshes : 0b0) |
		(Config.FlipUVsv ? aiProcess_FlipUVs : 0b0) |
		(Config.SortByPType ? aiProcess_SortByPType : 0b0) |
		(Config.JoinIdenticalVertices ? aiProcess_JoinIdenticalVertices : 0b0);
	const aiScene* Scene = Importer.ReadFile(path, process);


	if (Scene == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadMesh Failed : invalid Scene!\n%s"), Importer.GetErrorString());
		return out;
	}
	if (Scene->HasMeshes())
	{
		out.MeshNumbers = Scene->mNumMeshes;
		out.meshData.SetNum(Scene->mNumMeshes, false);
		FindMeshDataRecursive(Scene, Scene->mRootNode, out);

		for (uint32 i = 0; i < Scene->mNumMeshes; ++i)
		{
			//Triangle number
			for (uint32 l = 0; l < Scene->mMeshes[i]->mNumFaces; ++l)
			{
				for (uint32 m = 0; m < Scene->mMeshes[i]->mFaces[l].mNumIndices; ++m)
				{
					out.meshData[i].Triangles.Push(Scene->mMeshes[i]->mFaces[l].mIndices[m]);
				}
			}
		}
		out.bSuccess = true;
	}


	return out;
}

FMatPropertyRetValue UFlib_IO::LoadMeshMaterialProperty(const FString& FilePath, EMatPropertyKeyType key, unsigned int matIdx, EAssimpTextureType type, int32 number)
{
	if (FilePath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadMesh Failed : invalid path!"));
		return FMatPropertyRetValue();
	}

	std::string path = TCHAR_TO_UTF8(*FilePath);
	Assimp::Importer Importer;
	const aiScene* scene = Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes);



	FMatPropertyRetValue out;

	if (scene && scene->HasMaterials())
	{
		if (matIdx >= 0 && matIdx < scene->mNumMaterials)
		{
			aiMaterial* mat = scene->mMaterials[matIdx];
			if (mat)
			{

				aiColor3D color;
				aiString str;
				aiVector3D v3d;
				float f;
				int i;
				switch (key)
				{
				case EMatPropertyKeyType::None:
					break;
				case EMatPropertyKeyType::NAME:
				{

					if (AI_SUCCESS != mat->Get(AI_MATKEY_NAME, str))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(str);
					break;
				}

				case EMatPropertyKeyType::COLOR_DIFFUSE:
					if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_DIFFUSE, color))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					UE_LOG(LogTemp, Warning, TEXT(" Get Material Property COLOR_DIFFUSE %f,%f,%f!!"), color.r, color.g, color.b);
					out = FMatPropertyRetValue(color);
					break;
				case EMatPropertyKeyType::COLOR_SPECULAR:
					if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_SPECULAR, color))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(color);;
					break;
				case EMatPropertyKeyType::COLOR_AMBIENT:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_AMBIENT, color))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(color);
					break;
				}
				case EMatPropertyKeyType::COLOR_EMISSIVE: {
					if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_EMISSIVE, color))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(color);
					break;
				}
														break;
				case EMatPropertyKeyType::COLOR_TRANSPARENT:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_TRANSPARENT, color))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(color);
					break;
				}
				case EMatPropertyKeyType::COLOR_REFLECTIVE:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_REFLECTIVE, color))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(color);
					break;
				}
				case EMatPropertyKeyType::REFLECTIVITY:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_REFLECTIVITY, f))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(f);
					break;
				}
				case EMatPropertyKeyType::WIREFRAME:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_ENABLE_WIREFRAME, f))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(f);
					break;
				}
				case EMatPropertyKeyType::TWOSIDED:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_TWOSIDED, i))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(i);
					break;
				}
				case EMatPropertyKeyType::SHADING_MODEL:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_SHADING_MODEL, i))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(i);
					break;
				}
				case EMatPropertyKeyType::BLEND_FUNC:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_BLEND_FUNC, i))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(i);
					break;
				}
				case EMatPropertyKeyType::OPACITY:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_OPACITY, f))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					UE_LOG(LogTemp, Warning, TEXT(" Get Material Property OPACITY %F!!"), f);
					out = FMatPropertyRetValue(f);
					break;
				}
				case EMatPropertyKeyType::SHININESS:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_SHININESS, f))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(f);
					break;
				}
				case EMatPropertyKeyType::SHININESS_STRENGTH:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_SHININESS_STRENGTH, f))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(f);
					break;
				}
				case EMatPropertyKeyType::REFRACTI:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_REFRACTI, f))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(f);
					break;
				}
				case EMatPropertyKeyType::TEXTURE:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_TEXTURE(aiTextureType(int(type)), number), str))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(str);
					break;
				}
				case EMatPropertyKeyType::TEXBLEND:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_TEXBLEND(aiTextureType(int(type)), number), f))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(f);
					break;
				}
				case EMatPropertyKeyType::TEXOP:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_TEXOP(aiTextureType(int(type)), number), i))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(i);
					break;
				}
				case EMatPropertyKeyType::MAPPING:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_MAPPING(aiTextureType(int(type)), number), i))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(i);
					break;
				}
				case EMatPropertyKeyType::UVWSRC:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_UVWSRC(aiTextureType(int(type)), number), i))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(i);
					break;
				}
				case EMatPropertyKeyType::MAPPINGMODE_U:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_MAPPINGMODE_U(aiTextureType(int(type)), number), i))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(i);
					break;
				}
				case EMatPropertyKeyType::MAPPINGMODE_V:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_MAPPINGMODE_V(aiTextureType(int(type)), number), i))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(i);
					break;
				}
				case EMatPropertyKeyType::TEXMAP_AXIS:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_TEXMAP_AXIS(aiTextureType(int(type)), number), v3d))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(v3d);
					break;
				}
				case EMatPropertyKeyType::TEXFLAGS:
				{
					if (AI_SUCCESS != mat->Get(AI_MATKEY_TEXFLAGS(aiTextureType(int(type)), number), i))
					{
						UE_LOG(LogTemp, Warning, TEXT(" Get Material Property Failed!!"));
					}
					out = FMatPropertyRetValue(i);
					break;
				}
				default:
					break;
				}

			}
		}

	}


	return out;
}
//Discern Texture Type
static TSharedPtr<IImageWrapper> GetImageWrapperByExtention(const FString& InImagePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::GetModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));



	if (InImagePath.EndsWith(".png"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	}
	else if (InImagePath.EndsWith(".jpg") || InImagePath.EndsWith(".jpeg"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	}
	else if (InImagePath.EndsWith(".bmp"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	}
	else if (InImagePath.EndsWith(".ico"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO);
	}
	else if (InImagePath.EndsWith(".exr"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
	}
	else if (InImagePath.EndsWith(".icns"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS);
	}
	return nullptr;
}
UTexture2D* UFlib_IO::LoadTexture2DFromFile(const FString& FilePath, uint8 type, bool& IsValid, int32& Width, int32& Height)
{

	IsValid = false;
	UTexture2D* LoadedT2D = NULL;


	TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(FilePath);

	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *FilePath, 0)) return NULL;

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		TArray<uint8> UncompressedBGRA;
		if (ImageWrapper->GetRaw(ERGBFormat(FMath::Clamp(int32(type), -1, 2)), 8, UncompressedBGRA))
		{

			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			if (!LoadedT2D) return NULL;

			Width = ImageWrapper->GetWidth();
			Height = ImageWrapper->GetHeight();

			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

			LoadedT2D->UpdateResource();
		}
	}

	IsValid = true;
	return LoadedT2D;
}



bool UFlib_IO::LoadTextureAsByteFromFile(const FString& FilePath, uint8 type, TArray<uint8>& OutArray)
{
	if (FilePath.IsEmpty())
	{
		return false;
	}
	TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(FilePath);
	TArray<uint8> RawFileData;
	if (FFileHelper::LoadFileToArray(RawFileData, *FilePath, 0))
	{
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
		{
			TArray<uint8> UncompressedBGRA;
			if (ImageWrapper->GetRaw(ERGBFormat(FMath::Clamp(int32(type),-1,2)), 8, UncompressedBGRA))
			{
				OutArray = UncompressedBGRA;
				return true;
			}
		}
	}
	return false;
}

TArray<FColor> UFlib_IO::ByteToColor(const TArray<uint8>& origin)
{
	TArray<FColor> uncompressedFColor;
	uint8 auxOrigin;
	FColor auxDst;

	for (int i = 0; i < origin.Num(); i++) {
		auxOrigin = origin[i];
		auxDst.R = auxOrigin;
		i++;
		auxOrigin = origin[i];
		auxDst.G = auxOrigin;
		i++;
		auxOrigin = origin[i];
		auxDst.B = auxOrigin;
		i++;
		auxOrigin = origin[i];
		auxDst.A = auxOrigin;
		uncompressedFColor.Add(auxDst);
	}

	return  uncompressedFColor;
}

UTexture2D* UFlib_IO::MakeTexture2dFromColor(const int32 SrcWidth, const int32 SrcHeight, const TArray<FColor>& SrcData, const bool UseAlpha)
{
	// 创建Texture2D纹理  
	UTexture2D* MyScreenshot = UTexture2D::CreateTransient(SrcWidth, SrcHeight, PF_B8G8R8A8);

	// 锁住他的数据，以便修改  
	uint8* MipData = static_cast<uint8*>(MyScreenshot->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	// 创建纹理数据  
	uint8* DestPtr = NULL;
	const FColor* SrcPtr = NULL;
	for (int32 y = 0; y < SrcHeight; y++)
	{
		DestPtr = &MipData[(SrcHeight - 1 - y) * SrcWidth * sizeof(FColor)];
		SrcPtr = const_cast<FColor*>(&SrcData[(SrcHeight - 1 - y) * SrcWidth]);
		for (int32 x = 0; x < SrcWidth; x++)
		{
			*DestPtr++ = SrcPtr->B;
			*DestPtr++ = SrcPtr->G;
			*DestPtr++ = SrcPtr->R;
			if (UseAlpha)
			{
				*DestPtr++ = SrcPtr->A;
			}
			else
			{
				*DestPtr++ = 0xFF;
			}
			SrcPtr++;
		}
	}

	// 解锁纹理  
	MyScreenshot->PlatformData->Mips[0].BulkData.Unlock();
	MyScreenshot->UpdateResource();

	return MyScreenshot;
}

bool UFlib_IO::ExportTextureRenderTarget2D2PNG(UTextureRenderTarget2D* TextureRenderTarget, const FString& FilePath)
{

	FTextureRenderTargetResource* rtResource = TextureRenderTarget->GameThread_GetRenderTargetResource();
	FReadSurfaceDataFlags readPixelFlags(RCM_UNorm);

	TArray<FColor> outBMP;

	for (FColor& color : outBMP)
	{
		color.A = 255;
	}
	outBMP.AddUninitialized(TextureRenderTarget->GetSurfaceWidth() * TextureRenderTarget->GetSurfaceHeight());
	rtResource->ReadPixels(outBMP, readPixelFlags);

	FIntPoint destSize(TextureRenderTarget->GetSurfaceWidth(), TextureRenderTarget->GetSurfaceHeight());
	TArray<uint8> CompressedBitmap;
	FImageUtils::CompressImageArray(destSize.X, destSize.Y, outBMP, CompressedBitmap);
	bool imageSavedOk = FFileHelper::SaveArrayToFile(CompressedBitmap, *FilePath);

	return imageSavedOk;
}

class USoundWave* UFlib_IO::LoadWaveDataFromFile(const FString& FilePath)
{
	USoundWave* sw = NewObject<USoundWave>(USoundWave::StaticClass());

	if (!sw)
		return nullptr;


	TArray < uint8 > rawFile;

	FFileHelper::LoadFileToArray(rawFile, FilePath.GetCharArray().GetData());
	FWaveModInfo WaveInfo;

	if (WaveInfo.ReadWaveInfo(rawFile.GetData(), rawFile.Num()))
	{
		sw->InvalidateCompressedData();

		sw->RawData.Lock(LOCK_READ_WRITE);
		void* LockedData = sw->RawData.Realloc(rawFile.Num());
		FMemory::Memcpy(LockedData, rawFile.GetData(), rawFile.Num());
		sw->RawData.Unlock();

		int32 DurationDiv = *WaveInfo.pChannels * *WaveInfo.pBitsPerSample * *WaveInfo.pSamplesPerSec;
		if (DurationDiv)
		{
			sw->Duration = *WaveInfo.pWaveDataSize * 8.0f / DurationDiv;
		}
		else
		{
			sw->Duration = 0.0f;
		}

		sw->SetSampleRate(*WaveInfo.pSamplesPerSec);
		sw->NumChannels = *WaveInfo.pChannels;
		sw->RawPCMDataSize = WaveInfo.SampleDataSize;
		sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
	}
	else {
		return nullptr;
	}

	return sw;
}

bool UFlib_IO::ReadFile(const FString& FilePath, FString& ReturnString)
{
	FString Cache = "";
	bool Sucess = false;
	Sucess = FFileHelper::LoadFileToString(Cache, FilePath.GetCharArray().GetData());
	ReturnString = Cache;
	return Sucess;
}



bool UFlib_IO::RenameFile(const FString& FileName, const FString& NewName)
{
	if (DoesFileExist(FileName) && !NewName.IsEmpty())
	{
		rename(TCHAR_TO_UTF8(*FileName),TCHAR_TO_UTF8(*NewName));
		return true;
	}
	return false;
	
}

bool UFlib_IO::DoesFileExist(const FString& File)
{
	if (File.IsEmpty())
	{
		return false;
	}
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	return PlatformFile.FileExists(*File);
}

bool UFlib_IO::CreateDirectory(const FString& Directory)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	return PlatformFile.CreateDirectory(*Directory);

	
}

bool UFlib_IO::DoesDirectoryExist(const FString& Directory)
{
	if (Directory.IsEmpty())
	{
		return false;
	}
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	return PlatformFile.DirectoryExists(*Directory);
}

bool UFlib_IO::WriteFile(const FString& FilePath, const FString& FileString)
{
	if (FilePath.IsEmpty())
	{
		return false;
	}
	bool Sucess;
	Sucess = FFileHelper::SaveStringToFile(FileString, *FilePath);
	return Sucess;
}

bool UFlib_IO::DeleteFile(const FString& FilePath)
{
	if (FilePath.IsEmpty())
	{
		return false;
	}
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	

	if (PlatformFile.DeleteFile(*FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("deleteFile: Delete the flie successfully!"));

		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("deleteFile: Not delete the flie!"));

		return false;
	}
}

bool UFlib_IO::MoveFile(const FString& Source, const FString& Dest)
{
	if (Source.IsEmpty() || Dest.IsEmpty())
	{
		return false;
	}
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.MoveFile(*Dest,*Source);
}

bool UFlib_IO::DeleteFiles(const FString& FilePath)
{
	if (FilePath.IsEmpty())
	{
		return false;
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();



	if (PlatformFile.DeleteDirectoryRecursively(*FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("deleteFile: Delete the flie successfully!"));

		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("deleteFile: Not delete the flie!"));

		return false;
	}
}

bool UFlib_IO::CopyFile(const FString& FilePath, const FString& ToPath)
{
	if (FilePath.IsEmpty() || ToPath.IsEmpty())
	{
		return false;
	}
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.CopyFile(*ToPath, *FilePath);
}

TArray<FString> UFlib_IO::OpenWindowsFilesDialog(const FString& Path, const FString& fileName, const FString& SufStr)
{

	TArray<FString> OpenFileNames;//获取的文件绝对路径

	FString ExtensionStr = SufStr.IsEmpty() ? TEXT("*.*") : SufStr;//文件类型
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	DesktopPlatform->OpenFileDialog(nullptr, TEXT("打开文件"), Path, fileName, *SufStr, EFileDialogFlags::None, OpenFileNames);
	return OpenFileNames;
}

void UFlib_IO::OpenExe(const FString& Path, const FString& Args)
{

	FWindowsPlatformProcess::CreateProc(*Path, *Args, true, false, false, nullptr, 0, nullptr, nullptr);

	
}

EMsgRetType UFlib_IO::OpenMessageWindow(EMsgType Type, FText Msg)
{

	EAppMsgType::Type  tp = EAppMsgType::Ok;
	switch (Type)
	{
	case EMsgType::Ok:
		tp = EAppMsgType::Ok;
		break;
	case EMsgType::YesNo:
		tp = EAppMsgType::YesNo;
		break;
	case EMsgType::OkCancel:
		tp = EAppMsgType::OkCancel;
		break;
	case EMsgType::YesNoCancel:
		tp = EAppMsgType::YesNoCancel;
		break;
	case EMsgType::CancelRetryContinue:
		tp = EAppMsgType::CancelRetryContinue;
		break;
	case EMsgType::YesNoYesAllNoAll:
		tp = EAppMsgType::YesNoYesAllNoAll;
		break;
	case EMsgType::YesNoYesAllNoAllCancel:
		tp = EAppMsgType::YesNoYesAllNoAllCancel;
		break;
	case EMsgType::YesNoYesAll:
		tp = EAppMsgType::YesNoYesAll;
		break;
	}
	EAppReturnType::Type retType = FMessageDialog::Open(tp, Msg);
	EMsgRetType returnType = EMsgRetType::Ok;

	switch (retType)
	{
	case EAppReturnType::No:
		returnType = EMsgRetType::No;
		break;
	case EAppReturnType::Yes:
		returnType = EMsgRetType::Yes;
		break;
	case EAppReturnType::YesAll:
		returnType = EMsgRetType::YesAll;
		break;
	case EAppReturnType::NoAll:
		returnType = EMsgRetType::NoAll;
		break;
	case EAppReturnType::Cancel:
		returnType = EMsgRetType::Cancel;
		break;
	case EAppReturnType::Ok:
		returnType = EMsgRetType::Ok;
		break;
	case EAppReturnType::Retry:
		returnType = EMsgRetType::Retry;
		break;
	case EAppReturnType::Continue:
		returnType = EMsgRetType::Continue;
		break;
	}
	return returnType;
}

void UFlib_IO::GenericUStructToJsonObjectString(const UStruct* StructDefinition, const void* Struct, FString& OutJsonString, int64 CheckFlags, int64 SkipFlags)
{
	FJsonObjectConverter::UStructToJsonObjectString(StructDefinition, Struct, OutJsonString, CheckFlags, SkipFlags);
}

FString UFlib_IO::HandleData_String(const FString& FilePath, EMatPropertyKeyType Key, int32 Index, EAssimpTextureType Type, int32 N)
{
	FMatPropertyRetValue out = LoadMeshMaterialProperty(FilePath, Key, Index, Type, N);
	return out.RetString;
}


int32 UFlib_IO::HandleData_Int(const FString& FilePath, EMatPropertyKeyType Key, int32 Index, EAssimpTextureType Type, int32 N)
{
	FMatPropertyRetValue out = LoadMeshMaterialProperty(FilePath, Key, Index, Type, N);
	return out.RetInt;
}

float UFlib_IO::HandleData_Float(const FString& FilePath, EMatPropertyKeyType Key, int32 Index, EAssimpTextureType Type, int32 N)
{
	FMatPropertyRetValue out = LoadMeshMaterialProperty(FilePath, Key, Index, Type, N);
	return out.RetFloat;
}

FVector UFlib_IO::HandleData_Vector(const FString& FilePath, EMatPropertyKeyType Key, int32 Index, EAssimpTextureType Type, int32 N)
{
	FMatPropertyRetValue out = LoadMeshMaterialProperty(FilePath, Key, Index, Type, N);
	return out.RetVector;
}

FLinearColor UFlib_IO::HandleData_Color(const FString& FilePath, EMatPropertyKeyType Key, int32 Index, EAssimpTextureType Type, int32 N)
{
	FMatPropertyRetValue out = LoadMeshMaterialProperty(FilePath, Key, Index, Type, N);
	return out.RetColor;
}

FString UFlib_IO::OpenWindowsDirectory(const FString& Path)
{
	FString oldPath;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	DesktopPlatform->OpenDirectoryDialog(nullptr, TEXT("打开路径"), Path, oldPath);
	return oldPath;
}

void UFlib_IO::OpenWindowsFolder(const FString& AbsolutePath)
{
	FWindowsPlatformProcess::ExploreFolder(*AbsolutePath);
}
