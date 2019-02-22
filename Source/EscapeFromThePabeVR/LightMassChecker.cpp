// Fill out your copyright notice in the Description page of Project Settings.

#include "LightMassChecker.h"
#include "Gameframework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LightComponent.h"

#define OUT

// Sets default values for this component's properties
ULightMassChecker::ULightMassChecker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULightMassChecker::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) UE_LOG(LogTemp, Error, TEXT("Missing Trigger Volume on %s"), *GetOwner()->GetName());
}

void ULightMassChecker::TurnGreen()
{
	ULightComponent* lightComponent = Owner->FindComponentByClass<ULightComponent>();

	/// Control if not already green
	if (lightComponent->GetLightColor() != FLinearColor::Green) {
		USoundAttenuation * AttenuationSettings = nullptr;
		USoundConcurrency * ConcurrencySettings = nullptr;

		lightComponent->SetLightColor(FLinearColor::Green);

		UGameplayStatics::PlaySoundAtLocation
		(
			lightComponent,
			soundCorrect,
			lightComponent->GetComponentLocation(),
			lightComponent->GetComponentRotation(),
			1.0,
			1.0,
			0.0,
			AttenuationSettings,
			ConcurrencySettings,
			Owner
		);
	}
	return;
}

void ULightMassChecker::TurnRed()
{
	ULightComponent* lightComponent = Owner->FindComponentByClass<ULightComponent>();

	/// Control if not already green
	if (lightComponent->GetLightColor() != FLinearColor::Red) {
		USoundAttenuation * AttenuationSettings = nullptr;
		USoundConcurrency * ConcurrencySettings = nullptr;

		lightComponent->SetLightColor(FLinearColor::Red);

		UGameplayStatics::PlaySoundAtLocation
		(
			lightComponent,
			soundError,
			lightComponent->GetComponentLocation(),
			lightComponent->GetComponentRotation(),
			1.0,
			1.0,
			0.0,
			AttenuationSettings,
			ConcurrencySettings,
			Owner
		);
	}

	return;
}

void ULightMassChecker::TurnOff()
{
	Owner->FindComponentByClass<ULightComponent>()->SetLightColor(FLinearColor::Black);
	return;
}


// Called every frame
void ULightMassChecker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float massOnPlate = GetMassOnPlate();

	/// If the mass is equal to the value set in the editor, it's ok
	if (FMath::IsNearlyEqual(massOnPlate, ExactMass, 0)) {
		UE_LOG(LogTemp, Warning, TEXT("Mass is %f from %f"), massOnPlate, ExactMass);
		TurnGreen();
	}

	/// If is other mass in the trigger volume, it's wrong
	else if (massOnPlate > 0.f) {
		UE_LOG(LogTemp, Warning, TEXT("Mass is %f from %f"), massOnPlate, ExactMass);
		TurnRed();
	}

	/// No mass, no light needed
	else TurnOff();
}

float ULightMassChecker::GetMassOnPlate()
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

