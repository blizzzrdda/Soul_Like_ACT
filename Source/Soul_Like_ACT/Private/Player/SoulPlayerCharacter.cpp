// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Player/SoulPlayerCharacter.h"
#include "Soul_Like_ACT.h"
#include "Player/PlayerTargetingComponent.h"
#include "Player/SoulPlayerController.h"
#include "Item/WeaponActor.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/EquipmentManager.h"
#include "Player/SoulPlayerCamera.h"

ASoulPlayerCharacter::ASoulPlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1000.f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->MaxWalkSpeed = 550.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Soul-Like Cam
	Soul_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Soul Camera Boom"));
	Soul_CameraBoom->SetupAttachment(RootComponent);
	Soul_CameraBoom->TargetArmLength = 400.0f;
	Soul_CameraBoom->bUsePawnControlRotation = true;

	Soul_CameraActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Soul Follow Camera"));
	Soul_CameraActor->SetupAttachment(Soul_CameraBoom, USpringArmComponent::SocketName);
	Soul_CameraActor->SetChildActorClass(ASoulPlayerCamera::StaticClass());
	// Soul_CameraActor->CreateChildActor();

	//TPS Cam
	TPS_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPS Camera Boom"));
	TPS_CameraBoom->SetupAttachment(RootComponent);
	TPS_CameraBoom->TargetArmLength = 650.0f;
	TPS_CameraBoom->bUsePawnControlRotation = false;
	TPS_CameraBoom->bInheritPitch = false;
	TPS_CameraBoom->bInheritRoll = false;
	TPS_CameraBoom->bInheritYaw = false;
	TPS_CameraBoom->SetRelativeRotation(FRotator(0.f, -55.f, 0.f));

	TPS_CameraActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("TPS Follow Camera"));
	TPS_CameraActor->SetupAttachment(TPS_CameraBoom, USpringArmComponent::SocketName);
	TPS_CameraActor->SetChildActorClass(ASoulPlayerCamera::StaticClass());
	// TPS_CameraActor->CreateChildActor();

	// Arrow Component for Target Lock Component
	TargetLockArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("TargetLockArrow"));
	TargetLockArrow->SetupAttachment(RootComponent);
	TargetLockArrow->SetUsingAbsoluteRotation(true);

	TargetingComponent = CreateDefaultSubobject<UPlayerTargetingComponent>(TEXT("TargetLockingComponent"));

	Faction = EActorFaction::Player;

	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
	 TEXT("AIPerceptionStimuliSource"));
}

void ASoulPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MakeMove();
}

void ASoulPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASoulPlayerCharacter::CalculateLeanValue);

	PlayerInputComponent->BindAxis("TurnRate", this, &ASoulPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASoulPlayerCharacter::LookUpAtRate);

	PlayerInputComponent->BindAxis("Zoom", this, &ASoulPlayerCharacter::ZoomCamera);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASoulPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASoulPlayerCharacter::MoveRight);

	FInputActionBinding Binding_Targeting("Targeting", IE_Pressed);
	Binding_Targeting.ActionDelegate.GetDelegateForManualSet().BindLambda(
																		  [this]()
																		  {
																			  TargetingComponent->
																				  ToggleCameraLock(GetSoulController()->
																				   GetCamMode() ==
																				   ECameraMode::BackFront);
																		  });

	FInputActionBinding Binding_LeftTarget("LeftTarget", IE_Pressed);
	Binding_Targeting.ActionDelegate.GetDelegateForManualSet().BindLambda(
																		  [this]()
																		  {
																			  TargetingComponent->
																				  Toggle_InDirection(ETargetFindingDirection::Left);
																		  });

	FInputActionBinding Binding_RightTarget("RightTarget", IE_Pressed);
	Binding_Targeting.ActionDelegate.GetDelegateForManualSet().BindLambda(
																		  [this]()
																		  {
																			  TargetingComponent->
																				  Toggle_InDirection(ETargetFindingDirection::Right);
																		  });
}

