// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"
#include "Character/EnemyCharacter.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "GamePlayerTag.h"
#include "Kismet/GameplayStatics.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 1vs1 前提のため索敵処理を行わず、Possess した時点でプレイヤーを直接ターゲットにする。
	TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	TimeSinceLastAttack = 0.f;

	GetWorldTimerManager().SetTimer(BehaviorTimerHandle, this, &AEnemyAIController::UpdateBehavior, BehaviorTickInterval, true);
}

void AEnemyAIController::OnUnPossess()
{
	GetWorldTimerManager().ClearTimer(BehaviorTimerHandle);

	Super::OnUnPossess();
}

void AEnemyAIController::UpdateBehavior()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetPawn());
	if (!EnemyCharacter || !TargetPawn || EnemyCharacter->IsDead())
	{
		StopMovement();
		GetWorldTimerManager().ClearTimer(BehaviorTimerHandle);
		return;
	}

	TimeSinceLastAttack += BehaviorTickInterval;

	const float DistanceToTarget = FVector::Dist(EnemyCharacter->GetActorLocation(), TargetPawn->GetActorLocation());

	if (DistanceToTarget > EnemyCharacter->AttackRange)
	{
		MoveToActor(TargetPawn, EnemyCharacter->AttackRange * 0.9f);
	}
	else
	{
		StopMovement();
		TryAttack(EnemyCharacter);
	}
}

void AEnemyAIController::TryAttack(AEnemyCharacter* EnemyCharacter)
{
	if (TimeSinceLastAttack < EnemyCharacter->AttackCooldown) return;

	if (UGameAbilitySystemComponent* ASC = EnemyCharacter->GetGameAbilitySystemComponent())
	{
		ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(GamePlayTags::Enemy_Ability_Attack_Melee));
	}

	TimeSinceLastAttack = 0.f;
}
