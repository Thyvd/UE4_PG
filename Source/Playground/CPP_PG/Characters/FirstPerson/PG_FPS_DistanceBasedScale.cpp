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
	selectedObjectSpringArm->TargetArmLength = 0;
	
	SetbCanSelect(true);
	SetSelectDistance(5000.f);
}

void APG_FPS_DistanceBasedScale::BeginPlay()
{
	Super::BeginPlay();

}

void APG_FPS_DistanceBasedScale::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (selectedObject != NULL)
	{
		FHitResult hitResultLocal = GetRaycastHitResult(true);
		// Bug : raycast must ignore the hitResult

		float distanceFromObstacle = hitResultLocal.Distance;
		selectedObjectSpringArm->TargetArmLength = distanceFromObstacle * -1;
		FVector hitResultLocalLoc = hitResultLocal.GetComponent()->GetRelativeLocation();
		hitResultLocal.GetComponent()->SetRelativeLocation(FVector(hitResultLocalLoc.X, 0, hitResultLocalLoc.Z));
		
		float distancePercentage = distanceFromObstacle / initialDistance;
		FVector hitResultScale = hitResult.Actor->GetActorScale3D();
		hitResult.Actor->SetActorScale3D(hitResultScale * distancePercentage);
		UE_LOG(LogTemp, Log, TEXT("Distance From Wall: %f"), distanceFromObstacle);
	}
}

/** Distance Based Scale Functions*/
void APG_FPS_DistanceBasedScale::SelectObject()
{
	if (GetbCanSelect())
	{
		hitResult = GetRaycastHitResult(bDebugMode);

		if (SelectedObjectIsValid(hitResult))
		{
			selectedObject = hitResult.GetActor();
			hitResult.Component->SetSimulatePhysics(false);
			hitResult.Actor->AttachToComponent(selectedObjectSpringArm, FAttachmentTransformRules::KeepWorldTransform, USpringArmComponent::SocketName);
			initialDistance = hitResult.Distance;
			FVector hitResultRelLoc = hitResult.GetComponent()->GetRelativeLocation();
			//hitResult.GetComponent()->SetRelativeLocation(FVector(hitResultRelLoc.X, 0, hitResultRelLoc.Z), false, nullptr, ETeleportType::ResetPhysics);
			//hitResult.GetComponent()->SetRelativeLocation(FVector(0, 0, 0), false, nullptr, ETeleportType::ResetPhysics);
			//hitResult.GetComponent()->SetWorldLocation(FVector(hitResultRelLoc.X, 0, hitResultRelLoc.Z), false, nullptr, ETeleportType::ResetPhysics);
			UE_LOG(LogTemp, Log, TEXT("You hit: %s"), *hitResult.GetComponent()->GetRelativeLocation().ToString());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Out of Range"));
		}
	}
}

void APG_FPS_DistanceBasedScale::DeselectObject()
{
	if (selectedObject != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Deselected"));
		hitResult.Component->SetSimulatePhysics(true);
		hitResult.Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		selectedObject = NULL;
	}
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

bool APG_FPS_DistanceBasedScale::SelectedObjectIsValid(FHitResult hitResultLocal)
{
	//bool L_isBlockingRaycast = hitResultLocal.bBlockingHit;
	//bool L_isPhysicsEnabled = hitResultLocal.Component->IsSimulatingPhysics();
	//bool L_isObjectAtRest = hitResultLocal.Actor->GetVelocity().Equals(FVector(0,0,0), 1.f);

	return (hitResultLocal.bBlockingHit && hitResultLocal.Component->IsSimulatingPhysics()) ? true : false;
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