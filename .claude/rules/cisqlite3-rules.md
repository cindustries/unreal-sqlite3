# CISQLite3 House Rules

Apply to every task in this repository unless explicitly overridden. Bias: caution over
speed on non-trivial work; use judgment on trivial tasks. Loaded automatically at launch
(same priority as `CLAUDE.md`). Subagents get their discipline from the skills
force-loaded via `briefing.skills` — this file is for the orchestrating agent.

**CISQLite3 is an Unreal Engine 5 SQLite3 plugin.** UE4 is end-of-life; the repo is being
migrated to UE5 and the old UE4 code stays in git history. Nothing should be built toward
UE4.

## Engineering discipline

1. **Think before coding** — State assumptions. When uncertain, ask rather than guess.
   Push back when a simpler approach exists. Stop when confused; name what's unclear.
2. **Simplicity first** — Minimum code that solves the problem. Nothing speculative.
3. **Surgical changes** — Touch only what you must. Don't "improve" adjacent code. The
   exception is the UE4→UE5 migration: an old UE4 pattern in a file you are already
   editing gets migrated, not preserved.
4. **Read before you write** — Read the surrounding wrapper (`SQLiteDatabase.cpp`, the
   Blueprint library) and the public headers before adding code.
5. **Match conventions** — Conformance over taste; surface a harmful convention, don't
   fork silently.
6. **Fail loud** — "Done" is wrong if anything was skipped silently. "It compiles" is
   wrong if you did not compile it against a UE5 project — say what you actually verified.

## Delegation

This rule depends on whether the Agent/Task tool is available to you.

- **You can spawn subagents** (orchestrating main agent): Do NOT touch behavior-relevant
  code yourself — delegate to `cisqlite3-worker`. Your lane: coordinate, inspect, plan,
  review diffs, edit non-behavioral docs. When in doubt, delegate. Why: only the
  `cisqlite3-*` agents get their skills force-loaded via `briefing.skills`; you get no
  briefing and would touch internals with too little context.

  | Task | Agent |
  |---|---|
  | Implement / refactor / debug / migrate wrapper code | `cisqlite3-worker` (default) |
  | Commits, `.uplugin` version bump, pre-release audit | `cisqlite3-release-manager` |

  **Only `cisqlite3-release-manager` commits.** The worker leaves a commit-ready tree and
  reports; you then dispatch the release-manager to cut the commit.

- **You cannot spawn subagents** (you ARE a `cisqlite3-*` agent): the delegation lock does
  not apply — implement, refactor, debug, and migrate per these rules.

Behavior-relevant = the `USQLiteDatabase` / `USQLiteBlueprintFunctionLibrary` wrapper,
query construction, the Blueprint nodes, error handling, the module and `Build.cs`, the
`.uplugin`. Pure prose docs (`README.md`) are not.

## Release — never without permission

Building/compiling is fine anytime. Tagging, pushing, and cutting a GitHub release are
STRICTLY forbidden without the maintainer's explicit go-ahead — even if a plan lists
"release" as the next step. For anything heading toward release: stop and ask.

## Public issues — never act without instruction

The GitHub issue tracker (github.com/cindustries/unreal-sqlite3/issues) carries real
users' bug reports under the maintainer's account. **Never act on a public issue on your
own initiative — not even to read it.** No listing, viewing, commenting, closing, or
creating unless the user explicitly says to handle a specific issue; every write publishes
under the maintainer's name.

## Hazards — read the mechanism, not the moral

- **The vendored SQLite amalgamation is off-limits.** `Source/CISQLite3/Private/sqlite3.c`
  and `Public/sqlite3.{h,ext.h}` are upstream code compiled as-is (UBT builds `sqlite3.c`
  as its own translation unit — it is not `#include`d anywhere). Never hand-edit them;
  updating SQLite means dropping in a fresh amalgamation. Behavior changes go in the
  wrapper. Do not switch to UE5's bundled `SQLiteCore` — vendoring is the point of this fork.
- **No fixed-size buffers for SQL text.** A `TCHAR[128]` cast that truncated queries at
  128 chars was deliberately removed (commit 4684205, issue #2). SQL strings are `FString`,
  unbounded.
- **No automated tests.** Verification is a compile against a host UE5 project. There is
  no `t/` suite — an agent claiming "tests pass" is wrong; the bar is a clean UE5 build.

## C++/UE conventions — reference, don't restate

The amalgamation build model, the public surface, the name-vs-filename registry, the
UTF-8/TCHAR boundary, and the UE4→UE5 conventions live in skill `cisqlite3-core`
(force-loaded for `cisqlite3-*` agents). Do not duplicate that content here.
