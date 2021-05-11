// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "SoulPlayerCamera.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API ASoulPlayerCamera : public ACameraActor
{
	GENERATED_BODY()

public:
    ASoulPlayerCamera();
    
    friend class ASoulPlayerCharacter;    
};
