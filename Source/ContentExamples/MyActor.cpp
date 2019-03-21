// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"
#include "Elbow.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "MyStaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupTexture = CreateDefaultSubobject<UTexture2D>(FName("ItemTexture"));
	static ConstructorHelpers::FObjectFinder<UTexture2D>Texture(TEXT("Texture2D'/Game/Arigrid/2DTexture/Box_1x1x1.Box_1x1x1'"));
	if (Texture.Succeeded())
	{
		PickupTexture = Texture.Object;
	}

	Triangle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Triangle"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BoxMeshAsset(TEXT("StaticMesh'/Game/Arigrid/triangle.triangle'"));
	if (BoxMeshAsset.Succeeded())
	{
		Triangle->SetStaticMesh(BoxMeshAsset.Object);
		Triangle->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		SetRootComponent(Triangle);
		Triangle->SetSimulatePhysics(true);
	}

	Red = CreateDefaultSubobject<UMaterialInstance>(TEXT("Red"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial(TEXT("MaterialInstanceConstant'/Game/Arigrid/Red.Red'"));
	if (FoundMaterial.Succeeded())
	{
		Red = FoundMaterial.Object;
		Triangle->SetMaterial(0, Red);
	}

	SphereOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("SphereOverlap"));
	SphereOverlap->SetupAttachment(Triangle);
	SphereOverlap->SetNotifyRigidBodyCollision(true);
	SphereOverlap->SetHiddenInGame(true);
	SphereOverlap->SetCollisionProfileName("Trigger");
	SphereOverlap->InitSphereRadius(20.f);
	SphereOverlap->SetRelativeLocation(FVector(-34.f, 14.0f, -2.0f));

	Plane1 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate1"));
	Plane1->SetRelativeLocationAndRotation(FVector( -49.2f, -21.0f, 0.0f), FRotator(90.0, 180.0, 180.0));
	Plane1->AttachTo(Triangle);
	Plane1->SetSimulatePhysics(false);
	Plane1->SetMaterial(0, Red);

	Plane2 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate2"));
	Plane2->SetRelativeLocationAndRotation(FVector(0.0f, 28.2f, 0.0f), FRotator( 0.0, 0.0, 90.0));
	Plane2->AttachTo(Triangle);
	Plane2->SetSimulatePhysics(false);
	Plane2->SetMaterial(0, Red);
	
}


// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::ClearMemory()
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
/* ~ */
void AMyActor::MakeDynamicConnection(AActor * OtherConstraintObject)
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
		ConstraintComp->SetConstrainedComponents(Triangle, NAME_None, Cast<AElbow>(OtherConstraintObject)->Part2, NAME_None);
	else
		MakeConnection(OtherConstraintObject, ConstraintComp);
}

TArray<AActor*> AMyActor::GetOterConstraintActors()
{
	TArray<AActor*> OterConstraintActors;
	for (auto& Elem : DynamicConstraints)
		OterConstraintActors.Emplace(Elem->ConstraintActor2);
	return OterConstraintActors;
}

void AMyActor::MakeConnection(AActor * OtherConstraintObject, UPhysicsConstraintComponent * ConstraintComp)
{
	ConstraintComp->ConstraintActor1 = this;
	ConstraintComp->ConstraintActor2 = OtherConstraintObject;
	ConstraintComp->InitComponentConstraint();
}