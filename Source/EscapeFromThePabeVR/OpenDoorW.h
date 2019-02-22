// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoorW.generated.h"

UENUM()
enum DoorType
{
	PressurePlate     UMETA(DisplayName = "Pressure Plate"),
	ExactMass      UMETA(DisplayName = "Exact Mass"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEFROMTHEPABEVR_API UOpenDoorW : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoorW();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnClose;

private:
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
		float TriggerMass = 30.f;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<DoorType> doorType;

	AActor* Owner = nullptr;

	float GetTotalMassOfActorsOnPlate();

	bool isAllGreenLighted();
	
};
