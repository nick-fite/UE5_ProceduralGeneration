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
	UPROPERTY(EditAnywhere, Category = "Chunk")
	int ChunkNumX = 1;
	UPROPERTY(EditAnywhere, Category = "Chunk")
	int ChunkNumY = 1;
	UPROPERTY(EditAnywhere, Category = "Chunk")
	int Size = 32;
	UPROPERTY(EditAnywhere, Category = "Chunk")
	float heightMulti = 1;
	UPROPERTY(EditAnywhere, Category = "Chunk")
	int Scale = 1;
	UPROPERTY(EditAnywhere, Category = "Chunk")
	float TerrainPerlinNoiseFrequency = .01;

	UPROPERTY(EditDefaultsOnly, Category="Chunk")
	TObjectPtr<UMaterial> DesertMaterial;
	UPROPERTY(EditDefaultsOnly, Category="Chunk")
	TObjectPtr<UMaterial> PlaneMaterial;
	UPROPERTY(EditDefaultsOnly, Category="Chunk")
	TObjectPtr<UMaterial> ForestMaterial;
	UPROPERTY(EditDefaultsOnly, Category="Chunk")
	TObjectPtr<UMaterial> SnowMaterial;

	UFUNCTION()
	void StartGen(AChunk* chunk, const FTransform& Transform);

	int32 TestFunc();
	
	CustomPerlin::FNoiseGenerator2D NoiseGenerator;
};

