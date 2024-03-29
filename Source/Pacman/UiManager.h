#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UiManager.generated.h"


// The UUiManager has the task to show on the viewport the selected menu, and to switch the UInputMappingContext to the context relative to the shown menu in the APacmanPlayerController.
UCLASS()
class PACMAN_API UUiManager : public UActorComponent {
	GENERATED_BODY()

public:	
	UUiManager();

	UFUNCTION(BlueprintCallable)
	virtual void Play();

	UFUNCTION(BlueprintCallable)
	virtual void Pause();

	UFUNCTION(BlueprintCallable)
	virtual void Resume();

	UFUNCTION(BlueprintCallable)
	virtual void ShowGameOverScreen();

	UFUNCTION(BlueprintCallable)
	virtual void ShowMainMenu();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere)
	class TSubclassOf<UUserWidget> MainMenuClass;
	UPROPERTY() // UPROPERTY is needed to avoid garbage collection
	class UUserWidget* MainMenu;
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* MainMenuContext;

	UPROPERTY(EditAnywhere)
	class TSubclassOf<UUserWidget> HudClass;
	UPROPERTY() // UPROPERTY is needed to avoid garbage collection
	class UUserWidget* Hud;
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* HudContext;

	UPROPERTY(EditAnywhere)
	class TSubclassOf<UUserWidget> PauseMenuClass;
	UPROPERTY() // UPROPERTY is needed to avoid garbage collection
	class UUserWidget* PauseMenu;
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* PauseMenuContext;

	UPROPERTY(EditAnywhere)
	class TSubclassOf<UUserWidget> GameOverScreenClass;
	UPROPERTY() // UPROPERTY is needed to avoid garbage collection
	class UUserWidget* GameOverScreen;
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* GameOverContex;
};
