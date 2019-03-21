// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BladeMotor.generated.h"

UCLASS()
class CONTENTEXAMPLES_API ABladeMotor : public AActor
{
	GENERATED_BODY()
private:
	void MakeConnection(AActor* OtherConstraintObject, UPhysicsConstraintComponent* ConstraintComp);
public:	
	// Sets default values for this actor's properties
	ABladeMotor();
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UStaticMeshComponent* BoxMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UStaticMeshComponent* Blade;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))	 class UMaterialInstance* Blue;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))	 class UMaterial* Yellow;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate1;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate2;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate3;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate4;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate5;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UStaticMeshComponent* Circle;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UPhysicsConstraintComponent* Constraint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class USphereComponent* SphereOverlap;
	UPROPERTY(EditAnywhere, Category = "PickupProperties") float Speed = 100;
	UPROPERTY(EditAnywhere, Category = "PickupProperties") bool Status = false;
	UPROPERTY(EditAnywhere, Category = "PickupProperties") float ForceModul = 1000000;
	//********TMap<typename Key, typename value>**********
	TArray<UMyStaticMeshComponent*> Plates;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "PickupProperties")	UTexture2D* PickupTexture;
	/*The name of the item*/
	UPROPERTY(EditAnywhere, Category = "PickupProperties")	FString ItemName = "Box_1x1x1";

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SwitchRotation();
	TArray<UPhysicsConstraintComponent*> DynamicConstraints;
	void ClearMemory();
	void MakeDynamicConnection(AActor * OtherConstraintObject);
	TArray<AActor*> GetOterConstraintActors();
};
