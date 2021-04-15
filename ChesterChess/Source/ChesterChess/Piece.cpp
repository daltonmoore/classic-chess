// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece.h"

// Sets default values
APiece::APiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PieceColor = 1;
	PieceValue = 0;
	FirstMove = true;
	pieceType = EPawn;

}

// Called when the game starts or when spawned
void APiece::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TSet<int> APiece::CalculateMoves(TArray<APiece*> Pieces, int CurrentPos)
{
	static TSet<int32> PossibleMoves;
	PossibleMoves.Add(2);
	PossibleMoves.Add(3);

	TSet<int32>* pointer = &PossibleMoves;

	return PossibleMoves;
}

void APiece::SetFirstMove()
{
	FirstMove = false;
}

void APiece::SetFutureMoves(TArray<int32> Moves)
{
	FutureMoves = Moves;
}

