#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.generated.h"

class UGameAbilitySystemComponent;
class UGameAttributeSet;

UCLASS()
class BOSSONLY_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
protected:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UGameAbilitySystemComponent* WarriorAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UGameAttributeSet* WarriorAttributeSet;

public:
	FORCEINLINE UGameAbilitySystemComponent* GetGameAbilitySystemComponent() const { return WarriorAbilitySystemComponent; }

	FORCEINLINE UGameAttributeSet* GetGameAttributeSet() const { return WarriorAttributeSet; }
};
