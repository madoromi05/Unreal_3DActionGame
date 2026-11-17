// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BossOnlyGameplayAbility.h"
#include "GA_MeleeAttack.generated.h"

class IMeleeWeaponInterface;
struct FHitResult;

/**
 * 装備武器の当たり判定を一定時間有効化し、ヒットした相手に AttackPower 分のダメージを与える
 * 近接攻撃アビリティ。Player・Enemy 双方の武器攻撃で共通利用する基底クラス。
 * 発動タグは派生クラス（GA_PlayerAttack / GA_EnemyAttack）のコンストラクタで設定する。
 */
UCLASS()
class BOSSONLY_API UGA_MeleeAttack : public UBossOnlyGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_MeleeAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// 武器コリジョンを有効にしておく時間（秒）。アニメーション Notify 連携に置き換わるまでの仮実装。
	UPROPERTY(EditDefaultsOnly, Category = "MeleeAttack")
	float AttackActiveDuration = 0.3f;

	void HandleWeaponHit(AActor* HitActor, const FHitResult& HitResult);

	UFUNCTION()
	void OnAttackWindowFinished();

private:
	IMeleeWeaponInterface* ActiveWeapon = nullptr;
	FDelegateHandle WeaponHitDelegateHandle;
};
