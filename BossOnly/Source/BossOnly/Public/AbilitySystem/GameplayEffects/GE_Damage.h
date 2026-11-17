// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_Damage.generated.h"

/**
 * Health を即時に減少させる GameplayEffect。
 * ダメージ量は SetByCaller（GamePlayTags::Data_Damage）で GA 側から渡す。
 */
UCLASS()
class BOSSONLY_API UGE_Damage : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_Damage();
};
