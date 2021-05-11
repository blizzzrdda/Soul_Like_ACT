// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/DEPRECATED_ANS_WeaponSweeping.h"
#include "SoulCharacterBase.h"

void UDEPRECATED_ANS_WeaponSweeping::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
									  float TotalDuration)
{
	if(!MeshComp->IsValidLowLevel()) return;

	const auto MeshOwner = MeshComp->GetOwner();
	if(MeshOwner->IsA(ASoulCharacterBase::StaticClass()))
	{
		auto TempWeaponRef = Cast<ASoulCharacterBase>(MeshOwner)->GetWeaponActor();
		if(TempWeaponRef)
		{
			TempWeaponRef->StartSwing(DamageMulti, PostureDamageMulti);
			return;
		}
	}

	NotifyEnd(MeshComp, Animation);
}

void UDEPRECATED_ANS_WeaponSweeping::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(!MeshComp->IsValidLowLevel()) return;
	
	const auto MeshOwner = MeshComp->GetOwner();
	if(MeshOwner->IsA(ASoulCharacterBase::StaticClass()))
	{
		auto TempWeaponRef = Cast<ASoulCharacterBase>(MeshOwner)->GetWeaponActor();
		if(TempWeaponRef)
		{
			TempWeaponRef->EndSwing();
			return;
		}
	}

	NotifyEnd(MeshComp, Animation);
}