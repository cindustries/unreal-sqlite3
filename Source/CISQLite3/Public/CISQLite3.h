// Copyright (c) 2015 Jussi Saarivirta 2016 conflict.industries MIT License (MIT)

#pragma once

#include "ModuleManager.h"

class FCISQLite3 : public IModuleInterface
{
public:

  /** IModuleInterface implementation */
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;
};