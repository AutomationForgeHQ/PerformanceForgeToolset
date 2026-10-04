// The recording session, as a tool an agent or another panel can start.

#pragma once

#include "CoreMinimal.h"
#include "ToolsetRegistry/ToolCallAsyncResult.h"
#include "ToolsetRegistry/ToolsetDefinition.h"
#include "PerformanceForgeToolset.generated.h"

/** An async tool call that completes with a line of text - the choose runs a whole pipeline. */
UCLASS(BlueprintType)
class PERFORMANCEFORGETOOLSET_API UToolCallAsyncResultPerformanceString : public UToolCallAsyncResult
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "PerformanceForge")
	bool SetValue(const FString& InValue)
	{
		return MaybeBroadcastSuccessfulCompletion(FString(InValue), Value);
	}

	UPROPERTY(BlueprintReadOnly, Category = "PerformanceForge")
	FString Value;
};

/**
 * PerformanceForge's flow as Model Context Protocol tools: plan a session, start it, list the
 * jobs, choose a take. Every function forwards to `UPerformanceForgeSubsystem` and adds nothing,
 * so the whole director's flow is reachable by an agent exactly as it is by the panel.
 *
 * A separate plugin from the capability, which is the family rule and earns itself here twice
 * over: the registry is experimental and may be switched off, and PerformanceForge is sold while
 * this surface is published - so the thing that must keep working when the toolset is absent is
 * the capture stage, and it does.
 */
UCLASS()
class PERFORMANCEFORGETOOLSET_API UPerformanceForgeToolset : public UToolsetDefinition
{
	GENERATED_BODY()

public:
	/** Kept in step with the descriptor by tools/versions.ps1, which fails the check when it drifts. */
	/**
	 * The version an agent is told it is talking to, read from this plugin's own descriptor.
	 *
	 * Defined in the .cpp deliberately. UE_PLUGIN_NAME is a private UBT definition, correct
	 * only inside this module; a body here in a public header would resolve it to whichever
	 * plugin included the header. Nothing in this class is a second copy of the version, so
	 * there is nothing here that can drift from it.
	 */
	virtual FString GetToolsetVersion() const override;

	/**
	 * Queue a recording session and open the capture panel as a teleprompter.
	 *
	 * Takes the Speech Library's selection context - mode, voice treatment, face bank, and the
	 * lines with their text, speakers and cast voices - and turns the capture panel into a
	 * line-by-line session: record, next line, while each finished take converts, applies and
	 * solves in the background through the same tools this one sits beside.
	 *
	 * @param SelectionJson The selection context: { mode, treatment, faceBankPath, lines: [
	 *        { assetPath, lineId, text, direction, speaker, voiceId, voiceLabel } ] }.
	 * @return What was queued, or "ERROR: ..." with the reason.
	 */
	UFUNCTION(meta = (AICallable, SpeechLibraryAction = "Record Selected...", SpeechLibraryPage = "Perform"), Category = "PerformanceForge|Session")
	static FString QueueRecordingSession(const FString& SelectionJson);

	/**
	 * Save the selection as a planned session asset, without starting anything.
	 *
	 * The production entry: a producer plans on Monday, the operator opens the Performance
	 * Capture panel's ledger on Wednesday and the job is already written. Planned sessions
	 * default to the director's dials - every take kept on the ledger, nothing processed until
	 * a take is chosen - and either dial can be changed on the asset.
	 *
	 * @param SelectionJson The same selection context Record Selected takes.
	 * @return Content path of the session asset, or "ERROR: ..." with the reason.
	 */
	UFUNCTION(meta = (AICallable, SpeechLibraryAction = "Plan Session...", SpeechLibraryPage = "Perform"), Category = "PerformanceForge|Session")
	static FString PlanRecordingSession(const FString& SelectionJson);

	/**
	 * Open the Performance Capture panel with this session loaded.
	 *
	 * Pipeline navigation: the Speech Library's Perform page lists the sessions that cover a
	 * bank's lines, and this is what its Open button calls - found by reflection, never linked.
	 *
	 * @return What was queued, or "ERROR: ..." with the reason it did not load.
	 */
	UFUNCTION(meta = (AICallable), Category = "PerformanceForge|Session")
	static FString OpenRecordingSession(const FString& SessionAssetPath);

	/**
	 * Load a planned session into the capture panel's queue - lines, fixtures and dials.
	 *
	 * @param SessionAssetPath Content path of a Performance Session asset.
	 * @return What was queued, or "ERROR: ..." with the reason.
	 */
	UFUNCTION(meta = (AICallable), Category = "PerformanceForge|Session")
	static FString StartPlannedSession(const FString& SessionAssetPath);

	/** Every session asset in the project, newest first - the jobs, planned and ad-hoc. */
	UFUNCTION(meta = (AICallable), Category = "PerformanceForge|Session")
	static TArray<FString> ListPerformanceSessions();

	/**
	 * The director's choose: make one ledger take the line, through the pipeline that fits its
	 * kind. A recorded take converts (per the session's treatment), graduates the line and solves
	 * the face; a generated take applies its stored facts and refreshes the face bank. Re-choosing
	 * a different take later is just another choose - the hashes keep everything honest.
	 */
	UFUNCTION(meta = (AICallable), Category = "PerformanceForge|Session")
	static UToolCallAsyncResultPerformanceString* ChoosePerformanceTake(
		const FString& BankPath, const FString& LineId, const FString& TakeId);
};
