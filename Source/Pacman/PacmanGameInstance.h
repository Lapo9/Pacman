#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PacmanGameInstance.generated.h"


// Stores some global variables, such as the lives, the score and the current level.
UCLASS()
class PACMAN_API UPacmanGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual unsigned int GetLevel() const;

	// Initializes the lives, score and level
	virtual void StartNewGame();

	// Sets the level to the next one
	virtual void StartNextLevel();

	virtual int AddPoints(unsigned int value);

	virtual void AddExtraLife();

	UPROPERTY(BlueprintReadOnly)
	int Lives = 3; // Remaining lives.

protected:
	UPROPERTY(BlueprintReadOnly)
	int CurrentLevel;

	UPROPERTY(BlueprintReadOnly)
	int Points; // Points till now.
};
