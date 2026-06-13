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
	bUseControllerRotationYaw = false;		// 左右回転
	bUseControllerRotationRoll = false;		// ロール回転

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());

	CameraBoom->TargetArmLength = 350.f;					// キャラクターからカメラまでの基本距離
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 90.f);		// カメラの基準点のオフセット
	CameraBoom->bUsePawnControlRotation = true;				// コントローラーの回転に合わせてカメラも回転させる

	CameraBoom->bEnableCameraLag = true;					// カメラの移動を遅らせるか
	CameraBoom->CameraLagSpeed = 10.f;						// カメラの移動の遅れ具合
	CameraBoom->bEnableCameraRotationLag = true;			// カメラの回転を遅らせるか
	CameraBoom->CameraRotationLagSpeed = 15.f;				// カメラの回転の遅れ具合

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;			// キャラクターが移動する方向に向かって自動的に回転するか
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);	// キャラクターの回転速度
	GetCharacterMovement()->MaxWalkSpeed = 400.f;			 			// キャラクターの移動速度
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;		// キャラクターの減速率
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	ULocalPlayer* localPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UPlayerInputComponent* playerInputComponent = CastChecked<UPlayerInputComponent>(PlayerInputComponent);

	playerInputComponent->BindNativeInputAction(InputConfigDataAsset, GamePlayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	playerInputComponent->BindNativeInputAction(InputConfigDataAsset, GamePlayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
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
		AddControllerPitchInput(LookAxisVector.Y);
	}
}