// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActorWithDinamicConstrain.h"
#include "Elbow.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "MyStaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AMyActorWithDinamicConstrain::AMyActorWithDinamicConstrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupTexture = CreateDefaultSubobject<UTexture2D>(FName("ItemTexture"));
	static ConstructorHelpers::FObjectFinder<UTexture2D>Texture(TEXT("Texture2D'/Game/Arigrid/2DTexture/Box_1x1x1.Box_1x1x1'"));
	if (Texture.Succeeded())
	{
		PickupTexture = Texture.Object;
	}

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BoxMeshAsset(TEXT("StaticMesh'/Game/Arigrid/BOXX.BOXX'"));
	if (BoxMeshAsset.Succeeded())
	{
		BoxMesh->SetStaticMesh(BoxMeshAsset.Object);
		BoxMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		SetRootComponent(BoxMesh);
		BoxMesh->SetSimulatePhysics(true);
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
	SphereOverlap->SetupAttachment(BoxMesh);
	SphereOverlap->SetNotifyRigidBodyCollision(true);
	SphereOverlap->SetHiddenInGame(false);
	SphereOverlap->SetCollisionProfileName("Trigger");
	SphereOverlap->InitSphereRadius(52.f);

	Plate1 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate1"));
	Plate1->SetRelativeLocationAndRotation(FVector(49.500099f, 0.0f, 0.0f), FRotator(-90.0, 0.0, 0.0));
	Plates.Emplace(Plate1);

	Plate2 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate2"));
	Plate2->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -49.500099f), FRotator(0.0, 0.0, 180.0));
	Plates.Emplace(Plate2);

	Plate3 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate3"));
	Plate3->SetRelativeLocationAndRotation(FVector(0.0f, -49.500099f, 0.0f), FRotator(-90.0, 90.0, -180.0));
	Plates.Emplace(Plate3);

	Plate4 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate4"));
	Plate4->SetRelativeLocationAndRotation(FVector(-49.500099f, 0.0f, 0.0f), FRotator(-90.0, 0.0, 180.0));
	Plates.Emplace(Plate4);

	Plate5 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate5"));
	Plate5->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 49.500099f), FRotator::ZeroRotator);
	Plates.Emplace(Plate5);

	Plate6 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate6"));
	Plate6->SetRelativeLocationAndRotation(FVector(0.0f, 49.500099f, 0.0f), FRotator(-90.0, 180.0, -90.0));
	Plates.Emplace(Plate6);

	for (auto& Elem : Plates)
	{
		Elem->AttachTo(BoxMesh);
		Elem->SetSimulatePhysics(false);
		Elem->SetMaterial(0, Red);
	}

}

void AMyActorWithDinamicConstrain::ClearMemory()
{
	if(DynamicConstraints.Num() != 0)
		for (auto& Elem : DynamicConstraints)
		{
			Elem->BreakConstraint();
			Elem->InitComponentConstraint();
			Elem->ConditionalBeginDestroy();
			//UE_LOG(LogTemp, Warning, TEXT("Constrain cleared!"));
		}
}
/* ~ */
void AMyActorWithDinamicConstrain::MakeDynamicConnection(AActor * OtherConstraintObject)
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
	ConstraintComp->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	if (Cast<AElbow>(OtherConstraintObject))
		ConstraintComp->SetConstrainedComponents(BoxMesh, NAME_None, Cast<AElbow>(OtherConstraintObject)->Part2, NAME_None);
	else
		MakeConnection(OtherConstraintObject, ConstraintComp);
}

TArray<AActor*> AMyActorWithDinamicConstrain::GetOterConstraintActors()
{
	TArray<AActor*> OterConstraintActors;
	for (auto& Elem : DynamicConstraints)
		OterConstraintActors.Emplace(Elem->ConstraintActor2);
	return OterConstraintActors;
}

void AMyActorWithDinamicConstrain::MakeConnection(AActor * OtherConstraintObject, UPhysicsConstraintComponent * ConstraintComp)
{
	ConstraintComp->ConstraintActor1 = this;
	ConstraintComp->ConstraintActor2 = OtherConstraintObject;
	ConstraintComp->InitComponentConstraint();
}

// Called when the game starts or when spawned
void AMyActorWithDinamicConstrain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActorWithDinamicConstrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

