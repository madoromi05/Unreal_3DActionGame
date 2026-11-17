// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MeleeWeaponInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponHitSignature, AActor*, const FHitResult&);

UINTERFACE(MinimalAPI)
class UMeleeWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 「近接武器として振る舞える」能力を表すインターフェース。
 * Actor 実装（ACharacterWeaponBase、既存 Player 武器）と Component 実装（UWeaponComponentBase、
 * 新規 Enemy 武器）の両方をこれ一本で扱えるようにし、GA_MeleeAttack 側は実体の型を意識しなくてよい。
 */
class BOSSONLY_API IMeleeWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void EnableCollision() = 0;
	virtual void DisableCollision() = 0;
	virtual FOnWeaponHitSignature& GetOnWeaponHit() = 0;
};
