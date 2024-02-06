#include "PacmanEditor.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"


IMPLEMENT_GAME_MODULE(FPacmanEditorModule, PacmanEditor);

DEFINE_LOG_CATEGORY(PacmanEditor)

#define LOCTEXT_NAMESPACE "PacmanEditor"

void FPacmanEditorModule::StartupModule() {
    UE_LOG(PacmanEditor, Warning, TEXT("PacmanEditor: Log Started"));

    if (GUnrealEd != NULL) {
    // Register actor path visualizer
        TSharedPtr<FActorPathVisualizer> ActorPathVisualizer = MakeShareable(new FActorPathVisualizer);
        if (ActorPathVisualizer.IsValid()) {
            GUnrealEd->RegisterComponentVisualizer(UActorPath::StaticClass()->GetFName(), ActorPathVisualizer);
            ActorPathVisualizer->OnRegister();
        }
    }
}

void FPacmanEditorModule::ShutdownModule() {
    UE_LOG(PacmanEditor, Warning, TEXT("PacmanEditor: Log Ended"));

    if (GUnrealEd != NULL) {
        // Unregister actor path visualizer
        GUnrealEd->UnregisterComponentVisualizer(UActorPath::StaticClass()->GetFName());
    }
}

#undef LOCTEXT_NAMESPACE