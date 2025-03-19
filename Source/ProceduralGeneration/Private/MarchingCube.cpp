// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingCube.h"

// Sets default values
AMarchingCube::AMarchingCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Voxels.SetNum((Size + 1) * (Size + 1) * (Size + 1));

}

// Called when the game starts or when spawned
void AMarchingCube::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMarchingCube::GenerateHeightMap()
{
	const FVector Pos = GetActorLocation() / 100;

	for (int x = 0; x <= Size; x++)
	{
		
		for (int y = 0; y <= Size; y++)
		{
			for (int z = 0; z <= Size; z++)
			{
				Voxels[getVoxelIndex(x,y,z)] = FMath::PerlinNoise3D(FVector(x + Pos.X, y + Pos.Y, z + Pos.Z));
		
			}
		}
	}
}

void AMarchingCube::GenerateMesh()
{
	if(SurfaceLevel > 0.0f)
	{
		TriangleOrder[0] = 0;
		TriangleOrder[1] = 1;
		TriangleOrder[2] = 2;
	}
	else
	{
		TriangleOrder[0] = 2;
		TriangleOrder[1] = 1;
		TriangleOrder[2] = 0;
	}
	
}

// Called every frame
void AMarchingCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AMarchingCube::getVoxelIndex(int X, int Y, int Z) const
{
	return Z * (Size + 1) * (Size + 1) + Y * (Size + 1) + X; 
}

