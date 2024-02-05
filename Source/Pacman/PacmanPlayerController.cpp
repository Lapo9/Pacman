#include "PacmanPlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "PacmanPawn.h"
#include "PacmanGameMode.h"
#include "Kismet/GameplayStatics.h"

APacmanPlayerController::APacmanPlayerController() {
}

void APacmanPlayerController::BeginPlay() {
	Super::BeginPlay();
	
	// Find the first pacman to possess
	TArray<AActor*> pacmans;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APacmanPawn::StaticClass(), pacmans);
	checkf(pacmans.Num() == 1, TEXT("There should be only a single APacmanPawn in the level."));
	Possess(Cast<APawn>(pacmans[0]));
}


void APacmanPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	auto enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	enhancedInputComponent->BindAction(InputToTurnNorth, ETriggerEvent::Started, this, &APacmanPlayerController::TurnNorth);
	enhancedInputComponent->BindAction(InputToTurnEast, ETriggerEvent::Started, this, &APacmanPlayerController::TurnEast);
	enhancedInputComponent->BindAction(InputToTurnSouth, ETriggerEvent::Started, this, &APacmanPlayerController::TurnSouth);
	enhancedInputComponent->BindAction(InputToTurnWest, ETriggerEvent::Started, this, &APacmanPlayerController::TurnWest);
	enhancedInputComponent->BindAction(InputToPauseGameplay, ETriggerEvent::Started, this, &APacmanPlayerController::PauseGameplay);
	enhancedInputComponent->BindAction(InputToResumeGameplay, ETriggerEvent::Started, this, &APacmanPlayerController::ResumeGameplay);
	enhancedInputComponent->BindAction(InputToSkipGameOverScreen, ETriggerEvent::Started, this, &APacmanPlayerController::SkipGameOverScreen);
}


void APacmanPlayerController::ChangeInputMappingContext(UInputMappingContext* context) {
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player)) {
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
			if(InputMappingContext) InputSystem->RemoveMappingContext(InputMappingContext); // Remove old context
			if (context) {
				InputSystem->AddMappingContext(context, 1); // Set new context
				UE_LOG(LogTemp, Display, TEXT("Input mapping context changed from %s to %s"), InputMappingContext ? *InputMappingContext->GetName() : TEXT("null"), *context->GetName());
				InputMappingContext = context;
			}
		}
	}
}


void APacmanPlayerController::TurnNorth(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Display, TEXT("TurnNorth command"));
	auto pacman = Cast<APacmanPawn>(GetPawn());
	if (pacman) pacman->TurnDirection(EMovingDirection::NORTH);
}


void APacmanPlayerController::TurnEast(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Display, TEXT("TurnEast command"));
	auto pacman = Cast<APacmanPawn>(GetPawn());
	if (pacman) pacman->TurnDirection(EMovingDirection::EAST);
}


void APacmanPlayerController::TurnSouth(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Display, TEXT("TurnSouth command"));
	auto pacman = Cast<APacmanPawn>(GetPawn());
	if (pacman) pacman->TurnDirection(EMovingDirection::SOUTH);
}


void APacmanPlayerController::TurnWest(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Display, TEXT("TurnWest command"));
	auto pacman = Cast<APacmanPawn>(GetPawn());
	if (pacman) pacman->TurnDirection(EMovingDirection::WEST);
}


void APacmanPlayerController::PauseGameplay(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Display, TEXT("PauseGameplay command"));
	Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode())->GetUiManager()->Pause();
}


void APacmanPlayerController::ResumeGameplay(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Display, TEXT("ResumeGameplay command"));
	Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode())->GetUiManager()->Resume();
}


void APacmanPlayerController::SkipGameOverScreen(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Display, TEXT("SkipGameOverScreen command"));
	Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode())->GetUiManager()->ShowMainMenu();
}
