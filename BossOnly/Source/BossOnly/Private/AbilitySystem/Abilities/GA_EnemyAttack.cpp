// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_EnemyAttack.h"
#include "GamePlayerTag.h"

UGA_EnemyAttack::UGA_EnemyAttack()
{
	SetAssetTags(FGameplayTagContainer(GamePlayTags::Enemy_Ability_Attack_Melee));
}
