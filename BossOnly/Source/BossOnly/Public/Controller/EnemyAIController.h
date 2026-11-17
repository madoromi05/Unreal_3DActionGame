// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class AEnemyCharacter;

/**
 * 1vs1 専用の敵 AI。索敵は行わず、Possess した時点で唯一のプレイヤーを常にターゲットとして
 * 認識し、間合いに応じて接近・攻撃を切り替えるだけの単純なステートレス FSM。
 */
UCLASS()
class BOSSONLY_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	void UpdateBehavior();
	void TryAttack(AEnemyCharacter* EnemyCharacter);

	UPROPERTY()
	APawn* TargetPawn = nullptr;

	FTimerHandle BehaviorTimerHandle;
	float TimeSinceLastAttack = 0.f;

	// 意思決定の周期（秒）。毎フレームではなく間引いて判定する。
	static constexpr float BehaviorTickInterval = 0.2f;
};
