// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_FPS_Base.h"
#include "PG_FPS_DistanceBasedScale.generated.h"

/**
 * 
 */
UCLASS()
class PLAYGROUND_API APG_FPS_DistanceBasedScale : public APG_FPS_Base
{
	GENERATED_BODY()
	
public:
	APG_FPS_DistanceBasedScale();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSpringArm)
		class USpringArmComponent* selectedObjectSpringArm;

	void SelectObject();
	void DeselectObject();

	FHitResult GetRaycastHitResult(bool bDebugModeLocal);
	bool SelectedObjectIsValid(FHitResult hitResultLocal);
	
	void SetbCanSelect(bool value);
	void SetSelectDistance(float value);
	
	bool GetbCanSelect();
	float GetSelectDistance();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	FHitResult hitResult;
	AActor* selectedObject;
	bool bCanSelect;
	float selectDistance;
	float objectSpringArmLength;
	float initialDistance;
	float initialDistancePlayerToObject;

	UPROPERTY(EditAnywhere, Category = "Debug")
		bool bDebugMode;
};
