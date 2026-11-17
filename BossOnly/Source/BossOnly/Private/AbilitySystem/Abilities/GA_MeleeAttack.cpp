// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_MeleeAttack.h"
#include "AbilitySystem/GameplayEffects/GE_Damage.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/CharacterBase.h"
#include "Items/Weapons/MeleeWeaponInterface.h"
#include "GamePlayerTag.h"

UGA_MeleeAttack::UGA_MeleeAttack()
{
	// ActiveWeapon / 委譲ハンドルをインスタンスごとに保持するため、単一 Actor 単位でインスタンス化する。
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacterBase* AttackingCharacter = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());
	ActiveWeapon = AttackingCharacter ? AttackingCharacter->GetActiveMeleeWeapon() : nullptr;

	if (!ActiveWeapon || !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WeaponHitDelegateHandle = ActiveWeapon->GetOnWeaponHit().AddUObject(this, &UGA_MeleeAttack::HandleWeaponHit);
	ActiveWeapon->EnableCollision();

	if (UAbilityTask_WaitDelay* WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, AttackActiveDuration))
	{
		WaitDelayTask->OnFinish.AddDynamic(this, &UGA_MeleeAttack::OnAttackWindowFinished);
		WaitDelayTask->ReadyForActivation();
	}
}

void UGA_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ActiveWeapon)
	{
		ActiveWeapon->DisableCollision();
		ActiveWeapon->GetOnWeaponHit().Remove(WeaponHitDelegateHandle);
		ActiveWeapon = nullptr;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_MeleeAttack::HandleWeaponHit(AActor* HitActor, const FHitResult& HitResult)
{
	ACharacterBase* AttackingCharacter = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!AttackingCharacter || !AttackingCharacter->GetGameAttributeSet() || !SourceASC) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
	if (!TargetASC) return;

	const float DamageAmount = AttackingCharacter->GetGameAttributeSet()->GetAttackPower();

	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(AttackingCharacter);

	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(UGE_Damage::StaticClass(), GetAbilityLevel(), ContextHandle);
	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(GamePlayTags::Data_Damage, -DamageAmount);
		SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	}
}

void UGA_MeleeAttack::OnAttackWindowFinished()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
