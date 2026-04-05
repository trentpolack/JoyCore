// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/SystemicGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_System_Context_Intensity_None, "System.Context.Intensity.");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Context_Intensity_Low, "System.Context.Intensity.");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Context_Intensity_Medium, "System.Context.Intensity.");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Context_Intensity_High, "System.Context.Intensity.");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Context_Intensity_Max, "System.Context.Intensity.");

/**
 *	Event Tags.
 *		Events are used to trigger reactions based on a set of rules and conditions.
 */
// Event Priority Tags.
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Priority_Default, "System.Event.Priority.Default");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Priority_Low, "System.Event.Priority.Low");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Priority_Normal, "System.Event.Priority.Normal");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Priority_High, "System.Event.Priority.High");

// Lifecycle Event Tags.
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Created, "System.Event.Created");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Destroyed, "System.Event.Destroyed");

UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Downed, "System.Event.Downed");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_HealthChanged, "System.Event.HealthChanged");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Killed, "System.Event.Killed");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Revived, "System.Event.Revived");

// Collision Event Tags.
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Contact_Hit, "System.Event.Contact.Hit");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Contact_BeginOverlap, "System.Event.Contact.BeginOverlap");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Event_Contact_EndOverlap, "System.Event.Contact.EndOverlap");

/**
 *	Rule Tags.
 *   Rule tags provide additional information about a given rule; largely used for prioritization.
 */
// Rule priority tags.
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Rule_Priority_Default, "System.Rule.Priority.Default");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Rule_Priority_Low, "System.Rule.Priority.Low");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Rule_Priority_Normal, "System.Rule.Priority.Normal");
UE_DEFINE_GAMEPLAY_TAG(TAG_System_Rule_Priority_High, "System.Rule.Priority.High");