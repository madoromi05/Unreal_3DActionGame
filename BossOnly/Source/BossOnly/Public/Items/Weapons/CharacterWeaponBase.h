// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CharacterWeaponBase.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponHitSignature, AActor*, const FHitResult&);

UCLASS()
class BOSSONLY_API ACharacterWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ACharacterWeaponBase();

	void EnableCollision();
	void DisableCollision();

	FOnWeaponHitSignature OnWeaponHit;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UBoxComponent* WeaponCollisionBox;

private:
	TSet<AActor*> HitActorsThisSwing;

	UFUNCTION()
	void OnCollisionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox; }
};
