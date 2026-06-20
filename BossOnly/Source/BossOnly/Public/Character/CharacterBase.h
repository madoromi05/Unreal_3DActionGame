#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterBase.generated.h"

class UGameAbilitySystemComponent;
class UGameAttributeSet;
class ACharacterWeaponBase;

UCLASS()
class BOSSONLY_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UGameAbilitySystemComponent* WarriorAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UGameAttributeSet* WarriorAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ACharacterWeaponBase> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocketName = TEXT("hand_r");

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	ACharacterWeaponBase* EquippedWeapon = nullptr;

public:
	FORCEINLINE UGameAbilitySystemComponent* GetGameAbilitySystemComponent() const { return WarriorAbilitySystemComponent; }
	FORCEINLINE UGameAttributeSet* GetGameAttributeSet() const { return WarriorAttributeSet; }
	FORCEINLINE ACharacterWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; }
};
