#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ActorPath.generated.h"


// Represents one of the points of the UActorPath. Each FPathPoint stores the data of the segment whose has this point as starting point.
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

	float DistanceToNextPathPoint;

	FVector GetLocation(FVector origin = FVector{0.f,0.f,0.f}) const { return origin + Location; }
};



// Represents a static linear path an actor must follow.
// Its tasks are to store tha path itself and to actually move the owned actor on the path.
UCLASS(ClassGroup = (Pacman), meta = (BlueprintSpawnableComponent))
class PACMAN_API UActorPath : public UActorComponent {
	GENERATED_BODY()

public:	
	UActorPath();

	// Called every frame
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	// Returns the color of the gizmo representing this path
	const FLinearColor& GetGizmoColor() const;

	// Returns the point that is considered the origin of the reference cartesian system of the points of this path.
	FVector GetPathOrigin() const;
	
#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
#endif

	UPROPERTY(EditAnywhere)
	mutable TArray<FPathPoint> PathPoints;

protected:
	// Finishes the initialization of the FPathPoints (e.g. stores the distance to the next point), and other variables.
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
	bool bInObjectLocalSpace; // Whether the path points are in world space or in local space.

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

	float DistanceToCurrentPathPointLastFrame; // The last distance from the current targeted path point

	FVector PathOrigin; // The origin of the path (can be in local space or in world space)
};
