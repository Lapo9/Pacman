#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PacmanPlayerController.generated.h"


UCLASS()
class PACMAN_API APacmanPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APacmanPlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void ChangeInputMappingContext(UInputMappingContext* context);

protected:
	void TurnNorth(const struct FInputActionValue& Value);
	void TurnEast(const struct FInputActionValue& Value);
	void TurnSouth(const struct FInputActionValue& Value);
	void TurnWest(const struct FInputActionValue& Value);
	void PauseGameplay(const struct FInputActionValue& Value);
	void ResumeGameplay(const struct FInputActionValue& Value);
	void SkipGameOverScreen(const struct FInputActionValue& Value);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* InputToTurnNorth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* InputToTurnEast;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* InputToTurnSouth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* InputToTurnWest;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* InputToPauseGameplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* InputToResumeGameplay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* InputToSkipGameOverScreen;
};
