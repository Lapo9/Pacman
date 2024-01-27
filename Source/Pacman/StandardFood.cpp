#include "StandardFood.h"
#include "PacmanPawn.h"
#include "PacmanLevelState.h"


AStandardFood::AStandardFood() {
}


void AStandardFood::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("New standard food %s created"), *GetName());
}


// Destroys this actor (with an animation/particle effect)
void AStandardFood::AutoDestroy() {
	auto name = GetName();
	verifyf(Destroy(), TEXT("AStandardFood could not be destroyed for unknown reasons."));
	UE_LOG(LogTemp, Display, TEXT("Standard food %s eaten"), *name);
	// TODO particle effect
}


// Called when another component starts overlapping with this one.
void AStandardFood::OnBeginOverlap(AActor* overlappedActor, AActor* otherActor) {
	if (overlappedActor->IsA(APacmanPawn::StaticClass())) {
		LevelState->NotifyStandardFoodEaten(Value);
		AutoDestroy();
	}
}
