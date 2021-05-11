#include "Abilities/TargetActors/GATA_Reusable.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

AGATA_Reusable::AGATA_Reusable()
{
	bDestroyOnConfirmation = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;
	MaxHitResultsPerTrace = 20;
	NumberOfTraces = 1;
	bIgnoreBlockingHits = false;
	bTraceTowardPlayerFront = false;
	TraceRange = 100.f;
	bUsePersistentHitResults = false;
}

void AGATA_Reusable::ResetSpread()
{
	// bUseAimingSpreadMod = false;
	// BaseSpread = 0.0f;
	// AimingSpreadMod = 0.0f;
	// TargetingSpreadIncrement = 0.0f;
	// TargetingSpreadMax = 0.0f;
	// CurrentTargetingSpread = 0.0f;
}

float AGATA_Reusable::GetCurrentSpread() const
{
	// float FinalSpread = BaseSpread + CurrentTargetingSpread;
	// if (bUseAimingSpreadMod && AimingTag.IsValid() && AimingRemovalTag.IsValid())
	// {
	// 	UAbilitySystemComponent* ASC = OwningAbility->GetCurrentActorInfo()->AbilitySystemComponent.Get();
	// 	if (ASC && (ASC->GetTagCount(AimingTag) > ASC->GetTagCount(AimingRemovalTag)))
	// 	{
	// 		FinalSpread *= AimingSpreadMod;
	// 	}
	// }

	return 0.f;
}

void AGATA_Reusable::SetStartLocation(const FGameplayAbilityTargetingLocationInfo& InStartLocation)
{
	StartLocation = InStartLocation;
}

void AGATA_Reusable::SetDestroyOnConfirmation(bool bInDestroyOnConfirmation)
{
	bDestroyOnConfirmation = bInDestroyOnConfirmation;
}

void AGATA_Reusable::StartTargeting(UGameplayAbility* Ability)
{
	// Don't call to Super because we can have more than one Reticle

	SetActorTickEnabled(true);

	OwningAbility = Ability;
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

	// This is a lazy way of emptying and repopulating the ReticleActors.
	// We could come up with a solution that reuses them.
	DestroyReticleActors();

	if (ReticleClass)
	{
		for (int32 i = 0; i < MaxHitResultsPerTrace * NumberOfTraces; i++)
		{
			SpawnReticleActor(GetActorLocation(), GetActorRotation());
		}
	}

	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty();
	}
}

void AGATA_Reusable::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	if (SourceActor)
	{
		TArray<FHitResult> HitResults = PerformTrace(SourceActor);
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(HitResults);
		TargetDataReadyDelegate.Broadcast(Handle);

#if ENABLE_DRAW_DEBUG
		if (bDebug)
		{
			ShowDebugTrace(HitResults, EDrawDebugTrace::Type::ForDuration, 2.0f);
		}
#endif
	}

	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty();
	}
}

void AGATA_Reusable::CancelTargeting()
{
	const FGameplayAbilityActorInfo* ActorInfo = (OwningAbility ? OwningAbility->GetCurrentActorInfo() : nullptr);
	UAbilitySystemComponent* ASC = (ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr);
	if (ASC)
	{
		ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::GenericCancel,
											OwningAbility->GetCurrentAbilitySpecHandle(),
											OwningAbility->GetCurrentActivationInfo().GetActivationPredictionKey()).
			 Remove(GenericCancelHandle);
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("AGameplayAbilityTargetActor::CancelTargeting called with null ASC! Actor %s"),
					*GetName());
	}

	CanceledDelegate.Broadcast(FGameplayAbilityTargetDataHandle());

	SetActorTickEnabled(false);

	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty();
	}
}

void AGATA_Reusable::BeginPlay()
{
	Super::BeginPlay();

	// Start with Tick disabled. We'll enable it in StartTargeting() and disable it again in StopTargeting().
	// For *instant confirmations*, tick will never happen because we StartTargeting(), ConfirmTargeting(), and immediately StopTargeting().
	SetActorTickEnabled(false);
}

void AGATA_Reusable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyReticleActors();

	Super::EndPlay(EndPlayReason);
}

void AGATA_Reusable::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TArray<FHitResult> HitResults;
	if (bDebug || bUsePersistentHitResults)
	{
		// Only need to trace on Tick if we're showing debug or if we use persistent hit results, otherwise we just use the confirmation trace
		HitResults = PerformTrace(SourceActor);
	}

#if ENABLE_DRAW_DEBUG
	if (SourceActor && bDebug)
	{
		//todo duration
		ShowDebugTrace(HitResults, EDrawDebugTrace::Type::ForOneFrame);
		//ShowDebugTrace(HitResults, EDrawDebugTrace::Type::ForDuration);
	}
#endif
}

