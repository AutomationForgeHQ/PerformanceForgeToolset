#pragma once

#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

PERFORMANCEFORGETOOLSET_API DECLARE_LOG_CATEGORY_EXTERN(LogPerformanceForgeToolset, Log, All);

/**
 * Registers PerformanceForge's tools with the toolset registry, and nothing else.
 *
 * The capability plugin loads and works with this one absent - it simply has no agent surface.
 * That is the whole reason the toolset is a separate plugin: a registry that fails to load, or an
 * experimental plugin somebody turned off, must not take performance capture down with it.
 */
class FPerformanceForgeToolsetModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	bool bRegistered = false;
};
