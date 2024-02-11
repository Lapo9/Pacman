#include "FruitFood.h"
#include "PacmanPawn.h"
#include "PacmanLevelState.h"
#include "PacmanUtilities.h"


AFruitFood::AFruitFood() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


void AFruitFood::Tick(float deltaTime) {
	AddActorLocalRotation(FRotator{ 0.f, deltaTime * RotationSpeed, 0.f }); // Rotate the fruit (just an animation, could be done in the shader maybe)
}


void AFruitFood::AutoDestroy() {
	GetWorld()->GetTimerManager().ClearTimer(Timer);
	Super::AutoDestroy();
}


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
