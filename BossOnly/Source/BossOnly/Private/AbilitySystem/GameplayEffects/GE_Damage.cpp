// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayEffects/GE_Damage.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "GamePlayerTag.h"

UGE_Damage::UGE_Damage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo DamageModifier;
	DamageModifier.Attribute = UGameAttributeSet::GetHealthAttribute();
	DamageModifier.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat SetByCallerMagnitude;
	SetByCallerMagnitude.DataTag = GamePlayTags::Data_Damage;
	DamageModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerMagnitude);

	Modifiers.Add(DamageModifier);
}
