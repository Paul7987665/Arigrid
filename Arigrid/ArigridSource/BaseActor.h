// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.generated.h"

UCLASS()
class CONTENTEXAMPLES_API ABaseActor : public AActor
{
	GENERATED_BODY()
private:
	void MakeConnection(AActor* OtherConstraintObject, UPhysicsConstraintComponent* ConstraintComp);
public:	
	// Sets default values for this actor's properties
	ABaseActor();
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class USphereComponent* SphereOverlap;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UStaticMeshComponent* BoxMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))	 class UMaterialInstance* Red;
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
	//********TArray<class MyStaticMeshComponent>**********
	TArray<UMyStaticMeshComponent*> Plates;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<UPhysicsConstraintComponent*> DynamicConstraints;
	void ClearMemory();
	void MakeDynamicConnection(AActor * OtherConstraintObject);
	TArray<AActor*> GetOterConstraintActors();
};
