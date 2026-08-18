// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleManager.h"

/**
 * JoyCore Module Definition.
 */
class FJoyCoreModule : public IModuleInterface
{
public:

	// IModuleInterface.
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// ~IModuleInterface;
};
