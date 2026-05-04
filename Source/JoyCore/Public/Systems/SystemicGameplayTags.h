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
// Event Priority Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Priority_Default);           // Default priority of an event (equivalent to Normal-priority).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Priority_Low);               // Low-priority event.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Priority_Normal);            // Normal-priority event.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Priority_High);              // Highest-priority event.

// High-level Event Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Created);                    // Object Created (e.g., ::BeginPlay or constructor).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Destroyed);                  // Object Destroyed (e.g., ::EndPlay or deconstructor).

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Broken);                     // Object has been broken.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Freeze);                     // Object is frozen (due to low-temperature threshold reached).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Freeze_Warmed);              // Object has warmed enough to remove frozen state..
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_HealthChanged);              // Object health changed (damage/heal event).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Ignite);                     // Object on fire (due to spreading fire and/or autoignition by temperature).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Ignite_Cooled);              // Object has reduced temperature enough to remove the ignition state..
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Interacted);                 // Object interacted with (e.g., used, picked up, etc.).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_TemperatureChanged);         // Object temperature changed (cold/hot event).

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Spawned);                    // Object spawned (distinct from Created).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Downed);                     // Object downed (can be revived; will trigger at the same time as Object Killed if revival is not supported).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Revived);                    // Object revived (distinct from spawned).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Killed);                     // Object killed (distinct from Destroyed).

// Collision Event Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Contact_Hit);                // Contact Hit (blocking collision).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Contact_OverlapBegin);       // Contact Begin Overlap (overlapping collision).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Contact_OverlapEnd);         // Contact End Overlap (overlapping collision).

/**
 * Rule Tags.
 *   Rule tags provide additional information about a given rule; largely used for prioritization.
 */
// Rule Priority Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Rule_Priority_Default);            // Default rule priority (equivalent to Normal-priority).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Rule_Priority_Low);                // Low-priority rule.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Rule_Priority_Normal);             // Normal-priority rule.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Rule_Priority_High);               // Highest-priority rule.

/**
 * Trait Tags.
 *   A baseline set of Trait tags that can be used in a wide variety of projects.
 */

// Trait Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Breakable);                  // Can be broken.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Damageable);                 // Can be damaged.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Flammable);                  // Can be lit on fire.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Freezable);                  // Can be frozen.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Gas);                        // Gaseous matter state.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Interactable);               // Is interactable.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Liquid);                     // Liquid matter state.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Solid);                      // Solid matter state.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_TemperatureSource);          // Is a temperature source (-cold, +hot).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Throwable);                  // Can be picked up and thrown.

// State Tags (Lifecycle).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Alive);                // Is currently alive.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Downed);               // Is downed, but not dead.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Dead);                 // Is dead.

// State Tags (Status).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Broken);               // Is broken.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Ignited);              // Is on fire.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Frozen);               // Is frozen.
