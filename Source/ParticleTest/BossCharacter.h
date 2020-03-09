// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BloodProjectile.h"
#include "Minion.h"
#include "Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Portal.h"
#include "BloodPool.h"
#include "BossCharacter.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class BossPositionState : uint8
{
	OnThrone 	UMETA(DisplayName = "OnThrone"),
	ThroneDash 	UMETA(DisplayName = "ThroneDash"),
	JumpToThrone	UMETA(DisplayName = "JumpToThrone"),
	OnGround	UMETA(DisplayName = "OnGround")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class BossPhase : uint8
{
	Phase1 	UMETA(DisplayName = "Phase1"),
	Phase2 	UMETA(DisplayName = "Phase2"),
	Phase3	UMETA(DisplayName = "Phase3")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class BossState : uint8
{
	Recovery 	UMETA(DisplayName = "Recovery"),
	Break 	UMETA(DisplayName = "Break"),
	Vulnerable	UMETA(DisplayName = "Vulnerable"),
	Attack UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class AttackType : uint8
{
	Lunge 	UMETA(DisplayName = "Lunge"),
	Arc3	UMETA(DisplayName = "Arc3"),
	AOE UMETA(DisplayName = "AOE")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class AOEType : uint8
{
	None 	UMETA(DisplayName = "None"),
	AOE	UMETA(DisplayName = "AOE"),
	Wave UMETA(DisplayName = "Wave")
};

UCLASS()
class PARTICLETEST_API ABossCharacter : public AEnemy
{
	GENERATED_BODY()

private:
	bool isLeft = false; // portal
	float breakTimer = 0.0f;
	float vulnerableTimer = 0.0f;
	float vulnerableDuration = 4.0f;
	bool isTracking = false;
	class AMyAIController* aiController;
	float bloodPoolDamageTimer;
	float bloodPoolDamageDuration = 1.0f;
	float spitDamageTimer = 0.0f;
	float spitDamageDuration = 0.2f;
	float blockTimer = 0.0f;
	float blockDuration = 0.6f;
	float stanceRecoverySpeed;
	bool isInterrupted = false;
	float isDamagedResetTimer = 0.0f;
	float tempHealth;
	float tempHealthTimer = 0.0f;
	float tempHealthDuration = 1.5f;
	
	void CheckJumpStab();
	void CheckEngage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	ABossCharacter();
	USceneComponent* skeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	AActor* throne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		BossPositionState bossPositionState;

	BossPhase bossPhase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss State")
	BossState bossState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aoe Type")
	AOEType aoeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aoe Type")
	bool isWave = false;
	//UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	TArray <APortal*> portalList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	APortal* targetPortal;

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void TeleportRandomPortalLocation();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Look At Player")
	void LookAtPlayer(float speed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float rotSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool isLookAtPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isYariPush = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isDisengage = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Behavior")
		void Dash();
	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Behavior")
		void ForcePush();
	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Behavior")
		void JumpDash(float DeltaSeconds);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Boss Behavior")
		void ArcJump();
	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Behavior")
		void CallBackYari();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Boss Behavior")
		void SpitCollisionEnable(bool b);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable,Category = "Portal")
		void PortalDash();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "Boss Behavior")
		void JumpBackThrone();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool isAtk = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpitCollision")
	USceneComponent* spitRadiusRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	float distance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	float farDistance = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	float stopDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int damageTaken = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	int dashCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	int maxDashCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	bool isPortalDash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	TArray<FHitResult> hitList;

	UPROPERTY(EditAnywhere, Category = "Boss Behavior")
	TSubclassOf<ABloodProjectile> BloodProjectileBlueprint;

	UPROPERTY(EditAnywhere, Category = "Boss Behavior")
	TSubclassOf<AMinion> MinionBlueprint;

	UPROPERTY(EditAnywhere, Category = "Boss Behavior")
	float bloodLaunchSpeed = 2000;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Boss Behavior")
	float yariLaunchSpeed = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	TArray<ABloodPool*> bloodPoolList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isFromThrone = false;

	bool isDash = false;
	bool isBackJump = false;
	bool isNormalPush = false;
	bool isAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isStart = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isHealing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isDrinking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isJumpDash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isSpitting = false;

	UPROPERTY(BlueprintReadWrite, Category = "Boss Behavior")
	bool isSpitDamaged = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isOnThrone = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	float pushBackForce = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isHit = false;

	float spitDelay = 0.25f;
	FVector AimDirection = FVector(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Trigger Capsule")
	class UCapsuleComponent* PushCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isYariThrow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blood Pool")
		bool isBloodPoolDamaged = false;

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void SetBloodPoolCollision(bool b);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float atkDistance;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void SetPushCollison(bool b);

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void FindNearestPortal();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetHit(bool b);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void JumpStab();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopAttack();

	UFUNCTION(BlueprintCallable, Category = "Spit")
	void SetSpitRot(float r);

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void Break();

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void SetVulnerable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Location")
	FVector playerLastPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BackJump")
	bool isBackToThrone = false;

	FVector handSocketLocation = FVector::ZeroVector;

	int healCount = 0;
	int maxHealCount = 1;

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	bool isAttackingVital();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	int jumpingStabCount = 0;

	// Animations
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayDrinking();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayDrinkingEnd();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlaySpitting();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayHealing();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayVulnerable();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayDefenseBreak();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayDefenseFeedback(bool isLight);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayLungeStabAttack();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayYariSmash();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayArc3Hit();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayBreakState();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayBackJumpStart();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayBackJumpLand();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayBackJumpOnGround();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayArcJump();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayFireClaw();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Boss Behavior")
		void BackJump();

	UFUNCTION(BlueprintCallable, Category = "Animations")
		void ResetAttack();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayStopSpitting();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayThrowYari();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayCallBackYari();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayGrabYari();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayHumanoidDeadAnimation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayRoundHouseAOE();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayYariPush();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayPortalDash();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void StopPortalDash();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void ResetAnimation();

	UFUNCTION(BlueprintCallable, Category = "Animations")
		void TrackingPlayer();

	UFUNCTION(BlueprintCallable, Category = "Animations")
		void ResetVulnerable();

	UFUNCTION(BlueprintCallable, Category = "Animations")
		void StopTrackingPlayer();

	UFUNCTION(BlueprintCallable, Category = "Animations")
		void DashForward(float DeltaTime,FVector targetLocation,float speed);

	UFUNCTION(BlueprintCallable, Category = "Animations")
	void GroundPunch();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Attack")
	void DamagePlayer(float d,bool shouldStun= false);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Attack")
	void EnableBossCollision(bool b);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Attack")
	void AOEDamage(FVector location,float damage,float radius);

	bool GetIsTracking();

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetTempHealth();

	// Stance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stance")
	float maxStance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stance")
	float currentStance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stance")
	float stanceReceive;

	UFUNCTION(BlueprintCallable, Category = "Stance")
	void AddStance(float stance);

	UFUNCTION(BlueprintCallable, Category = "GetDistance")
	float GetPlayerDistance();

	UFUNCTION(BlueprintCallable, Category = "CheckAction")
	bool isBreakable();

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool isDead();

	UFUNCTION(BlueprintCallable, Category = "CheckAction")
	bool CheckRoundHouse();

	UFUNCTION(BlueprintCallable, Category = "CheckAction")
	bool CheckPercentage(int p);

	UFUNCTION(BlueprintCallable, Category = "CheckAction")
	bool CheckYariPush();

	UFUNCTION(BlueprintCallable, Category = "CheckAction")
	void CheckDisengageAction();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ResetTempHealthTimer();

	float stanceRecoveryTimer;
	float stanceRecoveryDuration;

	void ResetStanceRecovery();
};
