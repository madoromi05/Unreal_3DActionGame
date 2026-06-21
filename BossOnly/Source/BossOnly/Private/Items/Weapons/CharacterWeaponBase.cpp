// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/CharacterWeaponBase.h"
#include "Components/BoxComponent.h"

ACharacterWeaponBase::ACharacterWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACharacterWeaponBase::OnCollisionOverlap);
}

void ACharacterWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterWeaponBase::EnableCollision()
{
	HitActorsThisSwing.Empty();
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACharacterWeaponBase::DisableCollision()
{
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitActorsThisSwing.Empty();
}

void ACharacterWeaponBase::OnCollisionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || HitActorsThisSwing.Contains(OtherActor)) return;

	AActor* WeaponOwner = GetOwner();
	if (!WeaponOwner || OtherActor == WeaponOwner) return;

	// 同じキャラの他のコンポーネントを弾く
	if (OtherActor == WeaponOwner->GetOwner()) return;

	HitActorsThisSwing.Add(OtherActor);
	OnWeaponHit.Broadcast(OtherActor, SweepResult);
}
