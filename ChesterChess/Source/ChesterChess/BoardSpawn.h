// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardSpawn.generated.h"

UCLASS()
class CHESTERCHESS_API ABoardSpawn : public AActor
{
	GENERATED_BODY()

		/*UPROPERTY(VisibleDefaultsOnly, Category = Root)
		class URootComponent* Root;*/

		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* Floor;

		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* ChessSquare;

		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* ChessPiece;
		
		/*UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UMaterial* PieceColor;*/
	
public:	
	// Sets default values for this actor's properties
	ABoardSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pieces)
		TArray<int> PiecesPositioning;

	UFUNCTION()
	void BoardGeneration();


};
