// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoorW.h"
#include "Gameframework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "Components/LightComponent.h"
#include "EngineUtils.h"

#define OUT

// Sets default values for this component's properties
UOpenDoorW::UOpenDoorW()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorW::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (doorType == DoorType::PressurePlate && !PressurePlate) UE_LOG(LogTemp, Error, TEXT("Missing Trigger Volume on %s"), *GetOwner()->GetName());
}


// Called every frame
void UOpenDoorW::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (doorType)
	{
	case DoorType::PressurePlate:
		if (GetTotalMassOfActorsOnPlate() > TriggerMass)
		{
			OnOpen.Broadcast();
		}
		else
		{
			OnClose.Broadcast();
		}
		break;
	case DoorType::ExactMass:
		if (isAllGreenLighted()) 
		{
			OnOpen.Broadcast();
		}
		else
		{
			OnClose.Broadcast();
		}
		break;
	default:
		break;
	}
}


float UOpenDoorW::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	/// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return 0.f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	/// Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetName());
	}

	return TotalMass;
}

bool UOpenDoorW::isAllGreenLighted()
{
	int32 greenLightedCount = 0;
	int32 GREEN_LIGHTS_TO_OPEN = 3;

	for (TObjectIterator<ULightComponent> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		ULightComponent *Component = *Itr;
		if (FLinearColor::Green == Itr->GetLightColor()) { greenLightedCount++; }
	}

	return greenLightedCount == GREEN_LIGHTS_TO_OPEN;
}

