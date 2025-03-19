// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BVoxel.generated.h"

UENUM()
enum VoxelType
{
	Top,
	Side,
	Water
};

UCLASS()
class ABVoxel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABVoxel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Cube;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* TopMat;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* SideMat;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* WaterMat;
	VoxelType Type;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(CallInEditor, Category="Voxel")
	void SetTop();
	UFUNCTION(CallInEditor, Category="Voxel")
	void SetSide();
	UFUNCTION(CallInEditor, Category="Voxel")
	void SetWater();

};
