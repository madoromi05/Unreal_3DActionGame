// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameAttributeSet.generated.h"

/**
 * Player・Enemy 共通のステータス（HP・攻撃力・スタミナ）を保持する AttributeSet。
 * このプロジェクトはオフライン 1vs1 専用でレプリケーションを必要としないため、
 * OnRep 通知は実装せず PostGameplayEffectExecute でのクランプ・死亡通知のみ行う。
 */
UCLASS()
class BOSSONLY_API UGameAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UGameAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UGameAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Combat")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS_BASIC(UGameAttributeSet, AttackPower)

	// スタミナゲージ用。現時点では消費処理は実装せず、値の保持と外部公開のみ行う。
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS_BASIC(UGameAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Stamina")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS_BASIC(UGameAttributeSet, MaxStamina)

	// Health が 0 になった瞬間に通知する。所有 Actor の死亡処理はこれを購読して行う。
	FSimpleMulticastDelegate OnHealthZero;

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
