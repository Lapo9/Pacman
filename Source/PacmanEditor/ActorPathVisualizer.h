#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"
#include "Pacman/ActorPath.h"


struct HPathPointVisProxy : public HComponentVisProxy {
    DECLARE_HIT_PROXY();

    HPathPointVisProxy(const UActorComponent* InComponent, FPathPoint* pathPoint) : HComponentVisProxy(InComponent, HPP_Wireframe), PathPoint{ pathPoint } {}

    FPathPoint* PathPoint;
};


class PACMANEDITOR_API FActorPathVisualizer : public FComponentVisualizer {
public:

    // Draws the path.
	virtual void DrawVisualization(const class UActorComponent* component, const class FSceneView* view, class FPrimitiveDrawInterface* pdi) override;
    
    // Stores a pointer to the clicked point of the path.
    virtual bool VisProxyHandleClick(FEditorViewportClient* viewportClient, HComponentVisProxy* visProxy, const FViewportClick& click) override;
    
    // Shows a transform widget to move the selected path point.
    virtual bool GetWidgetLocation(const FEditorViewportClient* viewportClient, FVector& outLocation) const override;
    
    // Moves the selected path point.
    virtual bool HandleInputDelta(FEditorViewportClient* viewportClient, FViewport* viewport, FVector& deltaTranslate, FRotator& deltaRotate, FVector& deltaScale) override;

private:
    FPathPoint* SelectedPathPoint;
};
