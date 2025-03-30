// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Utils/CustomPerlin.h"
#include "Chunk.generated.h"


enum class EDirection;
enum class EBlock;

UCLASS()
class AChunk : public AActor
{
	GENERATED_BODY()

	struct FMask
	{
		EBlock Block;
		int Normal;
	};
	
public:	
	// Sets default values for this actor's properties
	AChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

/********************/
/*    chunk vals    */
/********************/
public:
	UPROPERTY(EditDefaultsOnly, Category = "Chunk")
	int32 Size = 32;
	UPROPERTY(EditDefaultsOnly, Category = "Chunk")
	int Scale = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Chunk")
	float TerrainPerlinNoiseFrequency = .01;
	UPROPERTY(EditDefaultsOnly, Category = "Chunk")
	float HeightMulti = 1;

private:
	UPROPERTY()
	UProceduralMeshComponent* Mesh;
	TArray<EBlock> CaveBlocks;
	TArray<EBlock> TerrainBlocks;
	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FVector2D> UVData;
	UPROPERTY()
	TArray<FColor> Colors;

	CustomPerlin::FNoiseGenerator2D NoiseGenerator2D;

	int VertexCount = 0;

	const FVector BlockVertexData[8] = {
		FVector(100,100,100),
		FVector(100,0,100),
		FVector(100,0,0),
		FVector(100,100,0),
		FVector(0,0,100),
		FVector(0,100,100),
		FVector(0,100,0),
		FVector(0,0,0)
	};

	const int BlockTriangleData[24] = {
		0,1,2,3, // Forward
		5,0,3,6, // Right
		4,5,6,7, // Back
		1,4,7,2, // Left
		5,4,1,0, // Up
		3,2,7,6  // Down
	};

	EBlock GetBlock(FVector Pos) const;
	void CreateFace(EDirection Dir, const FVector& Pos);
	TArray<FVector> GetFaceVertices(EDirection Dir, const FVector& Pos) const;
	FVector GetPositionInDirection(FVector Pos, EDirection Dir);
	int GetBlockIndex(int X, int Y, int Z) const;
	void GenerateCave();
	void GenerateTerrain();
	void GenerateMesh(TArray<EBlock> BlocksToGenerate, int ZOffset, bool UseMaxHeight);
	void ApplyMesh() const;

	void Generate();

	//Multithreading :)
	TAtomic<int32> GenerationsCompleted = TAtomic(0);
	int32 TotalGenerations = 4; //we have 4 rounds of generation, if we add more we'll increase this
	void CheckGenerationCompleted() const;
};
