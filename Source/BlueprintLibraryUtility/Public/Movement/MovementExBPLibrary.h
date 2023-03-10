// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Movement\Proxy\ActorMoveProxy.h"
#include "Movement\Proxy\MoveProxy.h"
#include "Movement\Proxy\PathMoveProxy.h"
#include "Movement\Proxy\RotateProxy.h"
#include "Proxy/RotationAndMoveProxy.h"
#include "MovementExBPLibrary.generated.h"


UENUM(BlueprintType)
enum EMotionInterpType
{
	Auto,
	BaseOnTranslation,
	BaseOnRotation,
};




UCLASS()
class BLUEPRINTLIBRARYUTILITY_API UFlibMovementEx : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()


	/*
	* AI MoveTo 函数版, 回调事件通过动态代理返回
	* * Target优先于Des
	*/
	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static bool V_AIMoveToDelegate(APawn* Pawn, FVector Des, AActor* Target, const FMoveSimpleDelegate& MoveEndEvent, float AcceptanceRadius = 5.0, bool StopOnOverlap = false);
	/*
	* AI MoveTo 函数版, 回调事件通过绑定返回的Obj手动绑定
	* * Target优先于Des
	*/
	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static UMoveProxy* V_AIMoveTo(APawn* Pawn, FVector Des, AActor* Target, float AcceptanceRadius = 5.0, bool StopOnOverlap = false);

	/*
	* 路径版本AI MoveTo, 回调事件通过动态代理返回
	* * Target优先于Des
	*/
	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static bool V_AIPathMoveToDelegate(APawn* Pawn, TArray<FVector> DesPath, const FPathMoveDelegate& MoveEndEvent, const FPathMoveDelegate2& MoveOneStepEvent, float AcceptanceRadius = 5.0, bool StopOnOverlap = false);

	/*
	* 路径点集 MoveTo, 回调事件通过动态代理返回
	* Target优先于Des
	*/
	UFUNCTION(BlueprintCallable, Category = "MovementEx")
		static UPathMoveProxy* V_AIPathMoveTo(APawn* Pawn, TArray<FVector> DesPath, float AcceptanceRadius = 5.0, bool StopOnOverlap = false);

	
	/*
	* Actor版本的 MoveTo, 回调事件通过绑定返回的Obj手动绑定
	* Target优先于Des
	* TurnSpeed: 转身速度,<=0 意味着不会转身
	*/
	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static	UActorMoveProxy* V_ActorSimpleMoveTo(AActor* MoveActor, FVector Des, AActor* Target, float LinearSpeed = 300, float TurnSpeed = 90, float AcceptanceRadius = 5.0, bool bTurnPitch = true, bool bTurnYaw = true, bool bTurnRoll = true);

	/*
	* Actor版本的 MoveTo, 回调事件通过动态代理返回
	* Target优先于Des
	* TurnSpeed: 转身速度,<=0 意味着不会转身
	*/
	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 6))
		static	bool V_ActorSimpleMoveToDelegate(AActor* MoveActor, FVector Des, AActor* Target, const FActorSimpleMoveDlg& MoveEvent, float LinearSpeed = 300, float TurnSpeed = 90, float AcceptanceRadius = 5.0, bool bTurnPitch = true, bool bTurnYaw = true, bool bTurnRoll = true);

	/*
	* Actor版本的 MoveTo, 回调事件通过绑定返回的Obj手动绑定
	* Target优先于Des
	* 如果传入有效的Target类,那么DynamicTargetDetectInterval参数将生效
	* DynamicTargetDetectInterval:动态检测间隔
	* TurnSpeed: 转身速度,<=0 意味着不会转身
	*/
	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static	UActorMoveProxy* V_ActorNavMoveTo(AActor* MoveActor, FVector Des, AActor* Target, float LinearSpeed = 300, float TurnSpeed = 90, float AcceptanceRadius = 5.0, float DynamicTargetDetectInterval = 2.0f, bool bDebug = false, bool bTurnPitch = true, bool bTurnYaw = true, bool bTurnRoll = true, bool bKeepLanded = true);


	/*
	* 旋转actor, 回调事件通过绑定返回的Obj手动绑定
	* Speed: 旋转速度,<=0 不可以小于等于0
	* bIsAdditive: 是否是增加多少旋转值
	*/
	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static URotateProxy* V_ActorRotateBySpeed(AActor* RotateActor, float roll, float pitch, float yaw, float Speed = 90, bool bIsAdditive = false);


	/*
	* 旋转actor代理版本,  回调事件通过动态代理返回
	* Speed: 旋转速度,<=0 不可以小于等于0
	* bIsAdditive: 是否是增加多少旋转值
	*/
	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static bool V_ActorRotateBySpeedDelegate(AActor* RotateActor, float roll, float pitch, float yaw, const FActorSimpleRotateDlg& RotationEndEvent, float Speed = 90, bool bIsAdditive = false);


	/*
	* 旋转actor, 回调事件通过绑定返回的Obj手动绑定
	* Time: 旋转时间, 不可以小于等于0
	* bIsAdditive: 是否是增加多少旋转值
	*/
	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static URotateProxy* V_ActorRotateByTime(AActor* RotateActor, float roll, float pitch, float yaw, float Time = 1, bool bIsAdditive = false);



	/*
	* 旋转actor, 回调事件通过绑定返回的Obj手动绑定
	* Time: 旋转时间, 不可以小于等于0
	* bIsAdditive: 是否是增加多少旋转值
	*/
	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static bool V_ActorRotateByTimeDelegate(AActor* RotateActor, float roll, float pitch, float yaw, const FActorSimpleRotateDlg& RotationEndEvent, float Time = 1, bool bIsAdditive = false);





	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static UMoveAndRotateProxy* V_ActorMoveAndRotateBySpeed(AActor* Target, FVector Destination, bool bUseOffset, float MoveSpeed, 
			float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f, float RotateSpeed = 90.f, bool bRotatorIsAdditive = false);

	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static bool V_ActorMoveAndRotateBySpeedDelegate(AActor* Target, const FActorMoveAndRotateDlg& EndEvent,  FVector Destination, bool bUseOffset, float MoveSpeed,
			float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f, float RotateSpeed = 90.f, bool bRotatorIsAdditive = false);

	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static UMoveAndRotateProxy* V_ActorMoveAndRotateByTime(AActor* Target, FVector Destination, bool bUseOffset, float Duration, 
			float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f, bool bRotatorIsAdditive = false);

	UFUNCTION(BlueprintCallable, Category = "MovementEx", meta = (AdvancedDisplay = 5))
		static bool V_ActorMoveAndRotateByTimeDelegate(AActor* Target, const FActorMoveAndRotateDlg& EndEvent, FVector Destination, bool bUseOffset, float Duration,
			float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f, bool bRotatorIsAdditive = false);
};
