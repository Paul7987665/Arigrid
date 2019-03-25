// Fill out your copyright notice in the Description page of Project Settings.
#include "DPMainComponent.h"
#include "MyStaticMeshComponent.h"
#include "MyActorWithDinamicConstrain.h"
#include "Engine.h"
#include "MyStaticMeshComponent.h"
#include "MyActor.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Elbow.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#define OUT
#define PRINT(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, text)
FHitResult Hit;




// Sets default values for this component's properties
UDPMainComponent::UDPMainComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDPMainComponent::BeginPlay()
{
	Super::BeginPlay();

	LastItemSeen = nullptr;

	AActor* Pawn = GetOwner();
	Pawn->InputComponent->BindAction("ChoseElbowRotation", IE_Pressed, this, &UDPMainComponent::ChoseElbowRotation);
	Pawn->InputComponent->BindAction("ChoseTriangleRotation", IE_Pressed, this, &UDPMainComponent::ChoseTriangleRotation);
	Pawn->InputComponent->BindAction("ChoseTypeOfConnection", IE_Pressed, this, &UDPMainComponent::ChoseTypeOfConnection);
	Pawn->InputComponent->BindAction("WriteElbowRotationPart", IE_Pressed, this, &UDPMainComponent::WrteElbowRotation);
	Pawn->InputComponent->BindAction("SwitchMotor", IE_Pressed, this, &UDPMainComponent::SetStatus);
	Pawn->InputComponent->BindAction("Pickup", IE_Pressed, this, &UDPMainComponent::PickupItem);
	Pawn->InputComponent->BindAction("Spawn", IE_Pressed, this, &UDPMainComponent::Spawn);
	Pawn->InputComponent->BindAction("Turn", IE_Pressed, this, &UDPMainComponent::TurnElbow);
}

void UDPMainComponent::WrteElbowRotation()
{
	if (Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rotator : %s"), *(FRotator(FQuat(LastBoxSeen->GetRightVector(), 90.f)).ToString()));
		//UE_LOG(LogTemp, Warning, TEXT("Actor Up Vector: %s"), *(Cast<AElbow>(Hit.GetActor())->GetActorRotation().ToString()));
		//UE_LOG(LogTemp, Warning, TEXT("Forward     Vector         A: %s"), *(Cast<AElbow>(Owner)->GetActorForwardVector().Rotation().ToString()));
		//UE_LOG(LogTemp, Warning, TEXT("Forward     Vector         A: %s"), *(Cast<AElbow>(Owner)->GetActorForwardVector().ToString()));
	}
}

// Called every frame
void UDPMainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Raycast every frame
	Raycast();
}

void UDPMainComponent::Raycast()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	//RayCast()
	FCollisionQueryParams Param(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByChannel(OUT Hit, PlayerViewPointLocation, LineTraceEnd, ECollisionChannel::ECC_WorldDynamic, Param);
	Owner = Hit.GetActor();
	UMyStaticMeshComponent* Comp = Cast<UMyStaticMeshComponent>(Hit.GetComponent());

	if (LastItemSeen && LastItemSeen != Comp)
		LastItemSeen->SetGlowEffect(false);

	if (Comp)
	{
		LastItemSeen = Comp;
		Comp->SetGlowEffect(true);
	}
	else
		LastItemSeen = nullptr;
}

void UDPMainComponent::SetStatus()
{
	if (Cast<ABladeMotor>(Hit.GetActor()))
		if(Cast<ABladeMotor>(Hit.GetActor())->Status != true)
		{
			Cast<ABladeMotor>(Hit.GetActor())->Status = true;
			PRINT(TEXT("Status true"));
		}
		else
		{
			Cast<ABladeMotor>(Hit.GetActor())->Status = false;
			PRINT(TEXT("Status false"));
		}
}

void UDPMainComponent::TurnElbow()
{
	if (Cast<AElbow>(Owner))
	{
		Cast<AElbow>(Owner)->Turn();
	}
}

void UDPMainComponent::PickupItem()
{
	if (Cast<AMyActorWithDinamicConstrain>(Hit.GetActor()))
	{
		Cast<AMyActorWithDinamicConstrain>(Owner)->ClearMemory();
	}
	if(Owner)
		Owner->Destroy();
}
void UDPMainComponent::Spawn()
{
	if (GetWorld() != nullptr)
	{
		switch(Iter)
		{ 
			case static_cast<int>(Items::Box1) : 
				Spawn(Cast<AMyActorWithDinamicConstrain>(SpawnedActor));
				break;
			case static_cast<int>(Items::Box2) :
				Spawn(Cast<ABaseActor>(SpawnedActor));
				break;
			case static_cast<int>(Items::Triangle) :
				Spawn(Cast<AMyActor>(SpawnedActor));
				break;
			case static_cast<int>(Items::BladeMotor1) :
				Spawn(Cast<ABladeMotor>(SpawnedActor));
				break;
			case static_cast<int>(Items::BladeMotor2) :
				Spawn(Cast<ABladeMotor2>(SpawnedActor));
				break;
			case static_cast<int>(Items::Elbow1) :
				Spawn(Cast<AElbow>(SpawnedActor));
				break;
			case static_cast<int>(Items::Elbow2) :
				Spawn(Cast<AElbow2>(SpawnedActor));
				break;
			case static_cast<int>(Items::Piston) :
				Spawn(Cast<APiston>(SpawnedActor));
				break;
			default:
				PRINT(TEXT("ERROR Item"));
		}
	}
}

