#include "Mob/MobRageManager.h"

UMobRageManager::UMobRageManager()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UMobRageManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    RageScore = FMath::Lerp(RageScore, 0.f, DeltaTime * 0.33f);
    //GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::SanitizeFloat(RageScore));
}

void UMobRageManager::UpdateRageScore(float AdditiveRageScore, bool bOverride, int32& UpdatedRageScore)
{
    RageScore = FMath::Clamp(bOverride? AdditiveRageScore : RageScore + AdditiveRageScore , -100.f, 100.f);

    UpdatedRageScore = RageScore;
    
    if(OnRagePointUpdated.IsBound())
        OnRagePointUpdated.Broadcast();
}
