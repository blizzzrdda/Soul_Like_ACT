// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulCharacterBase.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/SoulGameplayAbility.h"
#include "Abilities/SoulModifierManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "NavigationSystem.h"

// Sets default values
ASoulCharacterBase::ASoulCharacterBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<USoulAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<USoulAttributeSet>(TEXT("AttributeSet"));

	ModifierManager = CreateDefaultSubobject<USoulModifierManager>(TEXT("ModifierManager"));

	GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);

	TargetIcon = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockIconWidget"));
	TargetIcon->SetupAttachment(RootComponent);
	TargetIcon->SetWidgetSpace(EWidgetSpace::Screen);
	TargetIcon->SetDrawSize(FVector2D{35.f, 35.f});

	AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.
							AddUObject(this, &ASoulCharacterBase::BP_OnGameplayEffectApplied);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().
							AddUObject(this, &ASoulCharacterBase::BP_OnGameplayEffectRemoved);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetMoveSpeedAttribute()).AddLambda([&](const FOnAttributeChangeData& AttriData)
	{
		GetCharacterMovement()->MaxWalkSpeed = AttriData.NewValue;
	});

}

void ASoulCharacterBase::ToggleLockIcon()
{
	if (!IsTargetable() || TargetIcon->IsVisible())
	{
		TargetIcon->SetVisibility(false);
	}
	else
	{
		TargetIcon->SetVisibility(true);
	}
}

void ASoulCharacterBase::TriggerSlowMotion_WithDelay(float Delay)
{
	if (GetWorldTimerManager().GetTimerRemaining(Handler_SlowMotionDelay) <= 0.f)
	{
		GetWorldTimerManager().SetTimer(Handler_SlowMotionDelay, this, &ASoulCharacterBase::TriggerSlowMotion, 1.f, 0,
										Delay);
	}
}

void ASoulCharacterBase::HandleOnDead(const FHitResult& HitInfo,
									  const FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter,
									  AActor* DamageCauser)
{
	BP_OnDead(HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ASoulCharacterBase::HandleOnCrumble(float PostureDamageAmount, const bool IsCriticaled, const FHitResult& HitInfo,
										 const FGameplayTagContainer& DamageTags,
										 ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	BP_OnCrumbled(PostureDamageAmount, IsCriticaled, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ASoulCharacterBase::HandleDamage(float DamageAmount, const bool IsCriticaled, const bool bIsStun,
									  const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
									  ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, IsCriticaled, bIsStun, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ASoulCharacterBase::HandleDotDamage(float DamageAmount, const bool IsCriticaled, const bool bIsStun,
										 const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
										 ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	OnDotDamaged(DamageAmount, IsCriticaled, bIsStun, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ASoulCharacterBase::HandlePostureDamage(float PostureDamageAmount, const bool IsCriticaled,
											 const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
											 ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	OnPostureDamaged(PostureDamageAmount, IsCriticaled, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

bool ASoulCharacterBase::BodySweep_Init(const AActor* Target, bool bUseTarget, float InSweepingSpeed)
{
	BodySweep_ForwardVec = FVector::ZeroVector;

	if (Target && bUseTarget)
		BodySweep_ForwardVec = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal2D(.01);
	else
	{
		BodySweep_ForwardVec = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X).GetSafeNormal2D();
	}

	if (BodySweep_ForwardVec.IsNearlyZero())
		return false;
	else
	{
		//Face the desired direction
		SetActorRotation(BodySweep_ForwardVec.Rotation(), ETeleportType::TeleportPhysics);
		GetCapsuleComponent()->SetEnableGravity(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SweepingSpeed = InSweepingSpeed;

		return true;
	}
}

void ASoulCharacterBase::BodySweep_Tick(float Delta)
{
	//GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red, (BodySweep_ForwardVec  * SweepingSpeed).ToString());
	if (!BodySweep_ForwardVec.IsNearlyZero())
	{
		const FVector ResultLocation = GetActorLocation() + BodySweep_ForwardVec * SweepingSpeed * Delta;
		FNavLocation LocOnNavMesh;

		FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld())->ProjectPointToNavigation(
		 ResultLocation, LocOnNavMesh);

		SetActorLocation(LocOnNavMesh.Location + FVector{0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()});
	}
}

void ASoulCharacterBase::BodySweep_Finished()
{
	GetCapsuleComponent()->SetEnableGravity(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

UAbilitySystemComponent* ASoulCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

USoulModifierManager* ASoulCharacterBase::GetModifierManager() const
{
	return ModifierManager;
}

bool ASoulCharacterBase::IsInRivalFaction(ASoulCharacterBase* DamageDealer, ASoulCharacterBase* DamageReceiver)
{
	if (DamageDealer->Faction == EActorFaction::Player && DamageReceiver->Faction == EActorFaction::Enemy)
		return true;

	else if (DamageDealer->Faction == EActorFaction::Enemy && DamageReceiver->Faction == EActorFaction::Player)
		return true;

	return false;
}
