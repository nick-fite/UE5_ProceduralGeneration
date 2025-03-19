// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MarchingCube.h"
#include "GameFramework/Actor.h"
#include "ChunkWorld.generated.h"

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
	UPROPERTY(EditDefaultsOnly, Category="World")
	TSubclassOf<AMarchingCube> MarchingCubeType;
	
	UPROPERTY(EditDefaultsOnly, Category="World")
	int DrawDist = 5;

	UPROPERTY(EditDefaultsOnly, Category="World")
	TObjectPtr<UMaterialInstance> Mat;

	UPROPERTY(EditDefaultsOnly, Category="Chunk")
	int Size = 500;

	UPROPERTY(EditDefaultsOnly, Category="Height")
	float Freq = 0.03f;

	int chunkCount;
	UFUNCTION(CallInEditor)
	void GenerateWorld();
};
