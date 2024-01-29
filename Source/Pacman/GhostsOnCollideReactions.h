#pragma once

#include "CoreMinimal.h"
#include "GhostsOnCollideReactions.generated.h"

#define GHOST_ON_COLLIDE_REACTION_PARAMS class AActor* collidedWith, class UPrimitiveComponent* collidedWithComponent


UCLASS(Abstract)
class PACMAN_API UGhostOnCollideReactionBase : public UObject {
	GENERATED_BODY()
public:
	virtual void React(GHOST_ON_COLLIDE_REACTION_PARAMS) const PURE_VIRTUAL(UGhostOnCollideReactionBase::React,);
};


UCLASS()
class PACMAN_API UGhostOnCollideReactionKillPacman final : public UGhostOnCollideReactionBase {
	GENERATED_BODY()
public:
	void React(GHOST_ON_COLLIDE_REACTION_PARAMS) const;
};


UCLASS()
class PACMAN_API UGhostOnCollideReactionDie final : public UGhostOnCollideReactionBase {
	GENERATED_BODY()
public:
	void React(GHOST_ON_COLLIDE_REACTION_PARAMS) const;
};


UCLASS()
class PACMAN_API UGhostOnCollideReactionNothing final : public UGhostOnCollideReactionBase {
	GENERATED_BODY()
public:
	void React(GHOST_ON_COLLIDE_REACTION_PARAMS) const;
};

