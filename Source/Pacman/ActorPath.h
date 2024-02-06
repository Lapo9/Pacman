#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPath.generated.h"


USTRUCT()
struct PACMAN_API FPathPoint {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FVector Location;

	UPROPERTY(EditAnywhere)
	float Speed; // Top speed for this section

	UPROPERTY(EditAnywhere)
	UCurveFloat* SpeedPercentage; // Percentage of the top speed based on the percentage of the distance to the next point travelled
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float RestTimeOnWayAhead; // How much time should the actor rest on this point?

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float RestTimeOnWayBack; // Same as above, but applied when the actor is on the way back (only relevant if Rewind is true in the corresponding ActorPath).

	UPROPERTY(VisibleAnywhere, Category = "Real-time info")
	float DistanceToNextPathPoint;
};



UCLASS(ClassGroup = (Pacman), meta = (BlueprintSpawnableComponent))
class PACMAN_API UActorPath : public UActorComponent {
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorPath();

	// Called every frame
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	const FLinearColor& GetGizmoColor() const;
	
	UPROPERTY(EditAnywhere)
	mutable TArray<FPathPoint> PathPoints;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Tick function when the actor is going forward.
	virtual void TickOnWayAhead(float deltaTime);

	// Tick function when the actor is on his way back (only used if bRewind is true).
	virtual void TickOnWayBack(float deltaTime);

	// Called when the target path point is reached.
	virtual void OnNextPathPointReached(int index);

	// Called when the actor must move again after he woke up from the rest on a path point
	virtual void ResumeAfterRest(int index);
	

	UPROPERTY(EditAnywhere)
	bool bRewind; // Should the actor follow the path backwards once he reaches the last point? Or should he be teleported to the first point?

	UPROPERTY(EditAnywhere)
	bool bFaceMovementDirection; // Should the actor face the direction he is moving towards?

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int Repetitions; // Repetitions of the path until it stopes. If Rewind is true, a repetion is considered "half" the path.

	UPROPERTY(EditAnywhere)
	FLinearColor GizmoColor;

	UPROPERTY(VisibleAnywhere, Category = "Real-time info")
	int CurrentPathPointIndex; // The current point in the path.
	
	UPROPERTY(VisibleAnywhere, Category = "Real-time info")
	int DoneRepetitions; // How many repetitions have already been carried out?

	UPROPERTY(VisibleAnywhere, Category = "Real-time info")
	bool bActorCanMove; // Actor is allowed to move

	UPROPERTY(VisibleAnywhere, Category = "Real-time info")
	bool bCurrentDirectionIsAhead; // Is the actor going forward o backward?


	TFunction<void(float)> CurrentTickFunction; // Tick functio called (based on if the actor is going forward or backwards)

	AActor* OwnedActor; // The actor to move.

	float PointLikeThreshold; // How close to the next point to consider it an overlap.

	FTimerHandle RestTimer;
};
