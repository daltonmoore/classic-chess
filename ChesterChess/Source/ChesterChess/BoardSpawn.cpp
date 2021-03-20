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
	CurrentMoves = { NULL };


	//Root = CreateDefaultSubobject<URootComponent>(TEXT("RootComp")); 
	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	ChessPiece = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceName"));
	BlackMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("Black Pieces Material"));
	GoldMesh = CreateDefaultSubobject<UMaterial>(TEXT("Selected Square Mesh"));
	WhiteMat = CreateDefaultSubobject<UMaterial>(TEXT("White Squares Material"));
	BlackSquares = CreateDefaultSubobject<UMaterial>(TEXT("Black Squares Material"));
	/*static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Architecture/BlackFloor.BlackFloor'"));
	UStaticMesh* FloorMesh = MeshAsset.Object;*/
	//Floor->SetStaticMesh(FloorMesh);
	//this->SetRootComponent(Floor);

	//ChessSquare = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Square"));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance>ColorPiece(TEXT("MaterialInstanceConstant'/Game/StarterContent/Blueprints/Assets/M_LightStage_Skybox_Black.M_LightStage_Skybox_Black'")); //Get black material piece
	UMaterialInstance* BlackPiece = ColorPiece.Object;
	BlackMaterial = ColorPiece.Object;

	//Material'/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold'

	static ConstructorHelpers::FObjectFinder<UMaterial>SelectedSquare(TEXT("Material'/Game/StarterContent/Materials/M_Rock_Slate.M_Rock_Slate'")); //Get black material piece
	GoldMesh = SelectedSquare.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial>WhiteSquare(TEXT("Material'/Game/StarterContent/Materials/M_Basic_Wall.M_Basic_Wall'"));
	WhiteMat = WhiteSquare.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial>BlackSquare(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile'"));
	BlackSquares = BlackSquare.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>OneSquare(TEXT("StaticMesh'/Game/StarterContent/Architecture/WhiteFloor.WhiteFloor'")); //Get white floor
	UStaticMesh* WhiteMesh = OneSquare.Object;


	PiecesPositioning = { 2, 3, 4, 5, 6, 4, 3, 2,
		5, 5, 5, 5, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1,
		2, 3, 4, 5, 6, 4, 3 ,2 };

	for (int i = 0; i < 64; i++)
	{
		//Make a dynamic name for the subobject to avoid erros
		FString ComponentName = FString::FromInt(i);
		FString PieceName = FString::FromInt(i+64);

		ChessSquare = CreateDefaultSubobject<UStaticMeshComponent>((FName(ComponentName)));
		

		//static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Architecture/BlackFloor.BlackFloor'")); //Get black floor
		//UStaticMesh* BlackMesh = MeshAsset.Object;

		ChessSquare->SetStaticMesh(WhiteMesh);
		ChessSquare->SetRelativeLocation(FVector(i / 8 * 400, i % 8 * 400, 0));
		
		//Check if square should be white
		//if (i % 2 ^ (i/8)%2)
		/*{
			ChessSquare->SetStaticMesh(WhiteMesh);
			ChessSquare->SetRelativeLocation(FVector(i / 8 * 400, i % 8 * 400, 0));
		}*/

		//Else, spawn black square
		if(!(i % 2 ^ (i / 8) % 2))
		{
			ChessSquare->SetMaterial(0, BlackPiece);
			//ChessSquare->SetRelativeLocation(FVector(i / 8 * 400, i % 8 * 400, 0));
		}
		/*ChessSquare->AttachToComponent(Floor, FAttachmentTransformRules::KeepRelativeTransform);*/
		Squares.Add(FVector(i / 8 * 400, i % 8 * 400, 0));
		SquaresRef.Add(ChessSquare);


		ChessSquare->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

// Called when the game starts or when spawned
void ABoardSpawn::BeginPlay()
{
	Super::BeginPlay();
	/*ChessSquare = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Square"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAssetBoard(TEXT("StaticMesh'/Game/StarterContent/Architecture/WhiteFloor.WhiteFloor'"));
	UStaticMesh* SquareMesh = MeshAssetBoard.Object;
	ChessSquare->SetStaticMesh(SquareMesh);*/
	//UWorld* const World = GetWorld();

	for (int i = 0; i < 64; i++)
	{
		//FString PieceName = FString::FromInt(i + 64);

		const FRotator SpawnRotation = FRotator(0, 0, 0);
		FActorSpawnParameters SpawnParams;

		switch (PiecesPositioning[i])
		{
		case 0:
			PiecesArray.Add(nullptr);
			break;
		case 1:
		{
			//	ChessPiece = CreateDefaultSubobject<UStaticMeshComponent>((FName(PieceName)));
			//	//if (i > 32)
			//	//{
			//	//	static ConstructorHelpers::FObjectFinder<UStaticMesh>Pawn(TEXT("StaticMesh'/Game/StarterContent/Shapes/PawnWhite.PawnWhite'")); //Get white floor
			//	//	UStaticMesh* PawnMesh = Pawn.Object;
			//	//	ChessPiece->SetStaticMesh(PawnMesh);
			//	//}
			//	//else
			//	//{
			//	static ConstructorHelpers::FObjectFinder<UStaticMesh>Pawn(TEXT("StaticMesh'/Game/StarterContent/Shapes/PawnWhite.PawnWhite'"));
			//	UStaticMesh* PawnMesh = Pawn.Object;
			//	ChessPiece->SetStaticMesh(PawnMesh);
			//	//}

			PieceToSpawn = APiecePawn::StaticClass();

			APiece* PawnRef = (GetWorld()->SpawnActor<APiece>(PieceToSpawn, FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0), SpawnRotation, SpawnParams));
			PiecesArray.Add(PawnRef);

			//	ChessPiece->SetRelativeLocation(FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0));
			if (i > 32)
			{
				PawnRef->Mesh->SetMaterial(0, BlackMaterial);
				PawnRef->PieceColor = -1;
				ChessPiece->SetMaterial(0, BlackMaterial);
			}
			//	ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			//	Pieces.Add(ChessPiece);
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
			//	ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			//	Pieces.Add(ChessPiece);
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
			//	ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			//	Pieces.Add(ChessPiece);
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
			//	ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			//	Pieces.Add(ChessPiece);
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
			/*ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Pieces.Add(ChessPiece);*/
			break;
		}
		case 6:
		{
			PieceToSpawn = APieceKing::StaticClass();

			APiece* KingRef = (GetWorld()->SpawnActor<APiece>(PieceToSpawn, FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0), SpawnRotation, SpawnParams));
			PiecesArray.Add(KingRef);

			if (i > 32)
			{
				KingRef->Mesh->SetMaterial(0, BlackMaterial);
				KingRef->PieceColor = -1;
				ChessPiece->SetMaterial(0, BlackMaterial);
			}
			//	ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			//	Pieces.Add(ChessPiece);
			break;
		}

		}

		//GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Red, TEXT("for loop"));
	}


	
}

