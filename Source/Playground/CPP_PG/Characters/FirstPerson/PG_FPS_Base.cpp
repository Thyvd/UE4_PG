// Fill out your copyright notice in the Description page of Project Settings.


#include "PG_FPS_Base.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APG_FPS_Base::APG_FPS_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	cameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	cameraSpringArm->SetupAttachment(RootComponent);
	cameraSpringArm->TargetArmLength = 0.f;
	cameraSpringArm->bEnableCameraLag = true;
	cameraSpringArm->CameraLagSpeed = 50.f;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	camera->SetupAttachment(cameraSpringArm, USpringArmComponent::SocketName);
	camera->bUsePawnControlRotation = true;
	
	SetMouseXSensitivity(1.f);
	SetMouseYSensitivity(1.f);
}

// Called when the game starts or when spawned
void APG_FPS_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APG_FPS_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Movement Functions
void APG_FPS_Base::MoveForward(float rate)
{
	if (rate != 0)
	{
		AddMovementInput(cameraSpringArm->GetForwardVector(), rate);
	}
}
void APG_FPS_Base::MoveRight(float rate)
{
	if (rate != 0)
	{
		AddMovementInput(cameraSpringArm->GetRightVector(), rate);
	}
}
void APG_FPS_Base::MouseY(float rate)
{
	if (rate != 0)
	{
		AddControllerPitchInput(rate * GetMouseYSensitivity());
	}
}
void APG_FPS_Base::MouseX(float rate)
{
	if (rate != 0)
	{
		AddControllerYawInput(rate * GetMouseXSensitivity());
	}
}

/** Getters */
float APG_FPS_Base::GetMouseYSensitivity() { return mouseYSensitivity; }
float APG_FPS_Base::GetMouseXSensitivity() { return mouseXSensitivity; }

/** Setters */
void APG_FPS_Base::SetMouseYSensitivity(float sensitivity) { mouseYSensitivity = sensitivity; }
void APG_FPS_Base::SetMouseXSensitivity(float sensitivity) { mouseXSensitivity = sensitivity; }

// Called to bind functionality to input
void APG_FPS_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APG_FPS_Base::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APG_FPS_Base::MoveRight);
	PlayerInputComponent->BindAxis("MouseX", this, &APG_FPS_Base::MouseX);
	PlayerInputComponent->BindAxis("MouseY", this, &APG_FPS_Base::MouseY);
}