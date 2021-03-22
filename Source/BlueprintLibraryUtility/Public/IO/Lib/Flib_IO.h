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
	//它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化。
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool OptimizeMeshes = true;
	//沿着y方向翻转纹理坐标，这在渲染quake模型时候是必须的
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FlipUVsv;
	//将不同图元放置到不同的模型中去，图片类型可能是点、直线、三角形等
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool SortByPType;
	//自动合并相同的顶点 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool JoinIdenticalVertices = true;
	//如果原始顶点没有法向数据，Assimp会为顶点产生法向数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool GenSmoothNormals = true;
	//自动计算切线和副法线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CalcTangentSpace = true;
	//自动将四边形面转换为三角面
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Triangulate = true;
	//使用左手坐标系,比如DX; 而3dmax和OpenGL是右手坐标系
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