//TODO add keyboard predict direction
void ASoulPlayerCharacter::ForceOverrideFacingDirection(float Alpha /*=180.f*/)
{
	FRotator LookAtRotation;

	if (GetSoulController()->GetCamMode() == ECameraMode::DetachedCam)
	{
		LOG_FUNC_FAILURE("Camera detached from the character");
		return;
	}

	//Case: targeting
	if (TargetingComponent->GetIsTargetingEnabled())
	{
		AActor* Target = TargetingComponent->LockedTarget;

		//A: Use looking-at rotation
		if (Target)
			LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
																	Cast<AActor>(Target)->GetActorLocation());
		else
			return;
	}
		//Case: WSAD
	else if (IsMovementInputPressed())
	{
		FVector to_vector;
		float degree;

		PredictMovement(to_vector, degree);

		LookAtRotation = to_vector.Rotation();
	}
		//Case: Not-pressing the button
	else
	{
		//B: Use controller's rotation
		switch (GetSoulController()->GetCamMode())
		{
			case ECameraMode::BackFront:
				LookAtRotation = GetControlRotation();
				break;
			case ECameraMode::TopDown:
				return;
			case ECameraMode::DetachedCam:
				return;
			default:
				return;
		}
	}

	//trim-off roll and pitch axises
	LookAtRotation.Roll = LookAtRotation.Pitch = 0.f;

	const FRotator TargetRotation = UKismetMathLibrary::RLerp(GetActorRotation(), LookAtRotation, Alpha, true);
	SetActorRotation(TargetRotation);
}

void ASoulPlayerCharacter::GetActiveCameraAndSpringArm(ACameraActor*& ActiveCam, USpringArmComponent*& ActiveSpringArm,
													   bool& isValid) const
{
	if (!Controller) return;
	ASoulPlayerController* MyController = Cast<ASoulPlayerController>(GetController());

	if (MyController)
	{
		switch (MyController->GetCamMode())
		{
			case ECameraMode::BackFront:
				ActiveCam = Cast<ACameraActor>(Soul_CameraActor->GetChildActor());
				ActiveSpringArm = Soul_CameraBoom;
				isValid = true;
				break;
			case ECameraMode::TopDown:
				ActiveCam = Cast<ACameraActor>(TPS_CameraActor->GetChildActor());
				ActiveSpringArm = TPS_CameraBoom;
				break;
			case ECameraMode::DetachedCam:
				break;
			default:
				break;
		}
	}
}

void ASoulPlayerCharacter::ResetRotation()
{
	SetActorRotation(
					 FRotator{
						 GetActorRotation().Pitch, GetInstigator()->GetViewRotation().Yaw, GetActorRotation().Roll
					 });
}

AWeaponActor* ASoulPlayerCharacter::EquipGear(TSubclassOf<AWeaponActor> WeaponClassRef, bool bShowTracelines)
{
	//UnEquip and delete the item
	AWeaponActor* LocalWeapon = Cast<AWeaponActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
													GetWorld(), WeaponClassRef, FTransform::Identity,
													ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));

	LocalWeapon->SetInstigator(this);
	LocalWeapon->SetOwner(this);
	LocalWeapon->bEnableDrawTraceLine = bShowTracelines;

	return LocalWeapon;
}

ASoulPlayerController* ASoulPlayerCharacter::GetSoulController() const
{
	return Cast<ASoulPlayerController>(GetController());
}

AWeaponActor* ASoulPlayerCharacter::GetWeaponActor()
{
	const auto LocalController = GetSoulController();
	if (LocalController)
	{
		return LocalController->GetEquipmentManager()->CurrWeaponActor;
	}

	return nullptr;
}

ASoulPlayerCharacter* ASoulPlayerCharacter::GetSoulPlayerChar(UObject* WorldContextObj)
{
	if (WorldContextObj->IsValidLowLevel())
	{
		APlayerController* PlayerController = WorldContextObj->GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			APawn* PlayerChar = PlayerController->GetPawn();
			if (PlayerChar && PlayerChar->IsA(ASoulPlayerCharacter::StaticClass()))
			{
				return Cast<ASoulPlayerCharacter>(PlayerChar);
			}
		}
	}

	return nullptr;
}

