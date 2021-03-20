// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardSpawn.generated.h"

UCLASS()
class CHESTERCHESS_API ABoardSpawn : public AActor
{
	GENERATED_BODY()

		/*UPROPERTY(VisibleDefaultsOnly, Category = Root)
		class URootComponent* Root;*/

		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* Floor;

		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* ChessSquare;

		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* ChessPiece;

		
		class UMaterialInstance* BlackMaterial;
		class UMaterial* WhiteMat;
		class UMaterial* GoldMesh;
		class UMaterial* BlackSquares;


		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
			TArray<UStaticMeshComponent*> Pieces;

		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
			TArray<FVector> Squares;

		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
			TArray<UStaticMeshComponent*> SquaresRef;

		UPROPERTY(VisibleDefaultsOnly, Category = Positions)
			TMap<int32, FString> PiecesMap;
		
		/*UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UMaterial* PieceColor;*/
	
public:	
	// Sets default values for this actor's properties
	ABoardSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Pieces)
		TArray<int> PiecesPositioning;

	UPROPERTY(EditAnywhere, Category = Pieces)
		int LastIndex;

	UFUNCTION()
	void BoardGeneration();

	UFUNCTION()
	void Clicking(FVector Position);

	UPROPERTY(EditAnywhere, Category = PieceForSpawning)
		TSubclassOf<class APiece> PieceToSpawn;
	
	TArray<APiece*> PiecesArray;
	TArray<int32> CurrentMoves;


};
