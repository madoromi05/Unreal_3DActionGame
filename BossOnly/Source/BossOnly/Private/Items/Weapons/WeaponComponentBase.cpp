// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WeaponComponentBase.h"

UWeaponComponentBase::UWeaponComponentBase()
{
	// 攻撃していない間は判定を持たない。当たり判定用チャンネルは Pawn とのみ Overlap する。
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetGenerateOverlapEvents(true);

	OnComponentBeginOverlap.AddDynamic(this, &UWeaponComponentBase::OnWeaponOverlap);
}

void UWeaponComponentBase::EnableCollision()
{
	HitActorsThisSwing.Empty();
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UWeaponComponentBase::DisableCollision()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitActorsThisSwing.Empty();
}

void UWeaponComponentBase::OnWeaponOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || HitActorsThisSwing.Contains(OtherActor)) return;

	AActor* MyOwner = GetOwner();
	if (!MyOwner || OtherActor == MyOwner) return;

	HitActorsThisSwing.Add(OtherActor);
	OnWeaponHit.Broadcast(OtherActor, SweepResult);
}