UPlayerTargetingComponent* ASoulPlayerCharacter::GetPlayerTargetLockingComponent(UObject* WorldContextObject)
{
	auto PlayerChar = ASoulPlayerCharacter::GetSoulPlayerChar(WorldContextObject);

	if (!PlayerChar) return nullptr;

	return PlayerChar->TargetingComponent;
}

ESoulMovementMode ASoulPlayerCharacter::GetMovementMode() const
{
	// //No input -> idle
	// if (FMath::IsNearlyZero(ForwardAxisValue) && FMath::IsNearlyZero(RightAxisValue))
	//     return ESoulMovementMode::Idle;
	//
	// if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	//     return  ESoulMovementMode::Idle;
	//
	return Super::GetMovementMode();
}

//TODO this should be disabled when TPS mode
void ASoulPlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (GetSoulController()->GetCamMode() == ECameraMode::BackFront)
		AddControllerYawInput(_bFreeCamera * Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASoulPlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (GetSoulController()->GetCamMode() == ECameraMode::BackFront)
		AddControllerPitchInput(_bFreeCamera * Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASoulPlayerCharacter::ZoomCamera(float Rate)
{
	if (GetSoulController()->GetCamMode() == ECameraMode::BackFront)
	{
		Soul_CameraBoom->TargetArmLength += Rate * -100.f;
	}
	else if (GetSoulController()->GetCamMode() == ECameraMode::TopDown)
	{
		TPS_CameraBoom->TargetArmLength += Rate * -100.f;
	}
}

void ASoulPlayerCharacter::CalculateLeanValue(float TurnValue)
{
	if (TargetingComponent->GetIsTargetingEnabled() || GetMovementComponent()->Velocity.Size() < 10.f)
	{
		LeanAmount_Char = 0.f;
		LeanSpeed_Char = 10.f;
	}
	else
	{
		LeanAmount_Char = TurnValue * 10.f;
		LeanSpeed_Char = 1.f;
	}

	LeanAmount_Anim = FMath::FInterpTo(LeanAmount_Anim, LeanAmount_Char, GetWorld()->GetDeltaSeconds(), LeanSpeed_Char);
}

void ASoulPlayerCharacter::PredictMovement(FVector& DirectionVec, float& Degree) const
{
	FRotator YawRotation;

	if (GetSoulController()->GetCamMode() == ECameraMode::BackFront)
	{
		YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	}
	else if (GetSoulController()->GetCamMode() == ECameraMode::TopDown)
	{
		YawRotation = FRotator::ZeroRotator;
	}

	DirectionVec =
	(
		FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * ForwardAxisValue +
		FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * RightAxisValue
	).GetSafeNormal2D();

	if (!DirectionVec.IsNearlyZero())
	{
		Degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorForwardVector(), DirectionVec)));

		const float RightCosAngle = FVector::DotProduct(GetActorRightVector(), DirectionVec);
		if (RightCosAngle < 0)
			Degree *= -1;
	}
	else
		Degree = 0;
}


void ASoulPlayerCharacter::MoveForward(float Value)
{
	ForwardAxisValue = Value;
}

void ASoulPlayerCharacter::MoveRight(float Value)
{
	RightAxisValue = Value;
}

void ASoulPlayerCharacter::MakeMove()
{
	if (GetSoulController())
	{
		FVector Direction;
		float VelocityDegree;
		float MoveSpeedMulti_Velocity;

		PredictMovement(Direction, VelocityDegree);

		DegreeToMovementMultiplier(VelocityDegree, MoveSpeedMulti_Velocity);

		// const float MoveMulti = (TargetingComponent->GetIsTargetingEnabled() ? 0.6f : 1.f) * (bIsSprintingPressed ? 1.f : 0.6f) * DegreeMulti;

		const float MoveMulti = (bIsSprintingPressed ? 1.f : 0.6f) * MoveSpeedMulti_Velocity;

		AddMovementInput(Direction, MoveMulti);
	}
}
