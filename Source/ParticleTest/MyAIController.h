// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Yari.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyAIController.generated.h"

UCLASS()
class PARTICLETEST_API AMyAIController : public AAIController
{
	GENERATED_BODY()

	private:
	
		APawn* bossPawn;
		FVector playerLastPos;
		float bossLastDistance;
		FVector playerForcePushLocation;	

		float bloodPoolRadius = 20.0f;
		int currentHealAmount = 0;
		float healTimer = 0.0f;
		int maxHealAmount = 3;
		float drinkBloodDuration = 0.0f;
		float drinkBloodTimer = 0.0f;
		float spitTimer = 0.0f;
		float yariThrowTimer = 0.0f;
		float yariThrowDuration = 0.2f;
		int yariThrowCount = 0;
		float minionMaxDistance = 1000.0f;
		
		float spitRadiusYaw = -40.0f;

		int updateDelay = 0;

		float findPlayerTimer = 0.0f;
		float findPlayerDuration = 0.4f;

		FVector acceleration;
		FVector currentVelocity;

		void CheckNearestEnemy();
		void DashToPortalOrPlayer(float DeltaSeconds);
		FVector directionMoving;
		
		void LookAtVelocity();
		void CheckYariThrowDuration(float DeltaSeconds);

		float recoveryTimer = 0.0f;
		float recoveryDuration = 0.0f;
		void RandomizeRecovery();

		TArray<AttackType> AttackList;

	public:
		AMyAIController();
		
		virtual void BeginPlay() override;

		virtual void Tick(float DeltaSeconds) override;

		ACharacter* playerCharacter;
		FVector playerLocation;
		class ABossCharacter* bossActor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		class AYari* yari;			

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
		FRotator bossOriginalRot;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
		float startYariThrowDuration = 20.0f;

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void RecoverFromStun();

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void SpitBlood();

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void SummonMinion();

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void ThrowYari();

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void CallBackYari();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
		TArray<AMinion*> MinionList;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
		TArray<AMinion*> FireMinionList;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
		TArray<AMinion*> nearestMinionList;

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void AddMinion(AMinion * minion);

		void AddFireMinion(AMinion * minion);

		FTimerHandle SpitTimer;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
		FVector bossLastPos; //move back to Private after test

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
			float accelerationForce;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
			float maxMagnitude;

		UFUNCTION(BlueprintCallable, Category = "Portal")
		void LookAtPortal(float speed);

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
			void ResetVelocity();

		UFUNCTION(BlueprintCallable, Category = "Attack")
			void CheckAttackList();

		UFUNCTION(BlueprintCallable, Category = "Portal")
		void FindPortalDirection();

		UFUNCTION(BlueprintCallable, Category = "Portal")
		void FindPlayerDirection();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
			bool isEnterArena = false;

		UFUNCTION(BlueprintCallable, Category = "Follow")
		void FollowPlayer();

		UFUNCTION(BlueprintCallable, Category = "Timer")
			void ResetTimer();

		// Healing
		UFUNCTION(BlueprintCallable, Category = "Healing")
			void Heal();

		UFUNCTION(BlueprintCallable, Category = "Healing")
			void StopHeal();
};
