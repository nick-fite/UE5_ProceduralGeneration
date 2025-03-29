// Fill out your copyright notice in the Description page of Project Settings.

#include "Chunk.h"

#include "ProceduralGeneration/BlocksEnum.h"
#include "ProceduralGeneration/DirectionEnum.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	RootComponent = Mesh;
	//Blocks.SetNum(Size * Size * Size);
	Mesh->SetCastShadow(false);
	
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AChunk::BeginPlay"));
	Blocks.SetNum(Size * Size * Size);
	GenerateCave();
	GenerateMesh();
	ApplyMesh();
	
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AChunk::Check(FVector Pos) const
{
	if (Pos.X >= Size || Pos.Y >= Size || Pos.Z >= Size || Pos.X < 0 || Pos.Y < 0 || Pos.Z < 0)
	{
		return true;
	}
	return Blocks[GetBlockIndex(Pos.X, Pos.Y, Pos.Z)] == EBlock::Air;
}

void AChunk::CreateFace(const EDirection Dir, const FVector& Pos)
{
	Vertices.Append(GetFaceVertices(Dir, Pos));
	UE_LOG(LogTemp, Warning, TEXT("Verticies: %d"), Vertices.Num())
	UVData.Append({FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1)});
	Triangles.Append({VertextCount + 3, VertextCount + 2, VertextCount, VertextCount + 2, VertextCount + 1, VertextCount});
	VertextCount += 4;
}

TArray<FVector> AChunk::GetFaceVertices(EDirection Dir, const FVector& Pos) const
{
	TArray<FVector> NewVertices;
	for (int i = 0; i < 4; i++)
	{
		NewVertices.Add(BlockVertexData[BlockTriangleData[i + static_cast<int>(Dir) * 4]] * Scale + Pos);
	}
	return NewVertices;
}

FVector AChunk::GetPositionInDirection(FVector Pos, EDirection Dir)
{
	switch (Dir)
	{
		case EDirection::Forward: return Pos + FVector::ForwardVector;
		case EDirection::Back: return Pos + FVector::BackwardVector;
		case EDirection::Right: return Pos + FVector::RightVector;
		case EDirection::Left: return Pos + FVector::LeftVector;
		case EDirection::Up: return Pos + FVector::UpVector;
		case EDirection::Down: return Pos + FVector::DownVector;
		default: UE_LOG(LogTemp, Error, TEXT("DIR NOT VALID"))  return Pos;
	}
}

/*FVector AChunk::GetNormal(FVector Pos)
{
}*/

int AChunk::GetBlockIndex(int X, int Y, int Z) const
{
	return Z * Size * Size + Y * Size + X;
}

void AChunk::GenerateCave()
{
	UE_LOG(LogTemp, Warning, TEXT("AChunk::GenerateBlocks"));
	const FVector Loc = GetActorLocation();
	for (int x = 0; x < Size; ++x)
	{
		for (int y = 0; y < Size; ++y)
		{
			for (int z = 0; z < Size; ++z)
			{
				const float XPos = x + .1f + Loc.X;
				const float YPos = y + .1f + Loc.Y;
				const float ZPos = z + .1f + Loc.Z;

				const int val = FMath::Clamp(FMath::RoundToInt((FMath::PerlinNoise3D(FVector(XPos * .1, YPos * .1, ZPos * .1)) + 1) * Size / 2), 0, Size);
				UE_LOG(LogTemp, Warning, TEXT("%f , %f: Block %d"), XPos, YPos, val);
				if (val > Size/2)
					Blocks[GetBlockIndex(x,y,z)] = EBlock::Stone;
				else
					Blocks[GetBlockIndex(x,y,z)] = EBlock::Air;
			}
			/*const float XPos = x + .1f + Loc.X;
			const float YPos = y + .1f + Loc.Y;

			UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f"), XPos, YPos);

			const int Height = FMath::Clamp(FMath::RoundToInt((FMath::PerlinNoise3D(FVector(XPos, YPos, 0)) + 1) * Size / 2), 0, Size);
			UE_LOG(LogTemp, Warning, TEXT("Height: %d"), Height);

			for (int z = 0; z < Size; z++)
			{
				if (z < Height)
					Blocks[GetBlockIndex(x,y,z)] = EBlock::Stone;
				else
					Blocks[GetBlockIndex(x,y,z)] = EBlock::Air;
			}*/
		}
	}
}

void AChunk::GenerateMesh()
{
	UE_LOG(LogTemp, Warning, TEXT("AChunk::GenerateMesh"));
	for (int x = 0; x < Size; ++x)
	{
		 for (int y = 0; y < Size; ++y)
		 {
			 for (int z = 0; z < Size; ++z)
			 {
				 if (Blocks[GetBlockIndex(x,y,z)] == EBlock::Air)
				 	continue;

			 	const FVector Pos = FVector(x, y, z);
				for (const EDirection Dir : {EDirection::Forward, EDirection::Right, EDirection::Back, EDirection::Left, EDirection::Up, EDirection::Down})
				{
					if (Check(GetPositionInDirection(Pos, Dir)))
					{
						CreateFace(Dir, Pos * 100);
					}
				}
			 }
		 }
	}
}

void AChunk::ApplyMesh() const
{
	UE_LOG(LogTemp, Warning, TEXT("AChunk::ApplyMesh"));
	Mesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVData, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

void AChunk::Regenerate()
{
	VertextCount = 0;
	Vertices = TArray<FVector>();
	Mesh->ClearMeshSection(0);
	GenerateCave();
	GenerateMesh();
	ApplyMesh();
}

