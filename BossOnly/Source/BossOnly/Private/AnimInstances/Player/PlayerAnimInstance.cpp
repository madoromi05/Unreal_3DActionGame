// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Player/PlayerAnimInstance.h"
#include "Character/PlayerCharacter.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    if (OwningCharacter) 
    {
        OwningPlayerCharacter = Cast<APlayerCharacter>(OwningCharacter);
    }
}

void UPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

    if (OwningPlayerCharacter)
    {
        bIsDashing = OwningPlayerCharacter->GetIsDashing();
    }
}
