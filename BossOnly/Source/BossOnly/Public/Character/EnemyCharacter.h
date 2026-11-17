// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "EnemyCharacter.generated.h"

class UWeaponComponentBase;

/**
 * 1vs1 のボス/敵キャラクター。AIController（AEnemyAIController）が戦闘の意思決定を行い、
 * このクラスは AI がテンプル参照する戦闘用の数値（間合い・攻撃間隔）を保持するだけに留める。
 * 武器は Player の Actor 版とは異なり、Actor を余分にスポーンしない Component 版を使う。
 */
UCLASS()
class BOSSONLY_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual IMeleeWeaponInterface* GetActiveMeleeWeapon() const override;

	// この距離まで近づいたら攻撃を試みる（cm）。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Combat")
	float AttackRange = 200.f;

	// 攻撃後、次の攻撃を試みるまでのクールダウン（秒）。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Combat")
	float AttackCooldown = 2.f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UWeaponComponentBase> WeaponComponentClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UWeaponComponentBase* EquippedWeaponComponent = nullptr;
};
