#pragma once

// Made by VoIPVR

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Online.h"
#include "VoIPVOIP.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VOIPAVATARS_API UVoIPVOIP : public UActorComponent
{
	GENERATED_BODY()

public:

	bool bTalkerRegistered;

	FString RegisteredTalkerOculusId;

public:
	// Sets default values for this component's properties
	UVoIPVOIP();

	UFUNCTION(BlueprintCallable, Category = "OculusVOIP")
		bool RegisterRemoteTalker(FString OculusID);

	UFUNCTION(BlueprintCallable, Category = "OculusVOIP")
		void UnregisterRemoteTalker();

protected:
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
};
