#include "DataAssets/Input/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FWarriorInputActionConfig& InputActionConfig : NativeInputActions)
	{
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			return InputActionConfig.InputAction;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[InputConfig] FindNativeInputActionByTag: タグ '%s' に対応するActionが見つかりません"), *InInputTag.ToString());
	return nullptr;
}