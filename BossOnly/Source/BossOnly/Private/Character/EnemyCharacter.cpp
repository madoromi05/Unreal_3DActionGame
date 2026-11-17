// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "Controller/EnemyAIController.h"
#include "Items/Weapons/EnemyWeaponComponent.h"
#include "AbilitySystem/Abilities/GA_EnemyAttack.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstance.h"

AEnemyCharacter::AEnemyCharacter()
{
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// BP サブクラスを作らずレベルへ直接配置できるよう、見た目・武器・アビリティを C++ の既定値として持たせる。
	// 専用のボス素材が無い現段階では、プレースホルダーとして Player と同じ Assassin パックを流用する。
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshFinder(TEXT("/Game/Assassin/Mesh/SKM_Assassin_Skin1.SKM_Assassin_Skin1"));
	if (SkeletalMeshFinder.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshFinder.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprintFinder(TEXT("/Game/PlayerCharacter/AnimBP/ABP_Player.ABP_Player_C"));
	if (AnimBlueprintFinder.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBlueprintFinder.Class);
	}

	// Player の武器（ACharacterWeaponBase 派生）とは異なり、Actor を余分にスポーンしない Component 版を使う。
	WeaponComponentClass = UEnemyWeaponComponent::StaticClass();
	StartupAbilities.Add(UGA_EnemyAttack::StaticClass());
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponComponentClass)
	{
		EquippedWeaponComponent = NewObject<UWeaponComponentBase>(this, WeaponComponentClass);
		if (EquippedWeaponComponent)
		{
			EquippedWeaponComponent->RegisterComponent();

			FAttachmentTransformRules AttachRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				true);
			EquippedWeaponComponent->AttachToComponent(GetMesh(), AttachRules, WeaponSocketName);
		}
	}
}

IMeleeWeaponInterface* AEnemyCharacter::GetActiveMeleeWeapon() const
{
	return Cast<IMeleeWeaponInterface>(EquippedWeaponComponent);
}
