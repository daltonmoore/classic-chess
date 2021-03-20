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

TArray<int> APiece::CalculateMoves(TArray<APiece*> Pieces, int CurrentPos)
{
	static TArray<int32> PossibleMoves;
	PossibleMoves.Add(2);
	PossibleMoves.Add(3);

	TArray<int32>* pointer = &PossibleMoves;

	return PossibleMoves;
}

void APiece::TestingCalculateMoves()
{
	//TArray<int32> Point = APiece::CalculateMoves();
}

void APiece::SetFirstMove()
{
	FirstMove = false;
}

