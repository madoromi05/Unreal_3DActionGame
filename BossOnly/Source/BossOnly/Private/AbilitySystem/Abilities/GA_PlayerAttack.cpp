// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_PlayerAttack.h"
#include "GamePlayerTag.h"

UGA_PlayerAttack::UGA_PlayerAttack()
{
	SetAssetTags(FGameplayTagContainer(GamePlayTags::Player_Ability_Attack_Light));
}
