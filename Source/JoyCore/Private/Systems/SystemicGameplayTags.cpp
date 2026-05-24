// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/SystemicGameplayTags.h"

/**
 *	Context Tags.
 *		Intensity context tags.
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Context_Intensity_None, "System.Context.Intensity.None", "0.0 intensity ([0.0, 1.0] scale).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Context_Intensity_Low, "System.Context.Intensity.Low", "0.25 intensity ([0.0, 1.0] scale).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Context_Intensity_Medium, "System.Context.Intensity.Medium", "0.5 intensity ([0.0, 1.0] scale).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Context_Intensity_High, "System.Context.Intensity.High", "0.75 intensity ([0.0, 1.0] scale).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Context_Intensity_Max, "System.Context.Intensity.Max", "1.0 intensity ([0.0, 1.0] scale).");

/**
 *	Event Tags.
 *		Events are used to trigger reactions based on a set of rules and conditions.
 */

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event, "System.Event", "Systemic Event parent tag.");

// Event Priority Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Priority_Default, "System.Event.Priority.Default", "Default priority of an event (equivalent to Normal-priority)");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Priority_Low, "System.Event.Priority.Low", "Low-priority event.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Priority_Normal, "System.Event.Priority.Normal", "Normal-priority event.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Priority_High, "System.Event.Priority.High", "High-priority event.");

// Instance Lifecycle Event Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Created, "System.Event.Created", "Systemic Object Created (e.g., ::BeginPlay or constructor).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Destroyed, "System.Event.Destroyed", "Systemic Object Destroyed (e.g., ::EndPlay or destructor).");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_TraitsChanged, "System.Event.TraitsChanged", "A Systemic Object's trait(s) has/have changed (e.g., Breakable, Conductor, etc.).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_StateChanged, "System.Event.StateChanged", "A Systemic Object's state has changed (e.g., Broken, Electrified, Frozen, etc.).");

// Systemic Object State Change Event Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Broken, "System.Event.Broken", "Object has been broken.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Electrified, "System.Event.Electrified", "Object has been electrified (is a conductor).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Frozen, "System.Event.Frozen", "Object has frozen due to low-temperature threshold reached.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_HealthChanged, "System.Event.HealthChanged", "Object health changed (damage/heal event).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_HealthMaxChanged, "System.Event.HealthMaxChanged", "Object max health changed.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Ignited, "System.Event.Ignited", "Object ignited due to spreading fire and/or autoignition by temperature.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Interacted, "System.Event.Interacted", "Object interacted with (e.g., used, picked up, etc.).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_TemperatureChanged, "System.Event.TemperatureChanged", "Object temperature changed (cold/hot event).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Wet, "System.Event.Wet", "Object has become wet (is not waterproof).");

// Systemic Object Lifecycle Event Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_LifecycleChanged, "System.Event.LifecycleChanged", "Object Lifecycle changed (separate from Created/Destroyed). This is a general lifecycle state change; specific Spawned/Downed/Revived/Killed events are also triggered.");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Lifecycle, "System.Event.Lifecycle", "Systemic Event Lifecycle parent tag.");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Lifecycle_Spawned, "System.Event.Lifecycle.Spawned", "Object spawned (distinct from Created).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Lifecycle_Downed, "System.Event.Lifecycle.Downed", "Object downed (can be revived; will trigger at the same time as Object Killed if revival is not supported).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Lifecycle_Revived, "System.Event.Lifecycle.Revived", "Object revived (distinct from spawned).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Lifecycle_Killed, "System.Event.Lifecycle.Killed", "Object killed (distinct from Destroyed).");

// Collision Event Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Contact_Hit, "System.Event.Contact.Hit", "Contact Hit (blocking collision).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Contact_OverlapBegin, "System.Event.Contact.OverlapBegin", "Contact Begin Overlap (overlapping collision).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Contact_OverlapEnd, "System.Event.Contact.OverlapEnd", "Contact End Overlap (overlapping collision).");

// World State Event Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_World_TimeOfDayChanged, "System.Event.World.TimeOfDayChanged", "Change in Time of Day (Dawn, Day, Dusk, Night).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_World_WeatherChanged, "System.Event.World.WeatherChanged", "Change in weather states.");

