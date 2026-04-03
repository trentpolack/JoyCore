// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "NativeGameplayTags.h"

/**
 *	Native Tag Declarations for Systems, Events, Rules, Conditions, and Traits.
 *		A general-purpose set of tags that can apply to a wide variety of projects and provide a foundation and convention for more defined in native or in the Gameplay Tag Manager.
 */

/**
 *	Context Tags.
 *		Context tags are intended to flesh out event details with temporal, spatial, or other descriptive information.
 */
// Intensity Context Tags ([0,1] scale).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Context_Intensity_None);           // 0.0.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Context_Intensity_Low);            // 0.25.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Context_Intensity_Medium);         // 0.5.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Context_Intensity_High);           // 0.75.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Context_Intensity_Max);            // 1.0.

/**
 *	Event Tags.
 *		Events are used to trigger reactions based on a set of rules and conditions.
 */
// Lifecycle Event Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Created);                    // Object Created.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Destroyed);                  // Object Destroyed.

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Downed);                     // Object Downed (can be revived; will trigger at the same time as Object Killed if revival is not supported).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_HealthChanged);              // Object Health Changed (Healed or Damaged).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Killed);                     // Object Killed (distinct from Object Destroyed).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Revived);                    // Object Revived.

// Collision Event Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Contact_Hit);                // Contact Hit (blocking collision).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Contact_BeginOverlap);       // Contact Begin Overlap (overlapping collision).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Contact_EndOverlap);         // Contact End Overlap (overlapping collision).