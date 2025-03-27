// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestMesh.generated.h"

class UProceduralMeshComponent;

//Running into interesting issues, pulling this straight from https://superyateam.com/2021/06/26/how-to-use-proceduralmeshcomponent-in-ue4/
UCLASS()
class ATestMesh : public AActor
{
	GENERATED_BODY()
private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;
    
	UPROPERTY()
	UProceduralMeshComponent* ProcMesh;
    
	void CreateMesh();
public:	
	// Sets default values for this actor's properties
	ATestMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
