// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Piece.generated.h"

UENUM()

enum PieceType
{
	EPawn, EKnight, EBishop, ERook, EQueen, EKing
};

UCLASS()
class CHESTERCHESS_API APiece : public AActor
{
	GENERATED_BODY()
		

	
public:	
	// Sets default values for this actor's properties
	APiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* Mesh;

		int PieceColor;
	
	UFUNCTION()
		virtual TSet<int32> CalculateMoves(TArray<APiece*> Pieces, int CurrentPos);
		
	void SetFirstMove();
	void SetFutureMoves(TArray<int32> Moves);

	int PieceValue;
	PieceType pieceType;

	UPROPERTY(EditAnywhere, Category = Pieces)
	bool FirstMove;

	TArray<int32> FutureMoves;


};
