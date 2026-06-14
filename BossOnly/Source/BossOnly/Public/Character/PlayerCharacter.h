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
#pragma endregion

// ---------------------------------------�_�b�V���֘A�̕ϐ����J---------------------------------------
#pragma region Dash
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashSpeed = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashDuration = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashCooldown = 1.0f;

	bool bIsDashing = false;
	bool bCanDash = true;
	bool bJustDashed = false;

	FTimerHandle DashTimerHandle;
	FTimerHandle DashCooldownTimerHandle;
	FTimerHandle DashJumpBlockTimerHandle;

	void OnDashEnd();
	void OnDashCooldownEnd();
#pragma endregion
};
