// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseActor.h"
#include "Elbow.h"
#include "Components/SphereComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "MyStaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


// Sets default values
ABaseActor::ABaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BoxMeshAsset(TEXT("StaticMesh'/Game/Arigrid/Box.Box'"));
	if (BoxMeshAsset.Succeeded())
	{
		BoxMesh->SetStaticMesh(BoxMeshAsset.Object);
		BoxMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		SetRootComponent(BoxMesh);
		BoxMesh->SetSimulatePhysics(true);
		BoxMesh->SetMobility(EComponentMobility::Movable);
		BoxMesh->SetWorldScale3D(FVector(2.f, 2.f, 2.f));
	}

	Red = CreateDefaultSubobject<UMaterialInstance>(TEXT("Red"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial(TEXT("MaterialInstanceConstant'/Game/Arigrid/Red.Red'"));
	if (FoundMaterial.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("!!!"));
		Red = FoundMaterial.Object;
		BoxMesh->SetMaterial(0, Red);
	}

	SphereOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("SphereOverlap"));
	SphereOverlap->SetupAttachment(RootComponent);
	SphereOverlap->SetNotifyRigidBodyCollision(true);
	SphereOverlap->SetHiddenInGame(true);
	SphereOverlap->SetCollisionProfileName("Trigger");
	SphereOverlap->SetSphereRadius(55.f);

	Plate1 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate1"));
	Plate1->SetRelativeLocationAndRotation(FVector(49.500099f, 25.0f, 25.0f), FRotator(-90.0, 0.0, 0.0));
	Plates.Emplace(Plate1);

	Plate2 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate2"));
	Plate2->SetRelativeLocationAndRotation(FVector(25.0f, 25.0f, -49.500099f), FRotator(0.0, 0.0, 180.0));
	Plates.Emplace(Plate2);

	Plate3 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate3"));
	Plate3->SetRelativeLocationAndRotation(FVector(25.0f, -49.500099f, 25.0f), FRotator(-90.0, 90.0, -180.0));
	Plates.Emplace(Plate3);

	Plate4 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate4"));
	Plate4->SetRelativeLocationAndRotation(FVector(-49.500099f, 25.0f, 25.0f), FRotator(-90.0, 0.0, 180.0));
	Plates.Emplace(Plate4);

	Plate5 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate5"));
	Plate5->SetRelativeLocationAndRotation(FVector(25.0f, 25.0f, 49.500099f), FRotator(0.0, 0.0, 0.0));
	Plates.Emplace(Plate5);

	Plate6 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate6"));
	Plate6->SetRelativeLocationAndRotation(FVector(25.0f, 49.500099f, 25.0f), FRotator(-90.0, 180.0, -90.0));
	Plates.Emplace(Plate6);

	Plate7 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate7"));
	Plate7->SetRelativeLocationAndRotation(FVector(49.500099f, 25.0f, -25.0f), FRotator(-90.0, 0.0, 0.0));
	Plates.Emplace(Plate7);

	Plate8 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate8"));
	Plate8->SetRelativeLocationAndRotation(FVector( 25.0f, -25.0f, -49.500099f), FRotator(0.0, 0.0, 180.0));
	Plates.Emplace(Plate8);

	Plate9 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate9"));
	Plate9->SetRelativeLocationAndRotation(FVector(25.0f, -49.500099f, -25.0f), FRotator(-90.0, 90.0, -180.0));
	Plates.Emplace(Plate9);

	Plate10 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate10"));
	Plate10->SetRelativeLocationAndRotation(FVector(-49.500099f, -25.0f, 25.0f), FRotator(-90.0, 0.0, 180.0));
	Plates.Emplace(Plate10);

	Plate11 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate11"));
	Plate11->SetRelativeLocationAndRotation(FVector(25.0f, -25.0f, 49.500099f), FRotator(0.0, 0.0, 0.0));
	Plates.Emplace(Plate11);

	Plate12 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate12"));
	Plate12->SetRelativeLocationAndRotation(FVector(-25.0f, 49.500099f, 25.0f), FRotator(-90.0, 180.0, -90.0));
	Plates.Emplace(Plate12);

	Plate13 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate13"));
	Plate13->SetRelativeLocationAndRotation(FVector(49.500099f, -25.0f, 25.0f), FRotator(-90.0, 0.0, 0.0));
	Plates.Emplace(Plate13);

	Plate14 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate14"));
	Plate14->SetRelativeLocationAndRotation(FVector(-25.0f, 25.0f, -49.500099f), FRotator(0.0, 0.0, 180.0));
	Plates.Emplace(Plate14);

	Plate15 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate15"));
	Plate15->SetRelativeLocationAndRotation(FVector(-25.0f, -49.500099f, 25.0f), FRotator(-90.0, 90.0, -180.0));
	Plates.Emplace(Plate15);

	Plate16 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate16"));
	Plate16->SetRelativeLocationAndRotation(FVector(-49.500099f, 25.0f, -25.0f), FRotator(-90.0, 0.0, 180.0));
	Plates.Emplace(Plate16);

	Plate17 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate17"));
	Plate17->SetRelativeLocationAndRotation(FVector(-25.0f, 25.0f, 49.500099f), FRotator(0.0, 0.0, 0.0));
	Plates.Emplace(Plate17);

	Plate18 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate18"));
	Plate18->SetRelativeLocationAndRotation(FVector(25.0f, 49.500099f, -25.0f), FRotator(-90.0, 180.0, -90.0));
	Plates.Emplace(Plate18);

	Plate19 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate19"));
	Plate19->SetRelativeLocationAndRotation(FVector(49.500099f, -25.0f, -25.0f), FRotator(-90.0, 0.0, 0.0));
	Plates.Emplace(Plate19);

	Plate20 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate20"));
	Plate20->SetRelativeLocationAndRotation(FVector(-25.0f, -25.0f, -49.500099f), FRotator(0.0, 0.0, 180.0));
	Plates.Emplace(Plate20);

	Plate21 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate21"));
	Plate21->SetRelativeLocationAndRotation(FVector(-25.0f, -49.500099f, -25.0f), FRotator(-90.0, 90.0, -180.0));
	Plates.Emplace(Plate21);

	Plate22 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate22"));
	Plate22->SetRelativeLocationAndRotation(FVector(-49.500099f, -25.0f, -25.0f), FRotator(-90.0, 0.0, 180.0));
	Plates.Emplace(Plate22);

	Plate23 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate23"));
	Plate23->SetRelativeLocationAndRotation(FVector(-25.0f, -25.0f, 49.500099f), FRotator(0.0, 0.0, 0.0));
	Plates.Emplace(Plate23);

	Plate24 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate24"));
	Plate24->SetRelativeLocationAndRotation(FVector(-25.0f, 49.500099f, -25.0f), FRotator(-90.0, 180.0, -90.0));
	Plates.Emplace(Plate24);

	for (auto& Elem : Plates)
	{
		Elem->AttachTo(BoxMesh);
		Elem->SetWorldScale3D(FVector::OneVector);
		Elem->SetSimulatePhysics(false);
		Elem->SetMaterial(0, Red);
	}
}

