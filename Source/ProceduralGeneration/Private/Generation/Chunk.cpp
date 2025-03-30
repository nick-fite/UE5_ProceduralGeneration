// Fill out your copyright notice in the Description page of Project Settings.

#include "Chunk.h"

#include "ProceduralGeneration/BlocksEnum.h"
#include "ProceduralGeneration/DirectionEnum.h"
#include "Utils/CustomPerlin.h"

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

	//UE_LOG(LogTemp, Warning, TEXT("AChunk::BeginPlay"));

	NoiseGenerator2D.Frequency = TerrainPerlinNoiseFrequency;

	Generate();
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EBlock AChunk::GetBlock(TArray<EBlock> Blocks, FIntVector Pos) const
{
	if (Pos.X >= Size || Pos.Y >= Size || Pos.Z >= Size || Pos.X < 0 || Pos.Y < 0 || Pos.Z < 0)
	{
		return EBlock::Air;
	}
	return Blocks[GetBlockIndex(Pos.X, Pos.Y, Pos.Z)];
}

void AChunk::CreateFace(const EDirection Dir, const FVector& Pos)
{
	Vertices.Append(GetFaceVertices(Dir, Pos));
	Colors.Append({FColor::Red, FColor::Red, FColor::Red, FColor::Red});
	UVData.Append({FVector2D(1, 1), FVector2D(1, 0), FVector2D(0, 0), FVector2D(0, 1)});
	Triangles.Append({VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount});
	VertexCount += 4;
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
	default: UE_LOG(LogTemp, Error, TEXT("DIR NOT VALID"))
		return Pos;
	}
}

int AChunk::GetBlockIndex(int X, int Y, int Z) const
{
	return Z * Size * Size + Y * Size + X;
}

void AChunk::GenerateCave()
{
	//UE_LOG(LogTemp, Warning, TEXT("AChunk::GenerateBlocks"));
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

				const int val = FMath::Clamp(
					FMath::RoundToInt((FMath::PerlinNoise3D(FVector(XPos * .1, YPos * .1, ZPos * .1)) + 1) * Size / 2),
					0, Size);
				//UE_LOG(LogTemp, Warning, TEXT("%f , %f: Block %d"), XPos, YPos, val);
				if (val > Size / 2)
					CaveBlocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
				else
					CaveBlocks[GetBlockIndex(x, y, z)] = EBlock::Air;
			}
		}
	}
}

void AChunk::GenerateTerrain()
{
	//UE_LOG(LogTemp, Warning, TEXT("AChunk::GenerateTerrain"));
	FVector Loc = GetActorLocation() / 100;
	Loc = FVector(Loc.X, Loc.Y, Loc.Z + Size);
	int32 MaxHeight = Size * HeightMulti;
	for (int x = 0; x < Size; ++x)
	{
		for (int y = 0; y < Size; ++y)
		{
			const float XPos = x + Loc.X + .1;
			const float YPos = y + Loc.Y + .1;

			//UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f"), XPos, YPos);

			const int Height = FMath::Clamp(FMath::RoundToInt((NoiseGenerator2D.GetNoise(XPos, YPos) + 1) * MaxHeight / 2),
			                                0, MaxHeight);

			//UE_LOG(LogTemp, Warning, TEXT("Height: %d"), Height);

			for (int z = 0; z < MaxHeight; z++)
			{
				if (z < Height)
				{
					if (z == Height - 1)
					{
						TerrainBlocks[GetBlockIndex(x, y, z)] = EBlock::Grass;
					}
					else
					{
						TerrainBlocks[GetBlockIndex(x, y, z)] = EBlock::Dirt;
						
					}
				}
				else
					TerrainBlocks[GetBlockIndex(x, y, z)] = EBlock::Air;
			}
		}
	}
}

