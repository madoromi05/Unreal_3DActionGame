#include "Character/CharacterBase.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "AbilitySystem/GameAttributeSet.h"

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

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (WarriorAbilitySystemComponent)
	{
		WarriorAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}