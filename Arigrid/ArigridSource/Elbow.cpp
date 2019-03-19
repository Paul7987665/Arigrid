// Fill out your copyright notice in the Description page of Project Settings.

#include "Elbow.h"
#include "Engine.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "MyStaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "MyStaticMeshComponent.h"

// Sets default values
AElbow::AElbow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupTexture = CreateDefaultSubobject<UTexture2D>(FName("ItemTexture"));
	static ConstructorHelpers::FObjectFinder<UTexture2D>Texture(TEXT("Texture2D'/Game/Arigrid/2DTexture/Elbow_1x1x1.Elbow_1x1x1'"));
	if (Texture.Succeeded())
	{
		PickupTexture = Texture.Object;
	}

	Circle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Circle"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CircleAsset(TEXT("StaticMesh'/Game/Arigrid/CirclePart.CirclePart'"));
	if (CircleAsset.Succeeded())
	{
		Circle->SetStaticMesh(CircleAsset.Object);
		Circle->SetSimulatePhysics(true);
		RootComponent = Circle;
	}
	
	SphereOverlap1 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereOverlap1"));
	SphereOverlap1->SetNotifyRigidBodyCollision(true);
	SphereOverlap1->SetHiddenInGame(true);
	SphereOverlap1->SetCollisionProfileName("Trigger");
	SphereOverlap1->SetSphereRadius(40.f);
	

	SphereOverlap2 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereOverlap2"));
	SphereOverlap2->SetNotifyRigidBodyCollision(true);
	SphereOverlap2->SetHiddenInGame(true);
	SphereOverlap2->SetCollisionProfileName("Trigger");
	SphereOverlap2->SetSphereRadius(40.f);
	SphereOverlap2->SetRelativeLocationAndRotation(FVector(62.0f, 0.0f, 0.0f), FRotator(0.0, 0.0, 0.0));

	

	Part1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part1"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Part1Asset(TEXT("StaticMesh'/Game/Arigrid/ElbowPart.ElbowPart'"));
	if (Part1Asset.Succeeded())
	{
		Part1->SetStaticMesh(Part1Asset.Object);
		Part1->SetupAttachment(Circle);
		Part1->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -27.0f), FRotator(0, -180, -180));
	}
	SphereOverlap1->SetupAttachment(Part1);
	SphereOverlap1->SetRelativeLocationAndRotation(FVector(-62.0f, 0.0f, -27.0f), FRotator(0.0, 0.0, 0.0));
	

	Part2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part2"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Part2Asset(TEXT("StaticMesh'/Game/Arigrid/ElbowPart.ElbowPart'"));
	if (Part2Asset.Succeeded())
	{
		Part2->SetStaticMesh(Part2Asset.Object);
		Part2->SetupAttachment(Circle);
		Part2->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 27.0f), FRotator(0, 0, 0));
		Part2->SetSimulatePhysics(true);
	}
	SphereOverlap2->SetupAttachment(Part2);
	SphereOverlap2->SetRelativeLocationAndRotation(FVector(-62.0f, 0.0f, -27.0f), FRotator(0.0, 0.0, 0.0));
	Blue = CreateDefaultSubobject<UMaterialInstance>(TEXT("Blue"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial(TEXT("MaterialInstanceConstant'/Game/Arigrid/Blue.Blue'"));
	if (FoundMaterial.Succeeded())
	{
		Blue = FoundMaterial.Object;
		Part1->SetMaterial(0, Blue);
		Part2->SetMaterial(0, Blue);
	}

	Yellow = CreateDefaultSubobject<UMaterial>(TEXT("Yellow"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial0(TEXT("Material'/Game/Arigrid/Yellow.Yellow'"));
	if (FoundMaterial.Succeeded())
	{
		Yellow = FoundMaterial0.Object;
		Circle->SetMaterial(0, Yellow);
	}

	Plate1 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate1"));
	Plate1->SetupAttachment(Part1);
	Plate1->SetRelativeLocationAndRotation(FVector( -99.5f, 0.0f, -27.0f), FRotator( 90.0, 0.0, 0.0));
	Plate1->SetMaterial(0, Blue);

	Plate2 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate2"));
	Plate2->SetupAttachment(Part2);
	Plate2->SetRelativeLocationAndRotation(FVector(-99.5f, 0.0f, -27.0f), FRotator( -90.0, 0.0, 180.0));
	Plate2->SetMaterial(0, Blue);

	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
	Constraint->AttachTo(Circle);
	FConstraintInstance Constraintinstance;
	Constraintinstance.SetAngularSwing1Motion(EAngularConstraintMotion::ACM_Locked);
	Constraintinstance.SetAngularSwing2Motion(EAngularConstraintMotion::ACM_Locked);
	Constraintinstance.SetAngularTwistMotion(EAngularConstraintMotion::ACM_Locked);
	Constraint->ConstraintInstance = Constraintinstance;
	Constraint->SetConstrainedComponents(Part2, NAME_None, Circle, NAME_None);
}

// Called when the game starts or when spawned
void AElbow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AElbow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElbow::Turn()
{
	if (FMath::RadiansToDegrees(FQuat(GetActorRotation()).AngularDistance(FQuat(Part2->GetComponentRotation()))) < 89.f)
	{
		Constraint->BreakConstraint();
		Constraint->InitComponentConstraint();
		Part2->AddWorldRotation(FQuat(Part2->GetUpVector(), PI / 18));
		Constraint->SetConstrainedComponents(Part2, NAME_None, Circle, NAME_None);
	}
}

void AElbow::ClearMemory()
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
void AElbow::MakeDynamicConnection(AActor * OtherConstraintObject, UMyStaticMeshComponent* LastItemSeen)
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
	if (LastItemSeen == Plate1)
	{
		ConstraintComp->ConstraintActor1 = this;
		if (Cast<AElbow>(OtherConstraintObject))
		{
			ConstraintComp->SetConstrainedComponents(nullptr, NAME_None, Cast<AElbow>(OtherConstraintObject)->Part2, NAME_None);
		}
		else
		{
			ConstraintComp->ConstraintActor2 = OtherConstraintObject;
			ConstraintComp->InitComponentConstraint();
		}
	}
	else
	{
		if (Cast<AElbow>(OtherConstraintObject))
		{
			ConstraintComp->SetConstrainedComponents(Part2, NAME_None, Cast<AElbow>(OtherConstraintObject)->Part2, NAME_None);
		}
		else 
		{
			ConstraintComp->ConstraintActor2 = OtherConstraintObject;
			ConstraintComp->SetConstrainedComponents(Part2, NAME_None, nullptr, NAME_None);
		}
	}

}

TArray<AActor*> AElbow::GetOterConstraintActors()
{
	TArray<AActor*> OterConstraintActors;
	for (auto& Elem : DynamicConstraints)
		OterConstraintActors.Emplace(Elem->ConstraintActor2);
	return OterConstraintActors;
}