void AChunk::GenerateMesh(const TArray<EBlock>& BlocksToGenerate, int ZOffset, bool UseMaxHeight)
{
	//UE_LOG(LogTemp, Warning, TEXT("AChunk::GenerateMesh"));
	for (int Axis = 0; Axis < 3; ++Axis)
	{
		const int Axis1 = (Axis + 1) % 3;
		const int Axis2 = (Axis + 2) % 3;

		const int MainAxisLimit = Size;
		const int Axis1Limit = Size;
		const int Axis2Limit = Size;

		FIntVector DeltaAxis1 = FIntVector::ZeroValue;
		FIntVector DeltaAxis2 = FIntVector::ZeroValue;

		FIntVector ChunkItr = FIntVector(0,0, 0);
		FIntVector AxisMask = FIntVector::ZeroValue;

		AxisMask[Axis] = 1;

		TArray<FMask> Mask;
		Mask.SetNum(Axis1Limit * Axis2Limit);

		for (ChunkItr[Axis] =  -1; ChunkItr[Axis] < MainAxisLimit;)
		{
			int N = 0;

			for (ChunkItr[Axis2] = 0; ChunkItr[Axis2] < Axis2Limit; ++ChunkItr[Axis2])
			{
				for (ChunkItr[Axis1] = 0; ChunkItr[Axis1] < Axis1Limit; ++ChunkItr[Axis1])
				{
					const auto CurrentBlock = GetBlock(BlocksToGenerate, ChunkItr);
					const auto CompareBlock = GetBlock(BlocksToGenerate, ChunkItr + AxisMask);

					const bool CurrentBlockOpaque = CurrentBlock != EBlock::Air;
					const bool CompareBlockOpaque = CompareBlock != EBlock::Air;

					if (CurrentBlockOpaque == CompareBlockOpaque)
					{
						Mask[N++] = FMask{EBlock::Null, 0};
					}
					else if (CurrentBlockOpaque)
					{
						Mask[N++] = FMask{CurrentBlock, 1};
					}
					else
					{
						Mask[N++] = FMask{CompareBlock, -1};
					}
				}
			}
		
			++ChunkItr[Axis];
			N = 0;
			
			for (int j = 0; j < Axis2Limit; ++j)
			{
				for (int i = 0; i < Axis1Limit;)
				{
					if (Mask[N].Normal != 0)
					{
						const FMask currentMask = Mask[N];
						ChunkItr[Axis1] = i;
						ChunkItr[Axis2] = j;

						int Width;
						
						for (Width = 1; i + Width < Axis1Limit && CompareMask(Mask[N + Width], currentMask); ++Width){}
					
						int Height;
						bool Done = false;

						for (Height = 1; j + Height < Axis2Limit; ++Height)
						{
							for (int k = 0; k < Width; ++k)
							{
								if (CompareMask(Mask[N + k + Height * Axis1Limit], currentMask))
								{
									continue;
								}
								Done = true;
								break;
							}
						
							if (Done) break;
						}

						DeltaAxis1[Axis1] = Width;
						DeltaAxis2[Axis2] = Height;

						CreateQuad(
							currentMask,
							AxisMask,
							Width,
							Height,
							ChunkItr,
							ChunkItr + DeltaAxis1,
							ChunkItr + DeltaAxis2,
							ChunkItr + DeltaAxis1 + DeltaAxis2,
							ZOffset,
							UseMaxHeight
						);
						DeltaAxis1 = FIntVector::ZeroValue;
						DeltaAxis2 = FIntVector::ZeroValue;

						for (int l = 0; l < Height; ++l)
						{
							for (int k = 0; k < Width; ++k)
							{
								Mask[N + k + l * Axis1Limit] = FMask{EBlock::Null, 0};
							}
						}
						i += Width;
						N += Width;
					}
					else
					{
						i++;
						N++;
					}
				}
			}
		}
	}

	//old naive generation. Not as good.
	/*for (int x = 0; x < Size; ++x)
	{
		for (int y = 0; y < Size; ++y)
		{
			for (int z = 0; z < (UseMaxHeight ? static_cast<int>(Size * HeightMulti) : Size); ++z)
			{
				if (BlocksToGenerate[GetBlockIndex(x, y, z)] == EBlock::Air)
					continue;

				const FVector Pos = FVector(x, y, z + ZOffset);
				for (const EDirection Dir : {
						EDirection::Forward, EDirection::Right, EDirection::Back,
						EDirection::Left, EDirection::Up, EDirection::Down
				     })
				{
					if (z >= CaveBlocks.Num())
					{
						return;
					}
					if (Check(GetPositionInDirection(Pos, Dir)))
					{
						CreateFace(Dir, Pos * 100);
					}
				}
			}
		}
	}*/
}

