#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BossOnlyGameplayAbility.h"
#include "GA_PlayerLightAttack.generated.h"

class ACharacterWeaponBase;

UCLASS()
class BOSSONLY_API UGA_PlayerLightAttack : public UBossOnlyGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_PlayerLightAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float DamageAmount = 20.f;

private:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	void FinishAttack(bool bWasCancelled);
	void OnWeaponHit(AActor* HitActor, const FHitResult& HitResult);

	FDelegateHandle WeaponHitHandle;
};
