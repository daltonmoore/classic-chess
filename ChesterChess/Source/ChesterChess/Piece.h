// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Piece.generated.h"

UCLASS()
class CHESTERCHESS_API APiece : public AActor
{
	GENERATED_BODY()
		

	
public:	
	// Sets default values for this actor's properties
	APiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* Mesh;

	UPROPERTY()
		FString PieceType;

	UPROPERTY(EditAnywhere, Category = Pieces)
		int PieceColor;
	
	UFUNCTION()
		virtual TArray<int32> CalculateMoves(TArray<APiece*> Pieces, int CurrentPos);

	UFUNCTION()
		void TestingCalculateMoves();
		
	void SetFirstMove();

	int PieceValue;
	bool FirstMove;


};
