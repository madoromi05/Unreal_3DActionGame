#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class BOSSONLY_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	FORCEINLINE bool GetIsDashing() const { return bIsDashing; }

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

private:

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

#pragma endregion

#pragma region Inputs

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_DashStart(const FInputActionValue& InputActionValue);
	void Input_DashEnd(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);
	void Input_Attack(const FInputActionValue& InputActionValue);
#pragma endregion

// --------------------------------------- 移動・ダッシュ関連 ---------------------------------------
#pragma region Movement
	// 通常時の最大移動速度（ダッシュ解除時に戻す値）
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float NormalWalkSpeed = 400.f;

	// ダッシュ（押しっぱなしで加速）中の最大移動速度
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashSpeed = 1200.f;

	bool bIsDashing = false;
#pragma endregion
};
