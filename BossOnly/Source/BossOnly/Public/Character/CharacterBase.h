#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterBase.generated.h"

class UGameAbilitySystemComponent;
class UGameAttributeSet;
class ACharacterWeaponBase;
class IMeleeWeaponInterface;

UCLASS()
class BOSSONLY_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	// 装備武器を Actor 版／Component 版どちらで持っていても同じ扱いができるようにするための入口。
	// 既定実装は既存の Actor 版（EquippedWeapon）を返す。Component 版を使う派生クラスは override する。
	virtual IMeleeWeaponInterface* GetActiveMeleeWeapon() const;

	FORCEINLINE bool IsDead() const { return bIsDead; }

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	// AttributeSet の初期値（HP・攻撃力・スタミナ）を設定する。GameplayEffect を介さないため、
	// ASC の InitAbilityActorInfo より前（BeginPlay 時点）でも呼び出せる。
	virtual void InitializeDefaultAttributes();

	// Health が 0 になったときに呼ばれる。派生クラスで見た目・挙動を拡張する想定。
	virtual void HandleDeath();

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

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float InitialMaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float InitialAttackPower = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float InitialMaxStamina = 100.f;

	ACharacterWeaponBase* EquippedWeapon = nullptr;

	bool bIsDead = false;

public:
	FORCEINLINE UGameAbilitySystemComponent* GetGameAbilitySystemComponent() const { return WarriorAbilitySystemComponent; }
	FORCEINLINE UGameAttributeSet* GetGameAttributeSet() const { return WarriorAttributeSet; }
	FORCEINLINE ACharacterWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; }
};
