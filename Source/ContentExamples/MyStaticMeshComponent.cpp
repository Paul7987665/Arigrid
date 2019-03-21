// Fill out your copyright notice in the Description page of Project Settings.
#include "MyStaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

UMyStaticMeshComponent::UMyStaticMeshComponent()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Arigrid/Plane.Plane'"));
	if (MeshAsset.Succeeded())
	{
		SetStaticMesh(MeshAsset.Object);
	}
}

void UMyStaticMeshComponent::SetGlowEffect(bool Status)
{
	SetRenderCustomDepth(Status);
}
