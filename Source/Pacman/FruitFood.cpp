#include "FruitFood.h"
#include "PacmanPawn.h"
#include "PacmanLevelState.h"


void AFruitFood::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("New food spawned for %i seconds"), Duration);
	// Set the timer to destroy the food
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AFruitFood::AutoDestroy, Duration, false);
}


// Called when another component starts overlapping with this one.
void AFruitFood::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) {
	if (otherActor->IsA(APacmanPawn::StaticClass())) {
		LevelState->NotifyFruitEaten(Value);
		AutoDestroy();
	}
}
