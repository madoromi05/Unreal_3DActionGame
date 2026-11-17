#include "Character/CharacterBase.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "Items/Weapons/CharacterWeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

IMeleeWeaponInterface* ACharacterBase::GetActiveMeleeWeapon() const
{
	return Cast<IMeleeWeaponInterface>(EquippedWeapon);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeDefaultAttributes();

	if (WarriorAttributeSet)
	{
		WarriorAttributeSet->OnHealthZero.AddUObject(this, &ACharacterBase::HandleDeath);
	}

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

void ACharacterBase::InitializeDefaultAttributes()
{
	if (!WarriorAttributeSet) return;

	WarriorAttributeSet->InitMaxHealth(InitialMaxHealth);
	WarriorAttributeSet->InitHealth(InitialMaxHealth);
	WarriorAttributeSet->InitAttackPower(InitialAttackPower);
	WarriorAttributeSet->InitMaxStamina(InitialMaxStamina);
	WarriorAttributeSet->InitStamina(InitialMaxStamina);
}

void ACharacterBase::HandleDeath()
{
	if (bIsDead) return;
	bIsDead = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	if (WarriorAbilitySystemComponent)
	{
		WarriorAbilitySystemComponent->CancelAllAbilities();
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