// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyCharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h" 
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/BodyInstance.h"
#include "Enemy.h"
#include "Minion.h"
#include "BossCharacter.h"
#include "TPSCharacter.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESwitchModeEnum : uint8
{
	
	ES_Heavy 	UMETA(DisplayName = "Heavy"),
	ES_Light	UMETA(DisplayName = "Light")
};
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EPushbackTypeEnum : uint8
{

	EP_Short 	UMETA(DisplayName = "Short"),
	EP_Medium	UMETA(DisplayName = "Medium"),
	EP_Long	    UMETA(DisplayName = "Long"),
	
};

UCLASS()
class PARTICLETEST_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PushBack")
		float pushBackTimer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PushBack")
		FVector pushBackSourceLocation;
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "PushBack")
		void Pushback(float force, FVector pushSource, EPushbackTypeEnum pushbackType,bool isDamagingPlayer=false);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PushBack")
		void ArkThrow(float force, FVector pushSource, float arkParam);
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void BindInputs();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//

	// Sets default values for this character's properties
	ATPSCharacter(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		ESwitchModeEnum SwitchMode= ESwitchModeEnum::ES_Light;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
		ABossCharacter* bossActor;
	
	
	//UFUNCTION(BlueprintCallable, Category = "Something")
	//Refrence to main Character
	//Inputs
	UFUNCTION(BlueprintCallable,Category = "Player Movement")
	 void MoveForward(float value);
	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	 void MoveRight(float value);
	UPROPERTY(BlueprintReadWrite ,Category = "Player Movement")
		bool CanMove = true;
	UPROPERTY(BlueprintReadWrite, Category = "Player Movement")
		FVector targetForwardVector;
	UPROPERTY(BlueprintReadWrite, Category = "Player Movement")
		FVector targetRightVector;
	
	UPROPERTY(BlueprintReadWrite, Category = "Player Movement")
		FVector desiredDir;
	UFUNCTION(BlueprintCallable, Category = "Player Acceleration")
		void BeginEvadeAcceleration(FVector direction);
	UFUNCTION(BlueprintCallable, Category = "Player Acceleration")
		void BeginPushbackAcceleration(FVector direction);



	UPROPERTY(BlueprintReadWrite, Category = "Health Properties")
		float MaximumHealth;
	UPROPERTY(BlueprintReadWrite, Category = "Health Properties")
		float MaximumStamina;
	UPROPERTY(BlueprintReadWrite, Category = "Health Properties")
		float CurrentHealth;
	UPROPERTY(BlueprintReadWrite, Category = "Health Properties")
		float CurrentStamina;
	UPROPERTY(BlueprintReadWrite, Category = "Health Properties")
		float TargetStamina;


	
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		bool TargetLocked;	
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		bool ShouldRotateLockOn;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		UCameraComponent* camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On Properties")
		bool CanLockOnBoss = true;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		AEnemy* LockedTarget;



	FName enemyTag;
	
	

	UFUNCTION( BlueprintCallable, Category = "Melee Properties")
		void DoAttacks(bool DoCharge);
	UFUNCTION(BlueprintCallable, Category = "Melee Properties")
		void PlayerAttackedLight();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Melee Properties")
		void ResetCombo();
	UFUNCTION(BlueprintCallable, Category = "Melee Properties")
		void ContinueCombo();
	

	UFUNCTION(BlueprintCallable, Category = "Melee Properties")
		void SetCanContinueCombo(bool b);
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		int AttackCounts;
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		int HeavyChargeAttackCounterOrder;
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool inAttackAnimation;

	UPROPERTY(BlueprintReadWrite, Category = "Evade Properties")
		bool CanEvade = true;
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool CanAttackOnAir = true;

	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool CanContinueCombo = false;
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool PostCanContinueCombo = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool CanAttack = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation Montages")
		TArray<UAnimMontage*>montages;



	UFUNCTION(BlueprintImplementableEvent, Category = "Push Back Animation")
	void PushBackAnimation();
	/*UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void PushBack(float force, FVector pushLocation);*/
	ACharacter* GetGlobalPlayer();
	UPROPERTY(BlueprintReadWrite, Category = "PushBack")
	bool isPushingBack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade")
		bool isEvading = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PushBack")
	float pushBackSpeed = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Movement(FVector direction, float v);


	

	

};
