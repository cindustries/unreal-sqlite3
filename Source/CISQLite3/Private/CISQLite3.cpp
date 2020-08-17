// Copyright (c) 2016 conflict.industries MIT License (MIT)

#include "CISQLite3.h"
#include "CISQLite3PrivatePCH.h"

DEFINE_LOG_CATEGORY(LogDatabase)

#define LOCTEXT_NAMESPACE "FCISQLite3"

void FCISQLite3::StartupModule() {}
void FCISQLite3::ShutdownModule() {}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCISQLite3, CISQLite3)

