// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/ANS_SendHitEvent.h"
#include "DrawDebugHelpers.h"
#include "BPFL/BPFL_AbilitySystem.h"
#include "Kismet/KismetSystemLibrary.h"

void UANS_SendHitEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	_DrawDebug = bDrawDebugTrace || (MeshComp->GetWorld()->WorldType == EWorldType::Editor || MeshComp->GetWorld()->
		WorldType ==
		EWorldType::EditorPreview);
}

FVector UANS_SendHitEvent_Box::CalculateStartPosition(USkeletalMeshComponent* MeshComp) const
{
	return MeshComp->GetOwner()->GetActorLocation() +
	((MeshComp->GetWorld()->WorldType == EWorldType::Editor || MeshComp->GetWorld()->WorldType ==
		 EWorldType::EditorPreview)
		 ? FVector(0.f, Offset_Forward, 0.f)
		 : MeshComp->GetOwner()->GetActorForwardVector() * Offset_Forward);
}

FVector UANS_SendHitEvent_Box::CalculateEndPosition(USkeletalMeshComponent* MeshComp) const
{
	const auto StartLocation = MeshComp->GetOwner()->GetActorLocation();

	FVector Forward;
	if (MeshComp->GetWorld()->WorldType == EWorldType::Editor || MeshComp->GetWorld()->WorldType ==
		EWorldType::EditorPreview)
		Forward = FVector::RightVector;
	else
		Forward = MeshComp->GetOwner()->GetActorForwardVector();

	return StartLocation + Forward * (Length + Offset_Forward);
}

FVector UANS_SendHitEvent_Box::CalculateHalfSize(USkeletalMeshComponent* MeshComp) const
{
	if (MeshComp->GetWorld()->WorldType == EWorldType::Editor || MeshComp->GetWorld()->WorldType ==
		EWorldType::EditorPreview)
		return FVector(Width * .5f, 0.f, 100.f);

	return FVector(0.f, Width * .5f, 100.f);
}

FRotator UANS_SendHitEvent_Box::CalculateOrientation(USkeletalMeshComponent* MeshComp) const
{
	if (!MeshComp) return FRotator::ZeroRotator;

	if (MeshComp->GetWorld()->WorldType == EWorldType::Editor || MeshComp->GetWorld()->WorldType ==
		EWorldType::EditorPreview)
		return FVector::ForwardVector.Rotation();

	return MeshComp->GetOwner()->GetActorForwardVector().Rotation();
}

void UANS_SendHitEvent_Box::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
									   float FrameDeltaTime)
{
	if (!MeshComp->IsValidLowLevel() || !MeshComp->GetOwner() || !MeshComp->GetOwner()->GetWorld()) return;

	const auto Owner = MeshComp->GetOwner();
	const TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;

	//	Warning: causing game crash when turn off editor
	UKismetSystemLibrary::BoxTraceMultiByProfile(Owner,
												 CalculateStartPosition(MeshComp),
												 CalculateEndPosition(MeshComp),
												 CalculateHalfSize(MeshComp),
												 CalculateOrientation(MeshComp),
												 Profile == NAME_None ? Profile_Pawn : Profile,
												 false,
												 ActorsToIgnore,
												 _DrawDebug
													 ? EDrawDebugTrace::ForOneFrame
													 : EDrawDebugTrace::None,
												 HitResults,
												 true
												);

	for (auto& HitResult : HitResults)
	{
		if (!HitResult.IsValidBlockingHit()) continue;

		FGameplayEventData EventData;
		UBPFL_AbilitySystem::CreateEventData(HitResult.GetActor(), Owner, HitResult, EventTag, 1.f,
											 EventData);

		EventData.OptionalObject = NewObject<USoulJsonObjectWrapper>()->SetActionDamageMulties(Multi_Damage,
			Multi_PostureDamage, Multi_Crumble);

		UBPFL_AbilitySystem::SendGameplayEventToActor(Owner, EventTag, EventData);
	}
}
