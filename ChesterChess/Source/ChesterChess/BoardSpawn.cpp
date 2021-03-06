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

	for (int i = 0; i < 64; i++)
	{
		//Make a dynamic name for the subobject to avoid erros
		FString ComponentName = FString::FromInt(i);

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

