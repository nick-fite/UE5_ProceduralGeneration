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

	for (int x = 0; x <= Size; ++x)
	{
		for (int y = 0; y <= Size; ++y)
		{
			for (int z = 0; z <= Size; ++z)
			{
				Voxels[GetVoxelIndex(x,y,z)] = FMath::PerlinNoise3D(FVector(x + Pos.X, y + Pos.Y, z + Pos.Z));
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

	float Cube[8];
	for(int x = 0; x < Size; ++x)
	{
		for(int y = 0; y < Size; ++y)
		{
			for(int z = 0; z < Size; ++z)
			{
				for(int i = 0; i < 8; ++i)
				{
					Cube[i] = Voxels[GetVoxelIndex(x + VertexOffset[i][0], y + VertexOffset[i][1], z + VertexOffset[i][2])];
				}
				March(x,y,z, Cube);
		
			}
		}
	}
	
}

// Called every frame
void AMarchingCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMarchingCube::March(int X, int Y, int Z, const float Cube[8])
{
	int vertexMask = 0;
	FVector EdgeVertex[12];
	for(int i = 0; i < 8; ++i)
	{
		if(Cube[i] <= SurfaceLevel) vertexMask |= 1 << i;
	}

	const int EdgeMask = CubeEdgeFlags[vertexMask];

	if (EdgeMask == 0) return;

	for(int i = 0; i < 12; ++i)
	{
		if((EdgeMask & 1 << i) != 0)
		{
			const float Offset = Interpolation ? GetInterpolationOffset(Cube[EdgeConnection[i][0]], Cube[EdgeConnection[i][1]]) : 0.5;

			EdgeVertex[i].X = X + (VertexOffset[EdgeConnection[i][0]][0] + Offset * EdgeDirection[i][0]);
			EdgeVertex[i].Y = Y + (VertexOffset[EdgeConnection[i][0]][1] + Offset * EdgeDirection[i][1]);
			EdgeVertex[i].Z = Z + (VertexOffset[EdgeConnection[i][0]][2] + Offset * EdgeDirection[i][2]);
		}
	}

	for(int i = 0; i < 5; ++i)
	{
		if(TriangleConnectionTable[vertexMask][3 * i] < 0) break;

		FVector V1 = EdgeVertex[TriangleConnectionTable[vertexMask][3 * i]] * 100;
		FVector V2 = EdgeVertex[TriangleConnectionTable[vertexMask][3 * i + 1]] * 100;
		FVector V3 = EdgeVertex[TriangleConnectionTable[vertexMask][3 * i + 2]] * 100;

		FVector Normal = FVector::CrossProduct(V2 - V1, V3 - V1);
		Normal.Normalize();

		Vertices.Add(V1);
		Vertices.Add(V2);
		Vertices.Add(V3);

		Triangles.Add(VertexCount + TriangleConnectionTable[0]);
		Triangles.Add(VertexCount + TriangleConnectionTable[1]);
		Triangles.Add(VertexCount + TriangleConnectionTable[2]);

		Normals.Add(Normal);
		Normals.Add(Normal);
		Normals.Add(Normal);

		VertexCount += 3;
	}
	
}

int AMarchingCube::GetVoxelIndex(int X, int Y, int Z) const
{
	return Z * (Size + 1) * (Size + 1) + Y * (Size + 1) + X; 
}

float AMarchingCube::GetInterpolationOffset(float V1, float V2) const
{
	const float Delta = V1 - V2;
	return Delta == 0.f ? SurfaceLevel : (SurfaceLevel -V1) / Delta;
}

