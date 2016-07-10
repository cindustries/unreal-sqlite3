# Simple SQLite3 Source integration for Unreal Engine 4

Based on [SQLite3UE4 Plugin by Jussi Saarivirta & KhArtNJava](https://github.com/KhArtNJava/SQLite3UE4/) but with compiling SQLite3 as part of the module code, this opens up compiling it (hopefully) on all platforms without any extra process.

Please report any problems to the [issues tracker](https://github.com/cindustries/unreal-sqlite3/issues) on this GitHub, or join us at [#unrealengine on irc.freenode.net](https://webchat.freenode.net/?channels=#unrealengine)), you can msg me under **Getty**.

# Difference to SQLite3UE4

SQLite3UE4 integrates sqlite3 as a ThirdParty module, which leads to a chain of complexity and also requires people to build the sqlite3 library before using it. On CISQLite3, we integrated the sqlite3 code directly into the module, and so it gets linked like the rest of the module on compile.

# Tested on ...

**WARNING** The test is so far just compile test (this is all new, updates following regulary)

Windows x64 **WORKS**, Android **WORKS**

# Installation

Copy this plugin (like Download as ZIP) into the folder **Plugins/CISQLite3** on your project and a start of the project should compile the plugin automatically, if the project is C++. If you don't have a C++ project, then you can just make one for the sole purpose of compiling this plugin. Alternative you can install it as Engine Plugin in the Engine Plugins directory (like **Epic Games/4.12/Engine/Plugins/Runtime/CISQLite3**).

# Usage

(More usages to come....)

## C++

And here's a simple sample in C++:

Header:
```c++
UCLASS()
class SQLITE_API AMyActor : public AActor
{

  GENERATED_BODY()

public:

  UFUNCTION(BlueprintCallable, Category = "My Actor")
  bool GetMyStats();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Actor")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Actor")
  int32 Age;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Actor")
  FString Gender;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Actor")
  float Height;

};

```

CPP:

```c++
#include "MyActor.h"
#include "SQLiteDatabase.h"

bool AMyActor::GetMyStats()
{

  FString dbName = TEXT("TestDatabase");
  FString actorName = TEXT("Bruce Willis");

  if (!USQLiteDatabase::IsDatabaseRegistered(dbName))
  {
    USQLiteDatabase::RegisterDatabase(dbName, "Databases/TestDatabase.db", true);
  }

  bool didPopulate = USQLiteDatabase::GetDataIntoObject(dbName, FString::Printf(TEXT("SELECT Name, Age, Gender, Height FROM Actors WHERE Name = \"%s\""), *actorName), this);

  return didPopulate;

}
```

# License & Copyright

## CISQLite3

The MIT License (MIT)

Copyright (c) 2015 Jussi Saarivirta & KhArtNJava (SQLite3UE4)

Copyright (c) 2016 conflict.industries

## SQLite3

The author disclaims copyright to this source code. In place of a legal notice, here is a blessing:

May you do good and not evil.

May you find forgiveness for yourself and forgive others.

May you share freely, never taking more than you give.
