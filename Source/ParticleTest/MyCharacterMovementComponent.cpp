// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/Classes/Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Curves/CurveFloat.h"
#include "TPSCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Classes/GameFramework/Controller.h"



UMyCharacterMovementComponent::UMyCharacterMovementComponent()
{
	
}




#pragma region Movement Mode Implementations

void UMyCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	
	


	if (CustomMovementMode == ECustomMovementMode::CMOVE_JETPACK)
	{
		PhysJetpack(deltaTime, Iterations);
	}
	if (CustomMovementMode == ECustomMovementMode::CMOVE_GLIDE)
	{
		PhysGlide(deltaTime, Iterations);
	}
	if (CustomMovementMode == ECustomMovementMode::CMOVE_SPRINT)
	{
		PhysSprint(deltaTime, Iterations);
	}
	if (CustomMovementMode == ECustomMovementMode::CMOVE_EVADE)
	{
		PhysEvade(deltaTime, Iterations);
	}
	Super::PhysCustom(deltaTime, Iterations);
}

void UMyCharacterMovementComponent::PhysSprint(float deltaTime, int32 Iterations)
{

}

void UMyCharacterMovementComponent::PhysJetpack(float deltaTime, int32 Iterations)
{
	
}

void UMyCharacterMovementComponent::PhysGlide(float deltaTime, int32 Iterations)
{
	
}

void UMyCharacterMovementComponent::PhysEvade(float deltaTime, int32 Iterations)
{

	if (!IsCustomMovementMode(ECustomMovementMode::CMOVE_EVADE))
	{
		SetEvading(false);
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	if (!bWantsToEvade)
	{
		SetEvading(false);
		SetMovementMode(EMovementMode::MOVE_Walking);
		StartNewPhysics(deltaTime, Iterations);
		return;
	}
	
	

	
	
	
}

#pragma endregion

#pragma region Overrides

void UMyCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//recharge jetpack resource
	if (MovementMode != EMovementMode::MOVE_Custom || CustomMovementMode != ECustomMovementMode::CMOVE_JETPACK)
	{
		//fJetpackResource = FMath::Clamp<float>(fJetpackResource + (DeltaTime / JetpackFullRechargeSeconds), 0, 1);
	}


}


void UMyCharacterMovementComponent::ClearAccelerators()
{
	beginingEvadeAccelerationDirection = FVector::ZeroVector;
}

void UMyCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector & OldLocation, const FVector & OldVelocity)
{

	ATPSCharacter* player = Cast<ATPSCharacter>(GetOwner());
	//bool isOnAir = player->GetCharacterMovement()->IsFalling();
	
	//bool startedWalking = player->CanMove && ((player->GetLastMovementInputVector().Size() > 0) || player->inAttackAnimation) && !isOnAir;

	//float Speed = UKismetMathLibrary::SelectFloat(37.0f, 36.5f, startedWalking);
	//FVectorSpringState state;

	//beginingEvadeAccelerationDirection = UKismetMathLibrary::VectorSpringInterp(beginingEvadeAccelerationDirection, FVector::ZeroVector, state, 2.0f, 0.9f, DeltaSeconds*Speed, 60.0f);
	//beginingEvadeAccelerationDirection = UKismetMathLibrary::VLerp(beginingEvadeAccelerationDirection, FVector::ZeroVector, Speed* DeltaSeconds);
	
		Velocity = Velocity + beginingEvadeAccelerationDirection;
		beginingEvadeAccelerationDirection = FVector::ZeroVector;
	
	
	
	//beginingPushAccelerationDirection = FVector::ZeroVector;
	/////////

	if (bWantsToSprint)
	{
		if (CanSprint())
		{
			SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_SPRINT);
		}
		/*else if (!IsCustomMovementMode(ECustomMovementMode::CMOVE_SPRINT))
		{
			SetSprinting(false);
		}*/
	}

	if (bWantsToJetpack)
	{
		if (CanJetpack())
		{
			SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_JETPACK);
		}
		/*else if (!IsCustomMovementMode(ECustomMovementMode::CMOVE_JETPACK))
		{
			SetJetpacking(0);
		}*/
	}

	if (bWantsToGlide)
	{
		if (CanGlide())
		{
			SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_GLIDE);
		}
		else if (!IsCustomMovementMode(ECustomMovementMode::CMOVE_GLIDE))
		{
			SetGliding(false);
		}
	}
	if (bWantsToEvade)
	{
		if (CanEvade())
		{
			SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_EVADE);
		}
		else if (!IsCustomMovementMode(ECustomMovementMode::CMOVE_EVADE))
		{
			SetEvading(false);
		}
		
	}

	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void UMyCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	bool suppressSuperNotification = false;

	if (PreviousMovementMode == MovementMode && PreviousCustomMode == CustomMovementMode)
	{
		Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
		return;
	}