// Called when the game starts or when spawned
void ABaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseActor::MakeDynamicConnection(AActor * OtherConstraintObject)
{
	FConstraintInstance ConstraintInstance;
	ConstraintInstance.SetAngularSwing1Motion(EAngularConstraintMotion::ACM_Locked);
	ConstraintInstance.SetAngularSwing2Motion(EAngularConstraintMotion::ACM_Locked);
	ConstraintInstance.SetAngularTwistMotion(EAngularConstraintMotion::ACM_Locked);

	//New Object
	UPhysicsConstraintComponent* ConstraintComp = NewObject<UPhysicsConstraintComponent>(this);
	if (!ConstraintComp)
	{
		//UE_LOG constraint UObject could not be created!
		return;
	}
	DynamicConstraints.Emplace(ConstraintComp);
	ConstraintComp->ConstraintInstance = ConstraintInstance;
	ConstraintComp->SetWorldLocation(GetActorLocation());
	ConstraintComp->AttachTo(GetRootComponent(), NAME_None, EAttachLocation::KeepWorldPosition);
	ConstraintComp->SetRelativeLocationAndRotation(FVector(0.0, 0.0, 0.0), FRotator(0.0, 0.0, 0.0), false, nullptr, ETeleportType::None);
	if (Cast<AElbow>(OtherConstraintObject))
		ConstraintComp->SetConstrainedComponents(BoxMesh, NAME_None, Cast<AElbow>(OtherConstraintObject)->Part2, NAME_None);
	else
		MakeConnection(OtherConstraintObject, ConstraintComp);
}

void ABaseActor::MakeConnection(AActor* OtherConstraintObject, UPhysicsConstraintComponent* ConstraintComp)
{
	ConstraintComp->ConstraintActor1 = this;
	ConstraintComp->ConstraintActor2 = OtherConstraintObject;
	ConstraintComp->InitComponentConstraint();
}

TArray<AActor*> ABaseActor::GetOterConstraintActors()
{
	TArray<AActor*> OterConstraintActors;
	for (auto& Elem : DynamicConstraints)
		OterConstraintActors.Emplace(Elem->ConstraintActor2);
	return OterConstraintActors;
}

void ABaseActor::ClearMemory()
{
	if (DynamicConstraints.Num() != 0)
		for (auto& Elem : DynamicConstraints)
		{
			Elem->BreakConstraint();
			Elem->InitComponentConstraint();
			Elem->ConditionalBeginDestroy();
			//UE_LOG(LogTemp, Warning, TEXT("Constrain cleared!"));
		}
}


