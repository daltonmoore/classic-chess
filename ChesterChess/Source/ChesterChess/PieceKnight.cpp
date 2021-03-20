// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceKnight.h"

APieceKnight::APieceKnight(const FObjectInitializer& ObjectInitializer)
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Pawn(TEXT("StaticMesh'/Game/StarterContent/Shapes/KnightWhite.KnightWhite'"));
	UStaticMesh* PawnMesh = Pawn.Object;
	Mesh->SetStaticMesh(PawnMesh);
	PieceColor = 1;
	FirstMove = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Constructor"));
}

TArray<int32> APieceKnight::CalculateMoves(TArray<APiece*> Pieces, int CurrentPos)
{
	Super::CalculateMoves(Pieces, CurrentPos);
	int tempCurrentPos = CurrentPos;

	APiece* CurrentPiece = Pieces[CurrentPos];

	TArray<int32> KnightMoves;

	//Create and populate array with all movement directions for the King
	TArray<int> Directions;
	Directions.Add(15);
	Directions.Add(-15);
	Directions.Add(17);
	Directions.Add(-17);
	Directions.Add(6);
	Directions.Add(-6);
	Directions.Add(10);
	Directions.Add(-10);



	//For loop to check all directions
	for (int i = 0; i < Directions.Num(); i++)
	{
		tempCurrentPos = CurrentPos; //Reset position to check from 
		CurrentPiece = Pieces[CurrentPos]; //Reset current piece being checked 
		bool RowDone = false;

		//Check for limitations on Knight movement when on edges
		if (Directions[i] == 6 || Directions[i] == 15 || Directions[i] == -17 || Directions[i] == -10)
		{
			if (Directions[i] == 6 || Directions[i] == -10)			
				RowDone = (CurrentPos % 8 == 0 || (CurrentPos-1) % 8 == 0);			
			else
				RowDone = (CurrentPos % 8 == 0);
		}

		else if (Directions[i] == -6 || Directions[i] == -15 || Directions[i] == 17 || Directions[i] == 10)
		{
			if (Directions[i] == -6 || Directions[i] == 10)
				RowDone = ((CurrentPos + 2) % 8 == 0 || (CurrentPos + 1) % 8 == 0);
			else
			RowDone = ((CurrentPos+1) % 8 == 0);
		}




		tempCurrentPos += Directions[i];

		//Make sure we never go out of bounds
		if (tempCurrentPos < 64 && tempCurrentPos >= 0)
		{
			CurrentPiece = Pieces[tempCurrentPos];
			//UE_LOG(LogTemp, Error, TEXT("from current position %d"), CurrentPiece->PieceColor);

			if (!RowDone && (Pieces[tempCurrentPos] == nullptr || (Pieces[tempCurrentPos] != nullptr && CurrentPiece->PieceColor != PieceColor)))
			{
				KnightMoves.Add(tempCurrentPos);
				UE_LOG(LogTemp, Warning, TEXT("adding position %d"), tempCurrentPos);
			}

			/*else
				continue;*/

		}

		/*else
			break;*/



	}

	return KnightMoves;
}