#include "ActorPathVisualizer.h"

IMPLEMENT_HIT_PROXY(HPathPointVisProxy, HComponentVisProxy);


void FActorPathVisualizer::DrawVisualization(const UActorComponent* component, const FSceneView* view, FPrimitiveDrawInterface* pdi) {
	const UActorPath* actorPath = Cast<UActorPath>(component);
	if (!actorPath || actorPath->PathPoints.Num() <= 0) return; 
	PathOrigin = actorPath->GetPathOrigin(); // The path points can be in world coordinates or in local coordinates

	auto& pathPoints = actorPath->PathPoints;
	for (int i = 0; i < pathPoints.Num() - 1; ++i) {
		pdi->SetHitProxy(new HPathPointVisProxy(component, &pathPoints[i]));
		pdi->DrawPoint(pathPoints[i].GetLocation(PathOrigin), i == 0 ? FLinearColor::Green : actorPath->GetGizmoColor(), 10.f, 1); // First point is green
		pdi->DrawLine(pathPoints[i].GetLocation(PathOrigin), pathPoints[i + 1].GetLocation(PathOrigin), actorPath->GetGizmoColor(), 10.f);
		pdi->SetHitProxy(nullptr);
	}

	pdi->SetHitProxy(new HPathPointVisProxy(component, &pathPoints[pathPoints.Num() - 1]));
	pdi->DrawPoint(pathPoints[pathPoints.Num() - 1].GetLocation(PathOrigin), FLinearColor::Red, 10.f, 1); // Last point is red
	pdi->SetHitProxy(nullptr);
}


bool FActorPathVisualizer::VisProxyHandleClick(FEditorViewportClient* viewportClient, HComponentVisProxy* visProxy, const FViewportClick& click) {
	if (visProxy && visProxy->Component.IsValid()) {
		if (visProxy->IsA(HPathPointVisProxy::StaticGetType())) {
			SelectedPathPoint = ((HPathPointVisProxy*)visProxy)->PathPoint;
		}
		return true;
	}

	SelectedPathPoint = nullptr;
	return false;
	
}


bool FActorPathVisualizer::GetWidgetLocation(const FEditorViewportClient* viewportClient, FVector& outLocation) const {
	if (!SelectedPathPoint) return false;

	outLocation = SelectedPathPoint->GetLocation(PathOrigin);
	return true;
}


bool FActorPathVisualizer::HandleInputDelta(FEditorViewportClient* viewportClient, FViewport* viewport, FVector& deltaTranslate, FRotator& deltaRotate, FVector& deltaScale) {
	if (!SelectedPathPoint) return false;

	SelectedPathPoint->Location += deltaTranslate;
	return true;
}
