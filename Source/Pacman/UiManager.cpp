#include "UiManager.h"
#include "Blueprint/UserWidget.h"
#include "PacmanPlayerController.h"
#include "InputMappingContext.h"
#include "PacmanGameMode.h"
#include "Kismet/GameplayStatics.h"


UUiManager::UUiManager() {
}


// Called when the game starts
void UUiManager::BeginPlay() {
	Super::BeginPlay();
	MainMenu = CreateWidget(GetWorld(), MainMenuClass);
	Hud = CreateWidget(GetWorld(), HudClass);
	PauseMenu = CreateWidget(GetWorld(), PauseMenuClass);
	GameOverScreen = CreateWidget(GetWorld(), GameOverScreenClass);

	if (MainMenu == nullptr) { UE_LOG(LogTemp, Error, TEXT("Couldn't create main menu")); }
	else {
		MainMenu->AddToViewport();
		Cast<APacmanPlayerController>(GetWorld()->GetFirstPlayerController())->bShowMouseCursor = true;
	}
}


void UUiManager::Play() {
	MainMenu->RemoveFromParent();
	Hud->AddToViewport();
	auto* playerController = Cast<APacmanPlayerController>(GetWorld()->GetFirstPlayerController());
	playerController->ChangeInputMappingContext(HudContext); // Set the input mapping context
	playerController->bShowMouseCursor = false; // Hide the mouse
	Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode())->Start(); // Start play
}


void UUiManager::Pause() {
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	auto* playerController = Cast<APacmanPlayerController>(GetWorld()->GetFirstPlayerController());
	playerController->ChangeInputMappingContext(PauseMenuContext); // Set the input mapping context
	playerController->bShowMouseCursor = true;
	Hud->RemoveFromParent();
	PauseMenu->AddToViewport();
}


void UUiManager::Resume() {
	PauseMenu->RemoveFromParent();
	Hud->AddToViewport();
	auto* playerController = Cast<APacmanPlayerController>(GetWorld()->GetFirstPlayerController());
	playerController->ChangeInputMappingContext(HudContext); // Set the input mapping context
	playerController->bShowMouseCursor = false;
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}


void UUiManager::ShowGameOverScreen() {
	Hud->RemoveFromParent();
	GameOverScreen->AddToViewport();
	Cast<APacmanPlayerController>(GetWorld()->GetFirstPlayerController())->ChangeInputMappingContext(GameOverContex); // Set the input mapping context
}


void UUiManager::ShowMainMenu() {
	GameOverScreen->RemoveFromParent();
	MainMenu->AddToViewport();
	auto* playerController = Cast<APacmanPlayerController>(GetWorld()->GetFirstPlayerController());
	playerController->ChangeInputMappingContext(MainMenuContext); // Set the input mapping context
	playerController->bShowMouseCursor = true;
}
