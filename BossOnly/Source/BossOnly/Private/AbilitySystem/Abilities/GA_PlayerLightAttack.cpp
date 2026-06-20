#include "AbilitySystem/Abilities/GA_PlayerLightAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/CharacterBase.h"
#include "Items/Weapons/CharacterWeaponBase.h"
#include "GameFramework/Character.h"

UGA_PlayerLightAttack::UGA_PlayerLightAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_PlayerLightAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());
	ACharacterWeaponBase* Weapon = Character ? Character->GetEquippedWeapon() : nullptr;

	if (Weapon)
	{
		WeaponHitHandle = Weapon->OnWeaponHit.AddUObject(this, &UGA_PlayerLightAttack::OnWeaponHit);
		Weapon->EnableCollision();
	}

	if (AttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AttackMontage);

		MontageTask->OnCompleted.AddDynamic(this, &UGA_PlayerLightAttack::OnMontageCompleted);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_PlayerLightAttack::OnMontageCancelled);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_PlayerLightAttack::OnMontageCancelled);
		MontageTask->ReadyForActivation();
	}
	else
	{
		FinishAttack(false);
	}
}

void UGA_PlayerLightAttack::OnMontageCompleted()
{
	FinishAttack(false);
}

void UGA_PlayerLightAttack::OnMontageCancelled()
{
	FinishAttack(true);
}

void UGA_PlayerLightAttack::FinishAttack(bool bWasCancelled)
{
	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());
	ACharacterWeaponBase* Weapon = Character ? Character->GetEquippedWeapon() : nullptr;

	if (Weapon)
	{
		Weapon->DisableCollision();
		if (WeaponHitHandle.IsValid())
		{
			Weapon->OnWeaponHit.Remove(WeaponHitHandle);
			WeaponHitHandle.Reset();
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, bWasCancelled);
}

void UGA_PlayerLightAttack::OnWeaponHit(AActor* HitActor, const FHitResult& HitResult)
{
	if (!HitActor) return;

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor || HitActor == AvatarActor) return;

	AController* Instigator = nullptr;
	if (ACharacter* OwnerChar = Cast<ACharacter>(AvatarActor))
	{
		Instigator = OwnerChar->GetController();
	}

	FPointDamageEvent DamageEvent(DamageAmount, HitResult, -HitResult.ImpactNormal, nullptr);
	HitActor->TakeDamage(DamageAmount, DamageEvent, Instigator, AvatarActor);
}
