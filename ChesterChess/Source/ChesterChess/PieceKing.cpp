// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceKing.h"

APieceKing::APieceKing(const FObjectInitializer& ObjectInitializer)
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Pawn(TEXT("StaticMesh'/Game/StarterContent/Shapes/KingWhite.KingWhite'"));
	UStaticMesh* PawnMesh = Pawn.Object;
	Mesh->SetStaticMesh(PawnMesh);
	PieceColor = 1;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Constructor"));
}

TArray<int32> APieceKing::CalculateMoves(TArray<APiece*> Pieces, int CurrentPos)
{
	Super::CalculateMoves(Pieces, CurrentPos);
	int tempCurrentPos = CurrentPos;

	APiece* CurrentPiece = Pieces[CurrentPos];

	TArray<int32> KingMoves;

	//Create and populate array with all movement directions for the King
	TArray<int> Directions;
	Directions.Add(1);
	Directions.Add(-1);
	Directions.Add(8);
	Directions.Add(-8);
	Directions.Add(9);
	Directions.Add(-9);
	Directions.Add(7);
	Directions.Add(-7);



	//For loop to check all directions
	for (int i = 0; i < Directions.Num(); i++)
	{
		tempCurrentPos = CurrentPos; //Reset position to check from 
		CurrentPiece = Pieces[CurrentPos]; //Reset current piece being checked 

		
		bool RowDone = false;

		//Check if the row that the piece is on is done
		if (Directions[i] == 1 || Directions[i] == 9 || Directions[i] == -7)
		{
			RowDone = ((tempCurrentPos + 1) % 8 == 0);
			int rem = (tempCurrentPos + 1) % 8;
			//UE_LOG(LogTemp, Error, TEXT("from current position %d"), rem);

		}
		else if (Directions[i] == -1 || Directions[i] == -9 || Directions[i] == 7)
		{
			RowDone = (tempCurrentPos % 8 == 0);
		}




		tempCurrentPos += Directions[i];

		//Make sure we never go out of bounds
		if (tempCurrentPos < 64 && tempCurrentPos >= 0)
		{
			CurrentPiece = Pieces[tempCurrentPos];
			//UE_LOG(LogTemp, Error, TEXT("from current position %d"), CurrentPiece->PieceColor);

			if (!RowDone && (Pieces[tempCurrentPos] == nullptr || (Pieces[tempCurrentPos] != nullptr && CurrentPiece->PieceColor != PieceColor)))
			{
				//If there is a piece of different color, add the piece to the possible moves and break the loop
				/*if (Pieces[tempCurrentPos] != nullptr && CurrentPiece->PieceColor != PieceColor)
				{
					KingMoves.Add(tempCurrentPos);
					UE_LOG(LogTemp, Warning, TEXT("adding position %d"), tempCurrentPos);
					break;
				}*/

				KingMoves.Add(tempCurrentPos);
				UE_LOG(LogTemp, Warning, TEXT("adding position %d"), tempCurrentPos);

				if (Directions[i] == 1 && FirstMove)
				{
					if (Pieces[tempCurrentPos + 1] == nullptr && Pieces[tempCurrentPos + 2]->FirstMove)
						KingMoves.Add(tempCurrentPos + 1);
					/*bool CanCastle = true;
					for(int j = 1; j < 3; j++)
					{
						if (Pieces[tempCurrentPos + j] == nullptr)
						{
							continue;
						}

						CanCastle = false;
						break;
					}

					if (CanCastle)
					{
						if (Pieces[tempCurrentPos + 3]->FirstMove)
						{
							KingMoves.Add(tempCurrentPos + 3);
						}
					}*/
				}

				else if (Directions[i] == -1 && FirstMove)
				{
					if(Pieces[tempCurrentPos -1] == nullptr && (Pieces[tempCurrentPos - 2] == nullptr && Pieces[tempCurrentPos -3]->FirstMove))
						KingMoves.Add(tempCurrentPos - 1);

					/*bool CanCastle = true;
					for (int j = -1; j < -4; j--)
					{
						if (Pieces[tempCurrentPos + j] == nullptr)
						{
							continue;
						}

						CanCastle = false;
						break;
					}

					if (CanCastle)
					{
						if (Pieces[tempCurrentPos - 4]->FirstMove)
						{
							KingMoves.Add(tempCurrentPos - 4);
						}
					}*/
				}

			}

			/*else
				continue;*/

		}

		/*else
			break;*/

		

	}

	return KingMoves;
}

