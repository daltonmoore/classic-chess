// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardSpawn.h"
#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Containers/UnrealString.h"
#include "PieceKing.h"
#include "PieceQueen.h"
#include "PieceRook.h"
#include "PieceBishop.h"
#include "PieceKnight.h"
#include "PiecePawn.h"



#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Engine/Engine.h"
#include "Math/Rotator.h"
#include "Math/UnrealMathUtility.h"
#include "Components/TimelineComponent.h"
#include "TimerManager.h"

// Sets default values
ABoardSpawn::ABoardSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LastIndex = -1;
	CurrentTurn = 1;
	WhiteKingPos = 4;
	BlackKingPos = 60;
	isMoveLegal = true;
	CurrentMoves = { NULL };
	SquaresColor.SetNumUninitialized(2);



	//Root = CreateDefaultSubobject<URootComponent>(TEXT("RootComp")); 
	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	ChessPiece = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceName"));
	BlackMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("Black Pieces Material"));
	GoldMesh = CreateDefaultSubobject<UMaterial>(TEXT("Selected Square Mesh"));
	CheckedMat = CreateDefaultSubobject<UMaterial>(TEXT("White Squares Material"));


	static ConstructorHelpers::FObjectFinder<UMaterialInstance>ColorPiece(TEXT("MaterialInstanceConstant'/Game/StarterContent/Blueprints/Assets/M_LightStage_Skybox_Black.M_LightStage_Skybox_Black'")); //Get black material piece
	UMaterialInstance* BlackPiece = ColorPiece.Object;
	BlackMaterial = ColorPiece.Object;

	//Material'/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold'

	static ConstructorHelpers::FObjectFinder<UMaterial>SelectedSquare(TEXT("Material'/Game/StarterContent/Materials/M_Rock_Slate.M_Rock_Slate'")); //Get black material piece
	GoldMesh = SelectedSquare.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial>CheckSquare(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));//Get square for checks
	CheckedMat = CheckSquare.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial>WhiteSquare(TEXT("Material'/Game/StarterContent/Materials/M_Basic_Wall.M_Basic_Wall'")); //Get material for white squares
	SquaresColor[1] = WhiteSquare.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial>BlackSquare(TEXT("Material'/Game/StarterContent/Materials/M_Wood_Oak.M_Wood_Oak'")); //Get material for black squares
	SquaresColor[0] = BlackSquare.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>OneSquare(TEXT("StaticMesh'/Game/StarterContent/Architecture/WhiteFloor.WhiteFloor'")); //Get white floor
	UStaticMesh* WhiteMesh = OneSquare.Object;


	PiecesPositioning = { 2, 3, 4, 5, 6, 4, 3, 2,
		1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1,
		2, 3, 4, 5, 6, 4, 3 ,2 };


	for (int i = 0; i < 64; i++)
	{
		//Make a dynamic name for the subobject to avoid erros
		FString ComponentName = FString::FromInt(i);
		FString PieceName = FString::FromInt(i+64);

		ChessSquare = CreateDefaultSubobject<UStaticMeshComponent>((FName(ComponentName)));
	

		ChessSquare->SetStaticMesh(WhiteMesh);
		ChessSquare->SetRelativeLocation(FVector(i / 8 * 400, i % 8 * 400, 0));
		
		ChessSquare->SetMaterial(0, SquaresColor[(i % 2 ^ (i / 8) % 2)]);
		Squares.Add(FVector(i / 8 * 400, i % 8 * 400, 0));
		SquaresRef.Add(ChessSquare);


		ChessSquare->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

// Called when the game starts or when spawned
void ABoardSpawn::BeginPlay()
{
	Super::BeginPlay();

	//For loop to spawn pieces using the PiecesPositioning array
	for (int i = 0; i < 64; i++)
	{

		const FRotator SpawnRotation = FRotator(0, 0, 0);
		FActorSpawnParameters SpawnParams;

		switch (PiecesPositioning[i])
		{
		case 0:
			PiecesArray.Add(nullptr);
			break;
		case 1:
		{

			PieceToSpawn = APiecePawn::StaticClass();

			APiece* PawnRef = (GetWorld()->SpawnActor<APiece>(PieceToSpawn, FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0), SpawnRotation, SpawnParams));
			PiecesArray.Add(PawnRef);

			if (i > 32)
			{
				PawnRef->Mesh->SetMaterial(0, BlackMaterial);
				PawnRef->PieceColor = -1;
				ChessPiece->SetMaterial(0, BlackMaterial);
			}

			break;
		}

		case 2:
		{
			PieceToSpawn = APieceRook::StaticClass();

			APiece* RookRef = (GetWorld()->SpawnActor<APiece>(PieceToSpawn, FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0), SpawnRotation, SpawnParams));
			PiecesArray.Add(RookRef);

			if (i > 32)
			{
				RookRef->Mesh->SetMaterial(0, BlackMaterial);
				RookRef->PieceColor = -1;
				ChessPiece->SetMaterial(0, BlackMaterial);
			}

			break;
		}
		case 3:
		{
			PieceToSpawn = APieceKnight::StaticClass();

			APiece* KnightRef = (GetWorld()->SpawnActor<APiece>(PieceToSpawn, FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0), SpawnRotation, SpawnParams));
			PiecesArray.Add(KnightRef);

			if (i > 32)
			{
				KnightRef->Mesh->SetMaterial(0, BlackMaterial);
				KnightRef->PieceColor = -1;
				ChessPiece->SetMaterial(0, BlackMaterial);
			}

			break;
		}
		case 4:
		{
			PieceToSpawn = APieceBishop::StaticClass();

			APiece* BishopRef = (GetWorld()->SpawnActor<APiece>(PieceToSpawn, FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0), SpawnRotation, SpawnParams));
			PiecesArray.Add(BishopRef);

			if (i > 32)
			{
				BishopRef->Mesh->SetMaterial(0, BlackMaterial);
				BishopRef->PieceColor = -1;
				ChessPiece->SetMaterial(0, BlackMaterial);
			}

			break;
		}
		case 5:
		{

			PieceToSpawn = APieceQueen::StaticClass();

			APiece* QueenRef = (GetWorld()->SpawnActor<APiece>(PieceToSpawn, FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0), SpawnRotation, SpawnParams));
			PiecesArray.Add(QueenRef);
						
			if (i > 32)
			{
				QueenRef->Mesh->SetMaterial(0, BlackMaterial);
				QueenRef->PieceColor = -1;
				ChessPiece->SetMaterial(0, BlackMaterial);
			}

			break;
		}
		case 6:
		{
			PieceToSpawn = APieceKing::StaticClass();

			APiece* KingRef = (GetWorld()->SpawnActor<APiece>(PieceToSpawn, FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0), SpawnRotation, SpawnParams));
			PiecesArray.Add(KingRef);

			if (i > 32)
			{
				BlackKingPos = i;
				KingRef->Mesh->SetMaterial(0, BlackMaterial);
				KingRef->PieceColor = -1;
				ChessPiece->SetMaterial(0, BlackMaterial);
			}

			else
			{
				WhiteKingPos = i;
			}

			break;
		}

		}

	}


	
}

