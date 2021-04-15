// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceQueen.generated.h"

/**
 * 
 */
UCLASS()
class CHESTERCHESS_API APieceQueen : public APiece
{
	GENERATED_BODY()

		APieceQueen(const FObjectInitializer& ObjectInitializer);

		
		TSet<int32> CalculateMoves(TArray<APiece*> Pieces, int CurrentPos) override;

	virtual void Tick(float DeltaTime) override;
	
};
