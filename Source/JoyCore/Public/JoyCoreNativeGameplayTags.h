// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include "NativeGameplayTags.h"

/**
 *	Native Tag Declarations for the JoyCore plugin ecosystem.
 *		A general-purpose set of tags that can apply to a wide variety of projects and provide a foundation and convention for more defined in native or in the Gameplay Tag Manager.
 */

/**
 *	Context Tags.
 *		Context tags are intended to flesh out event details with temporal, spatial, or other descriptive information.
 */
// Intensity Context Tags (range: [0,1]).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Context_Intensity_None);           // 0.0.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Context_Intensity_Low);            // 0.25.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Context_Intensity_Medium);         // 0.5.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Context_Intensity_High);           // 0.75.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Context_Intensity_Max);            // 1.0.

/**
 *	Event Tags.
 *		Events are used to trigger reactions based on a set of rules and conditions.
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event);                            // Systemic Event parent tag.

// Event Priority Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Priority);                   // Systemic Event Priority parent tag.

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Priority_Default);           // Default priority of an event (equivalent to Normal-priority).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Priority_Low);               // Low-priority event.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Priority_Normal);            // Normal-priority event.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Priority_High);              // Highest-priority event.

// High-level Event Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Created);                    // Systemic Object Created (e.g., ::BeginPlay or constructor).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Destroyed);                  // Systemic Object Destroyed (e.g., ::EndPlay or deconstructor).

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_TraitsChanged);              // A Systemic Object's traits have changed (e.g., Breakable, Conductor, etc.).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_StateChanged);               // A Systemic Object's state has changed (e.g., Broken, Electrified, Frozen, etc.).

// Specific Event Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Broken);                     // Object has been broken.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Electrified);                // Object has been electrified (is a conductor).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Frozen);                     // Object is frozen (due to low-temperature threshold reached).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_HealthChanged);              // Object health changed (damage/heal event).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_HealthMaxChanged);           // Object max health changed.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Ignited);                    // Object on fire (due to spreading fire and/or autoignition by temperature).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Interacted);                 // Object interacted with (e.g., used, picked up, etc.).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_TemperatureChanged);         // Object temperature changed (cold/hot event).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Wet);                        // Object has become wet (is not waterproof).

// Lifecycle Event Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_LifecycleChanged);           // Object Lifecycle changed (separate from Created/Destroyed). This is a general lifecycle state change; specific Spawned/Downed/Revived/Killed events are also triggered.

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Lifecycle);                  // Systemic Event Lifecycle parent tag.

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Lifecycle_Spawned);          // Object spawned (distinct from Created).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Lifecycle_Downed);           // Object downed (can be revived; will trigger at the same time as Object Killed if revival is not supported).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Lifecycle_Revived);          // Object revived (distinct from spawned).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Lifecycle_Killed);           // Object killed (distinct from Destroyed).

// Collision Event Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Contact_Hit);                // Contact Hit (blocking collision).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Contact_OverlapBegin);       // Contact Begin Overlap (overlapping collision).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_Contact_OverlapEnd);         // Contact End Overlap (overlapping collision).

// World State Event Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_World_TimeOfDayChanged);     // Change in Time of Day (Dawn, Day, Dusk, Night).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Event_World_WeatherChanged);       // Change in weather states.

/**
 * Rule Tags.
 *   Rule tags provide additional information about a given rule; largely used for prioritization.
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Rule);                             // Systemic Rule parent tag.

// Rule Priority Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Rule_Priority);                    // Systemic Rule Priority parent tag.

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Rule_Priority_Default);            // Default rule priority (equivalent to Normal-priority).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Rule_Priority_Low);                // Low-priority rule.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Rule_Priority_Normal);             // Normal-priority rule.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Rule_Priority_High);               // Highest-priority rule.

/**
 * Trait Tags.
 *   A baseline set of Trait tags that can be used in a wide variety of projects.
 *    - Top-level trait tags define properties of the object they represent.
 *    - State trait tags describe the current state of the object.
 */

// Trait Tags.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait);                            // Systemic Trait parent tag.

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Breakable);                  // Can be broken.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Conductor);                  // Can be electrified.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Damageable);                 // Can be damaged.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Flammable);                  // Can be lit on fire.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Freezable);                  // Can be frozen.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Gas);                        // Gaseous matter state.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Interactable);               // Is interactable.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Invulnerable);               // Cannot be damaged.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Liquid);                     // Liquid matter state.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Solid);                      // Solid matter state.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_TemperatureSource);          // Is a temperature source (-cold, +hot).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Throwable);                  // Can be picked up and thrown.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_Waterproof);                 // Cannot get wet.


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State);                      // Systemic Trait State parent tag.

// State Tags (Status).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Broken);               // Is broken.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Electrified);          // Is electrified.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Ignited);              // Is on fire.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Frozen);               // Is frozen.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Wet);                  // Is wet.

// State Tags (Lifecycle).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Lifecycle);            // Systemic Trait State (Lifecycle) parent tag.

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Lifecycle_Alive);      // Is currently alive.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Lifecycle_Downed);     // Is downed, but not dead.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_System_Trait_State_Lifecycle_Dead);       // Is dead.

/**
 * World State Tags.
 *   A baseline set of World State tags that can be used in a wide variety of projects.
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_World_TimeOfDay_Dawn);             // Dawn world state (Night to Day).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_World_TimeOfDay_Day);              // Day world state.
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_World_TimeOfDay_Dusk);             // Dusk world state (Day to Night).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_World_TimeOfDay_Night);            // Night world state.

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_World_Weather_Clear);              // Clear weather state (mutually exclusive with Foggy).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_World_Weather_Foggy);              // Foggy weather state (mutually exclusive with Clear).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_World_Weather_Raining);            // Raining weather state (mutually exclusive with Snowing).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_World_Weather_Snowing);            // Snowing weather state (mutually exclusive with Raining).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_World_Weather_Storming);           // Storming weather state (mutually exclusive with Snowing).
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_World_Weather_Windy);              // Windy weather state.