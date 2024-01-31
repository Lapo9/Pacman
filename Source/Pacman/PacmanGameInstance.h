#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PacmanGameInstance.generated.h"


UCLASS()
class PACMAN_API UPacmanGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual unsigned int GetLevel() const;
	virtual void LevelEnded(bool win);

	virtual void AddPoints(unsigned int value);

	unsigned int Lives = 2; // Remaining lives.

protected:
	unsigned int CurrentLevel;
	unsigned int Points; // Points till now.
};
