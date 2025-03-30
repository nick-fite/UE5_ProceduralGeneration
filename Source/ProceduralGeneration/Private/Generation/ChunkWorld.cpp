// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/ChunkWorld.h"
#include "Chunk.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChunkWorld::AChunkWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//random seed for biomes
	BiomeNoiseGenerator.BaseSeed = 2352;
	BiomeNoiseGenerator.Frequency = PlaneNoiseFrequency;

	StructureNoiseGenerator.BaseSeed = 5634;
	StructureNoiseGenerator.Frequency = PlaneNoiseFrequency;
}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	Super::BeginPlay();
	//FVector Loc = GetActorLocation();
	for (int x = 0; x < ChunkNumX; x++)
	{
		for (int y = 0; y < ChunkNumY; y++)
		{
			FTransform Transform = FTransform(
            FRotator(0, 0, 0),
            FVector(x * Size * 100, y * Size * 100, 0),
            FVector::OneVector);

			AChunk* chunk = GetWorld()->SpawnActorDeferred<AChunk>(AChunk::StaticClass(), Transform, this);
			
			const float height = BiomeNoiseGenerator.GetNoise(x * Size,y * Size);
			if (height >= -1 && height <= -0.5)
			{
				UE_LOG(LogTemp, Display, TEXT("Snow"));
				chunk->Biome = EBiome::Snow;
				chunk->Material = SnowMaterial;
				chunk->TerrainPerlinNoiseFrequency = SnowNoiseFrequency;
			}
			else if (height > -0.5 && height <= 0)
			{
				UE_LOG(LogTemp, Display, TEXT("Planes"));
				chunk->Biome = EBiome::PLanes;
				chunk->Material = PlaneMaterial;
				chunk->TerrainPerlinNoiseFrequency = PlaneNoiseFrequency;
			}
			else if (height > 0 && height <= 0.5)
			{
				UE_LOG(LogTemp, Display, TEXT("Forest"));
				chunk->Biome = EBiome::Forest;
				chunk->Material = ForestMaterial;
				chunk->TerrainPerlinNoiseFrequency = ForestNoiseFrequency;
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Desert"));
				chunk->Biome = EBiome::Desert;
				chunk->Material = DesertMaterial;
				chunk->TerrainPerlinNoiseFrequency = DesertNoiseFrequency;
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Chunk created, changing size"));

			chunk->Size = Size;
			chunk->HeightMulti = heightMulti;
			chunk->Scale = Scale;

			// Finish spawning the actor on the game thread
			AsyncTask(ENamedThreads::GameThread, [chunk, Transform]()
			{
				UGameplayStatics::FinishSpawningActor(chunk, Transform);
			});
		}
	}
}

// Called every frame
void AChunkWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunkWorld::StartGen(AChunk* chunk, const FTransform& Transform)
{
	 UGameplayStatics::FinishSpawningActor(chunk, Transform); 
}

int32 AChunkWorld::TestFunc()
{
	return 4;
}
