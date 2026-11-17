// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WeaponComponentBase.h"
#include "EnemyWeaponComponent.generated.h"

/**
 * BP を作らなくても見た目が付くよう、既存の Assassin パックの剣メッシュを既定値として持つ。
 */
UCLASS(ClassGroup = (Weapon), meta = (BlueprintSpawnableComponent))
class BOSSONLY_API UEnemyWeaponComponent : public UWeaponComponentBase
{
	GENERATED_BODY()

public:
	UEnemyWeaponComponent();
};
