# PerformanceForgeToolset

Every released version of PerformanceForgeToolset, newest first. A release publishes **one** section of this
file — the one whose heading matches its tag — as its release notes; for an `open` plugin those
notes are posted to Discord `#releases` automatically. Write for someone who installs the plugin,
not for the commit log.

Headings are `## <x.y.z> — <date>`. Use `Added` / `Changed` / `Fixed` / `Compatibility` /
`Known issues`, only the ones that apply.

## 0.1.2 — 2026-10-04

### Changed
- The version this toolset reports to an agent is now read from the plugin's own
  descriptor rather than repeated in C++, so it can no longer answer a number the
  installed package does not carry.
- Copyright and licence notices now name Bojan Andrejek / MetaWorx LLC. It is still Apache 2.0, and nothing about how you may use it changed.

## 0.1.1 — 2026-09-08
- Packaging fix: the release now carries everything the register allows. `BuildPlugin`'s filter excludes `Config/` and every `public_extra` path, so earlier zips shipped without them.
- `GetToolsetVersion()` answers this plugin's real version; it had drifted from the descriptor.

## 0.1.0 — 2026-09-07
- PerformanceForge's toolset moves out; the providers' stay put
- A session is found from the bank it serves, and opens where it is used
- Every plugin now points at kovati.dev