// Called every frame
void ABoardSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoardSpawn::BoardGeneration()
{

}

void ABoardSpawn::Clicking(FVector Position)
{
	bool movablePiece = false;
	FVector DividedPosition = Position.operator/(FVector(400.f, 400.f, 1.f));
	int Column = floor(DividedPosition.X);
	int Lines = floor(DividedPosition.Y);

	//GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Red, FString::FromInt(Column) + " " + FString::FromInt(Lines));
	UE_LOG(LogTemp, Warning, TEXT("it got called"));

	if (LastIndex == -1)
	{
		int CheckForPiece = Column * 8 + Lines;
		//LastIndex = Column * 8 + Lines;

		LastIndex = (PiecesArray[CheckForPiece] == nullptr) ? -1 : CheckForPiece;

		if (!(PiecesArray[CheckForPiece] == nullptr))
		{
			SquaresRef[LastIndex]->SetMaterial(0, GoldMesh);
			movablePiece = true;
		}

		if (movablePiece)
		{
			CurrentMoves = PiecesArray[CheckForPiece]->CalculateMoves(PiecesArray, LastIndex);

			for(int Index:CurrentMoves)
			{
				SquaresRef[Index]->SetMaterial(0, GoldMesh);
			}
		}
		/*if (LastIndex > 32)
		{
			LastIndex -= 32;
		}*/
	}

	else
	{
		//Pieces[LastIndex]->SetRelativeLocation(Squares[LastIndex]);
		//Pieces[LastIndex]->SetRelativeLocation(FVector(Column * 400.f, Lines * 400.f, 0));
		int NewColumn = floor(DividedPosition.X);
		int NewLines = floor(DividedPosition.Y);
		int NewIndex = NewColumn * 8 + NewLines;

		bool LegalMove = false;

		for (int i = 0; i < CurrentMoves.Num(); i++)
		{
			if (CurrentMoves[i] == NewIndex)
			{
				LegalMove = true;
				break;
			}
		}
					

		if (LegalMove)
		{
			if (PiecesArray[NewIndex] != nullptr)
			{
				PiecesArray[NewIndex]->Destroy();
			}

			PiecesArray[LastIndex]->SetFirstMove();

			PiecesArray[LastIndex]->Mesh->SetWorldLocation(FVector(Squares[NewIndex].X + 200, Squares[NewIndex].Y + 200, 0));
			PiecesArray[NewIndex] = PiecesArray[LastIndex];
			PiecesArray[LastIndex] = nullptr;

			if (Cast<APiecePawn>(PiecesArray[NewIndex]))
			{
				if (NewIndex > 55)
				{
					PiecesArray[NewIndex]->Destroy();
					const FRotator SpawnRotation = FRotator(0, 0, 0);
					FActorSpawnParameters SpawnParams;
					APiece* QueenRef = (GetWorld()->SpawnActor<APiece>(APieceQueen::StaticClass(), FVector(Squares[NewIndex].X + 200, Squares[NewIndex].Y + 200, 0), SpawnRotation, SpawnParams));
					PiecesArray[NewIndex] = QueenRef;
				}

				else if(NewIndex < 8)
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

			else if (Cast<APieceKing>(PiecesArray[NewIndex]) && (abs(LastIndex - NewIndex) != 1))
			{
				if ((LastIndex - NewIndex) > 1)
				{
					if (PiecesArray[NewIndex]->PieceColor == 1)
					{
						PiecesArray[0]->Mesh->SetWorldLocation(FVector(Squares[3].X + 200, Squares[3].Y + 200, 0));
						PiecesArray[3] = PiecesArray[0];
						PiecesArray[0] = nullptr;
					}

					else
					{
						PiecesArray[56]->Mesh->SetWorldLocation(FVector(Squares[59].X + 200, Squares[3].Y + 200, 0));
						PiecesArray[59] = PiecesArray[56];
						PiecesArray[56] = nullptr;
					}
				}

				else
				{
					if (PiecesArray[NewIndex]->PieceColor == 1)
					{
						PiecesArray[7]->Mesh->SetWorldLocation(FVector(Squares[5].X + 200, Squares[5].Y + 200, 0));
						PiecesArray[5] = PiecesArray[7];
						PiecesArray[7] = nullptr;
					}

					else
					{
						PiecesArray[63]->Mesh->SetWorldLocation(FVector(Squares[61].X + 200, Squares[5].Y + 200, 0));
						PiecesArray[61] = PiecesArray[63];
						PiecesArray[63] = nullptr;
					}
				}
			}

			
		}

		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("ILLEGAL MOVE"));
		}

		int temp = ceil(LastIndex / 8) + LastIndex;

		if (!(temp % 2))
		{
			SquaresRef[LastIndex]->SetMaterial(0, BlackMaterial);
		}

		else
		{
			SquaresRef[LastIndex]->SetMaterial(0, WhiteMat);
		}

		for (int Index : CurrentMoves)
		{
			temp = ceil(Index / 8) + Index;

			if (!(temp % 2))
			{
				SquaresRef[Index]->SetMaterial(0, BlackMaterial);
			}

			else
			{
				SquaresRef[Index]->SetMaterial(0, WhiteMat);
			}

		}
		

		LastIndex = -1;
	}

}

