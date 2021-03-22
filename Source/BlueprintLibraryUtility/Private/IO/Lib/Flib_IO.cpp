// Fill out your copyright notice in the Description page of Project Settings.
#include "IO/Lib/Flib_IO.h"
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

//void FindMeshInfo(const aiScene* scene, aiNode* node, FReturnedData& result)
//{
//
//	for (uint32 i = 0; i < node->mNumMeshes; i++)
//	{
//		std::string TestString = node->mName.C_Str();
//		FString Fs = FString(TestString.c_str());
//		UE_LOG(LogTemp, Warning, TEXT("FindMeshInfo. %s\n"), *Fs);
//		int meshidx = *node->mMeshes;
//		aiMesh *mesh = scene->mMeshes[meshidx];
//		FMeshInfo &mi = result.meshData[meshidx];
//
//		//transform.
//		aiMatrix4x4 tempTrans = node->mTransformation;
//		FMatrix tempMatrix;
//		tempMatrix.M[0][0] = tempTrans.a1; tempMatrix.M[0][1] = tempTrans.b1; tempMatrix.M[0][2] = tempTrans.c1; tempMatrix.M[0][3] = tempTrans.d1;
//		tempMatrix.M[1][0] = tempTrans.a2; tempMatrix.M[1][1] = tempTrans.b2; tempMatrix.M[1][2] = tempTrans.c2; tempMatrix.M[1][3] = tempTrans.d2;
//		tempMatrix.M[2][0] = tempTrans.a3; tempMatrix.M[2][1] = tempTrans.b3; tempMatrix.M[2][2] = tempTrans.c3; tempMatrix.M[2][3] = tempTrans.d3;
//		tempMatrix.M[3][0] = tempTrans.a4; tempMatrix.M[3][1] = tempTrans.b4; tempMatrix.M[3][2] = tempTrans.c4; tempMatrix.M[3][3] = tempTrans.d4;
//		mi.RelativeTransform = FTransform(tempMatrix);
//
//		//vet
//		for (uint32 j = 0; j < mesh->mNumVertices; ++j)
//		{
//			FVector vertex = FVector(
//				mesh->mVertices[j].x,
//				mesh->mVertices[j].y,
//				mesh->mVertices[j].z);
//
//			vertex = mi.RelativeTransform.TransformFVector4(vertex);
//			mi.Vertices.Push(vertex);
//
//			//Normal
//			if (mesh->HasNormals())
//			{
//				FVector normal = FVector(
//					mesh->mNormals[j].x,
//					mesh->mNormals[j].y,
//					mesh->mNormals[j].z);
//
//				//normal = mi.RelativeTransform.TransformFVector4(normal);
//				mi.Normals.Push(normal);
//			}
//			else
//			{
//				mi.Normals.Push(FVector::ZeroVector);
//			}
//
//			//UV Coordinates - inconsistent coordinates
//			if (mesh->HasTextureCoords(0))
//			{
//				FVector2D uv = FVector2D(mesh->mTextureCoords[0][j].x, -mesh->mTextureCoords[0][j].y);
//				mi.UV0.Add(uv);
//			}
//
//			//Tangent
//			if (mesh->HasTangentsAndBitangents())
//			{
//				FProcMeshTangent meshTangent = FProcMeshTangent(
//					mesh->mTangents[j].x,
//					mesh->mTangents[j].y,
//					mesh->mTangents[j].z
//				);
//				mi.Tangents.Push(meshTangent);
//			}
//
//			//Vertex color
//			if (mesh->HasVertexColors(0))
//			{
//				FLinearColor color = FLinearColor(
//					mesh->mColors[0][j].r,
//					mesh->mColors[0][j].g,
//					mesh->mColors[0][j].b,
//					mesh->mColors[0][j].a
//				);
//				mi.VertexColors.Push(color);
//			}
//
//		}
//	}
//}
//
//
//void FindMesh(const aiScene* scene, aiNode* node, FReturnedData& retdata)
//{
//	FindMeshInfo(scene, node, retdata);
//
//	for (uint32 m = 0; m < node->mNumChildren; ++m)
//	{
//		FindMesh(scene, node->mChildren[m], retdata);
//	}
//}
//
//
//
//FReturnedData UFlib_IO::LoadMesh(FString filepath, EPathType type)
//{
//	FReturnedData result;
//	result.bSuccess = false;
//	result.meshData.Empty();
//	result.NumMeshes = 0;
//	//bEnableCollision:: StaticMeshSection
//	//result.meshData.Vertices.Empty();
//	//result.meshData.Triangles.Empty();
//	//result.meshData.Normals.Empty();
//	//result.meshData.UV0.Empty();
//	//result.meshData.VertexColors.Empty();
//	//result.meshData.Tangents.Empty();
//
//	if (filepath.IsEmpty())
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Runtime Mesh Loader: filepath is empty.\n"));
//		return result;
//	}
//
//	std::string file;
//	switch (type)
//	{
//	case EPathType::Absolute:
//		file = TCHAR_TO_UTF8(*filepath);
//		break;
//	case EPathType::Relative:
//		file = TCHAR_TO_UTF8(*FPaths::Combine(FPaths::ProjectContentDir(), filepath));
//		break;
//	}
//
//	Assimp::Importer mImporter;
//
//	const aiScene* mScenePtr = mImporter.ReadFile(file, aiProcess_Triangulate | aiProcess_MakeLeftHanded | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes);
//
//	if (mScenePtr == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Runtime Mesh Loader: Read mesh file failure.\n"));
//		return result;
//	}
//
//	if (mScenePtr->HasMeshes())
//	{
//		result.meshData.SetNum(mScenePtr->mNumMeshes, false);
//
//		FindMesh(mScenePtr, mScenePtr->mRootNode, result);
//
//		for (uint32 i = 0; i < mScenePtr->mNumMeshes; ++i)
//		{
//			//Triangle number
//			for (uint32 l = 0; l < mScenePtr->mMeshes[i]->mNumFaces; ++l)
//			{
//				for (uint32 m = 0; m < mScenePtr->mMeshes[i]->mFaces[l].mNumIndices; ++m)
//				{
//					result.meshData[i].Triangles.Push(mScenePtr->mMeshes[i]->mFaces[l].mIndices[m]);
//				}
//			}
//		}
//
//		result.bSuccess = true;
//	}
//
//	return result;
//}
//


template<typename TEnum>
static FString EnumToString(const FString& EnumName, TEnum Enum, bool bShortName)
{
	const UEnum* ep = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (ep)
	{
		if (bShortName)
		{
			FString tempStr = ep->GetNameByValue((int)Enum).ToString();
			TArray<FString> arr = UKismetStringLibrary::ParseIntoArray(tempStr, TEXT("::"));
			if (arr.Num() > 0)
			{
				return arr.Last();
			}
		}

		return ep->GetNameByValue((int)Enum).ToString();


	}
	return TEXT("");
}

template<typename TEnum>
static TEnum StringToEnum(const FString& EnumName, FString EnumValue)
{
	const UEnum* ep = FindObject<UEnum>(ANY_PACKAGE, *EnumName, 1);
	if (!ep)
	{
		return TEnum(0);
	}
	return (TEnum)ep->GetIndexByName(FName(*FString(EnumValue)));
}


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

FMatPropertyRetValue UFlib_IO::LoadMeshMaterialProperty(const FString& FilePath, EMatPropertyKeyType key, unsigned int matIdx, ETextureType type, int32 number)
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