/**
 *	Rule Tags.
 *   Rule tags provide additional information about a given rule; largely used for prioritization.
 */

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Rule, "System.Rule", "Systemic Rule parent tag.");

// Rule priority tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Rule_Priority, "System.Rule.Priority", "Systemic Rule Priority parent tag.");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Rule_Priority_Default, "System.Rule.Priority.Default", "Default rule priority (equivalent to Normal-priority).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Rule_Priority_Low, "System.Rule.Priority.Low", "Low-priority rule.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Rule_Priority_Normal, "System.Rule.Priority.Normal", "Normal-priority rule.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Rule_Priority_High, "System.Rule.Priority.High", "High-priority rule.");

/**
 * Trait Tags.
 *   A baseline set of Trait tags that can be used in a wide variety of projects.
 *    - Top-level trait tags define properties of the object they represent.
 *    - State trait tags describe the current state of the object.
 */

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait, "System.Trait", "Systemic Trait parent tag.");

// Trait Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Breakable, "System.Trait.Breakable", "Can be broken."); 
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Conductor, "System.Trait.Conductor", "Can be electrified.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Damageable, "System.Trait.Damageable", "Can be damaged.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Flammable, "System.Trait.Flammable", "Can be lit on fire.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Freezable, "System.Trait.Freezable", "Can be frozen.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Gas, "System.Trait.Gas", "Gaseous matter state.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Interactable, "System.Trait.Interactable", "Is interactable.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Invulnerable, "System.Trait.Invulnerable", "Cannot be damaged.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Liquid, "System.Trait.Liquid", "Liquid matter state.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Solid, "System.Trait.Solid", "Solid matter state.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_TemperatureSource, "System.Trait.TemperatureSource", "Is a temperature source (-cold, +hot).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Throwable, "System.Trait.Throwable", "Can be picked up and thrown.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Waterproof, "System.Trait.Waterproof", "Cannot get wet.");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_State, "System.Trait.State", "Systemic Trait State parent tag.");

// State Tags (Status).
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_State_Broken, "System.Trait.State.Broken", "Is broken.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_State_Electrified, "System.Trait.State.Electrified", "Is electrified.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_State_Ignited, "System.Trait.State.Ignited", "Is on fire.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_State_Frozen, "System.Trait.State.Frozen", "Is currently frozen.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_State_Wet, "System.Trait.State.Wet", "Is wet.");

// State Tags (Lifecycle).
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_State_Lifecycle_Alive, "System.Trait.State.Lifecycle.Alive", "Is currently alive.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_State_Lifecycle_Downed, "System.Trait.State.Lifecycle.Downed", "Is downed (but not dead).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_State_Lifecycle_Dead, "System.Trait.State.Lifecycle.Dead", "Is currently dead.");

/**
 * World State Tags.
 *   A baseline set of World State tags that can be used in a wide variety of projects.
 */
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_World_TimeOfDay_Dawn, "System.World.TimeOfDay.Dawn", "Dawn world state (Night to Day).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_World_TimeOfDay_Day, "System.World.TimeOfDay.Day", "Day world state.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_World_TimeOfDay_Dusk, "System.World.TimeOfDay.Dusk", "Dusk world state (Day to Night).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_World_TimeOfDay_Night, "System.World.TimeOfDay.Night", "Night world state.");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_World_Weather_Clear, "System.World.Weather.Clear", "Clear weather state (mutually exclusive with Foggy).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_World_Weather_Foggy, "System.World.Weather.Foggy", "Foggy weather state (mutually exclusive with Clear).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_World_Weather_Raining, "System.World.Weather.Raining", "Raining weather state (mutually exclusive with Snowing).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_World_Weather_Snowing, "System.World.Weather.Snowing", "Snowing weather state (mutually exclusive with Raining).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_World_Weather_Storming, "System.World.Weather.Storming", "Storming weather state (mutually exclusive with Snowing).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_World_Weather_Windy, "System.World.Weather.Windy", "Windy weather state.");