// Called every frame
void ABoardSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoardSpawn::PiecesMovement(FVector Position)
{
	bool MovablePiece = false;
	FVector DividedPosition = Position.operator/(FVector(400.f, 400.f, 1.f));
	int Column = floor(DividedPosition.X);
	int Lines = floor(DividedPosition.Y);

	//GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Red, FString::FromInt(Column) + " " + FString::FromInt(Lines));

	//Check if a piece has not been selected
	if (LastIndex == -1)
	{
		int CheckForPiece = Column * 8 + Lines;

		LastIndex = (PiecesArray[CheckForPiece] == nullptr) ? -1 : CheckForPiece; //If piece selected is not null, set LastIndex to the piece selected

		if (!(PiecesArray[CheckForPiece] == nullptr) && (PiecesArray[CheckForPiece]->PieceColor == CurrentTurn))
		{
			SquaresRef[LastIndex]->SetMaterial(0, GoldMesh);
			MovablePiece = true;
		}

		//If piece selected is valid get CurrentMoves for that piece
		if (MovablePiece)
		{
			CurrentMoves = PiecesArray[CheckForPiece]->CalculateMoves(PiecesArray, LastIndex);

			for(int Index:CurrentMoves)
			{
				SquaresRef[Index]->SetMaterial(0, GoldMesh);
			}
		}

		else
		{
			LastIndex = -1;
		}

	}

	//If a piece was selected
	else
	{
		int NewColumn = floor(DividedPosition.X);
		int NewLines = floor(DividedPosition.Y);
		int NewIndex = NewColumn * 8 + NewLines;

		bool LegalMove = CurrentMoves.Contains(NewIndex); //Check if attempted move is a possible move

					

		if (LegalMove)
		{		
			
			if (!(PiecesArray[LastIndex]->pieceType == EKing))
			{
				TArray<APiece*> TempArray = PiecesArray;
				TempArray[NewIndex] = PiecesArray[LastIndex];
				TempArray[LastIndex] = nullptr;
				
				if(CurrentTurn == 1)
				isMoveLegal = isKingSafe(WhiteKingPos, TempArray);
				else
				isMoveLegal = isKingSafe(BlackKingPos, TempArray);
			}
			else
			{
				isMoveLegal = isKingSafe(NewIndex, PiecesArray);

				if (isMoveLegal)
				{

					
					//Check if castling is legal
					if ((LastIndex - NewIndex) == 2)
					{
						if (PiecesArray[LastIndex]->PieceColor == 1 && isKingSafe(3, PiecesArray))
						{
							PiecesArray[0]->Mesh->SetWorldLocation(FVector(Squares[3].X + 200, Squares[3].Y + 200, 0));
							PiecesArray[3] = PiecesArray[0];
							PiecesArray[0] = nullptr;
						}

						else if (PiecesArray[LastIndex]->PieceColor == -1 && isKingSafe(59, PiecesArray))
						{
							PiecesArray[56]->Mesh->SetWorldLocation(FVector(Squares[59].X + 200, Squares[3].Y + 200, 0));
							PiecesArray[59] = PiecesArray[56];
							PiecesArray[56] = nullptr;
						}

						else
						{
							isMoveLegal = false;
							GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("King move set to false from castling"));

						}
					}

					else if ((LastIndex - NewIndex) == -2)
					{
						if (PiecesArray[LastIndex]->PieceColor == 1 && isKingSafe(5, PiecesArray))
						{
							PiecesArray[7]->Mesh->SetWorldLocation(FVector(Squares[5].X + 200, Squares[5].Y + 200, 0));
							PiecesArray[5] = PiecesArray[7];
							PiecesArray[7] = nullptr;
						}

						else if (PiecesArray[LastIndex]->PieceColor == -1 && isKingSafe(61, PiecesArray))
						{
							PiecesArray[63]->Mesh->SetWorldLocation(FVector(Squares[61].X + 200, Squares[5].Y + 200, 0));
							PiecesArray[61] = PiecesArray[63];
							PiecesArray[63] = nullptr;
						}

						else
						{
							isMoveLegal = false;
							GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("King move set to false from castling"));
						}
					}

				}
				
			}


			//If move is still legal
			if (isMoveLegal)
			{

				CurrentTurn *= -1; //Update turn

				//If a piece was captured, destroy that piece
				if (PiecesArray[NewIndex] != nullptr)
				{
					PiecesArray[NewIndex]->Destroy();
				}

				PiecesArray[LastIndex]->SetFirstMove();

				PiecesArray[LastIndex]->Mesh->SetWorldLocation(FVector(Squares[NewIndex].X + 200, Squares[NewIndex].Y + 200, 0));
				PiecesArray[NewIndex] = PiecesArray[LastIndex];
				PiecesArray[LastIndex] = nullptr;

				//Check if promotion is available for pawn
				//TODO: Give options to promote to any piece
				if (PiecesArray[NewIndex]->pieceType == EPawn)
				{
					if (NewIndex > 55)
					{
						PiecesArray[NewIndex]->Destroy();
						const FRotator SpawnRotation = FRotator(0, 0, 0);
						FActorSpawnParameters SpawnParams;
						APiece* QueenRef = (GetWorld()->SpawnActor<APiece>(APieceQueen::StaticClass(), FVector(Squares[NewIndex].X + 200, Squares[NewIndex].Y + 200, 0), SpawnRotation, SpawnParams));
						PiecesArray[NewIndex] = QueenRef;
					}

					else if (NewIndex < 8)
					{
						PiecesArray[NewIndex]->Destroy();
						const FRotator SpawnRotation = FRotator(0, 0, 0);
						FActorSpawnParameters SpawnParams;
						APiece* QueenRef = (GetWorld()->SpawnActor<APiece>(APieceQueen::StaticClass(), FVector(Squares[NewIndex].X + 200, Squares[NewIndex].Y + 200, 0), SpawnRotation, SpawnParams));
						QueenRef->Mesh->SetMaterial(0, BlackMaterial);
						QueenRef->PieceColor = -1;
						PiecesArray[NewIndex] = QueenRef;
					}
				}

				//If the king was moved update king's position
				else if (PiecesArray[NewIndex]->pieceType == EKing)
				{
					if (CurrentTurn == -1)
					{
						WhiteKingPos = NewIndex;
					}

					else
					{
						BlackKingPos = NewIndex;
					}
				}


			}

			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("ILLEGAL MOVE"));
			}

			
		}

		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("ILLEGAL MOVE"));
		}

		//Reset material for selected piece
		int temp = ceil(LastIndex / 8) + LastIndex;

		SquaresRef[LastIndex]->SetMaterial(0, SquaresColor[temp % 2]);
	
		//Reset material for CurrentMoves
		for (int Index : CurrentMoves)
		{
			temp = ceil(Index / 8) + Index;

			SquaresRef[Index]->SetMaterial(0, SquaresColor[temp%2]);
		}

		//If white to move, check if white king is in check
		if (CurrentTurn == 1)
		{
			SquaresRef[BlackKingPos]->SetMaterial(0, SquaresColor[((BlackKingPos / 8 + BlackKingPos) % 2)]);

			if (!isKingSafe(WhiteKingPos, PiecesArray))
			{
				SquaresRef[WhiteKingPos]->SetMaterial(0, CheckedMat);
			}

		}

		//If black to move check if black king is in check
		else if(CurrentTurn == -1)
		{
			SquaresRef[WhiteKingPos]->SetMaterial(0, SquaresColor[((WhiteKingPos / 8 + WhiteKingPos) % 2)]);

			if (!isKingSafe(BlackKingPos, PiecesArray))
			{
				SquaresRef[BlackKingPos]->SetMaterial(0, CheckedMat);

				/*TODO: Check for checkmate

				bool CheckMate = true;

				for (int Index : PiecesArray[BlackKingPos]->CalculateMoves(PiecesArray, LastIndex))
				{
					if (!isKinginCheck(Index, PiecesArray))
					{
						CheckMate = false;
						break;
					}
				}

				if (CheckMate)
				{
					for (APiece* Piece : PiecesArray)
					{
						if (Piece != nullptr && Piece->PieceColor == -1)
						{
							Piece->CalculateMoves()
						}
					}
				}
				*/

			}
		}
	
		LastIndex = -1;
	}

	

}