void UDPMainComponent::Spawn(AMyActorWithDinamicConstrain * SpawnedActor)
{
	if (LastItemSeen)
	{
		SpawnedActor = GetWorld()->SpawnActor<AMyActorWithDinamicConstrain>(AMyActorWithDinamicConstrain::StaticClass(), LastItemSeen->GetSocketLocation(FName("Socket")), LastItemSeen->GetSocketRotation(FName("Socket")));
		if (Cast<AMyActorWithDinamicConstrain>(Owner))
		{ 
			Cast<AMyActorWithDinamicConstrain>(Owner)->MakeDynamicConnection(SpawnedActor);
			TArray<AActor*>OverlappingActors;
			SpawnedActor->SphereOverlap->GetOverlappingActors(OverlappingActors);
			for (auto& Elem : OverlappingActors)
				if ((Elem != Cast<AMyActorWithDinamicConstrain>(Owner)) && (Elem != SpawnedActor))
					Cast<AMyActorWithDinamicConstrain>(Elem)->MakeDynamicConnection(SpawnedActor);
		}
		if (Cast<AElbow>(Owner))
			Cast<AElbow>(Owner)->MakeDynamicConnection(SpawnedActor, LastItemSeen);
		if (Cast<AMyActor>(Owner))
			Cast<AMyActor>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<ABladeMotor>(Owner))
			Cast<ABladeMotor>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<ABaseActor>(Owner))
			Cast<ABaseActor>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<ABladeMotor2>(Owner))
			Cast<ABladeMotor2>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<AElbow2>(Owner))
			Cast<AElbow2>(Owner)->MakeDynamicConnection(SpawnedActor, LastItemSeen);
		if (Cast<APiston>(Owner))
			Cast<APiston>(Owner)->MakeDynamicConnection(SpawnedActor, LastItemSeen);
	}
	else
	{
		FVector SpawnLocation = PlayerViewPointRotation.Vector() * Reach / 10 + PlayerViewPointLocation;
		SpawnedActor = GetWorld()->SpawnActor<AMyActorWithDinamicConstrain>(AMyActorWithDinamicConstrain::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
		UE_LOG(LogTemp, Warning, TEXT("Spawned: %s"), *(SpawnedActor->GetName()));
	}
}

void UDPMainComponent::Spawn(AElbow * SpawnedActor)
{
	if (LastItemSeen)
	{
		switch (Iter4)
		{
			case static_cast<int>(ElbowRotation::UpDown) : 
				SpawnedActor = GetWorld()->SpawnActor<AElbow>(AElbow::StaticClass(), LastItemSeen->GetSocketLocation(FName("ElbowUPDOWN")), LastItemSeen->GetSocketRotation(FName("ElbowUPDOWN")));
				break;
			case static_cast<int>(ElbowRotation::LeftRight) :
				SpawnedActor = GetWorld()->SpawnActor<AElbow>(AElbow::StaticClass(), LastItemSeen->GetSocketLocation(FName("ElbowLEFTRIGHT")), LastItemSeen->GetSocketRotation(FName("ElbowLEFTRIGHT")));
				break;
			default: PRINT(TEXT("ERORR!"));
		}
		if (Cast<AMyActorWithDinamicConstrain>(Owner))
			Cast<AMyActorWithDinamicConstrain>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<ABaseActor>(Owner))
			Cast<ABaseActor>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<AMyActor>(Owner))
			Cast<AMyActor>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<ABladeMotor2>(Owner))
			Cast<ABladeMotor2>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<ABladeMotor>(Owner))
			Cast<ABladeMotor>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<AElbow>(Owner))
			Cast<AElbow>(Owner)->MakeDynamicConnection(SpawnedActor, LastItemSeen);
		if (Cast<AElbow2>(Owner))
			Cast<AElbow2>(Owner)->MakeDynamicConnection(SpawnedActor, LastItemSeen);
	}
	else
	{
		FVector SpawnLocation = PlayerViewPointRotation.Vector() * Reach / 10 + PlayerViewPointLocation;
		SpawnedActor = GetWorld()->SpawnActor<AElbow>(AElbow::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	}
}

