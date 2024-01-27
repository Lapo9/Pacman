#include "BaseFood.h"
#include "PacmanLevelState.h"

ABaseFood::ABaseFood() {
	OnActorBeginOverlap.AddDynamic(this, &ABaseFood::OnBeginOverlap);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}


void ABaseFood::BeginPlay() {
	Super::BeginPlay();

	LevelState = Cast<APacmanLevelState>(GetWorld()->GetGameState());
}
