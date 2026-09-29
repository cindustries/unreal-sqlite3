# CISQLite3

An **Unreal Engine 5** plugin that exposes SQLite3 to C++ and Blueprint. Forked from
KhArtNJava/SQLite3UE4; the distinguishing choice is that the SQLite amalgamation is
compiled *inside* the module (`Source/CISQLite3/Private/sqlite3.c`), so no separate
ThirdParty build step is needed on any platform.

**The repo is being migrated from UE4 to UE5.** UE4 is end-of-life; new and touched code
follows UE5 conventions, and the old UE4 code stays in git history.

## Delegation

Delegate behavior-relevant code to the right agent instead of touching it yourself —
principle and lane are in `.claude/rules/cisqlite3-rules.md`.

| Task | Agent |
|---|---|
| Implement / refactor / debug / migrate wrapper code | `cisqlite3-worker` (default) |
| Commits, `.uplugin` version bump, pre-release audit | `cisqlite3-release-manager` |

The agents carry their knowledge via `briefing.skills` (see `.claude/agents/`); the main
agent delegates rather than loading them. The architecture, public surface and UE5
conventions live in skill `cisqlite3-core` under `.claude/skills/`; git/commit skills are
shared in via skilletor (`.claude/skilletor.json`).
