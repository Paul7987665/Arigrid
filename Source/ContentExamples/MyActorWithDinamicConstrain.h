// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyStaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorWithDinamicConstrain.generated.h"

UCLASS()
class CONTENTEXAMPLES_API AMyActorWithDinamicConstrain : public AActor
{
	GENERATED_BODY()
private:
		void MakeConnection(AActor* OtherConstraintObject, UPhysicsConstraintComponent* ConstraintComp);
public:	
	// Sets default values for this actor's properties
	AMyActorWithDinamicConstrain();
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UStaticMeshComponent* BoxMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))	 class UMaterialInstance* Red;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate1;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate2;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class USphereComponent* SphereOverlap;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate3;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate4;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate5;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))  class UMyStaticMeshComponent* Plate6;
	//********TArray<class MyStaticMeshComponent>**********
	TArray<UMyStaticMeshComponent*> Plates;
	TArray<UPhysicsConstraintComponent*> DynamicConstraints;
	void ClearMemory();
	void MakeDynamicConnection(AActor * OtherConstraintObject);
	TArray<AActor*> GetOterConstraintActors();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/*The Texture of the item in case we want to add it in the secrets or inventory*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupProperties")
		UTexture2D* PickupTexture;
	/*The name of the item*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupProperties")
		FString ItemName = "Box_1x1x1";
};
