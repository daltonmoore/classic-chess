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
		class UMaterial* CheckedMat;
		class UMaterial* GoldMesh;

		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
			TArray<UMaterial*> SquaresColor;


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
	void PiecesMovement(FVector Position);

	//TArray<int32> CheckForKingMove(int CurrentPos);

	UPROPERTY(EditAnywhere, Category = PieceForSpawning)
		TSubclassOf<class APiece> PieceToSpawn;
	
	int CurrentTurn;
	int BlackKingPos;
	int WhiteKingPos;

	bool isMoveLegal;

	TArray<APiece*> PiecesArray;
	TSet<int32> CurrentMoves;

	UPROPERTY(VisibleDefaultsOnly, Category = Positions)
		TMap<int32, bool> WhiteKingMovesMap;
	UPROPERTY(VisibleDefaultsOnly, Category = Positions)
		TMap<int32, bool> BlackKingMovesMap;

	bool isKingSafe(int PositionToCheck, TArray<APiece*> TempArray);



};
