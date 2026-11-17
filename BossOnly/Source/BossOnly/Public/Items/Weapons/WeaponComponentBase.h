// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Items/Weapons/MeleeWeaponInterface.h"
#include "WeaponComponentBase.generated.h"

/**
 * ACharacterWeaponBase（Actor 版、既存 Player 武器）と対になる Component 版の近接武器。
 * 武器のためだけに専用 Actor をワールドへスポーンせず、キャラクターへ直接アタッチする
 * StaticMeshComponent 自身に当たり判定を持たせることで Actor 数を増やさない。
 * IMeleeWeaponInterface を実装しているため、攻撃側（GA_MeleeAttack）は Actor 版と同じ扱いができる。
 */
UCLASS(ClassGroup = (Weapon), meta = (BlueprintSpawnableComponent))
class BOSSONLY_API UWeaponComponentBase : public UStaticMeshComponent, public IMeleeWeaponInterface
{
	GENERATED_BODY()

public:
	UWeaponComponentBase();

	virtual void EnableCollision() override;
	virtual void DisableCollision() override;
	virtual FOnWeaponHitSignature& GetOnWeaponHit() override { return OnWeaponHit; }

	FOnWeaponHitSignature OnWeaponHit;

private:
	TSet<AActor*> HitActorsThisSwing;

	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
