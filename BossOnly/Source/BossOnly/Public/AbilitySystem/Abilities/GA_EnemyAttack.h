// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GA_MeleeAttack.h"
#include "GA_EnemyAttack.generated.h"

/**
 * Enemy.Ability.Attack.Melee タグで発動する Enemy 用近接攻撃。EnemyAIController から発動される。
 */
UCLASS()
class BOSSONLY_API UGA_EnemyAttack : public UGA_MeleeAttack
{
	GENERATED_BODY()

public:
	UGA_EnemyAttack();
};
