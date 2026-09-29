// Copyright (c) 2015 Jussi Saarivirta 2016 conflict.industries MIT License (MIT)

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDatabase, All, All);

class FCISQLite3 : public IModuleInterface
{
public:

  /** IModuleInterface implementation */
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;
};
