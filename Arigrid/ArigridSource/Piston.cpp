// Fill out your copyright notice in the Description page of Project Settings.

#include "Piston.h"
#include "Engine.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "MyStaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "MyStaticMeshComponent.h"

// Sets default values
APiston::APiston()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Part2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part2"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Part2Asset(TEXT("StaticMesh'/Game/Arigrid/Piston_Part2.Piston_Part2'"));
	if (Part2Asset.Succeeded())
	{
		Part2->SetStaticMesh(Part2Asset.Object);
		SetRootComponent(Part2);
		Part2->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 27.0f), FRotator(0, 0, 0));
		Part2->SetSimulatePhysics(true);
	}

	Part1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part1"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Part1Asset(TEXT("StaticMesh'/Game/Arigrid/Piston_Part1.Piston_Part1'"));
	if (Part1Asset.Succeeded())
	{
		Part1->SetStaticMesh(Part1Asset.Object);
		Part1->SetupAttachment(Part2);
		Part1->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -35.0f), FRotator(0.f, -180.f, -180.f));
	}
	Plate1 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate1"));
	Plate1->SetupAttachment(Part1);
	Plate1->SetRelativeLocationAndRotation(FVector( 0.f, 0.0f, 50.0f), FRotator::ZeroRotator);
	

	Plate2 = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Plate2"));
	Plate2->SetupAttachment(Part2);
	Plate2->SetRelativeLocationAndRotation(FVector( 0.f, 0.0f, 65.0f), FRotator::ZeroRotator);

	Green = CreateDefaultSubobject<UMaterialInstance>(TEXT("Green"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial(TEXT("MaterialInstanceConstant'/Game/Arigrid/Green.Green'"));
	if (FoundMaterial.Succeeded())
	{
		Green = FoundMaterial.Object;
		Part1->SetMaterial(0, Green);
		Plate1->SetMaterial(0, Green);
	}
	
	Red = CreateDefaultSubobject<UMaterialInstance>(TEXT("Red"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial0(TEXT("MaterialInstanceConstant'/Game/Arigrid/Red.Red'"));
	if (FoundMaterial.Succeeded())
	{
		Red = FoundMaterial0.Object;
		Plate2->SetMaterial(0, Red);
		Part2->SetMaterial(0, Red);
	}
}

// Called when the game starts or when spawned
void APiston::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APiston::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APiston::shot()
{
	Part1->AddWorldOffset(Part2->GetUpVector()*-3.f);
}

void APiston::MakeDynamicConnection(AActor * OtherConstraintObject, UMyStaticMeshComponent* LastItemSeen)
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
	ConstraintComp->SetRelativeLocationAndRotation(FVector(0.0, 0.0, 0.0), FRotator(0.0, 0.0, 0.0));
	if (LastItemSeen == Plate1)
	{
		ConstraintComp->ConstraintActor2 = OtherConstraintObject;
		ConstraintComp->SetConstrainedComponents(Part1, NAME_None, nullptr, NAME_None);
	}
	else
	{
		ConstraintComp->ConstraintActor2 = OtherConstraintObject;
		ConstraintComp->SetConstrainedComponents(Part1, NAME_None, nullptr, NAME_None);
	}
}

TArray<AActor*> APiston::GetOterConstraintActors()
{
	TArray<AActor*> OterConstraintActors;
	for (auto& Elem : DynamicConstraints)
		OterConstraintActors.Emplace(Elem->ConstraintActor2);
	return OterConstraintActors;
}