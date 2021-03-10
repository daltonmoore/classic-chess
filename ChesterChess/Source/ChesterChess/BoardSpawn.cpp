// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardSpawn.h"
#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Containers/UnrealString.h"

// Sets default values
ABoardSpawn::ABoardSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Root = CreateDefaultSubobject<URootComponent>(TEXT("RootComp")); 
	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	/*static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Architecture/BlackFloor.BlackFloor'"));
	UStaticMesh* FloorMesh = MeshAsset.Object;*/
	//Floor->SetStaticMesh(FloorMesh);
	//this->SetRootComponent(Floor);

	//ChessSquare = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Square"));

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
		static ConstructorHelpers::FObjectFinder<UStaticMesh>OneSquare(TEXT("StaticMesh'/Game/StarterContent/Architecture/WhiteFloor.WhiteFloor'")); //Get white floor
		UStaticMesh* WhiteMesh = OneSquare.Object;

		static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Architecture/BlackFloor.BlackFloor'")); //Get black floor
		UStaticMesh* BlackMesh = MeshAsset.Object;
		
		//Check if square should be white
		if (i % 2 ^ (i/8)%2)
		{
			ChessSquare->SetStaticMesh(WhiteMesh);
			ChessSquare->SetRelativeLocation(FVector(i / 8 * 400, i % 8 * 400, 0));
		}

		//Else, spawn black square
		else
		{
			ChessSquare->SetStaticMesh(BlackMesh);
			ChessSquare->SetRelativeLocation(FVector(i / 8 * 400, i % 8 * 400, 0));
		}
		/*ChessSquare->AttachToComponent(Floor, FAttachmentTransformRules::KeepRelativeTransform);*/


		ChessSquare->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		static ConstructorHelpers::FObjectFinder<UMaterialInstance>ColorPiece(TEXT("MaterialInstanceConstant'/Game/StarterContent/Blueprints/Assets/M_LightStage_Skybox_Black.M_LightStage_Skybox_Black'")); //Get black material piece
		UMaterialInstance* BlackPiece = ColorPiece.Object;

		switch (PiecesPositioning[i])
		{
			case 0: 
				break; 
			case 1:
			{
				ChessPiece = CreateDefaultSubobject<UStaticMeshComponent>((FName(PieceName)));
				//if (i > 32)
				//{
				//	static ConstructorHelpers::FObjectFinder<UStaticMesh>Pawn(TEXT("StaticMesh'/Game/StarterContent/Shapes/PawnWhite.PawnWhite'")); //Get white floor
				//	UStaticMesh* PawnMesh = Pawn.Object;
				//	ChessPiece->SetStaticMesh(PawnMesh);
				//}
				//else
				//{
					static ConstructorHelpers::FObjectFinder<UStaticMesh>Pawn(TEXT("StaticMesh'/Game/StarterContent/Shapes/PawnWhite.PawnWhite'"));
					UStaticMesh* PawnMesh = Pawn.Object;
					ChessPiece->SetStaticMesh(PawnMesh);
				//}
		
				ChessPiece->SetRelativeLocation(FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0));
				if (i > 32)
				{
					ChessPiece->SetMaterial(0, BlackPiece);
				}
				ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				break;
			}

			case 2:
			{
				ChessPiece = CreateDefaultSubobject<UStaticMeshComponent>((FName(PieceName)));
				static ConstructorHelpers::FObjectFinder<UStaticMesh>Tower(TEXT("StaticMesh'/Game/StarterContent/Shapes/RookWhite.RookWhite'")); //Get white floor
				UStaticMesh* TowerMesh = Tower.Object;
				ChessPiece->SetStaticMesh(TowerMesh);
				ChessPiece->SetRelativeLocation(FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0));
				if (i > 32)
				{
					ChessPiece->SetMaterial(0, BlackPiece);
				}
				ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				break;
			}
			case 3:
			{
				ChessPiece = CreateDefaultSubobject<UStaticMeshComponent>((FName(PieceName)));
				static ConstructorHelpers::FObjectFinder<UStaticMesh>Knight(TEXT("StaticMesh'/Game/StarterContent/Shapes/KnightWhite.KnightWhite'")); //Get white floor
				UStaticMesh* KnightMesh = Knight.Object;
				ChessPiece->SetStaticMesh(KnightMesh);
				ChessPiece->SetRelativeLocation(FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0));
				if (i > 32)
				{
					ChessPiece->SetMaterial(0, BlackPiece);
				}
				ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				break;
			}
			case 4:
			{
				ChessPiece = CreateDefaultSubobject<UStaticMeshComponent>((FName(PieceName)));
				static ConstructorHelpers::FObjectFinder<UStaticMesh>Bishop(TEXT("StaticMesh'/Game/StarterContent/Shapes/BishopWhite.BishopWhite'")); //Get white floor
				UStaticMesh* BishopMesh = Bishop.Object;
				ChessPiece->SetStaticMesh(BishopMesh);
				ChessPiece->SetRelativeLocation(FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0));
				if (i > 32)
				{
					ChessPiece->SetMaterial(0, BlackPiece);
				}
				ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				break;
			}
			case 5:
			{
				ChessPiece = CreateDefaultSubobject<UStaticMeshComponent>((FName(PieceName)));
				static ConstructorHelpers::FObjectFinder<UStaticMesh>Queen(TEXT("StaticMesh'/Game/StarterContent/Shapes/QueenWhite.QueenWhite'")); //Get white floor
				UStaticMesh* QueenMesh = Queen.Object;
				ChessPiece->SetStaticMesh(QueenMesh);
				ChessPiece->SetRelativeLocation(FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0));
				if (i > 32)
				{
					ChessPiece->SetMaterial(0, BlackPiece);
				}
				ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				break;
			}
			case 6:
			{
				ChessPiece = CreateDefaultSubobject<UStaticMeshComponent>((FName(PieceName)));
				static ConstructorHelpers::FObjectFinder<UStaticMesh>King(TEXT("StaticMesh'/Game/StarterContent/Shapes/KingWhite.KingWhite'")); //Get white floor
				UStaticMesh* KingMesh = King.Object;
				ChessPiece->SetStaticMesh(KingMesh);
				ChessPiece->SetRelativeLocation(FVector(i / 8 * 400 + 200, i % 8 * 400 + 200, 0));
				if (i > 32)
				{
					ChessPiece->SetMaterial(0, BlackPiece);
				}
				ChessPiece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				break;
			}

		}

		//GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Red, TEXT("for loop"));
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

	
}

// Called every frame
void ABoardSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoardSpawn::BoardGeneration()
{

}

