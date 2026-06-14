#include "AnimInstances/CharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/CharacterBase.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<ACharacterBase>(TryGetPawnOwner());

	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	bIsInAir = OwningMovementComponent->IsFalling();
	VerticalVelocity = OwningCharacter->GetVelocity().Z;
}