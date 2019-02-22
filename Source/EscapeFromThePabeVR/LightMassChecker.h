// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "LightMassChecker.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEFROMTHEPABEVR_API ULightMassChecker : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULightMassChecker();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void TurnGreen();

	void TurnRed();

	void TurnOff();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
		float ExactMass = 0.f;

	UPROPERTY(EditAnywhere)
		USoundBase* soundCorrect;

	UPROPERTY(EditAnywhere)
		USoundBase* soundError;

	AActor * Owner = nullptr;
	
	float GetMassOnPlate();
};
