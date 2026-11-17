// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/EnemyWeaponComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

UEnemyWeaponComponent::UEnemyWeaponComponent()
{
	// 専用のボス武器素材が無い現段階では、プレースホルダーとして Assassin パックの剣メッシュを流用する。
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SwordMeshFinder(TEXT("/Game/Assassin/Mesh/Separated_Mesh/Weapon/SM_sword_Sword.SM_sword_Sword"));
	if (SwordMeshFinder.Succeeded())
	{
		SetStaticMesh(SwordMeshFinder.Object);
	}
}
