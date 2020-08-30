// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PG_FPS_Base.generated.h"

UCLASS()
class PLAYGROUND_API APG_FPS_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APG_FPS_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSpringArm)
		class USpringArmComponent* cameraSpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* camera;

	void MoveForward(float rate);
	void MoveRight(float rate);
	void MouseY(float rate);
	void MouseX(float rate);

	float GetMouseYSensitivity();
	float GetMouseXSensitivity();
	void SetMouseYSensitivity(float sensitivity);
	void SetMouseXSensitivity(float sensitivity);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	float mouseYSensitivity;
	float mouseXSensitivity;
};
