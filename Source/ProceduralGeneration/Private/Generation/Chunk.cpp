// Fill out your copyright notice in the Description page of Project Settings.

#include "Chunk.h"

#include "ProceduralGeneration/BlocksEnum.h"
#include "ProceduralGeneration/DirectionEnum.h"
#include "ProceduralGeneration/StructurePositions.h"
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

	NoiseGenerator.Frequency = TerrainPerlinNoiseFrequency;
	NoiseGenerator.BaseSeed = TerrainSeed;

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

				//makes cave floor
				if (z == 0)
				{
					CaveBlocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
					continue;
				}
				
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

			int Height = FMath::Clamp(FMath::RoundToInt((NoiseGenerator.GetNoise(XPos, YPos) + 1) * MaxHeight / 2),
			                                0, MaxHeight);


			float structure = FMath::RandRange(0.1, 1.0);
			//UE_LOG(LogTemp, Warning, TEXT("%f"), structure);
			
			if (structure > .999f)
			{
				for (int z = 0; z < MaxHeight; ++z)
				{
					TerrainBlocks[GetBlockIndex(x,y,z)]	= EBlock::Structure;
					
				}
				continue;
			}

			for (int z = 0; z < Size; z++)
			{
				if (z < Height)
				{
					if (z == Height - 1)
					{
						TerrainBlocks[GetBlockIndex(x, y, z)] = EBlock::Grass;
					}
					else if (z < 3)
					{
						TerrainBlocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
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
	//loops through all the axes
	for (int Axis = 0; Axis < 3; ++Axis)
	{
		//gets the other axes
		const int Axis1 = (Axis + 1) % 3;
		const int Axis2 = (Axis + 2) % 3;

		/*
		 * order axises are called:
			In a nutshell:
			When Axis = 0 (x is the Main Axis):
			Axis1 = y
			Axis2 = z

			When Axis = 1 (y is the Main Axis):
			Axis1 = z
			Axis2 = x

			When Axis = 2 (z is the Main Axis):
			Axis1 = x
			Axis2 = y
		 */
		

		//defines the limit for each axis
		int MainAxisLimit = Size;
		int Axis1Limit = Size;
		int Axis2Limit = Size;

		//increase the axis in Z direction if needed
		if (Axis == 0)
		{
			Axis2Limit *= (UseMaxHeight ? HeightMulti : 1);
		}
		else if (Axis == 1)
		{
			Axis1Limit *= (UseMaxHeight ? HeightMulti : 1);
		}
		else if (Axis == 2)
		{
			MainAxisLimit *= (UseMaxHeight ? HeightMulti : 1);
		}
		

		//stores displacement of vectors
		FIntVector DeltaAxis1 = FIntVector::ZeroValue;
		FIntVector DeltaAxis2 = FIntVector::ZeroValue;

		//used to traverse the grid
		FIntVector ChunkItr = FIntVector(0,0, 0);
		//used to shift to different axis
		FIntVector AxisMask = FIntVector::ZeroValue;
		//set it to the first one
		AxisMask[Axis] = 1;
		//holds the region of the grid
		TArray<FMask> Mask;
		Mask.SetNum(Axis1Limit * Axis2Limit * MainAxisLimit);

		// start looping
		for (ChunkItr[Axis] =  -1; ChunkItr[Axis] < MainAxisLimit;)
		{
			int N = 0;
			//goes through the other 2 axes
			for (ChunkItr[Axis2] = 0; ChunkItr[Axis2] < Axis2Limit; ++ChunkItr[Axis2])
			{
				for (ChunkItr[Axis1] = 0; ChunkItr[Axis1] < Axis1Limit; ++ChunkItr[Axis1])
				{
					//gets the block and the block in the axis mask direction
					const auto CurrentBlock = GetBlock(BlocksToGenerate, ChunkItr);
					const auto CompareBlock = GetBlock(BlocksToGenerate, ChunkItr + AxisMask);

					//checks to see if they're being blocked
					const bool CurrentBlockOpaque = CurrentBlock != EBlock::Air;
					const bool CompareBlockOpaque = CompareBlock != EBlock::Air;

					//if they're both the same set it to null, if it's not keep the data
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

			//up 1
			++ChunkItr[Axis];
			//reset
			N = 0;

			
			for (int j = 0; j < Axis2Limit*(UseMaxHeight ? HeightMulti : 1); ++j)
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
	UE_LOG(LogTemp, Warning, TEXT("Vertices: %d"), Vertices.Num());
	UE_LOG(LogTemp, Warning, TEXT("Triangles: %d"), Triangles.Num());
	UE_LOG(LogTemp, Warning, TEXT("Normals: %d"), Normals.Num());
	UE_LOG(LogTemp, Warning, TEXT("UVData: %d"), UVData.Num());
	UE_LOG(LogTemp, Warning, TEXT("Color: %d"), Colors.Num());
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
		GenerateMesh(TerrainBlocks, Size, false);
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
	if (UseHeightMult)
    {
        V1.Z *= HeightMulti; 
        V2.Z *= HeightMulti; 
        V3.Z *= HeightMulti; 
        V4.Z *= HeightMulti; 
    }
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
		case EBlock::Structure : return 3;
		default: return 255;
	}
}

void AChunk::ModifyMesh(FIntVector Pos, EBlock Block)
{
	const int Index = Pos.X + (Pos.Y * Size) + (Pos.Z * Size * Size);
	UE_LOG(LogTemp, Display, TEXT("ModifyMesh Block %d"), Index);
	UE_LOG(LogTemp, Display, TEXT("ModifyMesh terrain length %d"), TerrainBlocks.Num());
	if (!EditCave)
		if (Index < 0 || Index > TerrainBlocks.Num()) return;
	else
		if (Index < 0 || Index > CaveBlocks.Num()) return;

	UE_LOG(LogTemp, Display, TEXT("ModifyMesh Block %d success"), Index);
	EBlock temp = GetBlock(TerrainBlocks, Pos);
	switch (temp)
	{
		case EBlock::Grass:
			UE_LOG(LogTemp, Display, TEXT("Grass"));
		case EBlock::Dirt:
			UE_LOG(LogTemp, Display, TEXT("Dirt"));
		case EBlock::Stone:
			UE_LOG(LogTemp, Display, TEXT("Stone"));
		case EBlock::Structure:
			UE_LOG(LogTemp, Display, TEXT("Structure"));
		case EBlock::Air:
			UE_LOG(LogTemp, Display, TEXT("Air"));

		default:
			UE_LOG(LogTemp, Display, TEXT("Unknown"));
	}

	if (EditCave)
	{
		CaveBlocks[Index] = Block;
		return;
	}
	TerrainBlocks[Index] = Block;
	temp = GetBlock(TerrainBlocks, Pos);
	switch (temp)
	{
		case EBlock::Grass:
			UE_LOG(LogTemp, Display, TEXT("Grass"));
		case EBlock::Dirt:
			UE_LOG(LogTemp, Display, TEXT("Dirt"));
		case EBlock::Stone:
			UE_LOG(LogTemp, Display, TEXT("Stone"));
		case EBlock::Structure:
			UE_LOG(LogTemp, Display, TEXT("Structure"));
		case EBlock::Air:
			UE_LOG(LogTemp, Display, TEXT("Air"));

		default:
			UE_LOG(LogTemp, Display, TEXT("Unknown"));
	}


	UE_LOG(LogTemp, Error, TEXT("ALL ITEMS"));
	/*for (EBlock block : TerrainBlocks)
	{
	switch (block)
	{
		case EBlock::Grass:
			UE_LOG(LogTemp, Display, TEXT("Grass"));
		continue;
		case EBlock::Dirt:
			UE_LOG(LogTemp, Display, TEXT("Dirt"));
		continue;
		case EBlock::Stone:
			UE_LOG(LogTemp, Display, TEXT("Stone"));
		continue;
		case EBlock::Structure:
			UE_LOG(LogTemp, Display, TEXT("Structure"));
		continue;
		case EBlock::Air:
			UE_LOG(LogTemp, Display, TEXT("Air"));
		continue;

		default:
			UE_LOG(LogTemp, Display, TEXT("Unknown"));
		continue;
	}
		
	}*/
}

int AChunk::GetBlockIndex(FIntVector Pos)
{
    return Pos.X + (Pos.Y * Size) + (Pos.Z * Size * Size);
	
	//return Pos.Z * Size * Size + Pos.Y * Size + Pos.X;
	
}

void AChunk::ClearMesh()
{
	UE_LOG(LogTemp, Display, TEXT("Clearing Mesh"));
	VertexCount = 0;
	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	UVData.Empty();
	Colors.Empty();
}

void AChunk::ModifyVoxel(FIntVector Pos)
{
	UE_LOG(LogTemp, Warning, TEXT("AChunk::ModifyVoxel started"));
	
	//if (Pos.X <= Size || Pos.Y <= Size || Pos.X > 0 || Pos.Y > 0) return;
	
	UE_LOG(LogTemp, Warning, TEXT("AChunk::ModifyVoxel Success"));
	VertexCount = 0;
	Vertices = TArray<FVector>();
	Triangles = TArray<int32>();
	Normals = TArray<FVector>();
	UVData = TArray<FVector2D>();
	Colors = TArray<FColor>();
	Mesh->ClearMeshSection(0);

	ModifyMesh(Pos, EBlock::Air);

	GenerateMesh(CaveBlocks, 0, false);
	GenerateMesh(TerrainBlocks, Size, false);

	ApplyMesh();
}

FIntVector AChunk::GetBlockPos(FVector pos)
{
	FIntVector Result;
	int factor = Size * 100;
	FIntVector intPos = FIntVector(pos);
	
	if (intPos.X < 0)
	{
		Result.X  =  static_cast<int>(pos.X / factor) - 1;
	}
	else
	{
		Result.X = static_cast<int>(pos.X / factor);
	}

	if (intPos.Y < 0)
	{
		Result.Y  = static_cast<int>(pos.X / factor) - 1;
	}
	else
	{
		Result.Y = static_cast<int>(pos.Y / factor);
	}
	if (intPos.Z < 0)
	{
		Result.Z  = static_cast<int>(pos.X / factor) - 1;
	}
	else
	{
		Result.Z = static_cast<int>(pos.Y / factor);
	}

	
	FIntVector Result2 = FIntVector(pos)/100 - Result * Size;

	if (Result.X < 0) Result2.X--;
	if (Result.Y < 0) Result2.Y--;
	if (Result.Z < 0) Result2.Z--;

	Result2.Z -= Size;

	if (Result2.Z < 0)
	{
		EditCave = true;
		Result2.Z += Size;
	}
	else
	{
		EditCave = false;
	}

	
	return Result2;
}