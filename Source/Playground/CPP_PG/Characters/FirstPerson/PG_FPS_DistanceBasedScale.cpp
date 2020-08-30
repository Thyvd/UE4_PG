// Fill out your copyright notice in the Description page of Project Settings.


#include "PG_FPS_DistanceBasedScale.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/Vector.h"

// Sets Default Values
APG_FPS_DistanceBasedScale::APG_FPS_DistanceBasedScale()
{
	PrimaryActorTick.bCanEverTick = false;

	selectedObjectSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SelectedObjectSpringArmComponent"));
	selectedObjectSpringArm->SetupAttachment(camera);
	//selectedObjectSpringArm->TargetArmLength = objectSpringArmLength;
	selectedObjectSpringArm->TargetArmLength = 1000.f;
	
	SetbCanSelect(true);
	SetSelectDistance(1000.f);
}

void APG_FPS_DistanceBasedScale::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("%f"), objectSpringArmLength);
	UE_LOG(LogTemp, Log, TEXT("%f"), selectedObjectSpringArm->TargetArmLength);
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
		hitResult = GetRaycastHitResult(bDebugMode);

		if (hitResult.bBlockingHit && hitResult.Component->IsSimulatingPhysics())
		{
			hitResult.Component->SetSimulatePhysics(false);
			hitResult.Actor->AttachToComponent(selectedObjectSpringArm, FAttachmentTransformRules::KeepWorldTransform, USpringArmComponent::SocketName);

			UE_LOG(LogTemp, Log, TEXT("You hit: %s"), *hitResult.Actor->GetName());
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