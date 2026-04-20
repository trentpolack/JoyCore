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
// Event Priority Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Priority_Default, "System.Event.Priority.Default", "Default priority of an event (equivalent to Normal-priority)");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Priority_Low, "System.Event.Priority.Low", "Low-priority event.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Priority_Normal, "System.Event.Priority.Normal", "Normal-priority event.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Priority_High, "System.Event.Priority.High", "High-priority event.");

// Instance Lifecycle Event Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Created, "System.Event.Created", "Object Created (e.g., ::BeginPlay or constructor).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Destroyed, "System.Event.Destroyed", "Object Destroyed (e.g., ::EndPlay or destructor).");

// Character/Object State Change Event Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Broken, "System.Event.Broken", "Object has been broken.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Frozen, "System.Event.Frozen", "Object is frozen (due to low-temperature threshold reached).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_HealthChanged, "System.Event.HealthChanged", "Object health changed (damage/heal event).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Ignited, "System.Event.Ignited", "Object on fire (due to spreading fire and/or autoignition by temperature).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Interacted, "System.Event.Interacted", "Object interacted with (e.g., used, picked up, etc.).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_TemperatureChanged, "System.Event.TemperatureChanged", "Object temperature changed (cold/hot event).");

// Character/Object Lifecycle Event Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Spawned, "System.Event.Spawned", "Object spawned (distinct from Created).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Downed, "System.Event.Downed", "Object downed (can be revived; will trigger at the same time as Object Killed if revival is not supported).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Revived, "System.Event.Revived", "Object revived (distinct from spawned).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Killed, "System.Event.Killed", "Object killed (distinct from Destroyed).");

// Collision Event Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Contact_Hit, "System.Event.Contact.Hit", "Contact Hit (blocking collision).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Contact_BeginOverlap, "System.Event.Contact.BeginOverlap", "Contact Begin Overlap (overlapping collision).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Event_Contact_EndOverlap, "System.Event.Contact.EndOverlap", "Contact End Overlap (overlapping collision).");

/**
 *	Rule Tags.
 *   Rule tags provide additional information about a given rule; largely used for prioritization.
 */
// Rule priority tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Rule_Priority_Default, "System.Rule.Priority.Default", "Default rule priority (equivalent to Normal-priority).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Rule_Priority_Low, "System.Rule.Priority.Low", "Low-priority rule.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Rule_Priority_Normal, "System.Rule.Priority.Normal", "Normal-priority rule.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Rule_Priority_High, "System.Rule.Priority.High", "High-priority rule.");

/**
 * Trait Tags.
 *   A baseline set of Trait tags that can be used in a wide variety of projects.
 */
// Trait Tags.
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Breakable, "System.Trait.Breakable", "Can be broken."); 
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Damageable, "System.Trait.Damageable", "Can be damaged.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Flammable, "System.Trait.Flammable", "Can be lit on fire.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Freezable, "System.Trait.Freezable", "Can be frozen.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Gas, "System.Trait.Gas", "Gaseous matter state.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Interactable, "System.Trait.Interactable", "Is interactable.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Liquid, "System.Trait.Liquid", "Liquid matter state.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Solid, "System.Trait.Solid", "Solid matter state.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_TemperatureSource, "System.Trait.TemperatureSource", "Is a temperature source (-cold, +hot).");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_System_Trait_Throwable, "System.Trait.Throwable", "Can be picked up and thrown.");