bool ABoardSpawn::isKingSafe(int PositionToCheck, TArray<APiece*> TempArray)
{
	APiece* CurrentKing;

	if (CurrentTurn == 1)
	{
		CurrentKing = PiecesArray[WhiteKingPos];
	}

	else
		CurrentKing = PiecesArray[BlackKingPos];

	APiece* CurrentPiece = PiecesArray[PositionToCheck];

	int tempCurrentPos = PositionToCheck;

	TArray<int> Directions;
	Directions.SetNumUninitialized(8);
	Directions[0] = 1;
	Directions[1] = -1;
	Directions[2] = 8;
	Directions[3] = -8;
	Directions[4] = 9;
	Directions[5] = -9;
	Directions[6] = 7;
	Directions[7] = -7;



	//For loop to check all directions
	for (int i = 0; i < Directions.Num(); i++)
	{
		tempCurrentPos = PositionToCheck; //Reset position to check from 
		CurrentPiece = TempArray[PositionToCheck]; //Reset current piece being checked 

		//UE_LOG(LogTemp, Warning, TEXT("now checking for %d"), Directions[i]);
		//UE_LOG(LogTemp, Warning, TEXT("from current position %d"), tempCurrentPos);

		for (int j = 0; j < 7; j++)
		{
			bool RowDone = false;
			bool BreakLoop = false;

			//Check if the row that the piece is on is done
			if (Directions[i] == 1 || Directions[i] == 9 || Directions[i] == -7)
			{
				RowDone = ((tempCurrentPos + 1) % 8 == 0);

			}

			else if (Directions[i] == -1 || Directions[i] == -9 || Directions[i] == 7)
			{
				RowDone = (tempCurrentPos % 8 == 0);
			}




			tempCurrentPos += Directions[i];

			if (tempCurrentPos == LastIndex)
			{
				tempCurrentPos += Directions[i];
			}

			//Make sure we never go out of bounds
			if (tempCurrentPos < 64 && tempCurrentPos >= 0)
			{
				CurrentPiece = TempArray[tempCurrentPos];
				//UE_LOG(LogTemp, Error, TEXT("from current position %d"), CurrentPiece->PieceColor);
				
				

				if (!RowDone && (TempArray[tempCurrentPos] == nullptr))
					continue;

				if (TempArray[tempCurrentPos] != nullptr && TempArray[tempCurrentPos]->PieceColor == CurrentKing->PieceColor)
					break;

				else if (CurrentPiece != nullptr && CurrentPiece->PieceColor != CurrentKing->PieceColor)
				{
					UE_LOG(LogTemp, Warning, TEXT("from current position %d"), tempCurrentPos);
					switch (Directions[i])
					{
					case 1:
					case -1:
					case -8:
					case 8:
						if (CurrentPiece->pieceType == ERook || CurrentPiece->pieceType == EQueen || (j == 0 && CurrentPiece->pieceType == EKing))
						{
							GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("From case 8"));
							return false;
						}
						else if (CurrentPiece->pieceType == EBishop || CurrentPiece->pieceType == EPawn || CurrentPiece->pieceType == EKnight || (j > 0 && CurrentPiece->pieceType == EKing))
						{
							goto exit_loop;
						}

						break;

					case 9:
					case 7:
						if (CurrentPiece->pieceType == EBishop || CurrentPiece->pieceType == EQueen || (j == 0 && (CurrentPiece->pieceType == EKing || (CurrentPiece->pieceType == EPawn && CurrentPiece->PieceColor == -1))))
						{
							GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("From case 7"));
							return false;
						}

						else if (CurrentPiece->pieceType == ERook || CurrentPiece->pieceType == EQueen || CurrentPiece->pieceType == EKnight || (j > 0 && CurrentPiece->pieceType == EPawn))
						{
							BreakLoop = true;
							break;
						}
						break;

					case -9:
					case -7:
						if (CurrentPiece->pieceType == EBishop || CurrentPiece->pieceType == EQueen || (j == 0 && (CurrentPiece->pieceType == EKing || (CurrentPiece->pieceType == EPawn && CurrentPiece->PieceColor == 1))))
						{
							GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("From case - 7"));
							return false;
						}

						else if (CurrentPiece->pieceType == ERook || CurrentPiece->pieceType == EQueen || CurrentPiece->pieceType == EKnight || (j > 0 && CurrentPiece->pieceType == EPawn))
						{
							BreakLoop = true;
							break;
						}

						break;

						
					}

					if (BreakLoop)
						break;
				}

				

				

				else
					break;

			}

			else
				break;

		}

	exit_loop:;

	}
	//Check for knight checks

	Directions[0] = 15;
	Directions[1] = -15;
	Directions[2] = 17;
	Directions[3] = -17;
	Directions[4] = 6;
	Directions[5] = -6;
	Directions[6] = 10;
	Directions[7] = -10;

	for (int i = 0; i < Directions.Num(); i++)
	{
		tempCurrentPos = PositionToCheck; //Reset position to check from 
		CurrentPiece = TempArray[PositionToCheck]; //Reset current piece being checked 
		bool RowDone = false;

		//Check for limitations on Knight movement when on edges
		if (Directions[i] == 6 || Directions[i] == 15 || Directions[i] == -17 || Directions[i] == -10)
		{
			if (Directions[i] == 6 || Directions[i] == -10)
				RowDone = (tempCurrentPos % 8 == 0 || (tempCurrentPos - 1) % 8 == 0);
			else
				RowDone = (tempCurrentPos % 8 == 0);
		}

		else if (Directions[i] == -6 || Directions[i] == -15 || Directions[i] == 17 || Directions[i] == 10)
		{
			if (Directions[i] == -6 || Directions[i] == 10)
				RowDone = ((tempCurrentPos + 2) % 8 == 0 || (tempCurrentPos + 1) % 8 == 0);
			else
				RowDone = ((tempCurrentPos + 1) % 8 == 0);
		}

		tempCurrentPos += Directions[i];

		if (tempCurrentPos < 64 && tempCurrentPos >= 0 && !RowDone)
		{
			CurrentPiece = TempArray[tempCurrentPos];
			if (Cast<APieceKnight>(CurrentPiece) && CurrentKing->PieceColor != CurrentPiece->PieceColor)
			{
				return false;
			}
		}
	}





	return true;
}

