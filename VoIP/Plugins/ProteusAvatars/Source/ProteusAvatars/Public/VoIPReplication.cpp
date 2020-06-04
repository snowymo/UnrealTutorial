
#include "VoIPReplication.h"
#include "UnrealNetwork.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h"

UVoIPReplication::UVoIPReplication()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	//This is the tick group to use if your actor is intended to interact with physics objects, including physics-based attachments.
	//This way, the actor's movement is complete and can be factored into physics simulation.
	//Physics simulation data during this tick will be one frame old - i.e.the data that was rendered to the screen last frame.
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;
	bAutoActivate = false;

	bReplicates = true;
	bIsRemote = false;
}

void UVoIPReplication::BeginPlay()
{
	Super::BeginPlay();

	//Pointer to the Owning Pawn
	OwningPawn = Cast<APawn>(GetOwner());

	//Is remote only NOT when the Owning Pawn only the server replicates actors to connected clients (clients will never replicate actors to the server)
	bIsRemote = OwningPawn->Role != ROLE_Authority;
}

//Will enable component tick when head and 2 hands will be registered
void UVoIPReplication::SetComponentTickEnabled(bool bTickEnabled)
{
	if (!bTickEnabled || bHasRegisteredBody)
	{
		Super::SetComponentTickEnabled(bTickEnabled);
	}
}

void UVoIPReplication::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsAuthoritative())
	{
		SendTransformsToServer();
	}
	else
	{
		LerpReplicatedComponents(DeltaTime);
	}
}

bool UVoIPReplication::IsAuthoritative() const
{
	return OwningPawn->IsLocallyControlled();
}

//Called when RegisterVoIPForReplication fires
void UVoIPReplication::Activate(bool bReset)
{
	//The activation will happen even if ShouldActivate returns fals
	Super::Activate(bReset);
}

//CALLED IN BP
bool UVoIPReplication::RegisterVoIPForReplication(USceneComponent* Head, USceneComponent* LeftHand, USceneComponent* RightHand)
{
	if (Head == nullptr || LeftHand == nullptr || RightHand == nullptr)
	{
		return false;
	}

	HeadComponent = Head;
	LeftHandComponent = LeftHand;
	RightHandComponent = RightHand;

	bHasRegisteredBody = true;
	Activate(true);

	return true;
}

//TICK
//Only for locally controlled pawns, on every tick
void UVoIPReplication::SendTransformsToServer()
{
	//Define relative transforms for head and 2 hands
	ReplicatedVRBodyTransforms.HeadTransform = HeadComponent->GetRelativeTransform();
	ReplicatedVRBodyTransforms.LeftHandTransform = LeftHandComponent->GetRelativeTransform();
	ReplicatedVRBodyTransforms.RightHandTransform = RightHandComponent->GetRelativeTransform();

	//If the locally controlled pawns is remote, it will send to server the relative transforms data
	if (bIsRemote)
	{
		Server_UpdateReplicationData(ReplicatedVRBodyTransforms);
		//NO DEFINITION
	}
}

//Called from SendTransformsToServer
bool UVoIPReplication::Server_UpdateReplicationData_Validate(const FVoIPReplicationData& UpdatedData)
{
	return true;
}

void UVoIPReplication::Server_UpdateReplicationData_Implementation(const FVoIPReplicationData& UpdatedData)
{
	ReplicatedVRBodyTransforms = UpdatedData;
}

//TICK
//Linearly interpolates between the simulated transforms and the server transforms based on Alpha (100% of A when Alpha=0 and 100% of B when Alpha=1)
//Alpha = rate per second, = 10*DeltaTime
void UVoIPReplication::LerpReplicatedComponents(float DeltaTime)
{
	const float Alpha = 10.f * DeltaTime;
	LerpReplicatedComponent(HeadComponent, ReplicatedVRBodyTransforms.HeadTransform, DeltaTime, Alpha);
	LerpReplicatedComponent(LeftHandComponent, ReplicatedVRBodyTransforms.LeftHandTransform, DeltaTime, Alpha);
	LerpReplicatedComponent(RightHandComponent, ReplicatedVRBodyTransforms.RightHandTransform, DeltaTime, Alpha);
}

//Called from LerpReplicatedComponents
void UVoIPReplication::LerpReplicatedComponent(USceneComponent* Component, const FTransform_NetQuantize100& TargetTransform, float DeltaTime, float LerpAlpha)
{
	const FTransform_NetQuantize100& OldTransform = Component->GetRelativeTransform();
	const FTransform_NetQuantize100& NewTransform = LerpTransform(OldTransform, TargetTransform, DeltaTime, LerpAlpha);
	Component->SetRelativeLocationAndRotation(NewTransform.Location, NewTransform.Rotation);
}

//Called from LerpReplicatedComponent
// The constant interpolation rate to the authority location, helps keep small movements exact
// We const interp from the simulated transforms to the server transforms at this rate per second
FTransform_NetQuantize100 UVoIPReplication::LerpTransform(const FTransform_NetQuantize100& Start, const FTransform_NetQuantize100& End, float DeltaTime, float Alpha) const
{
	// Mixes both regular and constant rate interpolation for smooth updates during fast hand movement and accuracy in small movements
	const FVector FirstLerpLocation = FMath::Lerp(Start.Location, End.Location, Alpha);
	const FVector FinalLerpLocation = FMath::VInterpConstantTo(FirstLerpLocation, End.Location, DeltaTime, 100.f);

	const FQuat StartQuat(Start.Rotation);
	const FQuat EndQuat(End.Rotation);
	const FQuat LerpRotation = FQuat::FastLerp(StartQuat, EndQuat, Alpha + .1f);

	return FTransform_NetQuantize100(FinalLerpLocation, LerpRotation.Rotator());
}

void UVoIPReplication::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UVoIPReplication, ReplicatedVRBodyTransforms, COND_SkipOwner);
}

void UVoIPReplication::OnRep_ReplicatedTransforms()
{
	return;
}