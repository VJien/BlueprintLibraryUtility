// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProceduralMeshComponent.h"
#include "assimp/scene.h"
#include "IOTypes.generated.h"

UENUM(BlueprintType)
enum class EMsgType : uint8
{
	Ok,
	YesNo,
	OkCancel,
	YesNoCancel,
	CancelRetryContinue,
	YesNoYesAllNoAll,
	YesNoYesAllNoAllCancel,
	YesNoYesAll,

};
UENUM(BlueprintType)
enum class EMsgRetType : uint8
{
	No,
	Yes,
	YesAll,
	NoAll,
	Cancel,
	Ok,
	Retry,
	Continue,

};


//return value type
UENUM(BlueprintType)
enum class EReturnType :uint8
{
	None = 0,
	Int,
	Float,
	String,
	Vector,
	Color
};

UENUM(BlueprintType)
enum class EFilePathType : uint8
{
	Absolute,
	Content,
	Saved,
	Root
};



UENUM(BlueprintType)
enum class EMatPropertyKeyType : uint8
{
	None,
	//The name of the material, if available.
	NAME,
	//Diffuse color of the material. This is typically scaled by the amount of incoming diffuse light (e.g. using gouraud shading)
	COLOR_DIFFUSE,
	//Specular color of the material. This is typically scaled by the amount of incoming specular light (e.g. using phong shading)
	COLOR_SPECULAR,
	//Ambient color of the material. This is typically scaled by the amount of ambient light
	COLOR_AMBIENT,
	//Emissive color of the material. This is the amount of light emitted by the object. In real time applications it will usually not affect surrounding objects, but raytracing applications may wish to treat emissive objects as light sources.
	COLOR_EMISSIVE,
	//Defines the transparent color of the material, this is the color to be multiplied with the color of translucent light to construct the final ‘destination color’ for a particular position in the screen buffer.
	COLOR_TRANSPARENT,
	//Defines the reflective color of the material. This is typically scaled by the amount of incoming light from the direction of mirror reflection. Usually combined with an environment lightmap of some kind for real-time applications.
	COLOR_REFLECTIVE,
	//Scales the reflective color of the material.
	REFLECTIVITY,
	//Specifies whether wireframe rendering must be turned on for the material. 0 for false, !0 for true.
	WIREFRAME,
	//Specifies whether meshes using this material must be rendered without backface culling. 0 for false, !0 for true.
	TWOSIDED,
	//One of the #aiShadingMode enumerated values. Defines the library shading model to use for (real time) rendering to approximate the original look of the material as closely as possible.
	SHADING_MODEL,
	//One of the #aiBlendMode enumerated values. Defines how the final color value in the screen buffer is computed from the given color at that position and the newly computed color from the material. Simply said, alpha blending settings.
	BLEND_FUNC,
	//Defines the opacity of the material in a range between 0..1.
	OPACITY,
	//Defines the shininess of a phong-shaded material. This is actually the exponent of the phong specular equation
	SHININESS,
	//Scales the specular color of the material.
	SHININESS_STRENGTH,
	//Defines the Index Of Refraction for the material. That’s not supported by most file formats.
	REFRACTI,
	//Defines the path of the n’th texture on the stack ‘t’, where ‘n’ is any value >= 0 and ‘t’ is one of the #aiTextureType enumerated values. A file path to an external file or an embedded texture. Use aiScene::GetEmbeddedTexture to test if it is embedded for FBX files, in other cases embedded textures start with ‘*’ followed by an index into aiScene::mTextures.</td>
	TEXTURE,
	//Defines the strength the n’th texture on the stack ‘t’. All color components (rgb) are multiplied with this factor before any further processing is done.
	TEXBLEND,
	//One of the #aiTextureOp enumerated values. Defines the arithmetic operation to be used to combine the n’th texture on the stack ‘t’ with the n-1’th. <tt>TEXOP(t,0)</tt> refers to the blend operation between the base color for this stack (e.g. <tt>COLOR_DIFFUSE</tt> for the diffuse stack) and the first texture.
	TEXOP,
	//Defines how the input mapping coordinates for sampling the n’th texture on the stack ‘t’ are computed. Usually explicit UV coordinates are provided, but some model file formats might also be using basic shapes, such as spheres or cylinders, to project textures onto meshes.
	MAPPING,
	//Defines the UV channel to be used as input mapping coordinates for sampling the n’th texture on the stack ‘t’. All meshes assigned to this material share the same UV channel setup
	UVWSRC,
	//Any of the #aiTextureMapMode enumerated values. Defines the texture wrapping mode on the x axis for sampling the n’th texture on the stack ‘t’. ‘Wrapping’ occurs whenever UVs lie outside the 0..1 range.
	MAPPINGMODE_U,
	//Wrap mode on the v axis. See MAPPINGMODE_U.
	MAPPINGMODE_V,
	//Defines the base axis to to compute the mapping coordinates for the n’th texture on the stack ‘t’ from. This is not required for UV-mapped textures. For instance, if <tt>MAPPING(t,n)</tt> is #aiTextureMapping_SPHERE, U and V would map to longitude and latitude of a sphere around the given axis. The axis is given in local mesh space.
	TEXMAP_AXIS,
	//Defines miscellaneous flag for the n’th texture on the stack ‘t’. This is a bitwise combination of the #aiTextureFlags enumerated values.
	TEXFLAGS




};
UENUM(BlueprintType)
enum class ETextureType : uint8
	{
		NONE = 0x0,
		/** The texture is combined with the result of the diffuse
		 *  lighting equation.
		 */
		DIFFUSE = 0x1,

		 /** The texture is combined with the result of the specular
		  *  lighting equation.
		  */
		 SPECULAR = 0x2,

		  /** The texture is combined with the result of the ambient
		   *  lighting equation.
		   */
		  AMBIENT = 0x3,

		   /** The texture is added to the result of the lighting
			*  calculation. It isn't influenced by incoming light.
			*/
			aiEMISSIVE = 0x4,

			/** The texture is a height map.
			 *
			 *  By convention, higher gray-scale values stand for
			 *  higher elevations from the base height.
			 */
			HEIGHT = 0x5,

			 /** The texture is a (tangent space) normal-map.
			  *
			  *  Again, there are several conventions for tangent-space
			  *  normal maps. Assimp does (intentionally) not
			  *  distinguish here.
			  */
			 NORMALS = 0x6,

			  /** The texture defines the glossiness of the material.
			   *
			   *  The glossiness is in fact the exponent of the specular
			   *  (phong) lighting equation. Usually there is a conversion
			   *  function defined to map the linear color values in the
			   *  texture to a suitable exponent. Have fun.
			  */
			 SHININESS = 0x7,

			  /** The texture defines per-pixel opacity.
			   *
			   *  Usually 'white' means opaque and 'black' means
			   *  'transparency'. Or quite the opposite. Have fun.
			  */
			 OPACITY = 0x8,

			  /** Displacement texture
			   *
			   *  The exact purpose and format is application-dependent.
			   *  Higher color values stand for higher vertex displacements.
			  */
			 DISPLACEMENT = 0x9,

			  /** Lightmap texture (aka Ambient Occlusion)
			   *
			   *  Both 'Lightmaps' and dedicated 'ambient occlusion maps' are
			   *  covered by this material property. The texture contains a
			   *  scaling value for the final color value of a pixel. Its
			   *  intensity is not affected by incoming light.
			  */
			 LIGHTMAP = 0xA,

			  /** Reflection texture
			   *
			   * Contains the color of a perfect mirror reflection.
			   * Rarely used, almost never for real-time applications.
			  */
			 REFLECTION = 0xB,

			  /** Unknown texture
			   *
			   *  A texture reference that does not match any of the definitions
			   *  above is considered to be 'unknown'. It is still imported,
			   *  but is excluded from any further postprocessing.
			  */
			 UNKNOWN = 0xC,


};


