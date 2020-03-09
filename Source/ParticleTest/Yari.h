// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "BossCharacter.h"
#include "TPSCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Yari.generated.h"

UCLASS()
class PARTICLETEST_API AYari : public AStaticMeshActor
{
	GENERATED_BODY()
	
private:	
	// Sets default values for this actor's properties
	void StopYari();
	FTimerHandle StopTimer;
	float gravityScale;
	FVector initLocation;
	FRotator initRotation;
	ACharacter* playerCharacter;
	ATPSCharacter* playerActor;
	class AMyAIController* bossAI;
	//bool isDamaged = false;
	void ThrowOnThrone(float subDeltaTime);
	FVector acceleration;
	FVector currentVelocity;
	FVector playerLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AYari();
	UProjectileMovementComponent* ProjectileMovement;
	USceneComponent* SceneRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	ABossCharacter* bossActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Box")
		class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;



	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ThrowOnGround(float speed, FRotator angle);

	void BackToHandSocket(float DeltaSeconds);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapPlayer(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	bool isThrowing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleEffects")
	 UParticleSystem* groundImpactParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleEffects")
	UParticleSystem* shockwaveImpactParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleEffects")
	UParticleSystem* callbackParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleEffects")
		UParticleSystem* groundCrackParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleEffects")
		UParticleSystem* groundImpactSparkParticle;
	UFUNCTION()
	void PlayYariCallBackParticle();
	
	UFUNCTION(BlueprintCallable, Category = "ParticleEffects")
	void PlayYariHitGroundParticles(float scale);

	bool isBack = false;
	bool isFire = true;

	UFUNCTION(BlueprintCallable, Category = "Pos")
	void SetInitPos(FVector location, FRotator rot);

	UFUNCTION(BlueprintCallable, Category = "Pos")
	void SetLocalPos();

	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void RotateTowardsPlayer();

	UFUNCTION(BlueprintCallable, Category = "Size")
	void SetYariCollisionSize(float halfHeight,float radius);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radius")
		float pushBackRadius;

	void SetYariCollision(bool b);

	void DetachFromBoss();

	void StickToLocation(FVector location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acceleration")
		float maxMagnitude = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acceleration")
	float accelerationForce = 0.2f;

	// Event called every physics tick and sub-step.
	UFUNCTION(BlueprintNativeEvent)
		void PhysicsTick(float SubstepDeltaTime);
	virtual void PhysicsTick_Implementation(float SubstepDeltaTime);

	// Custom physics Delegate
	FCalculateCustomPhysics OnCalculateCustomPhysics;
	void CustomPhysics(float DeltaTime, FBodyInstance* BodyInstance);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "FireEffect")
	void SetFireEffectEnable(bool b);
};
