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
