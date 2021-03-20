// Fill out your copyright notice in the Description page of Project Settings.


#include "PiecePawn.h"

APiecePawn::APiecePawn(const FObjectInitializer& ObjectInitializer)
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Pawn(TEXT("StaticMesh'/Game/StarterContent/Shapes/PawnWhite.PawnWhite'"));
	UStaticMesh* PawnMesh = Pawn.Object;
	Mesh->SetStaticMesh(PawnMesh);
	PieceColor = 1;
	FirstMove = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Constructor"));
}

TArray<int32> APiecePawn::CalculateMoves(TArray<APiece*> Pieces, int CurrentPos)
{
	Super::CalculateMoves(Pieces, CurrentPos);
	int tempCurrentPos = CurrentPos;

	APiece* CurrentPiece = Pieces[CurrentPos];

	TArray<int32> PawnMoves;

	//Create and populate array with all movement directions for the King
	TArray<int> Directions;
	Directions.Add(8 * PieceColor);

	if (CurrentPos % 8 != 0)
	{
		if(PieceColor == 1)
			Directions.Add(7);
		else
			Directions.Add(-9);
	}

	if ((CurrentPos + 1) % 8 != 0)
	{
		if(PieceColor == 1)
			Directions.Add(9);
		Directions.Add(-7);
	}



	//For loop to check all directions
	for (int i = 0; i < Directions.Num(); i++)
	{
		tempCurrentPos = CurrentPos; //Reset position to check from 
		CurrentPiece = Pieces[CurrentPos]; //Reset current piece being checked 
		

		tempCurrentPos += Directions[i];

		//Make sure we never go out of bounds
		if (tempCurrentPos < 64 && tempCurrentPos >= 0)
		{
			CurrentPiece = Pieces[tempCurrentPos];
			//UE_LOG(LogTemp, Error, TEXT("from current position %d"), CurrentPiece->PieceColor);

			if (abs(Directions[i]) == 8)
			{
				if (Pieces[tempCurrentPos] == nullptr)
				{
					//If there is a piece of different color, add the piece to the possible moves and break the loop
					/*if (Pieces[tempCurrentPos] != nullptr && CurrentPiece->PieceColor != PieceColor)
					{
						KingMoves.Add(tempCurrentPos);
						UE_LOG(LogTemp, Warning, TEXT("adding position %d"), tempCurrentPos);
						break;
					}*/

					PawnMoves.Add(tempCurrentPos);
					UE_LOG(LogTemp, Warning, TEXT("adding position %d"), tempCurrentPos);

					if (FirstMove)
					{
						if (Pieces[tempCurrentPos + (8 * PieceColor)] == nullptr)
						{
							PawnMoves.Add(tempCurrentPos + (8 * PieceColor));
						}
					}

				}
			}

			else
			{
				if (Pieces[tempCurrentPos] != nullptr && CurrentPiece->PieceColor != PieceColor)
				{


					PawnMoves.Add(tempCurrentPos);
					UE_LOG(LogTemp, Warning, TEXT("adding position %d"), tempCurrentPos);

				}
			}

			


		}





	}

	return PawnMoves;
}