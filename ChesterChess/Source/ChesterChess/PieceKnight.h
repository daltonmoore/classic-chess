// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceKnight.generated.h"

/**
 * 
 */
UCLASS()
class CHESTERCHESS_API APieceKnight : public APiece
{
	GENERATED_BODY()
		APieceKnight(const FObjectInitializer& ObjectInitializer);


	TArray<int32> CalculateMoves(TArray<APiece*> Pieces, int CurrentPos) override;
	
};
