// Fill out your copyright notice in the Description page of Project Settings.


#include "PG_FPS_DistanceBasedScale.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

// Sets Default Values
APG_FPS_DistanceBasedScale::APG_FPS_DistanceBasedScale()
{
	PrimaryActorTick.bCanEverTick = false;

	selectedObjectSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SelectedObjectSpringArmComponent"));
	selectedObjectSpringArm->SetupAttachment(cameraSpringArm, USpringArmComponent::SocketName);
	selectedObjectSpringArm->TargetArmLength = objectSpringArmLength;
	
	SetbCanSelect(true);
	SetSelectDistance(1000.f);
}

void APG_FPS_DistanceBasedScale::BeginPlay()
{
	Super::BeginPlay();

}

void APG_FPS_DistanceBasedScale::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

/** Distance Based Scale Functions*/
void APG_FPS_DistanceBasedScale::SelectObject()
{
	if (GetbCanSelect())
	{
		FHitResult hitResultLocal = GetRaycastHitResult(bDebugMode);

		if (hitResultLocal.bBlockingHit && hitResultLocal.Component->IsSimulatingPhysics())
		{
			hitResultLocal.Component->SetSimulatePhysics(false);
			hitResultLocal.Component->SetupAttachment(selectedObjectSpringArm, USpringArmComponent::SocketName);
			UE_LOG(LogTemp, Log, TEXT("You hit: %s"), *hitResultLocal.Actor->GetName());


		}
	}
}

void APG_FPS_DistanceBasedScale::DeselectObject()
{

}

FHitResult APG_FPS_DistanceBasedScale::GetRaycastHitResult(bool bDebugModeLocal)
{
	FHitResult hitResultLocal;
	FVector start = camera->GetComponentLocation();
	FVector forwardVector = camera->GetForwardVector();
	FVector end = (start + (forwardVector * GetSelectDistance()));
	FCollisionQueryParams collisionParams;

	
	if (GetWorld()->LineTraceSingleByChannel(hitResultLocal, start, end, ECC_Visibility, collisionParams))
	{
		if (bDebugModeLocal) 
		{ 
			DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 5.f, 0, 1); 
		}

	}

	return hitResultLocal;
}

void APG_FPS_DistanceBasedScale::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APG_FPS_DistanceBasedScale::SelectObject);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APG_FPS_DistanceBasedScale::DeselectObject);
}

// Getters
bool APG_FPS_DistanceBasedScale::GetbCanSelect() { return bCanSelect; }
float APG_FPS_DistanceBasedScale::GetSelectDistance() { return selectDistance; }

// Setters
void APG_FPS_DistanceBasedScale::SetbCanSelect(bool value) { bCanSelect = value; }
void APG_FPS_DistanceBasedScale::SetSelectDistance(float value) { selectDistance = value; }