<!-- kovati:header -->
<p align="center"><a href="https://kovati.dev/tools/performanceforge/"><img src="https://raw.githubusercontent.com/AutomationForgeHQ/.github/main/profile/assets/performanceforge.jpg" width="100%" alt="PerformanceForge, part of Kovati" /></a></p>

<p align="center"><b>Agent toolset for PerformanceForge</b> · Part of <a href="https://kovati.dev"><b>Kovati</b></a>, production tools for Unreal games · <a href="https://kovati.dev/storytelling/">Storytelling</a><br />
<a href="https://kovati.dev/tools/performanceforge/">Product page</a> · <a href="https://docs.kovati.dev">Documentation</a> · <a href="https://kovati.dev/download/">Install with the Kovati Hub</a> · <a href="https://kovati.dev/discord">Discord</a></p>
<!-- /kovati:header -->

# PerformanceForgeToolset

PerformanceForge's flow as Model Context Protocol tools: plan a recording session, start it, list
the jobs, and choose a take.

```
plan a session  →  record or generate candidates  →  a director chooses  →  the take becomes the line
```

Every function forwards to `UPerformanceForgeSubsystem` and adds nothing. All logic, state and
safety live in the capability plugin; this is a surface, and deleting it changes nothing about how
PerformanceForge behaves — it only removes the agent's way in.

---

## The tools

| | |
|---|---|
| **Sessions** | `PlanRecordingSession`, `QueueRecordingSession`, `StartPlannedSession`, `ListPerformanceSessions`, `OpenRecordingSession` |
| **Takes** | `ChoosePerformanceTake` |

Two of them carry `SpeechLibraryAction` metadata, which is how they appear as buttons in
SpeechForge's Speech Library without either plugin linking against the other: **Record Selected…**
and **Plan Session…**. A panel that finds its buttons by scanning tool metadata gets the agent's
capabilities and the human's for free, from one declaration.

---

## Why a separate plugin

The family rule is that a capability and its toolset ship apart, so a registry that fails to load —
it is experimental, and people switch it off — cannot take the capability down with it. Here that
earns itself twice: **PerformanceForge is sold, and this surface is published.** The paid
implementation stays private; the shape an agent talks to does not, because a tool surface nobody
can read is a tool surface nobody integrates with.

This plugin was inside the capability module until 2026-09-01, as a deliberate, commented
departure while the surface was one function. It grew to five, which was the agreed trigger to
move it out.

---

## Conventions

**The signature is the schema.** Parameters and returns are plain types and `USTRUCT`s, so the
registry publishes something an agent can fill in without guessing, and the doc comments become
the descriptions it reads. They are written for that reader.

**No dependency on ModelContextProtocol.** Tools register with `ToolsetRegistry`; MCP is one
transport that reads the registry, and coupling to it would be backwards.

**Failures raise a script error** rather than returning an ok/error envelope, which the registry
turns into a tool error the agent can act on.

---

## Related

- **PerformanceForge** — the capture stage, the session assets, the ledger. The paid plugin this
  exposes.
- **SpeechForge** — where takes are stored and where lines live, deliberately on the free side.
- **FaceForge** — what solves a take's video into face animation.