void UDPMainComponent::Spawn(AElbow2* SpawnedActor)
{
	FRotator AdditionRotation;
	if (LastItemSeen)
	{
		if (Cast<ABaseActor>(Owner))
		{
			ABaseActor* BigBox = Cast<ABaseActor>(Owner);
			FRotator ElbowRotator;
			switch (Iter4)
			{
				case static_cast<int>(ElbowRotation::UpDown) : ElbowRotator = LastItemSeen->GetSocketRotation(FName("ElbowUPDOWN"));
					break;
				case static_cast<int>(ElbowRotation::LeftRight) : ElbowRotator = LastItemSeen->GetSocketRotation(FName("ElbowLEFTRIGHT"));
					break;
				default: PRINT(TEXT("ERORR!"));
			}
			switch (Iter2)
			{
				case static_cast<int>(ConnectionType::Center) :
					if ((LastItemSeen == BigBox->Plate2) || (LastItemSeen == BigBox->Plate8) || (LastItemSeen == BigBox->Plate14) || (LastItemSeen == BigBox->Plate20))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterDOWN")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate1) || (LastItemSeen == BigBox->Plate7) || (LastItemSeen == BigBox->Plate13) || (LastItemSeen == BigBox->Plate19))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterRIGHT")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate3) || (LastItemSeen == BigBox->Plate9) || (LastItemSeen == BigBox->Plate15) || (LastItemSeen == BigBox->Plate21))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterBACK")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate4) || (LastItemSeen == BigBox->Plate10) || (LastItemSeen == BigBox->Plate16) || (LastItemSeen == BigBox->Plate22))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterLEFT")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate5) || (LastItemSeen == BigBox->Plate11) || (LastItemSeen == BigBox->Plate17) || (LastItemSeen == BigBox->Plate23))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterUP")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate6) || (LastItemSeen == BigBox->Plate12) || (LastItemSeen == BigBox->Plate18) || (LastItemSeen == BigBox->Plate24))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterFORWARD")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					break;
				case static_cast<int>(ConnectionType::LeftRight) :
					if ((LastItemSeen == BigBox->Plate1) || (LastItemSeen == BigBox->Plate7))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTFORWARD")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate13) || (LastItemSeen == BigBox->Plate19))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTBACK")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate3) || (LastItemSeen == BigBox->Plate9))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKRIGHT")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate15) || (LastItemSeen == BigBox->Plate21))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKLEFT")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate10) || (LastItemSeen == BigBox->Plate22))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTBACK")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate4) || (LastItemSeen == BigBox->Plate16))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTFORWARD")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate12) || (LastItemSeen == BigBox->Plate24))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDLEFT")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate6) || (LastItemSeen == BigBox->Plate18))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDRIGHT")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate5) || (LastItemSeen == BigBox->Plate11))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("UPRIGHT")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate17) || (LastItemSeen == BigBox->Plate23))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("UPLEFT")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate2) || (LastItemSeen == BigBox->Plate8))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("DOWNRIGHT")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate14) || (LastItemSeen == BigBox->Plate20))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("DOWNLEFT")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					break;
				case static_cast<int>(ConnectionType::UpDown) :
					if ((LastItemSeen == BigBox->Plate1) || (LastItemSeen == BigBox->Plate13))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTUP")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate7) || (LastItemSeen == BigBox->Plate19))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTDOWN")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate3) || (LastItemSeen == BigBox->Plate15))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKUP")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate9) || (LastItemSeen == BigBox->Plate21))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKDOWN")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate16) || (LastItemSeen == BigBox->Plate22))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTDOWN")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate6) || (LastItemSeen == BigBox->Plate12))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDUP")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate18) || (LastItemSeen == BigBox->Plate24))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDDOWN")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate11) || (LastItemSeen == BigBox->Plate23))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("UPBACK")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate5) || (LastItemSeen == BigBox->Plate17))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("UPFORWARD")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate14) || (LastItemSeen == BigBox->Plate2))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("DOWNFORWARD")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate20) || (LastItemSeen == BigBox->Plate8))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("DOWNBACK")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate4) || (LastItemSeen == BigBox->Plate10))
					{
						SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTUP")) + 100.f*LastItemSeen->GetUpVector(), ElbowRotator);
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					break;	
				default: PRINT(TEXT("ERORR!"));
			}

		}
	}
	else
	{
		FVector SpawnLocation = PlayerViewPointRotation.Vector() * Reach / 10 + PlayerViewPointLocation;
		SpawnedActor = GetWorld()->SpawnActor<AElbow2>(AElbow2::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	}
}

