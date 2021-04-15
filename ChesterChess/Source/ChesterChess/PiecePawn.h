// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PiecePawn.generated.h"

/**
 * 
 */
UCLASS()
class CHESTERCHESS_API APiecePawn : public APiece
{
	GENERATED_BODY()

		APiecePawn(const FObjectInitializer& ObjectInitializer);


	TSet<int32> CalculateMoves(TArray<APiece*> Pieces, int CurrentPos) override;
	
};
