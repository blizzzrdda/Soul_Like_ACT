#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MobRageManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRagePointUpdated);

UCLASS()
class SOUL_LIKE_ACT_API UMobRageManager : public UActorComponent
{
public:
    GENERATED_BODY()

    UMobRageManager();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;
    
    UPROPERTY(BlueprintReadOnly, Category = RageSystem)
    float RageScore;

    UFUNCTION(BlueprintCallable, Category = RageSystem)
    void UpdateRageScore(float AdditiveRageScore, bool bOverride, int32& UpdatedRageScore);
    UPROPERTY(BlueprintAssignable)
    FOnRagePointUpdated OnRagePointUpdated;
};