void UDPMainComponent::Spawn(ABladeMotor* SpawnedActor)
{
	if (LastItemSeen)
	{
		SpawnedActor = GetWorld()->SpawnActor<ABladeMotor>(ABladeMotor::StaticClass(), LastItemSeen->GetSocketLocation(FName("Socket")), LastItemSeen->GetSocketRotation(FName("Socket")));
		if (Cast<AMyActorWithDinamicConstrain>(Owner))
			Cast<AMyActorWithDinamicConstrain>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<AElbow>(Owner))
			Cast<AElbow>(Owner)->MakeDynamicConnection(SpawnedActor, LastItemSeen);
		if (Cast<AMyActor>(Owner))
			Cast<AMyActor>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<ABaseActor>(Owner))
			Cast<ABaseActor>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<AElbow2>(Owner))
			Cast<AElbow2>(Owner)->MakeDynamicConnection(SpawnedActor, LastItemSeen);
		if (Cast<APiston>(Owner))
			Cast<APiston>(Owner)->MakeDynamicConnection(SpawnedActor, LastItemSeen);
	}
	else
	{
		FVector SpawnLocation = PlayerViewPointRotation.Vector() * Reach / 10 + PlayerViewPointLocation;
		SpawnedActor = GetWorld()->SpawnActor<ABladeMotor>(ABladeMotor::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	}
}

void UDPMainComponent::Spawn(ABladeMotor2 * SpawnedActor)
{
	if (LastItemSeen)
	{
		if (Cast<ABaseActor>(Owner))
		{
			ABaseActor* BigBox = Cast<ABaseActor>(Owner);
			switch (Iter2)
			{
				case static_cast<int>(ConnectionType::Center) :
				if ((LastItemSeen == BigBox->Plate2) || (LastItemSeen == BigBox->Plate8) || (LastItemSeen == BigBox->Plate14) || (LastItemSeen == BigBox->Plate20))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterDOWN")), BigBox->BoxMesh->GetSocketRotation(FName("CenterDOWN")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate1) || (LastItemSeen == BigBox->Plate7) || (LastItemSeen == BigBox->Plate13) || (LastItemSeen == BigBox->Plate19))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterRIGHT")), BigBox->BoxMesh->GetSocketRotation(FName("CenterRIGHT")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate3) || (LastItemSeen == BigBox->Plate9) || (LastItemSeen == BigBox->Plate15) || (LastItemSeen == BigBox->Plate21))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterBACK")), BigBox->BoxMesh->GetSocketRotation(FName("CenterBACK")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate4) || (LastItemSeen == BigBox->Plate10) || (LastItemSeen == BigBox->Plate16) || (LastItemSeen == BigBox->Plate22))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterLEFT")), BigBox->BoxMesh->GetSocketRotation(FName("CenterLEFT")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate5) || (LastItemSeen == BigBox->Plate11) || (LastItemSeen == BigBox->Plate17) || (LastItemSeen == BigBox->Plate23))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterUP")), BigBox->BoxMesh->GetSocketRotation(FName("CenterUP")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate6) || (LastItemSeen == BigBox->Plate12) || (LastItemSeen == BigBox->Plate18) || (LastItemSeen == BigBox->Plate24))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterFORWARD")), BigBox->BoxMesh->GetSocketRotation(FName("CenterFORWARD")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				break;
				case static_cast<int>(ConnectionType::LeftRight) :
				if ((LastItemSeen == BigBox->Plate1) || (LastItemSeen == BigBox->Plate7))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTFORWARD")), BigBox->BoxMesh->GetSocketRotation(FName("RIGHTFORWARD")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}										 
				if ((LastItemSeen == BigBox->Plate13) || (LastItemSeen == BigBox->Plate19))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTBACK")), BigBox->BoxMesh->GetSocketRotation(FName("RIGHTBACK")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate3) || (LastItemSeen == BigBox->Plate9))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKRIGHT")), BigBox->BoxMesh->GetSocketRotation(FName("BACKRIGHT")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate15) || (LastItemSeen == BigBox->Plate21))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKLEFT")), BigBox->BoxMesh->GetSocketRotation(FName("BACKLEFT")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate10) || (LastItemSeen == BigBox->Plate22))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTBACK")), BigBox->BoxMesh->GetSocketRotation(FName("LEFTBACK")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate4) || (LastItemSeen == BigBox->Plate16))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTFORWARD")), BigBox->BoxMesh->GetSocketRotation(FName("LEFTFORWARD")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate12) || (LastItemSeen == BigBox->Plate24))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDLEFT")), BigBox->BoxMesh->GetSocketRotation(FName("FORWARDLEFT")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate6) || (LastItemSeen == BigBox->Plate18))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDRIGHT")), BigBox->BoxMesh->GetSocketRotation(FName("FORWARDRIGHT")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				break;
				default: 
				if ((LastItemSeen == BigBox->Plate1) || (LastItemSeen == BigBox->Plate13))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTUP")), BigBox->BoxMesh->GetSocketRotation(FName("RIGHTUP")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate7) || (LastItemSeen == BigBox->Plate19))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTDOWN")), BigBox->BoxMesh->GetSocketRotation(FName("RIGHTDOWN")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate3) || (LastItemSeen == BigBox->Plate15))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKUP")), BigBox->BoxMesh->GetSocketRotation(FName("BACKUP")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate9) || (LastItemSeen == BigBox->Plate21))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKDOWN")), BigBox->BoxMesh->GetSocketRotation(FName("BACKDOWN")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate16) || (LastItemSeen == BigBox->Plate22))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTDOWN")), BigBox->BoxMesh->GetSocketRotation(FName("LEFTDOWN")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate6) || (LastItemSeen == BigBox->Plate12))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDUP")), BigBox->BoxMesh->GetSocketRotation(FName("FORWARDUP")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate18) || (LastItemSeen == BigBox->Plate24))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDDOWN")), BigBox->BoxMesh->GetSocketRotation(FName("FORWARDDOWN")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate11) || (LastItemSeen == BigBox->Plate23))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("UPBACK")), BigBox->BoxMesh->GetSocketRotation(FName("UPBACK")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate5) || (LastItemSeen == BigBox->Plate17))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("UPFORWARD")), BigBox->BoxMesh->GetSocketRotation(FName("UPFORWARD")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate14) || (LastItemSeen == BigBox->Plate2))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("DOWNFORWARD")), BigBox->BoxMesh->GetSocketRotation(FName("DOWNFORWARD")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate20) || (LastItemSeen == BigBox->Plate8))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("DOWNBACK")), BigBox->BoxMesh->GetSocketRotation(FName("DOWNBACK")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				if ((LastItemSeen == BigBox->Plate4) || (LastItemSeen == BigBox->Plate10))
				{
					SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTUP")), BigBox->BoxMesh->GetSocketRotation(FName("LEFTUP")));
					BigBox->MakeDynamicConnection(SpawnedActor);
				}
				break;
			}
		}
		
	}
	else
	{
		FVector SpawnLocation = PlayerViewPointRotation.Vector() * Reach / 10 + PlayerViewPointLocation;
		SpawnedActor = GetWorld()->SpawnActor<ABladeMotor2>(ABladeMotor2::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	}
}


