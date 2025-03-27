// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Chunk.generated.h"

enum class EDirection;
enum class EBlock;

UCLASS()
class AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Chunk")
	int32 Size = 32;
	UPROPERTY(EditDefaultsOnly, Category = "Chunk")
	int Scale = 1;

	UPROPERTY()
	UProceduralMeshComponent* Mesh;
	TArray<EBlock> Blocks;
	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FVector2D> UVData;

	int VertextCount = 0;

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

	bool Check(FVector Pos) const;
	void CreateFace(EDirection Dir, const FVector& Pos);
	TArray<FVector> GetFaceVertices(EDirection Dir, const FVector& Pos) const;
	FVector GetPositionInDirection(FVector Pos, EDirection Dir);
	//FVector GetNormal(FVector Pos);
	int GetBlockIndex(int X, int Y, int Z) const;
	void GenerateBlocks();
	void GenerateMesh();
	void ApplyMesh() const;
};
