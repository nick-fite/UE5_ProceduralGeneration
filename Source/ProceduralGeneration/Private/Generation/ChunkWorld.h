// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/CustomPerlin.h"
#include "ChunkWorld.generated.h"

class AChunk;



UCLASS()
class AChunkWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkWorld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "World")
	int ChunkNumX = 1;
	UPROPERTY(EditAnywhere, Category = "World")
	int ChunkNumY = 1;
	UPROPERTY(EditAnywhere, Category = "World")
	int Size = 32;
	UPROPERTY(EditAnywhere, Category = "World")
	float heightMulti = 1;
	UPROPERTY(EditAnywhere, Category = "World")
	int Scale = 1;
	UPROPERTY(EditAnywhere, Category = "World")
	int TerrainSeed = 1337;
	
	UPROPERTY(EditAnywhere, Category = "PlanesBiome")
	float PlaneNoiseFrequency = .008;
	UPROPERTY(EditDefaultsOnly, Category="PlanesBiome")
	TObjectPtr<UMaterial> PlaneMaterial;
	
	UPROPERTY(EditAnywhere, Category = "DesertBiome")
	float DesertNoiseFrequency = .006;
	UPROPERTY(EditDefaultsOnly, Category="DesertBiome")
	TObjectPtr<UMaterial> DesertMaterial;

	UPROPERTY(EditAnywhere, Category = "SnowBiome")
	float SnowNoiseFrequency = .01;
	UPROPERTY(EditDefaultsOnly, Category="SnowBiome")
	TObjectPtr<UMaterial> SnowMaterial;

	UPROPERTY(EditAnywhere, Category = "ForestBiome")
	float ForestNoiseFrequency = .02;
	UPROPERTY(EditDefaultsOnly, Category="ForestBiome")
	TObjectPtr<UMaterial> ForestMaterial;

	
	CustomPerlin::FNoiseGenerator BiomeNoiseGenerator;
};

