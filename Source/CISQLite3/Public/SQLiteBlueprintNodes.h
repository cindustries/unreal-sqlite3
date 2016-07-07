// Copyright (c) 2015 Jussi Saarivirta 2016 conflict.industries MIT License (MIT)

#pragma once
#include "SQLiteBlueprintNodes.generated.h"


USTRUCT(BlueprintType)
struct CISQLITE3_API FSQLiteQueryLogicExpectedNode
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLite Database Query")
	FString Query;

	FSQLiteQueryLogicExpectedNode(){}
	FSQLiteQueryLogicExpectedNode(FString LHSQuery, FString Append) : Query(LHSQuery) 
	{
		Query += Append;
	}
};

USTRUCT(BlueprintType)
struct CISQLITE3_API FSQLiteQueryTermExpectedNode
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLite Database Query")
	FString Query;

	FSQLiteQueryTermExpectedNode(){}
	FSQLiteQueryTermExpectedNode(FString LHSQuery, FString Append) : Query(LHSQuery) 
	{
		Query += Append;
	}
};

USTRUCT(BlueprintType)
struct CISQLITE3_API FSQLiteQueryFinalizedQuery
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLite Database Query")
	FString Query;

	FSQLiteQueryFinalizedQuery(){}
	FSQLiteQueryFinalizedQuery(FString Query) : Query(Query){}
};
