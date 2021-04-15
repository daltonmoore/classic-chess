// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceBishop.h"

APieceBishop::APieceBishop(const FObjectInitializer& ObjectInitializer)
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BishopM(TEXT("StaticMesh'/Game/StarterContent/Shapes/BishopWhite.BishopWhite'"));
	UStaticMesh* PawnMesh = BishopM.Object;
	Mesh->SetStaticMesh(PawnMesh);
	PieceColor = 1;
	PieceValue = 3;
	pieceType = EBishop;
	FirstMove = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Constructor"));
}

TSet<int32> APieceBishop::CalculateMoves(TArray<APiece*> Pieces, int CurrentPos)
{
	Super::CalculateMoves(Pieces, CurrentPos);
	int tempCurrentPos = CurrentPos;

	APiece* CurrentPiece = Pieces[CurrentPos];

	TSet<int32> BishopMoves;

	//Create and populate array with all movement directions for the Bishop
	TArray<int> Directions;
	Directions.SetNumUninitialized(4);
	Directions.Add(9);
	Directions.Add(-9);
	Directions.Add(7);
	Directions.Add(-7);



	//For loop to check all directions
	for (int i = 0; i < Directions.Num(); i++)
	{
		tempCurrentPos = CurrentPos; //Reset position to check from 
		CurrentPiece = Pieces[CurrentPos]; //Reset current piece being checked 

		//UE_LOG(LogTemp, Warning, TEXT("now checking for %d"), Directions[i]);
		//UE_LOG(LogTemp, Warning, TEXT("from current position %d"), tempCurrentPos);

		for (int j = 0; j < 7; j++)
		{
			bool RowDone = false;

			//Check if the row that the piece is on is done
			if (Directions[i] == 9 || Directions[i] == -7)
			{
				RowDone = ((tempCurrentPos + 1) % 8 == 0);
				int rem = (tempCurrentPos + 1) % 8;
				//UE_LOG(LogTemp, Error, TEXT("from current position %d"), rem);

			}
			else if (Directions[i] == -9 || Directions[i] == 7)
			{
				RowDone = (tempCurrentPos % 8 == 0);
			}

			tempCurrentPos += Directions[i];

			//Make sure we never go out of bounds
			if (tempCurrentPos < 64 && tempCurrentPos >= 0)
			{
				CurrentPiece = Pieces[tempCurrentPos];
				//UE_LOG(LogTemp, Error, TEXT("from current position %d"), CurrentPiece->PieceColor);

				if (!RowDone && (Pieces[tempCurrentPos] == nullptr || CurrentPiece->PieceColor != PieceColor))
				{
					//If there is a piece of different color, add the piece to the possible moves and break the loop
					if (Pieces[tempCurrentPos] != nullptr && CurrentPiece->PieceColor != PieceColor)
					{
						BishopMoves.Add(tempCurrentPos);
						UE_LOG(LogTemp, Warning, TEXT("adding position %d"), tempCurrentPos);
						break;
					}

					BishopMoves.Add(tempCurrentPos);
					UE_LOG(LogTemp, Warning, TEXT("adding position %d"), tempCurrentPos);

				}

				else
					break;

			}

			else
				break;

		}

	}

	return BishopMoves;
}