void AChunk::ApplyMesh() const
{
	//UE_LOG(LogTemp, Warning, TEXT("AChunk::ApplyMesh"));
	Mesh->SetMaterial(0, Material);
	Mesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVData, Colors, TArray<FProcMeshTangent>(),
	                        true);
}

void AChunk::Generate()
{
	const int MaxHeight = Size * HeightMulti;
	CaveBlocks.SetNum(Size * Size * MaxHeight);
	TerrainBlocks.SetNum(Size * Size * MaxHeight);
	Colors.SetNum(Size * Size * MaxHeight);
	VertexCount = 0;
	Vertices = TArray<FVector>();
	Colors = TArray<FColor>();
	Mesh->ClearMeshSection(0);

	GenerationsCompleted = 0;
	//GenerateCave();
	//GenerateMesh(CaveBlocks, 0, false);
	//GenerateTerrain();
	//GenerateMesh(TerrainBlocks, Size, false);
	ApplyMesh();

	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		GenerateCave();
		++GenerationsCompleted;
		CheckGenerationCompleted();
		
	});
	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		GenerateTerrain();
		++GenerationsCompleted;
		CheckGenerationCompleted();
	});
	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		GenerateMesh(CaveBlocks, 0, false);
		++GenerationsCompleted;
		CheckGenerationCompleted();
	});
	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		GenerateMesh(TerrainBlocks, Size, true);
		++GenerationsCompleted;
		CheckGenerationCompleted();
	});
}

bool AChunk::CompareMask(const FMask M1, const FMask M2) const
{
	return M1.Block == M2.Block && M1.Normal == M2.Normal;
}

void AChunk::CreateQuad(FMask Mask, FIntVector AxisMask, int Width, int Height, FIntVector V1, FIntVector V2,
	FIntVector V3, FIntVector V4, int ZOffset, bool UseHeightMult)
{
	const FVector Normal = FVector(AxisMask * Mask.Normal);
	
	Vertices.Append({
		FVector(V1.X, V1.Y, (V1.Z + ZOffset)) * 100,
		FVector(V2.X, V2.Y, (V2.Z + ZOffset)) * 100,
		FVector(V3.X, V3.Y, (V3.Z + ZOffset)) * 100,
		FVector(V4.X, V4.Y, (V4.Z + ZOffset)) * 100,
	});
	Triangles.Append({
		VertexCount,
		VertexCount + 2 + Mask.Normal,
		VertexCount + 2 - Mask.Normal,
		VertexCount + 3,
		VertexCount + 1 - Mask.Normal,
		VertexCount + 1 + Mask.Normal
	});
	Normals.Append({
		Normal,
		Normal,
		Normal,
		Normal
	});

	const FColor Color = FColor(0,0,0,GetTextureIndex(Mask.Block));
	Colors.Append({
		Color,
		Color,
		Color,
		Color
	});
	if (Normal.X == 1 || Normal.X == -1)
	{
		UVData.Append({
			FVector2D(Width, Height),
			FVector2D(0, Height),
			FVector2D(Width, 0),
			FVector2D(0, 0),
		});
	}
	else
	{
		UVData.Append({
			FVector2D(Height, Width),
			FVector2D(Height, 0),
			FVector2D(0, Width),
			FVector2D(0, 0),
		});
	}

	VertexCount += 4;
}

void AChunk::CheckGenerationCompleted() const
{
	if (GenerationsCompleted == TotalGenerations)
	{
		ApplyMesh();
	}
}

int AChunk::GetTextureIndex(EBlock BlockType) const
{
	switch (BlockType)
	{
		case EBlock::Grass: return 0;
		case EBlock::Dirt: return 1;
		case EBlock::Stone : return 2;
		default: return 255;
	}
}
