#include "PerformanceForgeToolset.h"

#include "PerformanceForge.h"
#include "PerformanceForgeSubsystem.h"

#include "Framework/Docking/TabManager.h"

FString UPerformanceForgeToolset::QueueRecordingSession(const FString& SelectionJson)
{
	UPerformanceForgeSubsystem* Subsystem = UPerformanceForgeSubsystem::Get();
	if (!Subsystem)
	{
		return TEXT("ERROR: the PerformanceForge subsystem is not available.");
	}

	// The context is a superset of the session shape on purpose: extra fields pass through
	// harmlessly, so the pushing surface never has to know which subset this tool reads.
	const FString Result = Subsystem->QueueSessionJson(SelectionJson);

	if (!Result.Contains(TEXT("ERROR")))
	{
		FGlobalTabmanager::Get()->TryInvokeTab(FName(TEXT("PerformanceCapture")));
	}

	return Result;
}

FString UPerformanceForgeToolset::PlanRecordingSession(const FString& SelectionJson)
{
	UPerformanceForgeSubsystem* Subsystem = UPerformanceForgeSubsystem::Get();
	if (!Subsystem)
	{
		return TEXT("ERROR: the PerformanceForge subsystem is not available.");
	}

	FString Error;
	const FString AssetPath = Subsystem->CreateSessionAsset(SelectionJson, /*bPlanned=*/true, Error);
	if (AssetPath.IsEmpty())
	{
		return FString::Printf(TEXT("ERROR: %s"), *Error);
	}

	return FString::Printf(
		TEXT("Planned session saved: %s. Open Performance Capture's ledger to run it - takes will be ")
		TEXT("kept, and nothing is processed until a take is chosen."),
		*AssetPath);
}

FString UPerformanceForgeToolset::OpenRecordingSession(const FString& SessionAssetPath)
{
	UPerformanceForgeSubsystem* Subsystem = UPerformanceForgeSubsystem::Get();
	if (!Subsystem)
	{
		return TEXT("ERROR: the PerformanceForge subsystem is not available.");
	}

	return Subsystem->OpenSessionAt(SessionAssetPath);
}

FString UPerformanceForgeToolset::StartPlannedSession(const FString& SessionAssetPath)
{
	UPerformanceForgeSubsystem* Subsystem = UPerformanceForgeSubsystem::Get();
	if (!Subsystem)
	{
		return TEXT("ERROR: the PerformanceForge subsystem is not available.");
	}

	const FString Result = Subsystem->QueueSessionFromAsset(SessionAssetPath);
	if (!Result.Contains(TEXT("ERROR")))
	{
		FGlobalTabmanager::Get()->TryInvokeTab(FName(TEXT("PerformanceCapture")));
	}
	return Result;
}

TArray<FString> UPerformanceForgeToolset::ListPerformanceSessions()
{
	UPerformanceForgeSubsystem* Subsystem = UPerformanceForgeSubsystem::Get();
	return Subsystem ? Subsystem->ListSessionAssets() : TArray<FString>();
}

UToolCallAsyncResultPerformanceString* UPerformanceForgeToolset::ChoosePerformanceTake(
	const FString& BankPath, const FString& LineId, const FString& TakeId)
{
	UPerformanceForgeSubsystem* Subsystem = UPerformanceForgeSubsystem::Get();
	if (!Subsystem)
	{
		return nullptr;
	}

	UToolCallAsyncResultPerformanceString* Async = NewObject<UToolCallAsyncResultPerformanceString>();

	Subsystem->ChooseTake(BankPath, LineId, TakeId,
		[Async](bool bSuccess, const FString& Message)
	{
		if (bSuccess)
		{
			Async->SetValue(Message);
		}
		else
		{
			Async->SetError(Message);
		}
	});

	return Async;
}
