#include "Abilities/TargetActors/GATA_Reusable_Sphere.h"

AGATA_Reusable_Sphere::AGATA_Reusable_Sphere()
{
	TraceSphereRadius = 100.0f;
}

void AGATA_Reusable_Sphere::Configure(const FGameplayAbilityTargetingLocationInfo& InStartLocation,
									  FGameplayTag InAimingTag, FGameplayTag InAimingRemovalTag,
									  FCollisionProfileName InTraceProfile,
									  FGameplayTargetDataFilterHandle InFilter,
									  TSubclassOf<AGameplayAbilityWorldReticle> InReticleClass,
									  FWorldReticleParameters InReticleParams,
									  bool bInIgnoreBlockingHits,
									  bool bInUsePersistentHitResults, bool bInDebug,
									  bool InbTraceTowardPlayerFront,
									  float InTraceRange, float InTraceSphereRadius,
									  int32 InMaxHitResultsPerTrace,
									  int32 InNumberOfTraces)
{
	StartLocation = InStartLocation;
	AimingTag = InAimingTag;
	AimingRemovalTag = InAimingRemovalTag;
	TraceProfile = InTraceProfile;
	Filter = InFilter;
	ReticleClass = InReticleClass;
	ReticleParams = InReticleParams;
	bTraceTowardPlayerFront = InbTraceTowardPlayerFront;
	bIgnoreBlockingHits = bInIgnoreBlockingHits;
	// ShouldProduceTargetDataOnServer = bInShouldProduceTargetDataOnServer;
	bUsePersistentHitResults = bInUsePersistentHitResults;
	bDebug = bInDebug;
	// bTraceAffectsAimPitch = bInTraceAffectsAimPitch;
	// bTraceFromPlayerViewPoint = bInTraceFromPlayerViewPoint;
	// bUseAimingSpreadMod = bInUseAimingSpreadMod;
	TraceRange = InTraceRange;
	TraceSphereRadius = InTraceSphereRadius;
	// BaseSpread = InBaseSpread;
	// AimingSpreadMod = InAimingSpreadMod;
	// TargetingSpreadIncrement = InTargetingSpreadIncrement;
	// TargetingSpreadMax = InTargetingSpreadMax;
	MaxHitResultsPerTrace = InMaxHitResultsPerTrace;
	NumberOfTraces = InNumberOfTraces;

	if (bUsePersistentHitResults)
	{
		NumberOfTraces = 1;
	}
}

void AGATA_Reusable_Sphere::SphereTraceWithFilter(TArray<FHitResult>& OutHitResults, const UWorld* World,
												  const FGameplayTargetDataFilterHandle FilterHandle,
												  const FVector& Start, const FVector& End, float Radius,
												  FName ProfileName, const FCollisionQueryParams Params)
{
	check(World);
	TArray<FHitResult> HitResults;
	World->SweepMultiByProfile(HitResults, Start, End, FQuat::Identity, ProfileName,
							   FCollisionShape::MakeSphere(Radius), Params);

	TArray<FHitResult> FilteredHitResults;

	for (int32 HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx)
	{
		FHitResult& Hit = HitResults[HitIdx];

		if (!Hit.Actor.IsValid() || FilterHandle.FilterPassesForActor(Hit.Actor))
		{
			Hit.TraceStart = Start;
			Hit.TraceEnd = End;

			FilteredHitResults.Add(Hit);
		}
	}

	OutHitResults = FilteredHitResults;

	return;
}

void AGATA_Reusable_Sphere::DoTrace(TArray<FHitResult>& HitResults, const UWorld* World,
									const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start,
									const FVector& End, FName ProfileName,
									const FCollisionQueryParams Params)
{
	SphereTraceWithFilter(HitResults, World, FilterHandle, Start, End, TraceSphereRadius, ProfileName, Params);
}

void AGATA_Reusable_Sphere::ShowDebugTrace(TArray<FHitResult>& HitResults, EDrawDebugTrace::Type DrawDebugType,
										   float Duration)
{
#if ENABLE_DRAW_DEBUG
	if (bDebug)
	{
		FVector TraceEnd = HitResults[0].TraceEnd;
		const FVector TraceStart = HitResults[0].TraceStart;
		if (NumberOfTraces > 1 || bUsePersistentHitResults)
		{
			TraceEnd = CurrentTraceEnd;
		}

		DrawDebugSphereTraceMulti(GetWorld(), TraceStart, TraceEnd, TraceSphereRadius, DrawDebugType, true, HitResults,
								  FLinearColor::Green, FLinearColor::Red, Duration);
	}
#endif
}

#if ENABLE_DRAW_DEBUG
void AGATA_Reusable_Sphere::DrawDebugSweptSphere(const UWorld* InWorld, FVector const& Start, FVector const& End,
												 float Radius, FColor const& Color, bool bPersistentLines,
												 float LifeTime, uint8 DepthPriority)
{
	FVector const TraceVec = End - Start;
	float const Dist = TraceVec.Size();

	FVector const Center = Start + TraceVec * 0.5f;
	float const HalfHeight = (Dist * 0.5f) + Radius;

	FQuat const CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	::DrawDebugCapsule(InWorld, Center, HalfHeight, Radius, CapsuleRot, Color, bPersistentLines, LifeTime,
					   DepthPriority);
}

void AGATA_Reusable_Sphere::DrawDebugSphereTraceMulti(const UWorld* World, const FVector& Start, const FVector& End,
													  float Radius, EDrawDebugTrace::Type DrawDebugType, bool bHit,
													  const TArray<FHitResult>& OutHits,
													  FLinearColor TraceColor, FLinearColor TraceHitColor,
													  float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && OutHits.Last().bBlockingHit)
		{
			// Red up to the blocking hit, green thereafter
			FVector const BlockingHitPoint = OutHits.Last().Location;
			DrawDebugSweptSphere(World, Start, BlockingHitPoint, Radius, TraceColor.ToFColor(true), bPersistent,
								 LifeTime);
			DrawDebugSweptSphere(World, BlockingHitPoint, End, Radius, TraceHitColor.ToFColor(true), bPersistent,
								 LifeTime);
		}
		else
		{
			// no hit means all red
			DrawDebugSweptSphere(World, Start, End, Radius, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}

		// draw hits
		for (int32 HitIdx = 0; HitIdx < OutHits.Num(); ++HitIdx)
		{
			FHitResult const& Hit = OutHits[HitIdx];
			::DrawDebugPoint(World, Hit.ImpactPoint, 16.0f,
							 (Hit.bBlockingHit ? TraceColor.ToFColor(true) : TraceHitColor.ToFColor(true)), bPersistent,
							 LifeTime);
		}
	}
}
#endif // ENABLE_DRAW_DEBUG