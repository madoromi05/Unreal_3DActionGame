#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DebugHelper.h"

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
	GetCharacterMovement()->MaxWalkSpeed = 400.f;						// キャラクターの移動速度
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;		// キャラクターの減速率
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Debug::Print(TEXT("Working"));
}
