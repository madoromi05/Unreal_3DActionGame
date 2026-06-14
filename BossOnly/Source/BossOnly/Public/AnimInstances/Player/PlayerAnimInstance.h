#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/CharacterAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;

/**
 * 
 */
UCLASS()
class BOSSONLY_API UPlayerAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
protected:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Refrences")
    APlayerCharacter* OwningPlayerCharacter;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    bool bIsDashing;
};
