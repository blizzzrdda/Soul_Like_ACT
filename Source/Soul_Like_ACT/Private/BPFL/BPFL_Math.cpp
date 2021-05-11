// Fill out your copyright notice in the Description page of Project Settings.


#include "BPFL/BPFL_Math.h"

#include "Soul_Like_ACT.h"
#include "GameFramework/Actor.h"
#include "Math/Matrix.h"

void UBPFL_Math::FindDegreeToTarget(const AActor* HitActor, const AActor* Attacker, float& Result)
{
	const FVector FacingVector = (Attacker->GetActorLocation() - HitActor->GetActorLocation());

	FindDegreeToHitNorm(HitActor->GetActorRotation(), FacingVector, Result);
}

void UBPFL_Math::FindDegreeToHitNorm(const FRotator& ActorRotation, const FVector& HitNorm, float& Result)
{
	const FMatrix RotMatrix = FRotationMatrix(ActorRotation);
	const FVector ForwardVec = RotMatrix.GetScaledAxis(EAxis::X);
	const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);

	const FVector ImpactNorm = HitNorm.GetSafeNormal2D(.1f);

	const float ForwardCosAngle = FVector::DotProduct(ForwardVec, ImpactNorm);
	float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

	const float RightCosAngle = FVector::DotProduct(RightVector, ImpactNorm);

	//Left
	if (RightCosAngle < 0.f)
		ForwardDeltaDegree *= -1.f;

	Result = ForwardDeltaDegree;
}

void UBPFL_Math::DegreeToFourDirection(float Degree, EFourDirection& Direction)
{
	if (Degree < 45.f && Degree > -45.f)
		Direction = EFourDirection::Forward;
	else if (Degree <= -45.f && Degree > -135.f)
		Direction = EFourDirection::Left;
	else if (Degree <= -135.f || Degree >= 135.f)
		Direction = EFourDirection::Backward;
	else if (Degree >= 45.f && Degree < 135.f)
		Direction = EFourDirection::Right;
}

bool UBPFL_Math::GetDeviationToFourDirection(float Degree, float& Deviation)
{
	if (Degree <= 45.f && Degree > -45.f)
		Deviation = Degree;
	else if (Degree <= -45. && Degree > -135.f)
		Deviation = Degree + 90.f;
	else if (Degree <= -135.f && Degree >= -180.f)
		Deviation = Degree + 180.f;
	else if (Degree >= 45.f && Degree < 135.f)
		Deviation = Degree - 90.f;
	else if (Degree >= 135.f && Degree <= 180.f)
		Deviation = Degree - 180.f;
	else
	{
		LOG_FUNC_FAILURE("Degree out of range")
		Deviation = 0.f;

		return false;
	}

	return true;
}
