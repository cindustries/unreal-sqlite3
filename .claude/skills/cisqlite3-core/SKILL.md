---
name: cisqlite3-core
description: Load on any edit to the CISQLite3 Unreal SQLite3 plugin (Source/CISQLite3) — the vendored sqlite3 amalgamation build model, the USQLiteDatabase / USQLiteBlueprintFunctionLibrary public surface, the name-vs-filename registry, the manual-query vs Blueprint-query split, the UTF-8/TCHAR boundary, and the Unreal Engine 5 target with the UE4→UE5 migration this repo is undergoing.
metadata:
  type: project
---

# CISQLite3 core

An Unreal Engine **Runtime** module (`CISQLite3`, plugin `CISQLite3.uplugin`) that
exposes SQLite3 to C++ and Blueprint. Forked from KhArtNJava/SQLite3UE4; the whole
reason this fork exists is the build model below.

## The amalgamation build model — the one decision everything rests on

The SQLite amalgamation ships **inside the module** and is compiled like any other
module source:

- `Source/CISQLite3/Private/sqlite3.c` — compiled by UBT as its own translation unit.
  It is **not** `#include`d anywhere; it links with the module. This is what lets the
  plugin build on every platform with no separate ThirdParty pre-build step (the
  upstream project made sqlite3 a ThirdParty module and required a manual lib build —
  we deliberately do not).
- `Source/CISQLite3/Public/{sqlite3.h,sqlite3ext.h}` — the matching upstream headers.

**These three files are vendored upstream code. Never hand-edit them.** To move to a
newer SQLite, drop in a fresh amalgamation (all three together, matching versions).
Every behavior change we want lives in the wrapper (`SQLiteDatabase.cpp` and friends),
never in `sqlite3.c`.

## Public surface — two UCLASSes, both in `Public/`

**`USQLiteDatabase`** (`UObject`, all methods `static`; it is a utility namespace, not
instantiated per database). Holds the registry `static TMap<FString,FString> Databases`
(name → filename). Methods, all `BlueprintCallable`:

- `RegisterDatabase(Name, Filename, RelativeToGameContentDirectory)` / `IsDatabaseRegistered(Name)`
- Reads: `GetData` / `GetDataBP`, `GetDataIntoObject` / `GetDataIntoObjectBP`
- DDL/DML: `CreateTable`, `CreateIndex(es)`, `DropIndex`, `DropTable`, `TruncateTable`,
  `IsTableExists`, `InsertRowsIntoTable`, `Vacuum`, `ExecSql`
- Private helpers do the real work: `PrepareStatement`, `RunQueryAndGetResults`,
  `ConstructQuery`, `CollectProperties`, `AssignResultsToObjectProperties`.

**`USQLiteBlueprintFunctionLibrary`** (`UBlueprintFunctionLibrary`): value casts
(`CastToInt/Boolean/Float`) plus the Blueprint query-builder nodes
(`QueryStart/QueryEnd/QueryTerm/QueryLogicAnd/QueryLogicOr/QueryFinal`) and the column
type/key nodes (`SQLiteINTEGER/TEXT/REAL/NUMERIC`, `SQLitePrimaryKey`, `SQLiteIndex`).

## Vocabulary — keep these distinct

- **Name vs filename.** `RegisterDatabase` maps a friendly *Name* to a file *path*;
  every other call takes the *Name*, never the path. An unregistered Name is the first
  thing to check when a call "does nothing".
- **Manual-query vs Blueprint API.** Each read op has two forms: manual
  (`GetData`, `GetDataIntoObject`) take a raw SQL string; Blueprint
  (`GetDataBP`, `GetDataIntoObjectBP`) build SQL from an `FSQLiteDatabaseReference`
  + `Fields` + an `FSQLiteQueryFinalizedQuery` assembled by the query-builder nodes.
- **Two result-type families.** Blueprint-exposed `USTRUCT`s
  (`FSQLiteQueryResult` → `FSQLiteQueryResultRow` → `FSQLiteKeyValuePair`) are the
  return values; the plain C++ structs (`SQLiteQueryResult` / `SQLiteResultValue` /
  `SQLiteResultField`, no `F`/`U` prefix) are the internal intermediaries filled while
  stepping the statement. Do not expose the internal ones to Blueprint.
- **GetDataIntoObject** populates a `UObject`'s `UPROPERTY`s by matching result column
  name → property name (`CollectProperties` + `AssignResultsToObjectProperties`).

## Type & encoding contract

At the SQLite boundary, columns are read by type: `sqlite3_column_int64` → `int64`,
`sqlite3_column_double` → `double`, `sqlite3_column_text` → `FString` via
**`UTF8_TO_TCHAR`**. Text is UTF-8 on the SQLite side, TCHAR inside UE — keep that
conversion on every text read/write; never pass a raw `char*` from SQLite into an
`FString` without it. For Blueprint, every value is normalized to `FString` via
`SQLiteResultField::ToString()` and cast back with the `CastTo*` helpers.

**Do not reintroduce fixed-size buffers for SQL text.** A `TCHAR[128]` cast used to
truncate queries at 128 chars; it was deliberately removed (commit 4684205, issue #2).
Query strings are `FString`, unbounded — keep them that way.

## Target: Unreal Engine 5 — migration in progress

**The target engine is Unreal Engine 5.** UE4 is end-of-life; we are moving the whole
plugin to UE5 and the old UE4 code stays in git history for anyone who needs it. The
checked-in code is still the original ~UE 4.12 shape, so migrating it *is* the work —
new and touched code follows UE5 conventions, and encountering an old pattern in a file
you are already editing is a reason to migrate it, not to preserve it.

UE4 → UE5 conventions (apply as you touch each area):

- **Reflection pointers:** `FProperty` (not `UProperty`); `TFieldIterator<FProperty>`.
  `CollectProperties` currently returns `TMap<FString, UProperty*>` — becomes `FProperty*`.
- **Generated body macros:** `GENERATED_BODY()` for the `UCLASS`/`USTRUCT` bodies
  (the code still uses `GENERATED_UCLASS_BODY()` / `GENERATED_USTRUCT_BODY()`).
- **Includes / IWYU:** drop the monolithic per-module PCH (`CISQLite3PrivatePCH.h` first
  in every `.cpp`) and `Engine.h`; use `#include "CoreMinimal.h"` plus the precise
  headers each unit needs (e.g. `Modules/ModuleManager.h`, `Misc/CString.h`).
- **`CISQLite3.Build.cs`:** signature `CISQLite3(ReadOnlyTargetRules Target)` (not
  `TargetInfo`); set `PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;` and a current
  `DefaultBuildSettings` value — match the exact UE5 version you are building against,
  don't guess a minor. Public deps stay `Engine`, `Core`, `CoreUObject`.
- **`CISQLite3.uplugin`:** add `"EngineVersion"` for the target UE5 release; `FileVersion`
  stays 3, the module stays `Runtime` / `Default`.

The amalgamation build model above is engine-independent and stays exactly as it is —
UE5 ships its own SQLite, but this plugin deliberately vendors its own, so do not switch
to the engine's `SQLiteCore`.

## Build & verify

No standalone build and no unit tests — verification is a **compile test** inside a
host UE5 C++ project (or as an engine plugin). There is no `t/` suite to run;
"it compiles clean against the target UE5 project" is the bar (the README's Windows x64
/ Android notes were the UE4 baseline; re-confirm platforms after the migration). The
shipped version lives in `CISQLite3.uplugin` (`Version` / `VersionName`), which is
`IsBetaVersion` and `EnabledByDefault: false`.
