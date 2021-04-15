// Fill out your copyright notice in the Description page of Project Settings.


#include "PiecePawn.h"

APiecePawn::APiecePawn(const FObjectInitializer& ObjectInitializer)
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>PawnM(TEXT("StaticMesh'/Game/StarterContent/Shapes/PawnWhite.PawnWhite'"));
	UStaticMesh* PawnMesh = PawnM.Object;
	Mesh->SetStaticMesh(PawnMesh);
	PieceColor = 1;
	PieceValue = 1;
	pieceType = EPawn;
	FirstMove = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Constructor"));
}

TSet<int32> APiecePawn::CalculateMoves(TArray<APiece*> Pieces, int CurrentPos)
{
	Super::CalculateMoves(Pieces, CurrentPos);
	int tempCurrentPos = CurrentPos;

	APiece* CurrentPiece = Pieces[CurrentPos];

	TSet<int32> PawnMoves;

	//Create array and check whether the pawn is white or black (moves upwards or downwards)
	TArray<int> Directions;
	Directions.Add(8 * PieceColor);

	//Add diagonal movement for capturing based on pawn's position
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

			//Upwards and downwards movement
			if (abs(Directions[i]) == 8)
			{
				if (Pieces[tempCurrentPos] == nullptr)
				{

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

			//Diagonal movement for capturing enemy pieces
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