void UDPMainComponent::Spawn(ABaseActor* SpawnedActor)
{
	if (LastItemSeen)
	{
		if (Cast<ABaseActor>(Owner))
		{
			ABaseActor* BigBox = Cast<ABaseActor>(Owner);
			switch (Iter2)
			{
				case static_cast<int>(ConnectionType::Center) :
					if ((LastItemSeen == BigBox->Plate2) || (LastItemSeen == BigBox->Plate8) || (LastItemSeen == BigBox->Plate14) || (LastItemSeen == BigBox->Plate20))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterDOWN")), BigBox->BoxMesh->GetSocketRotation(FName("CenterDOWN")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate1) || (LastItemSeen == BigBox->Plate7) || (LastItemSeen == BigBox->Plate13) || (LastItemSeen == BigBox->Plate19))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterRIGHT")), BigBox->BoxMesh->GetSocketRotation(FName("CenterRIGHT")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate3) || (LastItemSeen == BigBox->Plate9) || (LastItemSeen == BigBox->Plate15) || (LastItemSeen == BigBox->Plate21))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterBACK")), BigBox->BoxMesh->GetSocketRotation(FName("CenterBACK")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate4) || (LastItemSeen == BigBox->Plate10) || (LastItemSeen == BigBox->Plate16) || (LastItemSeen == BigBox->Plate22))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterLEFT")), BigBox->BoxMesh->GetSocketRotation(FName("CenterLEFT")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate5) || (LastItemSeen == BigBox->Plate11) || (LastItemSeen == BigBox->Plate17) || (LastItemSeen == BigBox->Plate23))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterUP")), BigBox->BoxMesh->GetSocketRotation(FName("CenterUP")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate6) || (LastItemSeen == BigBox->Plate12) || (LastItemSeen == BigBox->Plate18) || (LastItemSeen == BigBox->Plate24))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("CenterFORWARD")), BigBox->BoxMesh->GetSocketRotation(FName("CenterFORWARD")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					break;
				case static_cast<int>(ConnectionType::LeftRight) :
					if ((LastItemSeen == BigBox->Plate1) || (LastItemSeen == BigBox->Plate7))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTFORWARD")), BigBox->BoxMesh->GetSocketRotation(FName("RIGHTFORWARD")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate13) || (LastItemSeen == BigBox->Plate19))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTBACK")), BigBox->BoxMesh->GetSocketRotation(FName("RIGHTBACK")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate3) || (LastItemSeen == BigBox->Plate9))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKRIGHT")), BigBox->BoxMesh->GetSocketRotation(FName("BACKRIGHT")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate15) || (LastItemSeen == BigBox->Plate21))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKLEFT")), BigBox->BoxMesh->GetSocketRotation(FName("BACKLEFT")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate10) || (LastItemSeen == BigBox->Plate22))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTBACK")), BigBox->BoxMesh->GetSocketRotation(FName("LEFTBACK")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate4) || (LastItemSeen == BigBox->Plate16))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTFORWARD")), BigBox->BoxMesh->GetSocketRotation(FName("LEFTFORWARD")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate12) || (LastItemSeen == BigBox->Plate24))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDLEFT")), BigBox->BoxMesh->GetSocketRotation(FName("FORWARDLEFT")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate6) || (LastItemSeen == BigBox->Plate18))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDRIGHT")), BigBox->BoxMesh->GetSocketRotation(FName("FORWARDRIGHT")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate5) || (LastItemSeen == BigBox->Plate11))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("UPRIGHT")), BigBox->BoxMesh->GetSocketRotation(FName("UPRIGHT")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate17) || (LastItemSeen == BigBox->Plate23))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("UPLEFT")), BigBox->BoxMesh->GetSocketRotation(FName("UPLEFT")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate2) || (LastItemSeen == BigBox->Plate8))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("DOWNRIGHT")), BigBox->BoxMesh->GetSocketRotation(FName("DOWNRIGHT")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate14) || (LastItemSeen == BigBox->Plate20))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("DOWNLEFT")), BigBox->BoxMesh->GetSocketRotation(FName("DOWNLEFT")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					break;
				default: 
					if ((LastItemSeen == BigBox->Plate1) || (LastItemSeen == BigBox->Plate13))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTUP")), BigBox->BoxMesh->GetSocketRotation(FName("RIGHTUP")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate7) || (LastItemSeen == BigBox->Plate19))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("RIGHTDOWN")), BigBox->BoxMesh->GetSocketRotation(FName("RIGHTDOWN")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate3) || (LastItemSeen == BigBox->Plate15))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKUP")), BigBox->BoxMesh->GetSocketRotation(FName("BACKUP")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate9) || (LastItemSeen == BigBox->Plate21))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("BACKDOWN")), BigBox->BoxMesh->GetSocketRotation(FName("BACKDOWN")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate16) || (LastItemSeen == BigBox->Plate22))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTDOWN")), BigBox->BoxMesh->GetSocketRotation(FName("LEFTDOWN")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate6) || (LastItemSeen == BigBox->Plate12))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDUP")), BigBox->BoxMesh->GetSocketRotation(FName("FORWARDUP")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate18) || (LastItemSeen == BigBox->Plate24))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("FORWARDDOWN")), BigBox->BoxMesh->GetSocketRotation(FName("FORWARDDOWN")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate11) || (LastItemSeen == BigBox->Plate23))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("UPBACK")), BigBox->BoxMesh->GetSocketRotation(FName("UPBACK")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate5) || (LastItemSeen == BigBox->Plate17))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("UPFORWARD")), BigBox->BoxMesh->GetSocketRotation(FName("UPFORWARD")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate14) || (LastItemSeen == BigBox->Plate2))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("DOWNFORWARD")), BigBox->BoxMesh->GetSocketRotation(FName("DOWNFORWARD")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate20) || (LastItemSeen == BigBox->Plate8))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("DOWNBACK")), BigBox->BoxMesh->GetSocketRotation(FName("DOWNBACK")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					if ((LastItemSeen == BigBox->Plate4) || (LastItemSeen == BigBox->Plate10))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), BigBox->BoxMesh->GetSocketLocation(FName("LEFTUP")), BigBox->BoxMesh->GetSocketRotation(FName("LEFTUP")));
						BigBox->MakeDynamicConnection(SpawnedActor);
					}
					break;
			}
		}
		if (Cast<AElbow2>(Owner))
		{
			AElbow2* Elbow2 = Cast<AElbow2>(Owner);
			switch (Iter2)
			{
				case static_cast<int>(ConnectionType::Center) :
					if ((LastItemSeen == Elbow2->Plate2) || (LastItemSeen == Elbow2->Plate8) || (LastItemSeen == Elbow2->Plate4) || (LastItemSeen == Elbow2->Plate6))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part2->GetSocketLocation(FName("CenterFOEWARD")), Elbow2->Part2->GetSocketRotation(FName("CenterFORWARD")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					if ((LastItemSeen == Elbow2->Plate1) || (LastItemSeen == Elbow2->Plate3) || (LastItemSeen == Elbow2->Plate5) || (LastItemSeen == Elbow2->Plate7))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part1->GetSocketLocation(FName("CenterFOEWARD")), Elbow2->Part1->GetSocketRotation(FName("CenterFORWARD")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					if ((LastItemSeen == Elbow2->Plate14) || (LastItemSeen == Elbow2->Plate16))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part2->GetSocketLocation(FName("RIGHT")), Elbow2->Part2->GetSocketRotation(FName("RIGHT")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					if ((LastItemSeen == Elbow2->Plate10) || (LastItemSeen == Elbow2->Plate12))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part2->GetSocketLocation(FName("LEFT")), Elbow2->Part2->GetSocketRotation(FName("LEFT")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					if((LastItemSeen == Elbow2->Plate13) || (LastItemSeen == Elbow2->Plate19))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part1->GetSocketLocation(FName("RIGHT")), Elbow2->Part1->GetSocketRotation(FName("LEFT")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					if((LastItemSeen == Elbow2->Plate15) || (LastItemSeen == Elbow2->Plate9))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part1->GetSocketLocation(FName("LEFT")), Elbow2->Part1->GetSocketRotation(FName("RIGHT")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					break;
				case static_cast<int>(ConnectionType::LeftRight) :
					if ((LastItemSeen == Elbow2->Plate2) || (LastItemSeen == Elbow2->Plate4))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part2->GetSocketLocation(FName("CenterLEFT")), Elbow2->Part2->GetSocketRotation(FName("CenterLEFT")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					if ((LastItemSeen == Elbow2->Plate6) || (LastItemSeen == Elbow2->Plate8))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part2->GetSocketLocation(FName("CenterRIGHT")), Elbow2->Part2->GetSocketRotation(FName("CenterRIGHT")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					if ((LastItemSeen == Elbow2->Plate5) || (LastItemSeen == Elbow2->Plate7))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part1->GetSocketLocation(FName("CenterRIGHT")), Elbow2->Part1->GetSocketRotation(FName("CenterLEFT")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					if ((LastItemSeen == Elbow2->Plate1) || (LastItemSeen == Elbow2->Plate3))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part1->GetSocketLocation(FName("CenterLEFT")), Elbow2->Part1->GetSocketRotation(FName("CenterLEFT")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					break;
				case static_cast<int>(ConnectionType::UpDown) : 
					if ((LastItemSeen == Elbow2->Plate2) || (LastItemSeen == Elbow2->Plate8))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part2->GetSocketLocation(FName("CenterDOWN")), Elbow2->Part2->GetSocketRotation(FName("CenterDOWN")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					if ((LastItemSeen == Elbow2->Plate6) || (LastItemSeen == Elbow2->Plate4))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part2->GetSocketLocation(FName("CenterUP")), Elbow2->Part2->GetSocketRotation(FName("CenterUP")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					if ((LastItemSeen == Elbow2->Plate1) || (LastItemSeen == Elbow2->Plate7))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part1->GetSocketLocation(FName("CenterUP")), Elbow2->Part1->GetSocketRotation(FName("CenterUP")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					if ((LastItemSeen == Elbow2->Plate5) || (LastItemSeen == Elbow2->Plate3))
					{
						SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), Elbow2->Part1->GetSocketLocation(FName("CenterDOWN")), Elbow2->Part1->GetSocketRotation(FName("CenterDOWN")));
						Elbow2->MakeDynamicConnection(SpawnedActor, LastItemSeen);
					}
					break;
				default: PRINT(TEXT("ERORR!"));
			}
		}
	}
	else
	{
		FVector SpawnLocation = PlayerViewPointRotation.Vector() * Reach / 10 + PlayerViewPointLocation;
		SpawnedActor = GetWorld()->SpawnActor<ABaseActor>(ABaseActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	}
}

void UDPMainComponent::Spawn(AMyActor* SpawnedActor)
{
	if (LastItemSeen)
	{
		switch (Iter3)
		{
			case static_cast<int>(TriangleRotation::LEFT) : 
				SpawnedActor = GetWorld()->SpawnActor<AMyActor>(AMyActor::StaticClass(), LastItemSeen->GetSocketLocation(FName("TriangleLEFT")), LastItemSeen->GetSocketRotation(FName("TriangleLEFT")));
				break;
			case static_cast<int>(TriangleRotation::RIGHT) : PRINT(TEXT("Right Triangle Connection"));
				SpawnedActor = GetWorld()->SpawnActor<AMyActor>(AMyActor::StaticClass(), LastItemSeen->GetSocketLocation(FName("TriangleRIGHT")), LastItemSeen->GetSocketRotation(FName("TriangleRIGHT")));
				break;
			case static_cast<int>(TriangleRotation::UP) : PRINT(TEXT("Up Connection"));
				SpawnedActor = GetWorld()->SpawnActor<AMyActor>(AMyActor::StaticClass(), LastItemSeen->GetSocketLocation(FName("TriangleUP")), LastItemSeen->GetSocketRotation(FName("TriangleUP")));
				break;
			case static_cast<int>(TriangleRotation::DOWN) : PRINT(TEXT("Down Connection"));
				SpawnedActor = GetWorld()->SpawnActor<AMyActor>(AMyActor::StaticClass(), LastItemSeen->GetSocketLocation(FName("TriangleDOWN")), LastItemSeen->GetSocketRotation(FName("TriangleDOWN")));
				break;
			default: PRINT(TEXT("ERORR!"));
		}
		
		if (Cast<AMyActorWithDinamicConstrain>(Owner))
			Cast<AMyActorWithDinamicConstrain>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<AElbow>(Owner))
			Cast<AElbow>(Owner)->MakeDynamicConnection(SpawnedActor, LastItemSeen);
		if (Cast<AMyActor>(Owner))
			Cast<AMyActor>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<ABladeMotor>(Owner))
			Cast<ABladeMotor>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<ABaseActor>(Owner))
			Cast<ABaseActor>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<ABladeMotor2>(Owner))
			Cast<ABladeMotor2>(Owner)->MakeDynamicConnection(SpawnedActor);
		if (Cast<AElbow2>(Owner))
			Cast<AElbow2>(Owner)->MakeDynamicConnection(SpawnedActor, LastItemSeen);
		if (Cast<APiston>(Owner))
			Cast<APiston>(Owner)->MakeDynamicConnection(SpawnedActor, LastItemSeen);
	}
	else
	{
		FVector SpawnLocation = PlayerViewPointRotation.Vector() * Reach / 10 + PlayerViewPointLocation;
		SpawnedActor = GetWorld()->SpawnActor<AMyActor>(AMyActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	}
}

void UDPMainComponent::Spawn(APiston* SpawnedActor)
{
	if (LastItemSeen)
	{

	}
	else
	{
		FVector SpawnLocation = PlayerViewPointRotation.Vector() * Reach / 10 + PlayerViewPointLocation;
		SpawnedActor = GetWorld()->SpawnActor<APiston>(APiston::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	}
}

void UDPMainComponent::ShotPiston()
{
	if (Cast<APiston>(Owner))
		Cast<APiston>(Owner)->shot();
}
void UDPMainComponent::ChoseTypeOfConnection()
{
	Iter2++;
	if (Iter2 > static_cast<int>(ConnectionType::LeftRight))
		Iter2 = 0;
	switch (Iter2)
	{
		case static_cast<int>(ConnectionType::Center) : PRINT(TEXT("Center Connection"));
			break;
		case static_cast<int>(ConnectionType::LeftRight) : PRINT(TEXT("LeftRight Connection"));
			break;
		case static_cast<int>(ConnectionType::UpDown) : PRINT(TEXT("UpDown Connection"));
			break;
		default : PRINT(TEXT("ERORR!"));
	}
}
void UDPMainComponent::ChoseTriangleRotation()
{
	Iter3++;
	if (Iter3 > static_cast<int>(TriangleRotation::DOWN))
		Iter3 = 0;
	switch (Iter3)
	{
		case static_cast<int>(TriangleRotation::LEFT) : PRINT(TEXT("Left Triangle Connection"));
			break;
		case static_cast<int>(TriangleRotation::RIGHT) : PRINT(TEXT("Right Triangle Connection"));
			break;
		case static_cast<int>(TriangleRotation::UP) : PRINT(TEXT("Up Connection"));
			break;
		case static_cast<int>(TriangleRotation::DOWN) : PRINT(TEXT("Down Connection"));
			break;
		default: PRINT(TEXT("ERORR!"));
	}
}
void UDPMainComponent::ChoseElbowRotation()
{
	Iter4++;
	if (Iter4 > static_cast<int>(ElbowRotation::LeftRight))
		Iter4 = 0;
	switch (Iter4)
	{
		case static_cast<int>(ElbowRotation::UpDown) : PRINT(TEXT("UpDown Elbow Connection"));
			break;
		case static_cast<int>(ElbowRotation::LeftRight) : PRINT(TEXT("LeftRight Elbow Connection"));
			break;
		default: PRINT(TEXT("ERORR!"));
	}
}
/**/
			//	if((Elem != Cast<AMyActorWithDinamicConstrain>(Owner)->BoxMesh) && 
			//	(Elem != Cast<AMyActorWithDinamicConstrain>(Owner)->Plate1) && 
			//	(Elem != Cast<AMyActorWithDinamicConstrain>(Owner)->Plate2) &&
			//	(Elem != Cast<AMyActorWithDinamicConstrain>(Owner)->Plate3) &&
			//	(Elem != Cast<AMyActorWithDinamicConstrain>(Owner)->Plate4) &&
			//	(Elem != Cast<AMyActorWithDinamicConstrain>(Owner)->Plate5) &&
			//	(Elem != Cast<AMyActorWithDinamicConstrain>(Owner)->Plate6))