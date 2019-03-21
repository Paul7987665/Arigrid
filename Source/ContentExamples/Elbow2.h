// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "MyStaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elbow2.generated.h"

UCLASS()
class CONTENTEXAMPLES_API AElbow2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElbow2();
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UStaticMeshComponent* Part1;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UStaticMeshComponent* Part2;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UStaticMeshComponent* Circle;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate1;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate2;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate3;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate4;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate5;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate6;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate7;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate8;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate9;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate10;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate11;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate12;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate13;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate14;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate15;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate16;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate17;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate18;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate19;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate20;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate21;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate22;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate23;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate24;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))	 class UMaterialInstance* Blue;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))	 class UMaterial* Yellow;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class USphereComponent* SphereOverlap1;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class USphereComponent* SphereOverlap2;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UPhysicsConstraintComponent* Constraint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/*The Texture of the item in case we want to add it in the secrets or inventory*/
	UPROPERTY(EditAnywhere, Category = "PickupProperties")
		UTexture2D* PickupTexture;
	/*The name of the item*/
	UPROPERTY(EditAnywhere, Category = "PickupProperties")
		FString ItemName = "Elbow_1x1x1";

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<UPhysicsConstraintComponent*> DynamicConstraints;
	void ClearMemory();
	void MakeDynamicConnection(AActor * OtherConstraintObject, UMyStaticMeshComponent* LastItemSeen);
	TArray<AActor*> GetOterConstraintActors();
	void Turn();
};