//template return value, only one value is usefull
USTRUCT(BlueprintType)
struct FMatPropertyRetValue
{
	GENERATED_BODY()
		FMatPropertyRetValue()
		:RetString(TEXT("")), RetInt(-1),RetFloat(0.f),RetColor(FLinearColor::Black),RetVector(FVector::ZeroVector)
	{}
	FMatPropertyRetValue(aiColor3D aiColor):RetColor(aiColor.r, aiColor.g, aiColor.b)
	{}
	FMatPropertyRetValue(aiString aiStr):RetString(aiStr.C_Str())
	{}
	FMatPropertyRetValue(aiVector3D aiv3):RetVector(aiv3.x,aiv3.y,aiv3.z)
	{}
	FMatPropertyRetValue(float aiFloat) :RetFloat(aiFloat)
	{}
	FMatPropertyRetValue(int aiInt):RetInt(aiInt)
	{}

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MatProperty")
		FString RetString;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MatProperty")
		int32 RetInt;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MatProperty")
		float RetFloat;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MatProperty")
		FLinearColor RetColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MatProperty")
		FVector RetVector;
};





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






USTRUCT(BlueprintType)
struct FMeshData
{
	GENERATED_BODY()
		FMeshData()
		{}
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MeshData")
		TArray<FVector> Vertices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MeshData")
		TArray<int32> Triangles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MeshData")
		TArray<FVector> Normals;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MeshData")
		TArray<FVector2D> UV0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MeshData")
		TArray<FLinearColor> VertexColors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MeshData")
		TArray<FProcMeshTangent> Tangents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MeshData")
		FTransform RelativeTransform;
};

USTRUCT(BlueprintType)
struct FLoadedMeshData
{
	GENERATED_BODY()
		FLoadedMeshData():bSuccess(false), MeshNumbers(0)
		{}
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MeshData")
		bool bSuccess;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MeshData")
		int32 MeshNumbers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MeshData")
		TArray<FMeshData> meshData;

};