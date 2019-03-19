// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Piston.h"
#include "Elbow2.h"
#include "BaseActor.h"
#include "BladeMotor2.h"
#include "Elbow.h"
#include "BladeMotor.h"
#include "MyStaticMeshComponent.h"
#include "MyActorWithDinamicConstrain.h"
#include "Math/Matrix.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "BladeMotor.h"
#include "MyActor.h"
#include "Engine/StaticMeshActor.h"
#include "DPMainComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CONTENTEXAMPLES_API UDPMainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDPMainComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	/*Raycasts in front of the character to find usable items*/
	void Raycast();
	float Reach = 1000.f;
	
	AActor* Owner;
	UMyStaticMeshComponent* LastItemSeen;
	UBoxComponent* LastBoxSeen;

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	enum class Items
	{
		Box1,
		Box2,
		Triangle,
		BladeMotor1,
		BladeMotor2,	   
		Elbow1,
		Elbow2,	
		Piston,		   		   
		MaxItems		   
	};

	enum class ConnectionType
	{
		Center,
		UpDown,
		LeftRight
	};

	//enum class TriangleRotation
	//{
	//
	//};
	
	void SetStatus();
public:
	void TurnElbow();
	void WrteElbowRotation();
	/*The range of the raycast*/
	UPROPERTY(EditAnywhere)	float RaycastRange = 1000.f;
	/*Handles the Pickup Input*/
	UFUNCTION()	void PickupItem();
	UFUNCTION()	void Spawn();
	void Spawn(AMyActorWithDinamicConstrain* SpawnedActor);
	void Spawn(AElbow* SpawnedActor);
	void Spawn(ABladeMotor* SpawnedActor);
	void Spawn(ABladeMotor2* SpawnedActor);
	void Spawn(ABaseActor* SpawnedActor);
	void Spawn(AMyActor* SpawnedActor);
	void Spawn(AElbow2* SpawnedActor);
	void Spawn(APiston* SpawnedActor);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	int Iter = 0;
	int Iter2 = 0;
	UPROPERTY(EditAnywhere) float AngleTurn = 45;
	AActor* SpawnedActor;
	void ShotPiston();
	void ChoseTypeOfConnection();
};
