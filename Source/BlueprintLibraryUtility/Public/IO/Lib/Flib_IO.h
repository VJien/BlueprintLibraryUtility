// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralMeshComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IO/Types/IOTypes.h"
#include "Flib_IO.generated.h"
//
//UENUM(BlueprintType)
//enum class EPathType : uint8
//{
//	Absolute,
//	Relative
//};
//
//
//USTRUCT(BlueprintType)
//struct FMeshInfo
//{
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
//		TArray<FVector> Vertices;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
//		TArray<int32> Triangles;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
//		TArray<FVector> Normals;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
//		TArray<FVector2D> UV0;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
//		TArray<FLinearColor> VertexColors;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
//		TArray<FProcMeshTangent> Tangents;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
//		FTransform RelativeTransform;
//};
//
//USTRUCT(BlueprintType)
//struct FReturnedData
//{
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
//		bool bSuccess;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
//		int32 NumMeshes;
//
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
//		TArray<FMeshInfo> meshData;
//
//
//};


USTRUCT(BlueprintType)
struct FLoadConfig
{
	GENERATED_BODY()
public:
	//���Ὣ���С����ƴ��Ϊһ��������񣬼��ٻ��Ƶ��ôӶ������Ż���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool OptimizeMeshes = true;
	//����y����ת�������꣬������Ⱦquakeģ��ʱ���Ǳ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FlipUVsv;
	//����ͬͼԪ���õ���ͬ��ģ����ȥ��ͼƬ���Ϳ����ǵ㡢ֱ�ߡ������ε�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool SortByPType;
	//�Զ��ϲ���ͬ�Ķ��� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool JoinIdenticalVertices = true;
	//���ԭʼ����û�з������ݣ�Assimp��Ϊ���������������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool GenSmoothNormals = true;
	//�Զ��������ߺ͸�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CalcTangentSpace = true;
	//�Զ����ı�����ת��Ϊ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Triangulate = true;
	//ʹ����������ϵ,����DX; ��3dmax��OpenGL����������ϵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool MakeLeftHanded;

};


/**
 * 
 */
UCLASS()
class BLUEPRINTLIBRARYUTILITY_API UFlib_IO : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:


	UFUNCTION(BlueprintCallable, Category = "BlueprintLibraryUtility|IO")
		static FLoadedMeshData LoadMeshData(FString filepath, FLoadConfig Config);

	static FMatPropertyRetValue LoadMeshMaterialProperty(const FString& FilePath, EMatPropertyKeyType key, unsigned int matIdx, ETextureType type, int32 number);

};