void AGATA_Reusable::StopTargeting()
{
	SetActorTickEnabled(false);

	DestroyReticleActors();

	// Clear added callbacks
	TargetDataReadyDelegate.Clear();
	CanceledDelegate.Clear();

	if (GenericDelegateBoundASC)
	{
		GenericDelegateBoundASC->GenericLocalConfirmCallbacks.
								 RemoveDynamic(this, &AGameplayAbilityTargetActor::ConfirmTargeting);
		GenericDelegateBoundASC->GenericLocalCancelCallbacks.
								 RemoveDynamic(this, &AGameplayAbilityTargetActor::CancelTargeting);
		GenericDelegateBoundASC = nullptr;
	}
}

FGameplayAbilityTargetDataHandle AGATA_Reusable::MakeTargetData(const TArray<FHitResult>& HitResults) const
{
	FGameplayAbilityTargetDataHandle ReturnDataHandle;

	for (int32 i = 0; i < HitResults.Num(); i++)
	{
		/** Note: These are cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
		FGameplayAbilityTargetData_SingleTargetHit* ReturnData = new FGameplayAbilityTargetData_SingleTargetHit();
		ReturnData->HitResult = HitResults[i];
		ReturnDataHandle.Add(ReturnData);
	}

	return ReturnDataHandle;
}

TArray<FHitResult> AGATA_Reusable::PerformTrace(AActor* InSourceActor)
{
	bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(InSourceActor);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGSGATA_LineTrace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
	Params.bTraceComplex = false;
	Params.bIgnoreBlocks = bIgnoreBlockingHits;

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
	FVector TraceEnd;

	if (MasterPC)
	{
		FVector PlayerLocation = InSourceActor->GetActorLocation() + FVector(0, 0, 40.f);

		TraceStart = bTraceTowardPlayerFront ? PlayerLocation : TraceStart;
	}

	if (bUsePersistentHitResults)
	{
		// Clear any blocking hit results, invalid Actors, or actors out of range
		//TODO Check for visibility if we add AIPerceptionComponent in the future
		for (int32 i = PersistentHitResults.Num() - 1; i >= 0; i--)
		{
			FHitResult& HitResult = PersistentHitResults[i];

			if (HitResult.bBlockingHit || !HitResult.Actor.IsValid() ||
				FVector::DistSquared(TraceStart, HitResult.Actor.Get()->GetActorLocation()) > (TraceRange * TraceRange))
			{
				PersistentHitResults.RemoveAt(i);
			}
		}
	}

	TArray<FHitResult> ReturnHitResults;

	for (int32 TraceIndex = 0; TraceIndex < NumberOfTraces; TraceIndex++)
	{
		// AimWithPlayerController(InSourceActor, Params, TraceStart, TraceEnd);		//Effective on server and launching client only
		TraceEnd = InSourceActor->GetActorForwardVector() * TraceRange + TraceStart;

		// SetActorLocationAndRotation(TraceEnd, SourceActor->GetActorRotation());

		CurrentTraceEnd = TraceEnd;

		TArray<FHitResult> TraceHitResults;
		DoTrace(TraceHitResults, InSourceActor->GetWorld(), Filter, TraceStart, TraceEnd, TraceProfile.Name, Params);

		for (int32 j = TraceHitResults.Num() - 1; j >= 0; j--)
		{
			if (MaxHitResultsPerTrace >= 0 && j + 1 > MaxHitResultsPerTrace)
			{
				// Trim to MaxHitResultsPerTrace
				TraceHitResults.RemoveAt(j);
				continue;
			}

			FHitResult& HitResult = TraceHitResults[j];

			// Reminder: if bUsePersistentHitResults, Number of Traces = 1
			if (bUsePersistentHitResults)
			{
				// This is looping backwards so that further objects from player are added first to the queue.
				// This results in closer actors taking precedence as the further actors will get bumped out of the TArray.
				if (HitResult.Actor.IsValid() && (!HitResult.bBlockingHit || PersistentHitResults.Num() < 1))
				{
					bool bActorAlreadyInPersistentHits = false;

					// Make sure PersistentHitResults doesn't have this hit actor already
					for (int32 k = 0; k < PersistentHitResults.Num(); k++)
					{
						FHitResult& PersistentHitResult = PersistentHitResults[k];

						if (PersistentHitResult.Actor.Get() == HitResult.Actor.Get())
						{
							bActorAlreadyInPersistentHits = true;
							break;
						}
					}

					if (bActorAlreadyInPersistentHits)
					{
						continue;
					}

					if (PersistentHitResults.Num() >= MaxHitResultsPerTrace)
					{
						// Treat PersistentHitResults like a queue, remove first element
						PersistentHitResults.RemoveAt(0);
					}

					PersistentHitResults.Add(HitResult);
				}
			}
			else
			{
				// ReticleActors for PersistentHitResults are handled later
				int32 ReticleIndex = TraceIndex * MaxHitResultsPerTrace + j;
				if (ReticleIndex < ReticleActors.Num())
				{
					if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[ReticleIndex].Get())
					{
						const bool bHitActor = HitResult.Actor != nullptr;

						if (bHitActor && !HitResult.bBlockingHit)
						{
							LocalReticleActor->SetActorHiddenInGame(false);

							const FVector ReticleLocation = (bHitActor && LocalReticleActor->bSnapToTargetedActor)
																? HitResult.Actor->GetActorLocation()
																: HitResult.Location;

							LocalReticleActor->SetActorLocation(ReticleLocation);
							LocalReticleActor->SetIsTargetAnActor(bHitActor);
						}
						else
						{
							LocalReticleActor->SetActorHiddenInGame(true);
						}
					}
				}
			}
		} // for TraceHitResults

		if (!bUsePersistentHitResults)
		{
			if (TraceHitResults.Num() < ReticleActors.Num())
			{
				// We have less hit results than ReticleActors, hide the extra ones
				for (int32 j = TraceHitResults.Num(); j < ReticleActors.Num(); j++)
				{
					if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[j].Get())
					{
						LocalReticleActor->SetIsTargetAnActor(false);
						LocalReticleActor->SetActorHiddenInGame(true);
					}
				}
			}
		}

		if (TraceHitResults.Num() < 1)
		{
			// If there were no hits, add a default HitResult at the end of the trace
			FHitResult HitResult;
			// Start param could be player ViewPoint. We want HitResult to always display the StartLocation.
			HitResult.TraceStart = StartLocation.GetTargetingTransform().GetLocation();
			HitResult.TraceEnd = TraceEnd;
			HitResult.Location = TraceEnd;
			HitResult.ImpactPoint = TraceEnd;
			TraceHitResults.Add(HitResult);

			if (bUsePersistentHitResults && PersistentHitResults.Num() < 1)
			{
				PersistentHitResults.Add(HitResult);
			}
		}

		ReturnHitResults.Append(TraceHitResults);
	} // for NumberOfTraces

	// Reminder: if bUsePersistentHitResults, Number of Traces = 1
	if (bUsePersistentHitResults && MaxHitResultsPerTrace > 0)
	{
		// Handle ReticleActors
		for (int32 PersistentHitResultIndex = 0; PersistentHitResultIndex < PersistentHitResults.Num();
			 PersistentHitResultIndex++)
		{
			FHitResult& HitResult = PersistentHitResults[PersistentHitResultIndex];

			// Update TraceStart because old persistent HitResults will have their original TraceStart and the player could have moved since then
			HitResult.TraceStart = StartLocation.GetTargetingTransform().GetLocation();

			if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[PersistentHitResultIndex].Get())
			{
				const bool bHitActor = HitResult.Actor != nullptr;

				if (bHitActor && !HitResult.bBlockingHit)
				{
					LocalReticleActor->SetActorHiddenInGame(false);

					const FVector ReticleLocation = (bHitActor && LocalReticleActor->bSnapToTargetedActor)
														? HitResult.Actor->GetActorLocation()
														: HitResult.Location;

					LocalReticleActor->SetActorLocation(ReticleLocation);
					LocalReticleActor->SetIsTargetAnActor(bHitActor);
				}
				else
				{
					LocalReticleActor->SetActorHiddenInGame(true);
				}
			}
		}

		if (PersistentHitResults.Num() < ReticleActors.Num())
		{
			// We have less hit results than ReticleActors, hide the extra ones
			for (int32 PersistentHitResultIndex = PersistentHitResults.Num(); PersistentHitResultIndex < ReticleActors.
				 Num(); PersistentHitResultIndex++)
			{
				if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[PersistentHitResultIndex].Get())
				{
					LocalReticleActor->SetIsTargetAnActor(false);
					LocalReticleActor->SetActorHiddenInGame(true);
				}
			}
		}

		return PersistentHitResults;
	}

	return ReturnHitResults;
}

AGameplayAbilityWorldReticle* AGATA_Reusable::SpawnReticleActor(FVector Location, FRotator Rotation)
{
	if (ReticleClass)
	{
		AGameplayAbilityWorldReticle* SpawnedReticleActor = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle
		>(ReticleClass, Location, Rotation);
		if (SpawnedReticleActor)
		{
			SpawnedReticleActor->InitializeReticle(this, MasterPC, ReticleParams);
			SpawnedReticleActor->SetActorHiddenInGame(true);
			ReticleActors.Add(SpawnedReticleActor);

			// This is to catch cases of playing on a listen server where we are using a replicated reticle actor.
			// (In a client controlled player, this would only run on the client and therefor never replicate. If it runs
			// on a listen server, the reticle actor may replicate. We want consistancy between client/listen server players.
			// Just saying 'make the reticle actor non replicated' isnt a good answer, since we want to mix and match reticle
			// actors and there may be other targeting types that want to replicate the same reticle actor class).
			if (!ShouldProduceTargetDataOnServer)
			{
				SpawnedReticleActor->SetReplicates(false);
			}

			return SpawnedReticleActor;
		}
	}

	return nullptr;
}

void AGATA_Reusable::DestroyReticleActors()
{
	for (int32 i = ReticleActors.Num() - 1; i >= 0; i--)
	{
		if (ReticleActors[i].IsValid())
		{
			ReticleActors[i].Get()->Destroy();
		}
	}

	ReticleActors.Empty();
}
