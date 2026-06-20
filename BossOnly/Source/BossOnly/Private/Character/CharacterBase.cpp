#include "Character/CharacterBase.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "Items/Weapons/CharacterWeaponBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	WarriorAbilitySystemComponent = CreateDefaultSubobject<UGameAbilitySystemComponent>(TEXT("WarriorAbilitySystemComponent"));
	WarriorAttributeSet = CreateDefaultSubobject<UGameAttributeSet>(TEXT("WarriorAttributeSet"));
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return GetGameAbilitySystemComponent();
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		EquippedWeapon = GetWorld()->SpawnActor<ACharacterWeaponBase>(WeaponClass, SpawnParams);

		if (EquippedWeapon)
		{
			FAttachmentTransformRules AttachRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				true);
			EquippedWeapon->AttachToComponent(GetMesh(), AttachRules, WeaponSocketName);
		}
	}
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (WarriorAbilitySystemComponent)
	{
		WarriorAbilitySystemComponent->InitAbilityActorInfo(this, this);

		for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupAbilities)
		{
			if (AbilityClass)
			{
				WarriorAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass));
			}
		}
	}
}