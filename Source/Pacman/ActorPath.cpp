#include "ActorPath.h"
#include "PacmanSettings.h"


// Sets default values for this component's properties
UActorPath::UActorPath() : 
	CurrentTickFunction{ [this](float delta) { TickOnWayAhead(delta); } }, 
	bCurrentDirectionIsAhead{ true }, bActorCanMove{ true }, bFaceMovementDirection{ true }, GizmoColor{ FLinearColor::Yellow } {
		PrimaryComponentTick.bCanEverTick = true;
		OwnedActor = GetOwner();
}


// Finishes the initialization of the FPathPoints (e.g. stores the distance to the next point), and other variables.
void UActorPath::BeginPlay() {
	Super::BeginPlay();
	OwnedActor = GetOwner();
	DistanceToCurrentPathPointLastFrame = TNumericLimits<float>::Max();
	if (PathPoints.Num() < 2) {
		UE_LOG(LogTemp, Warning, TEXT("ActorPath of %s has less than 2 points"), *OwnedActor->GetName());
		bActorCanMove = false;
	}
	PointLikeThreshold = Cast<APacmanSettings>(OwnedActor->GetWorld()->GetWorldSettings())->PointLikeTriggersRadius;
	PathOrigin = bInObjectLocalSpace * OwnedActor->GetActorLocation(); // If it is in world space it is <0,0,0>
	OwnedActor->SetActorLocation(PathPoints[0].GetLocation(GetPathOrigin())); // Sets the starting position of the owned actor

	// Set distances
	for (int i = 0; i < PathPoints.Num() - 1; i++) {
		PathPoints[i].DistanceToNextPathPoint = (PathPoints[i].GetLocation(GetPathOrigin()) - PathPoints[i + 1].GetLocation(GetPathOrigin())).Length();
	}
}


void UActorPath::TickOnWayAhead(float deltaTime) {
	FVector dir = PathPoints[CurrentPathPointIndex + 1].GetLocation(GetPathOrigin()) - OwnedActor->GetActorLocation();
	float dirLength = dir.Length();

	// The second check is there because, if we miss the "hit" with the target due to low fps, we still catch the event in the next frame (the distance should be monotonically decreasing)
	if (dirLength < PointLikeThreshold || dirLength > DistanceToCurrentPathPointLastFrame) {
		DistanceToCurrentPathPointLastFrame = TNumericLimits<float>::Max();
		OnNextPathPointReached(++CurrentPathPointIndex);
	}
	else {
		auto& p = PathPoints[CurrentPathPointIndex];
		float alreadyCoveredDistancePercentage = 1.f - (dirLength / p.DistanceToNextPathPoint); // Based on this we get the speed the actor must reach at this point based on the float curve of the path point)
		dir.Normalize();
		FVector deltaPos = FMath::Abs(p.Speed * p.SpeedPercentage->GetFloatValue(alreadyCoveredDistancePercentage)) * deltaTime * dir;
		OwnedActor->SetActorLocation(OwnedActor->GetActorLocation() + deltaPos);
		DistanceToCurrentPathPointLastFrame = dirLength;
	}
}


void UActorPath::TickOnWayBack(float deltaTime) {
	FVector dir = PathPoints[CurrentPathPointIndex - 1].GetLocation(GetPathOrigin()) - OwnedActor->GetActorLocation();
	float dirLength = dir.Length();

	// The second check is there because, if we miss the "hit" with the target due to low fps, we still catch the event in the next frame (the distance should be monotonically decreasing)
	if (dirLength >= DistanceToCurrentPathPointLastFrame) {
		DistanceToCurrentPathPointLastFrame = TNumericLimits<float>::Max();
		OnNextPathPointReached(--CurrentPathPointIndex);
	}
	else {
		auto& p = PathPoints[CurrentPathPointIndex - 1]; // Info are taken from the previous path point
		float alreadyCoveredDistancePercentage = dirLength / p.DistanceToNextPathPoint;
		dir.Normalize();
		FVector deltaPos = FMath::Abs(p.Speed * p.SpeedPercentage->GetFloatValue(alreadyCoveredDistancePercentage)) * deltaTime * dir;
		OwnedActor->SetActorLocation(OwnedActor->GetActorLocation() + deltaPos);
		DistanceToCurrentPathPointLastFrame = dirLength;
	}
}


