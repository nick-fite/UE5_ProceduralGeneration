// Fill out your copyright notice in the Description page of Project Settings.


#include "BVoxel.h"

// Sets default values
ABVoxel::ABVoxel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Cube = CreateDefaultSubobject<UStaticMeshComponent>("Cube");
	Cube->SetMaterial(0,TopMat);
	Cube->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Type = Top;
}

// Called when the game starts or when spawned
void ABVoxel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABVoxel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABVoxel::SetTop()
{
	Type = Top;
	Cube->SetMaterial(0,TopMat);
	UE_LOG(LogTemp, Warning, TEXT("Setting to Top"));
}

void ABVoxel::SetSide()
{
	Type = Side;
	Cube->SetMaterial(0,SideMat);
	UE_LOG(LogTemp, Warning, TEXT("Setting to Side"));
}

void ABVoxel::SetWater()
{
	Type = Water;
	Cube->SetMaterial(0,WaterMat);
	UE_LOG(LogTemp, Warning, TEXT("Setting to Water"));
}
