// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "MyCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_JETPACK = 1,
	CMOVE_GLIDE = 2,
	CMOVE_SPRINT = 3,
	CMOVE_EVADE = 4
};
/**
 * 
 */
UCLASS()
class PARTICLETEST_API UMyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

		UMyCharacterMovementComponent();
	

#pragma region Overrides

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector & OldLocation, const FVector & OldVelocity) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	//virtual float GetMaxSpeed() const override;
	//virtual float GetMaxAcceleration() const override;
	//virtual bool IsFalling() const override;
	//virtual bool IsMovingOnGround() const override;
#pragma endregion	

#pragma region Helpers
	bool IsCustomMovementMode(uint8 cm) const;
#pragma endregion

#pragma region Movement Mode Implementations

	void PhysSprint(float deltaTime, int32 Iterations);
	void PhysJetpack(float deltaTime, int32 Iterations);
	void PhysGlide(float deltaTime, int32 Iterations);
	void PhysEvade(float deltaTime, int32 Iterations);
#pragma endregion	



public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		TEnumAsByte<ECustomMovementMode> ECustomMovementMode;

	
		

#pragma region State Variables

	//ATPSCharacter* player;
	
	bool bWantsToGlide : 1;
	bool bWantsToSprint : 1;
	bool bWantsToJetpack : 1;
	bool bWantsToEvade : 1;
	

#pragma endregion


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector beginingEvadeAccelerationDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector beginingPushAccelerationDirection;
	UFUNCTION(BlueprintCallable)
		void BeginEvadeAccelerate(FVector direction);
	UFUNCTION(BlueprintCallable)
		void ClearAccelerators();
		
#pragma region State Setters

	UFUNCTION(BlueprintCallable)
		void SetSprinting(bool wantsToSprint);
	UFUNCTION(BlueprintCallable)
		void SetJetpacking(float throttle);
	UFUNCTION(BlueprintCallable)
		void SetGliding(bool wantsToGlide);
	UFUNCTION(BlueprintCallable)
		void SetEvading(bool wantsToEvade);
#pragma endregion
#pragma region State Queries
	UFUNCTION(BlueprintCallable)
		bool IsSprinting();
	UFUNCTION(BlueprintCallable)
		bool IsJetpacking();
	UFUNCTION(BlueprintCallable)
		bool IsGliding();
	UFUNCTION(BlueprintCallable)
		bool IsEvading();
#pragma endregion

#pragma region State Conditions

	bool CanJetpack();
	bool CanGlide();
	bool CanSprint();
	bool CanEvade();


#pragma endregion

};
