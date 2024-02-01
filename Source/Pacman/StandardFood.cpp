#include "StandardFood.h"
#include "PacmanPawn.h"
#include "PacmanLevelState.h"


void AStandardFood::BeginPlay() {
	Super::BeginPlay();
	LevelState->AddStandardFood(); // Tell the level state there is one more food
	UE_LOG(LogTemp, Display, TEXT("New standard food %s created"), *GetName());
}


// Called when another component starts overlapping with this one.
void AStandardFood::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) {
	if (otherActor->IsA(APacmanPawn::StaticClass())) {
		LevelState->NotifyStandardFoodEaten(Value);
		AutoDestroy();
	}
}
