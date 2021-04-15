// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Engine/World.h"
#include "BoardSpawn.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent -> SetRelativeLocation(FVector(-40.f, 1.75f, 65.f));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	PController->bShowMouseCursor = true;

	/*FActorSpawnParameters SpawnParams;
	BoardRef = GetWorld()->SpawnActor<ABoardSpawn>(FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);*/
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Move Piece", IE_Pressed, this, &APlayerCharacter::MovePiece);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);


	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

}

void APlayerCharacter::MovePiece()
{
	APlayerController* PController = GetWorld()->GetFirstPlayerController(); //Get controller on the client 
	FHitResult hit;

	if (PController->GetHitResultUnderCursor(ECC_Visibility, true, hit)) //Check where client clicked 
	{
		TArray<AActor*> FoundActors;
		for (TActorIterator<ABoardSpawn> It(GetWorld()); It; ++It)
		{
			ABoardSpawn* BoardRef = *It;
			BoardRef->PiecesMovement(FVector(hit.Location));
		}
		
	}

}

void APlayerCharacter::MoveForward(float value)
{

	if (value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), value);
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Setting rotation"));

	}
}

void APlayerCharacter::MoveRight(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), value);

	}
}

void APlayerCharacter::TurnAtRate(float rate)
{
	AddControllerYawInput(rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float rate)
{
	AddControllerPitchInput(rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