#pragma region Leaving State Handlers

	if (PreviousMovementMode == EMovementMode::MOVE_Custom && PreviousCustomMode == ECustomMovementMode::CMOVE_SPRINT)
	{
		//SetSprinting(false);
		//MaxWalkSpeed /= SprintSpeedMultiplier;
	}
	if (PreviousMovementMode == EMovementMode::MOVE_Custom && PreviousCustomMode == ECustomMovementMode::CMOVE_JETPACK)
	{
		//SetJetpacking(0);
		//fEffectiveThrottle = 0;
	}
	if (PreviousMovementMode == EMovementMode::MOVE_Custom && PreviousCustomMode == ECustomMovementMode::CMOVE_GLIDE)
	{
		
	}
	if (PreviousMovementMode == EMovementMode::MOVE_Custom && PreviousCustomMode == ECustomMovementMode::CMOVE_EVADE)
	{

	}
#pragma endregion

#pragma region Entering State Handlers

	if (IsCustomMovementMode(ECustomMovementMode::CMOVE_SPRINT))
	{
		////MaxWalkSpeed *= SprintSpeedMultiplier;
		//suppressSuperNotification = true;
	}
	if (IsCustomMovementMode(ECustomMovementMode::CMOVE_JETPACK))
	{

	}
	if (IsCustomMovementMode(ECustomMovementMode::CMOVE_GLIDE))
	{
		
	}
	if (IsCustomMovementMode(ECustomMovementMode::CMOVE_EVADE))
	{

	}

#pragma endregion
	if (!suppressSuperNotification)
		Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	else
		CharacterOwner->OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

#pragma endregion

#pragma region Helpers

bool UMyCharacterMovementComponent::IsCustomMovementMode(uint8 cm) const
{
	if (MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == cm)
		return true;
	return false;
}

#pragma endregion

#pragma region State Queries

bool UMyCharacterMovementComponent::IsSprinting()
{
	return IsCustomMovementMode(ECustomMovementMode::CMOVE_SPRINT);
}

bool UMyCharacterMovementComponent::IsJetpacking()
{
	return IsCustomMovementMode(ECustomMovementMode::CMOVE_JETPACK);
}

bool UMyCharacterMovementComponent::IsGliding()
{
	return IsCustomMovementMode(ECustomMovementMode::CMOVE_GLIDE);
}
bool UMyCharacterMovementComponent::IsEvading()
{
	return IsCustomMovementMode(ECustomMovementMode::CMOVE_EVADE);
}

#pragma endregion

#pragma region State Conditions

bool UMyCharacterMovementComponent::CanJetpack()
{

	return true;
}

bool UMyCharacterMovementComponent::CanGlide()
{
	return true;
}

bool UMyCharacterMovementComponent::CanSprint()
{
	return Super::IsMovingOnGround();
}

bool UMyCharacterMovementComponent::CanEvade()
{
	//return Super::IsMovingOnGround();
	return true;
}

#pragma endregion

#pragma region State Setters

void UMyCharacterMovementComponent::SetJetpacking(float throttle)
{
	throttle = FMath::Clamp<float>(throttle, 0.0, 1.0);
	bWantsToJetpack = throttle > 0;
	//fDesiredThrottle = throttle;
}

void UMyCharacterMovementComponent::SetGliding(bool wantsToGlide)
{
	if (bWantsToGlide != wantsToGlide)
	{
		//execSetGliding(wantsToGlide);
		bWantsToGlide = wantsToGlide;
	}
}

void UMyCharacterMovementComponent::BeginEvadeAccelerate(FVector direction)
{
	beginingEvadeAccelerationDirection = direction;
}

void UMyCharacterMovementComponent::SetSprinting(bool wantsToSprint)
{

	if (bWantsToSprint != wantsToSprint)
	{
		//execSetSprinting(wantsToSprint);
		bWantsToSprint = wantsToSprint;

	}
}
void UMyCharacterMovementComponent::SetEvading(bool wantsToEvade)
{
	beginingEvadeAccelerationDirection = FVector::ForwardVector*-90000.0f;
	if (bWantsToEvade != wantsToEvade)
	{
		
		//execSetSprinting(wantsToSprint);
		bWantsToEvade = wantsToEvade;

	}
}



