#include "PerformanceForgeToolsetModule.h"

#include "PerformanceForgeToolset.h"
#include "ToolsetRegistry/UToolsetRegistry.h"

DEFINE_LOG_CATEGORY(LogPerformanceForgeToolset);

#define LOCTEXT_NAMESPACE "FPerformanceForgeToolsetModule"

void FPerformanceForgeToolsetModule::StartupModule()
{
	if (!UToolsetRegistry::IsAvailable())
	{
		// Expected outside the editor, and whenever the experimental plugins are off. Not an error.
		UE_LOG(LogPerformanceForgeToolset, Log,
			TEXT("Toolset registry unavailable - performance tools not registered."));
		return;
	}

	if (UToolsetRegistry::IsToolsetClassRegistered(UPerformanceForgeToolset::StaticClass()))
	{
		bRegistered = true;
		return;
	}

	UToolsetRegistry::RegisterToolsetClass(UPerformanceForgeToolset::StaticClass());
	bRegistered = UToolsetRegistry::IsToolsetClassRegistered(UPerformanceForgeToolset::StaticClass());

	// Worth logging either way. The failure this replaces was silent: the module loaded, the class
	// existed, and the tools simply were not there.
	UE_LOG(LogPerformanceForgeToolset, Log, TEXT("PerformanceForge toolset %s."),
		bRegistered ? TEXT("registered") : TEXT("failed to register"));
}

void FPerformanceForgeToolsetModule::ShutdownModule()
{
	if (bRegistered && UToolsetRegistry::IsAvailable())
	{
		UToolsetRegistry::UnregisterToolsetClass(UPerformanceForgeToolset::StaticClass());
		bRegistered = false;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPerformanceForgeToolsetModule, PerformanceForgeToolset)
