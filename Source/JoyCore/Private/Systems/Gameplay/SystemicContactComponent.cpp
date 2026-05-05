// Copyright (c) 2026 Trent Polack. All Rights Reserved.
// Licensed under the MIT License.

#include "Systems/Gameplay/SystemicContactComponent.h"

#include "Components/PrimitiveComponent.h"

#include "Systems/SystemicGameplayTags.h"
#include "Systems/SystemicWorldSubsystem.h"

#include "Systems/Events/EventData/SystemicContactEventData.h"

// Constructor.
USystemicContactComponent::USystemicContactComponent()
: CollisionComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Handle a hit notification and emit a systemic hit event if it meets the minimum impulse threshold.
void USystemicContactComponent::HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& HitResult)
{
	const float hitMagnitude = NormalImpulse.Size();
	if(hitMagnitude < HitImpulseThreshold)
	{
		// Hit doesn't satisfy magnitude threshold.
		return;
	}

	// Emit the collision event.
	EmitContactEvent(TAG_System_Event_Contact_Hit, OtherActor, OtherComponent, HitResult, hitMagnitude);
}

// Handle a begin-overlap notification and emit a systemic begin-overlap event.
void USystemicContactComponent::HandleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Skip magnitude check since overlaps aren't blocking.
	EmitContactEvent(TAG_System_Event_Contact_OverlapBegin, OtherActor, OverlappedComponent, SweepResult, 0.0f);
}

// Handle an end-overlap notification and emit a systemic end-overlap event.
void USystemicContactComponent::HandleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	const FVector location = IsValid(GetOwner()) ? GetOwner()->GetActorLocation() : OtherActor->GetActorLocation();
	FHitResult hitResult(GetOwner(), CollisionComponent, location, FVector::ZeroVector);

	// Skip magnitude check since ending this is just ending the overlap.
	EmitContactEvent(TAG_System_Event_Contact_OverlapEnd, OtherActor, OverlappedComponent, hitResult, 0.0f);
}

// Emit a systemic contact event with contact-specific event data.
bool USystemicContactComponent::EmitContactEvent(const FGameplayTag& EventTag, AActor* OtherActor, UObject* SourceObject, const FHitResult& HitResult, float Magnitude)
{
	AActor* pOwner = GetOwner();
	if(!IsValid(pOwner) || (OtherActor == pOwner))
	{
		// No collision for one reason or another.
		return false;
	}

	FSystemicEvent event;
	JOYCORE_POPULATE_EVENT_CONSTRUCTOR_ARGLIST(event, EventTag, pOwner, OtherActor, (SourceObject ? SourceObject : this), FSystemicContactEventData, HitResult);

	FSystemicContactEventData& eventData = event.EventDataInstance.GetMutable<FSystemicContactEventData>();
	eventData.Location = HitResult.Location;
	eventData.Value = Magnitude;

	return(USystemicWorldSubsystem::EmitEvent(pOwner, event));
}

// Set the collision component and setup the event bindings (and unbind if necessary).
void USystemicContactComponent::SetCollisionComponent(UPrimitiveComponent* Component)
{
	if(IsValid(CollisionComponent) && bCollisionEventsBound)
	{
		// Unbind the current events.
		CollisionComponent->OnComponentHit.RemoveDynamic(this, &USystemicContactComponent::HandleHit);
		CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &USystemicContactComponent::HandleOverlapBegin);
		CollisionComponent->OnComponentEndOverlap.RemoveDynamic(this, &USystemicContactComponent::HandleOverlapEnd);
		
		bCollisionEventsBound = false;
	}
	
	if(!IsValid(Component))
	{
		CollisionComponent = nullptr;
		return;
	}
	
	// Set the collision component and setup the event bindings.
	CollisionComponent = Component;
	
	if(bEmitHitEvents)
	{
		// Emit blocking collision events.
		CollisionComponent->OnComponentHit.AddUniqueDynamic(this, &USystemicContactComponent::HandleHit);
		
	}

	if(bEmitOverlapEvents)
	{
		// Emit overlap events.
		CollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &USystemicContactComponent::HandleOverlapBegin);
		CollisionComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &USystemicContactComponent::HandleOverlapEnd);
	}
	
	bCollisionEventsBound = true;
}

// Get the primitive component loosely bound to this component.
UPrimitiveComponent* USystemicContactComponent::GetCollisionComponent() const
{
	return CollisionComponent;
}

// Begin play logic; binds to the observed primitive component.
void USystemicContactComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(!bCollisionEventsBound && IsValid(CollisionComponent))
	{
		// Set the collision component in order to set the event bindings.
		SetCollisionComponent(CollisionComponent);
	}
}

// End play logic; unbinds from the observed primitive component.
void USystemicContactComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Unset the collision component to release the event bindings.
	SetCollisionComponent(nullptr);
	
	Super::EndPlay(EndPlayReason);
}