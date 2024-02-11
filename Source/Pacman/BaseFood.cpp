#include "BaseFood.h"
#include "PacmanLevelState.h"
#include "PacmanUtilities.h"
#include "Components/SphereComponent.h"


ABaseFood::ABaseFood() {
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionObjectType(ObjectChannel_BoardPawns); // BoardPawns object channel
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Disable all collisions
	Mesh->SetCollisionResponseToChannel(ObjectChannel_BoardPawns, ECollisionResponse::ECR_Overlap); // enable BoardPawns object channel
}


void ABaseFood::BeginPlay() {
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseFood::OnBeginOverlap); // For whatever reason this must stay here and not in the ctor
	LevelState = Cast<APacmanLevelState>(GetWorld()->GetGameState());
	Super::BeginPlay();
}

void ABaseFood::AutoDestroy() {
	auto name = GetName();
	verifyf(Destroy(), TEXT("Food %s could not be destroyed for unknown reasons."), *name);
	UE_LOG(LogTemp, Display, TEXT("Food %s destroyed"), *name);
}
