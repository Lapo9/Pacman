#include "PacmanPlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "PacmanPawn.h"
#include "Kismet/GameplayStatics.h"

APacmanPlayerController::APacmanPlayerController() {
}

void APacmanPlayerController::BeginPlay() {
	Super::BeginPlay();
	
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player)) {
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
			if (InputMappingContext) {
				InputSystem->AddMappingContext(InputMappingContext, 1);
				
			}
		}
	}

	// Find the first pacman to possess
	TArray<AActor*> pacmans;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APacmanPawn::StaticClass(), pacmans);
	checkf(pacmans.Num() == 1, TEXT("There should be only a single APacmanPawn in the level."));
	Possess(Cast<APawn>(pacmans[0]));
}


void APacmanPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	auto enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	enhancedInputComponent->BindAction(InputToTurnNorth, ETriggerEvent::Triggered, this, &APacmanPlayerController::TurnNorth);
	enhancedInputComponent->BindAction(InputToTurnEast, ETriggerEvent::Triggered, this, &APacmanPlayerController::TurnEast);
	enhancedInputComponent->BindAction(InputToTurnSouth, ETriggerEvent::Triggered, this, &APacmanPlayerController::TurnSouth);
	enhancedInputComponent->BindAction(InputToTurnWest, ETriggerEvent::Triggered, this, &APacmanPlayerController::TurnWest);
}


void APacmanPlayerController::TurnNorth(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Display, TEXT("TurnNorth"));
	auto pacman = Cast<APacmanPawn>(GetPawn());
	if (pacman) pacman->TurnDirection(EMovingDirection::NORTH);
}


void APacmanPlayerController::TurnEast(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Display, TEXT("TurnEast"));
	auto pacman = Cast<APacmanPawn>(GetPawn());
	if (pacman) pacman->TurnDirection(EMovingDirection::EAST);
}


void APacmanPlayerController::TurnSouth(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Display, TEXT("TurnSouth"));
	auto pacman = Cast<APacmanPawn>(GetPawn());
	if (pacman) pacman->TurnDirection(EMovingDirection::SOUTH);
}


void APacmanPlayerController::TurnWest(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Display, TEXT("TurnWest"));
	auto pacman = Cast<APacmanPawn>(GetPawn());
	if (pacman) pacman->TurnDirection(EMovingDirection::WEST);
}
