// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortalVRTCharacter.h"
#include "PortalVRTProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

#include "BaseBallActor.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APortalVRTCharacter

APortalVRTCharacter::APortalVRTCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Holding component setup
	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	HoldingComponent->SetupAttachment(GetRootComponent());
	HoldingComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 50.0f));

}

void APortalVRTCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	canBePickedUp = true;
}

//////////////////////////////////////////////////////////////////////////// Input

void APortalVRTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APortalVRTCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APortalVRTCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APortalVRTCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void APortalVRTCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APortalVRTCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//////////////////////////////////////////////////////////////////////////// Ball logic

void APortalVRTCharacter::EnablePickup()
{
	canBePickedUp = true;
}

void APortalVRTCharacter::PickupBall(ABaseBallActor* Ball)
{
	if (!Ball) {
		return;
	}

	if (canBePickedUp == false) {
		return;
	}

	canBePickedUp = false;
	HeldBall = Ball;
	HeldBall->SetActorEnableCollision(false);
	HeldBall->BallMesh->SetSimulatePhysics(false);
	HeldBall->AttachToComponent(HoldingComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// this can be edited to multi interact logic

void APortalVRTCharacter::Interact_Implementation()
{
	if (HeldBall)
	{
		GetWorld()->GetTimerManager().SetTimer(PickupCooldownTimer, this, &APortalVRTCharacter::EnablePickup, 10.0f, false);
		HeldBall->SetLastPlayer(PlayerName);
		HeldBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HeldBall->SetActorEnableCollision(true);
		HeldBall->BallMesh->SetSimulatePhysics(true);

		// Throwing
		FVector ForwardVector = GetControlRotation().Vector();
		FVector ThrowImpulse = ForwardVector * ThrowPower;
		HeldBall->BallMesh->AddImpulse(ThrowImpulse, NAME_None, true);

		HeldBall = nullptr;
	}
}