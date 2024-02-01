#include "PowerPelletFood.h"
#include "PacmanLevelState.h"
#include "PacmanPawn.h"


void APowerPelletFood::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("New power pellet food %s created"), *GetName());
}


// Called when another component starts overlapping with this one.
void APowerPelletFood::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) {
	if (otherActor->IsA(APacmanPawn::StaticClass())) {
		LevelState->NotifyPowerPelletEaten(Value);
		AutoDestroy();
	}
}
