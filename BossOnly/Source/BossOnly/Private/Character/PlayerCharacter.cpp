#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/PlayerInputComponent.h"
#include "GamePlayerTag.h"
#include "DebugHelper.h"

//Cameraの位置と動き
APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 100.f);

	// コントローラーの回転（カメラの向き）に合わせてキャラクター自身を回転させるか
	bUseControllerRotationPitch = false;	// 上下回転
	bUseControllerRotationYaw	= false;	// 左右回転
	bUseControllerRotationRoll	= false;	// ロール回転

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());

	CameraBoom->TargetArmLength = 250.f;					// キャラクターからカメラまでの基本距離
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 90.f);		// カメラの基準点のオフセット
	CameraBoom->bUsePawnControlRotation = true;				// コントローラーの回転に合わせてカメラも回転させる

	CameraBoom->bEnableCameraLag = false;					// カメラの移動を遅らせるか
	CameraBoom->bEnableCameraRotationLag = false;			// カメラの回転を遅らせるか

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;			// キャラクターが移動する方向に向かって自動的に回転するか
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);	// キャラクターの回転速度
	GetCharacterMovement()->MaxWalkSpeed = 400.f;			 			// キャラクターの移動速度
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;		// キャラクターの減速率
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (!InputConfigDataAsset)
	{
		Debug::Print(TEXT("[Input] ERROR: InputConfigDataAsset is NULL! Blueprintで設定してください"), FColor::Red, 1);
		checkf(false, TEXT("Forgot to assign a valid data asset as input config"));
		return;
	}

	if (!InputConfigDataAsset->DefaultMappingContext)
	{
		Debug::Print(TEXT("[Input] ERROR: DefaultMappingContext is NULL!"), FColor::Red, 2);
	}

	ULocalPlayer* localPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);

	if (!Subsystem)
	{
		Debug::Print(TEXT("[Input] ERROR: EnhancedInputSubsystem is NULL!"), FColor::Red, 3);
		check(false);
		return;
	}

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UPlayerInputComponent* playerInputComponent = CastChecked<UPlayerInputComponent>(PlayerInputComponent);

	// Debug::Print(FString::Printf(TEXT("[Input] NativeInputActions count: %d"), InputConfigDataAsset->NativeInputActions.Num()), FColor::Yellow, 4);
	for (int32 i = 0; i < InputConfigDataAsset->NativeInputActions.Num(); i++)
	{
		const auto& Entry = InputConfigDataAsset->NativeInputActions[i];
		Debug::Print(FString::Printf(TEXT("[Input] Action[%d] Tag=%s, Action=%s"),
			i,
			*Entry.InputTag.ToString(),
			Entry.InputAction ? *Entry.InputAction->GetName() : TEXT("NULL")),
			FColor::Yellow, 10 + i);
	}

	const FGameplayTag MoveTag = FGameplayTag::RequestGameplayTag(FName("InputTag.Move"));
	const FGameplayTag LookTag = FGameplayTag::RequestGameplayTag(FName("InputTag.Look"));
	const FGameplayTag DashTag = FGameplayTag::RequestGameplayTag(FName("InputTag.Dash"));

	playerInputComponent->BindNativeInputAction(InputConfigDataAsset, MoveTag, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	playerInputComponent->BindNativeInputAction(InputConfigDataAsset, LookTag, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	playerInputComponent->BindNativeInputAction(InputConfigDataAsset, DashTag, ETriggerEvent::Started, this, &ThisClass::Input_DashStart);
	playerInputComponent->BindNativeInputAction(InputConfigDataAsset, DashTag, ETriggerEvent::Completed, this, &ThisClass::Input_DashEnd);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Debug::Print(TEXT("Working"));
}

void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void APlayerCharacter::Input_DashStart(const FInputActionValue& InputActionValue)
{
	bIsDashing = true;
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
	Debug::Print(TEXT("DashStart"), FColor::Green, 20);
}

void APlayerCharacter::Input_DashEnd(const FInputActionValue& InputActionValue)
{
	bIsDashing = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	Debug::Print(TEXT("DashEnd"), FColor::Red, 21);
}

void APlayerCharacter::OnDashEnd()
{
	bIsDashing = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, this, &ThisClass::OnDashCooldownEnd, DashCooldown, false);
}

void APlayerCharacter::OnDashCooldownEnd()
{
	bCanDash = true;
}