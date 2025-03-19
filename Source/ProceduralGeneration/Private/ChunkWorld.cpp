// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkWorld.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AChunkWorld::AChunkWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChunkWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunkWorld::GenerateWorld()
{
	for(int x = -DrawDist; x <= DrawDist; x++)
	{
		for(int y = -DrawDist; y <= DrawDist; y++)
		{
			for(int z = -DrawDist; z <= DrawDist; z++)
			{
				FTransform Transform = FTransform(FRotator::ZeroRotator,
					FVector(x * Size * 100, y * Size * 100, z * Size * 100), FVector::OneVector);

				AActor* chunk = GetWorld()->SpawnActorDeferred<AMarchingCube>(MarchingCubeType, Transform, this);
				UGameplayStatics::FinishSpawningActor(chunk, Transform);

				AMarchingCube* marchChunk = Cast<AMarchingCube>(chunk);

				if(marchChunk)
				{
					marchChunk->Frequency = Freq;
					marchChunk->Size = Size;
					chunkCount++;
				}
				UGameplayStatics::FinishSpawningActor(chunk, Transform);
				
			}
		}
	}
}

