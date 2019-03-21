// Fill out your copyright notice in the Description page of Project Settings.

#include "Elbow2.h"
#include "Elbow.h"
#include "Engine.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "MyStaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "MyStaticMeshComponent.h"

// Sets default values
AElbow2::AElbow2()
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
		Circle->SetWorldScale3D(FVector(2.f, 2.f, 2.f));
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
	Plate1->SetRelativeLocationAndRotation(FVector(-99.5f, -25.0f, -52.0f), FRotator(90.0, 0.0, 0.0));
	Plate1->SetMaterial(0, Blue);
	Plate1->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate2 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate2"));
	Plate2->SetupAttachment(Part2);
	Plate2->SetRelativeLocationAndRotation(FVector(-99.5f, -25.0f, -52.0f), FRotator(90.0, 0.0, 0.0));
	Plate2->SetMaterial(0, Blue);
	Plate2->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate3 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate3"));
	Plate3->SetRelativeLocationAndRotation(FVector(-99.5f, -25.f, -2.f), FRotator(-90.0, 180.0, 0.0));
	Plate3->SetupAttachment(Part1);
	Plate3->SetMaterial(0, Blue);
	Plate3->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate4 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate4"));
	Plate4->SetRelativeLocationAndRotation(FVector(-99.5f, -25.f, -2.f), FRotator(-90.0, 0.0, 180.0));
	Plate4->SetupAttachment(Part2);
	Plate4->SetMaterial(0, Blue);
	Plate4->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate5 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate5"));
	Plate5->SetRelativeLocationAndRotation(FVector(-99.5f, 25.0f, -2.f), FRotator(90.0, 0.0, 0.0));
	Plate5->SetupAttachment(Part1);
	Plate5->SetMaterial(0, Blue);
	Plate5->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate6 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate6"));
	Plate6->SetRelativeLocationAndRotation(FVector(-99.5f, 25.0f, -2.f), FRotator(90.0, 0.0, 0.0));
	Plate6->SetupAttachment(Part2);
	Plate6->SetMaterial(0, Blue);
	Plate6->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate7 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate7"));
	Plate7->SetRelativeLocationAndRotation(FVector(-99.5f, 25.0f, -52.0f), FRotator(-90.0, 180.0, 0.0));
	Plate7->SetupAttachment(Part1);
	Plate7->SetMaterial(0, Blue);
	Plate7->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate8 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate8"));
	Plate8->SetRelativeLocationAndRotation(FVector(-99.5f, 25.0f, -52.0f), FRotator(-90.0, 180.0, 0.0));
	Plate8->SetupAttachment(Part2);
	Plate8->SetMaterial(0, Blue);
	Plate8->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate9 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate9"));
	Plate9->SetRelativeLocationAndRotation(FVector(-75.0f, -49.500099f, -2.0f), FRotator(-90.0, 90.0, -180.0));
	Plate9->SetupAttachment(Part1);
	Plate9->SetMaterial(0, Blue);
	Plate9->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate10 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate10"));
	Plate10->SetRelativeLocationAndRotation(FVector(-75.0f, -49.500099f, -2.0f), FRotator(-90.0, 90.0, -180.0));
	Plate10->SetupAttachment(Part2);
	Plate10->SetMaterial(0, Blue);
	Plate10->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate11 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate11"));
	Plate11->SetRelativeLocationAndRotation(FVector(-75.0f, -49.500099f, -52.f), FRotator(0.0, 0.0, -90.0));
	Plate11->SetupAttachment(Part1);
	Plate11->SetMaterial(0, Blue);
	Plate11->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate12 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate12"));
	Plate12->SetRelativeLocationAndRotation(FVector(-75.0f, -49.500099f, -52.f), FRotator(0.0, 0.0, -90.0));
	Plate12->SetupAttachment(Part2);
	Plate12->SetMaterial(0, Blue);
	Plate12->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate13 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate13"));
	Plate13->SetRelativeLocationAndRotation(FVector(-75.f, 49.500099f, -2.f), FRotator(0.0, 0.0, 90.0));
	Plate13->SetupAttachment(Part1);
	Plate13->SetMaterial(0, Blue);
	Plate13->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate14 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate14"));
	Plate14->SetRelativeLocationAndRotation(FVector(-75.f, 49.500099f, -2.f), FRotator( 0.0, 0.0, 90.0));
	Plate14->SetupAttachment(Part2);
	Plate14->SetMaterial(0, Blue);
	Plate14->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate15 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate15"));
	Plate15->SetRelativeLocationAndRotation(FVector( -75.0f, -49.500099f, -52.0f), FRotator( 0.0, 0.0, -90.0));
	Plate15->SetupAttachment(Part1);
	Plate15->SetMaterial(0, Blue);
	Plate15->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate16 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate16"));
	Plate16->SetRelativeLocationAndRotation(FVector(-75.0f, 49.500099f, -52.0f), FRotator(0.0, 180.0, -90.0));
	Plate16->SetupAttachment(Part2);
	Plate16->SetMaterial(0, Blue);
	Plate16->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate17 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate17"));
	Plate17->SetRelativeLocationAndRotation(FVector(-75.0f, 25.0f, 22.7f), FRotator(0.0, 0.0, 0.0));
	Plate17->SetupAttachment(Part1);
	Plate17->SetMaterial(0, Blue);
	Plate17->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate18 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate18"));
	Plate18->SetRelativeLocationAndRotation(FVector(-75.0f, 25.0f, 22.7f), FRotator(0.0, 0.0, 0.0));
	Plate18->SetupAttachment(Part2);
	Plate18->SetMaterial(0, Blue);
	Plate18->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate19 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate19"));
	Plate19->SetRelativeLocationAndRotation(FVector(-75.0f, 49.5f, -52.f), FRotator( 0.0, 0.0, 90.0));
	Plate19->SetupAttachment(Part1);
	Plate19->SetMaterial(0, Blue);
	Plate19->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate20 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate20"));
	Plate20->SetRelativeLocationAndRotation(FVector(-75.0f, 25.0f, 22.7f), FRotator(0.0, 0.0, 0.0));
	Plate20->SetupAttachment(Part2);
	Plate20->SetMaterial(0, Blue);
	Plate20->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate21 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate21"));
	Plate21->SetRelativeLocationAndRotation(FVector(-75.0f, -25.f, -76.5f), FRotator( 0.0, 0.0, 180.0));
	Plate21->SetupAttachment(Part1);
	Plate21->SetMaterial(0, Blue);
	Plate21->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate22 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate22"));
	Plate22->SetRelativeLocationAndRotation(FVector(-75.0f, -25.f, -76.5f), FRotator( 0.0, 0.0, 180.0));
	Plate22->SetupAttachment(Part2);
	Plate22->SetMaterial(0, Blue);
	Plate22->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate23 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate23"));
	Plate23->SetRelativeLocationAndRotation(FVector(-75.0f, 25.f, -76.5f), FRotator(0.0, 0.0, 180.0));
	Plate23->SetupAttachment(Part1);
	Plate23->SetMaterial(0, Blue);
	Plate23->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	Plate24 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate24"));
	Plate24->SetRelativeLocationAndRotation(FVector(-75.0f, 25.f, -76.5f), FRotator(0.0, 0.0, 180.0));
	Plate24->SetupAttachment(Part2);
	Plate24->SetMaterial(0, Blue);
	Plate24->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

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
void AElbow2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AElbow2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElbow2::Turn()
{
	if (FMath::RadiansToDegrees(FQuat(GetActorRotation()).AngularDistance(FQuat(Part2->GetComponentRotation()))) < 89.f)
	{
		Constraint->BreakConstraint();
		Constraint->InitComponentConstraint();
		Part2->AddWorldRotation(FQuat(Part2->GetUpVector(), PI / 18));
		Constraint->SetConstrainedComponents(Part2, NAME_None, Circle, NAME_None);
	}
}

void AElbow2::ClearMemory()
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
void AElbow2::MakeDynamicConnection(AActor * OtherConstraintObject, UMyStaticMeshComponent* LastItemSeen)
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
	if ((LastItemSeen == Plate1) || (LastItemSeen == Plate3) || (LastItemSeen == Plate5) || (LastItemSeen == Plate7)
		|| (LastItemSeen == Plate9) || (LastItemSeen == Plate11) || (LastItemSeen == Plate13) || (LastItemSeen == Plate15)
		|| (LastItemSeen == Plate17) || (LastItemSeen == Plate19) || (LastItemSeen == Plate21) || (LastItemSeen == Plate23))
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

TArray<AActor*> AElbow2::GetOterConstraintActors()
{
	TArray<AActor*> OterConstraintActors;
	for (auto& Elem : DynamicConstraints)
		OterConstraintActors.Emplace(Elem->ConstraintActor2);
	return OterConstraintActors;
}