// Fill out your copyright notice in the Description page of Project Settings.


#include "PG_FPS_DistanceBasedScale.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/Vector.h"

// Sets Default Values
APG_FPS_DistanceBasedScale::APG_FPS_DistanceBasedScale()
{
	PrimaryActorTick.bCanEverTick = true;

	selectedObjectSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SelectedObjectSpringArmComponent"));
	selectedObjectSpringArm->SetupAttachment(camera);
	//selectedObjectSpringArm->TargetArmLength = objectSpringArmLength;
	selectedObjectSpringArm->TargetArmLength = 0;
	
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
	//UE_LOG(LogTemp, Log, TEXT("TICK"));

	if (selectedObject != NULL)
	{
		FHitResult hitResultLocal = GetRaycastHitResult(false);
		// Bug : raycast must ignore the hitResult

		float distanceFromObstacle = hitResultLocal.Distance;
		//selectedObjectSpringArm->TargetArmLength = (distanceFromObstacle * -1);
		FVector hitResultLoc = hitResult.Component->GetRelativeLocation();
		//hitResult.GetComponent()->SetRelativeLocation(FVector(hitResultLoc.X, distanceFromObstacle, hitResultLoc.Z));
		
		float distancePercentage = distanceFromObstacle / initialDistance;
		FVector hitResultScale = hitResult.Actor->GetActorScale3D();
		selectedObjectSpringArm->TargetArmLength = initialDistancePlayerToObject * -((distanceFromObstacle - initialDistancePlayerToObject)/ initialDistancePlayerToObject);
		//selectedObjectSpringArm->TargetArmLength = initialDistancePlayerToObject * -(distancePercentage);
		//selectedObjectSpringArm->TargetArmLength = initialDistancePlayerToObject + (distancePercentage);
		hitResult.Actor->SetActorScale3D(FVector(1,1,1) * distancePercentage);
		UE_LOG(LogTemp, Log, TEXT("Distance From Wall: %f"), distanceFromObstacle);
		UE_LOG(LogTemp, Log, TEXT("initial Distance: %f"), initialDistance);
		UE_LOG(LogTemp, Log, TEXT("Distance Percentage: %f"), distancePercentage);
		UE_LOG(LogTemp, Log, TEXT("Hit result rel loc: %s"), *hitResultLoc.ToString());
		UE_LOG(LogTemp, Log, TEXT("New Distance: %f"), initialDistancePlayerToObject * ((distanceFromObstacle - initialDistancePlayerToObject) / initialDistancePlayerToObject));

		
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
			hitResult.Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
			hitResult.Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
			FVector hitResultRelLoc = hitResult.GetComponent()->GetRelativeLocation();
			hitResult.GetComponent()->SetRelativeLocation(FVector(hitResultRelLoc.X, 0, hitResultRelLoc.Z), false, nullptr, ETeleportType::ResetPhysics);
			//hitResult.GetComponent()->SetRelativeLocation(FVector(0, 0, 0), false, nullptr, ETeleportType::ResetPhysics);
			//hitResult.GetComponent()->SetWorldLocation(FVector(hitResultRelLoc.X, 0, hitResultRelLoc.Z), false, nullptr, ETeleportType::ResetPhysics);
			//hitResult.GetComponent()->SetRelativeLocation(FVector(hitResultLoc.X, distanceFromObstacle, hitResultLoc.Z));
			initialDistancePlayerToObject = hitResult.Distance;
			initialDistance = GetRaycastHitResult(false).Distance;
			//UE_LOG(LogTemp, Log, TEXT("Distance From Wall: %f"), distanceFromObstacle);
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
		hitResult.Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		hitResult.Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
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