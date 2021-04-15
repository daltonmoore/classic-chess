// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceBishop.generated.h"

/**
 * 
 */
UCLASS()
class CHESTERCHESS_API APieceBishop : public APiece
{
	GENERATED_BODY()
		APieceBishop(const FObjectInitializer& ObjectInitializer);


	TSet<int32> CalculateMoves(TArray<APiece*> Pieces, int CurrentPos) override;
	
};