void UActorPath::OnNextPathPointReached(int index) {
	// Make the actor rest (if needed)
	if ((bCurrentDirectionIsAhead && PathPoints[index].RestTimeOnWayAhead > 0) || (!bCurrentDirectionIsAhead && PathPoints[index].RestTimeOnWayBack > 0)) {
		bActorCanMove = false;
		GetWorld()->GetTimerManager().SetTimer(RestTimer, [this, index]() { ResumeAfterRest(index); }, bCurrentDirectionIsAhead ? PathPoints[index].RestTimeOnWayAhead : PathPoints[index].RestTimeOnWayBack, false);
	}
	else {
		ResumeAfterRest(index);
	}
}


void UActorPath::ResumeAfterRest(int index) {
	bActorCanMove = true;
	// Set next mode based on rewind flag and repetitions

	// If we were going forward and reached the final point
	if (index >= PathPoints.Num() - 1) {
		DoneRepetitions++;
		if (Repetitions > 0 && DoneRepetitions >= Repetitions) {
			bActorCanMove = false;
			return;
		}
		else if (bRewind) {
			bCurrentDirectionIsAhead = false;
			CurrentTickFunction = [this](float delta) { TickOnWayBack(delta); };
		}
		else {
			CurrentPathPointIndex = 0;
			OwnedActor->SetActorLocation(PathPoints[0].GetLocation(GetPathOrigin()));
		}
	}

	// If we were going backward and reached the final (i.e. first) point
	else if (index <= 0) {
		DoneRepetitions++;
		if (Repetitions > 0 && DoneRepetitions >= Repetitions) {
			bActorCanMove = false;
			return;
		}
		else {
			bCurrentDirectionIsAhead = true;
			CurrentTickFunction = [this](float delta) { TickOnWayAhead(delta); };
		}
	}

	// Rotate the actor towards the moving direction if the option is activated
	if (!bFaceMovementDirection) return;
	auto dir = bCurrentDirectionIsAhead ? 
		PathPoints[CurrentPathPointIndex + 1].GetLocation(GetPathOrigin()) - PathPoints[CurrentPathPointIndex].GetLocation(GetPathOrigin()) :
		PathPoints[CurrentPathPointIndex - 1].GetLocation(GetPathOrigin()) - PathPoints[CurrentPathPointIndex].GetLocation(GetPathOrigin());
	// If the speed is negative it means the actor must move backwards
	if (PathPoints[CurrentPathPointIndex - (int)!bCurrentDirectionIsAhead].Speed < 0) dir = -dir;
	dir.Normalize();
	auto rotator = dir.ToOrientationRotator();
	OwnedActor->SetActorRotation(rotator);
}


// Called every frame
void UActorPath::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) {
	if (!bActorCanMove) return;
	Super::TickComponent(deltaTime, tickType, thisTickFunction);
	CurrentTickFunction(deltaTime);
}


const FLinearColor& UActorPath::GetGizmoColor() const {
	return GizmoColor;
}


FVector UActorPath::GetPathOrigin() const {
	return PathOrigin;
}


#if WITH_EDITOR
void UActorPath::PostEditChangeProperty(FPropertyChangedEvent& e) {
	Super::PostEditChangeProperty(e);
	// Set the path origin (TODO this doesn't get called if the parent actor is moved, which would be very useful)
	PathOrigin = bInObjectLocalSpace * OwnedActor->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Origin: <%f ,%f >"), PathOrigin.X, PathOrigin.Y);
}
#endif
