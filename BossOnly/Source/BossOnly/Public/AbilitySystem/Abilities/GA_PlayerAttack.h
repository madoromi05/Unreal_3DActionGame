// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GA_MeleeAttack.h"
#include "GA_PlayerAttack.generated.h"

/**
 * Player.Ability.Attack.Light タグで発動する Player 用近接攻撃。
 */
UCLASS()
class BOSSONLY_API UGA_PlayerAttack : public UGA_MeleeAttack
{
	GENERATED_BODY()

public:
	UGA_PlayerAttack();
};
