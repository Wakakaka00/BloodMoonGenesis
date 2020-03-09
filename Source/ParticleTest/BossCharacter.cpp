// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TPSCharacter.h"
#include "MyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	PushCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	PushCapsule->InitCapsuleSize(83.0f, 30.0f);;
	PushCapsule->SetCollisionProfileName(TEXT("Trigger"));
	PushCapsule->SetupAttachment(RootComponent);
	PushCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);

	//ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	//ProjectileMovement->bAutoActivate = false;

	maxHealth = 2000.0f;
	currentHealth = maxHealth;
	tempHealth = currentHealth;
	bossPhase = BossPhase::Phase1;
	bossState = BossState::Attack;
	maxStance = 100.0f;
	currentStance = 0.0f;
	ResetStanceRecovery();
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	skeletalMesh = GetMesh();
	aiController = Cast<AMyAIController>(GetController());
	aoeType = AOEType::AOE;
}

void ABossCharacter::TeleportRandomPortalLocation()
{
	int n = FMath::RandRange(0, portalList.Num() - 1);
	SetActorLocation(portalList[n]->GetActorLocation());
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(isStart) handSocketLocation = GetMesh()->GetSocketLocation("Bip001-R-Hand");
	/*if (bossState == BossState::Break)
	{
		breakTimer += DeltaTime;
		if (bossPhase == BossPhase::Phase1)
		{
			if (breakTimer >= 4.0f)
			{
				breakTimer = 0.0f;
				bossState = BossState::Recovery;
			}
		}
		else if (bossPhase == BossPhase::Phase2)
		{
			if (breakTimer >= 3.0f)
			{
				breakTimer = 0.0f;
				bossState = BossState::Recovery;
			}
		}
		else if (bossPhase == BossPhase::Phase3)
		{
			if (breakTimer >= 2.0f)
			{
				breakTimer = 0.0f;
				bossState = BossState::Recovery;
			}
		}
	}*/

	if (tempHealth > currentHealth)
	{
		tempHealthTimer += DeltaTime;
		if (tempHealthTimer >= tempHealthDuration)
		{
			tempHealth -= 100.0f * DeltaTime;
			if (tempHealth < currentHealth)
			{
				tempHealth = currentHealth;
				tempHealthTimer = 0.0f;
			}
		}
	}
	//else if (bossState == BossState::Vulnerable)
	//{
		//vulnerableTimer += DeltaTime;
		//if (vulnerableTimer >= vulnerableDuration)
		//{
		//	LookAtPlayer(rotSpeed);
		//	vulnerableTimer = 0.0f;
		//	bossState = BossState::Recovery;
		//	isAtk = true;
		//	if (bloodPoolList.Num() < 3)
		//	{
		//		bossState = BossState::Attack;
		//		isDrinking = true;
		//		PlayDrinking();
		//	}
		//	else
		//	{
		//		//JumpBackThrone();
		//		PlayBackJumpStart();
		//	}
		//}
	//}

	if (isBloodPoolDamaged)
	{
		bloodPoolDamageTimer += DeltaTime;
		if (bloodPoolDamageTimer >= bloodPoolDamageDuration)
		{
			isBloodPoolDamaged = false;
			SetBloodPoolCollision(true);
			bloodPoolDamageTimer = 0.0f;
		}
	}

	if (bossState == BossState::Recovery)
	{
		if (currentStance > 0.0f)
		{
			if (stanceRecoveryTimer >= stanceRecoveryDuration)
			{
				currentStance -= DeltaTime * stanceRecoverySpeed;
				if (currentStance <= 0.0f)
				{
					currentStance = 0.0f;
					stanceRecoveryTimer = 0.0f;
				}
			}
			else
			{
				stanceRecoveryTimer += DeltaTime;
			}
		}
	}
	

	if (isSpitDamaged && isSpitting)
	{
		if (spitDamageTimer >= spitDamageDuration)
		{
			spitDamageTimer = 0.0f;
			isSpitDamaged = false;
			SpitCollisionEnable(true);
		}
		else
		{
			spitDamageTimer += DeltaTime;
		}
	}

	if (currentHealth > 0)
	{
		if (isHit)
		{
			blockTimer += DeltaTime;
			if (blockTimer >= blockDuration)
			{
				blockTimer = 0.0f;
				if (!isYariPush)
				{
					ResetAttack();
					ResetAnimation();
				}
			}
		}
	}
	

	if (GetIsDamaged())
	{
		isDamagedResetTimer += DeltaTime;
		if (isDamagedResetTimer >= 0.5f)
		{
			isDamagedResetTimer = 0.0f;
			SetIsDamaged(false);
		}
	}
	else
	{
		if (isDamagedResetTimer > 0.0f)
		{
			isDamagedResetTimer = 0.0f;
		}
	}
}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABossCharacter::SetBloodPoolCollision(bool b)
{
	if (!b)
	{
		isBloodPoolDamaged = true;
		for (int i = 0; i < bloodPoolList.Num(); i++)
		{
			bloodPoolList[i]->boxCollision->SetCollisionProfileName("NoCollision");
		}		
	}
	else
	{
		isBloodPoolDamaged = false;
		for (int i = 0; i < bloodPoolList.Num(); i++)
		{
			bloodPoolList[i]->boxCollision->SetCollisionProfileName("Trigger");
		}	
	}
}

void ABossCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag("Player"))
	{
		if (isYariPush)
		{
			playerActor->Pushback(550.0f, GetActorLocation(),EPushbackTypeEnum::EP_Medium,false);
		}
		else
		{
			playerActor->Pushback(pushBackForce, GetActorLocation(), EPushbackTypeEnum::EP_Medium,true);
			DamagePlayer(20.0f,false);
		}		
		//SetPushCollison(false);
	}
}

void ABossCharacter::SetPushCollison(bool b)
{
	if (!b) PushCapsule->SetCollisionProfileName("NoCollision");
	else PushCapsule->SetCollisionProfileName("YariAttackPreset");
}

void ABossCharacter::LookAtPlayer(float speed)
{
	FVector playerPos = playerPawn->GetActorLocation();
	FVector bossPos = GetActorLocation();
	playerPos.Z = 0.0f;
	bossPos.Z = 0.0f;
	FRotator Rot = FRotationMatrix::MakeFromX(playerPos - bossPos).Rotator();
	if (speed == -1.0f) SetActorRotation(Rot);
	else SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), Rot, GetWorld()->DeltaTimeSeconds * speed, true));
	
}

void ABossCharacter::Attack()
{
	aiController->yari->SetYariCollision(true);
}

void ABossCharacter::SetHit(bool b)
{
	isHit = b;
	if (b == true)
	{
		blockTimer = 0.0f;
	}
}

void ABossCharacter::StopAttack()
{
	aiController->yari->SetYariCollision(false);
}

void ABossCharacter::FindNearestPortal()
{	
	if (dashCount == 0)
	{
		targetPortal = portalList[0];
	}
	else
	{
		for (int i = 0; i < portalList.Num(); i++)
		{
			if (isLeft)
			{
				if (!portalList[i]->isLeft)
				{
					targetPortal = portalList[i];
					break;
				}
			}
			else
			{
				if (portalList[i]->isLeft)
				{
					targetPortal = portalList[i];
					break;
				}
			}
		}
	}
	for (int i = 0; i < portalList.Num(); i++)
	{
		if (portalList[i] != targetPortal)
		{
			if (dashCount == 0)
			{
				float shortestDistance = FVector::Distance(targetPortal->GetActorLocation(), hitList[0].ImpactPoint);
				float distance = FVector::Distance(portalList[i]->GetActorLocation(), hitList[0].ImpactPoint);
				if (distance < shortestDistance)
				{
					targetPortal = portalList[i];
				}
			}
			else
			{
				if (isLeft)
				{
					if (!portalList[i]->isLeft)
					{
						float shortestDistance = FVector::Distance(targetPortal->GetActorLocation(), hitList[1].ImpactPoint);
						float distance = FVector::Distance(portalList[i]->GetActorLocation(), hitList[1].ImpactPoint);
						if (distance < shortestDistance)
						{
							targetPortal = portalList[i];
						}
					}
				}
				else
				{
					if (portalList[i]->isLeft)
					{
						float shortestDistance = FVector::Distance(targetPortal->GetActorLocation(), hitList[1].ImpactPoint);
						float distance = FVector::Distance(portalList[i]->GetActorLocation(), hitList[1].ImpactPoint);
						if (distance < shortestDistance)
						{
							targetPortal = portalList[i];
						}
					}
				}
			}
		}
	}

	isLeft = targetPortal->isLeft;
}

void ABossCharacter::Break()
{
	bossState = BossState::Break;
	PlayDefenseBreak();
}

void ABossCharacter::SetVulnerable()
{
	bossState = BossState::Vulnerable;
	if (isDrinking || isSpitting || isHealing)
	{
		isInterrupted = true;
		aiController->ResetTimer();
	}
	PushCapsule->SetCollisionProfileName("NoCollision");
	isSpitting = false;
	isAtk = false;
	isDrinking = false;
	isHealing = false;
	PlayVulnerable();
}

bool ABossCharacter::isBreakable()
{
	if (isHealing || isSpitting || isDrinking)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ABossCharacter::isAttackingVital()
{
	FVector currentPos = GetActorLocation();
	currentPos.Z = 0.0f;
	FVector playerPos = playerPawn->GetActorLocation();
	playerPos.Z = 0.0f;
	FVector direction = UKismetMathLibrary::GetDirectionUnitVector(currentPos, playerPos);
	float dot = FVector::DotProduct(direction, GetActorForwardVector());
	float angle = FMath::RadiansToDegrees(FMath::Acos(dot));
	if (angle >= 150.0f)
	{
		return true;
	}
	return false;
}

void ABossCharacter::ResetAttack()
{
	bossState = BossState::Recovery;
	isAtk = false;
	isHit = false;
	if (isYariPush) isYariPush = false;
	aiController->CheckAttackList();
	aiController->yari->SetYariCollisionSize(240.0f, 82.0f);
	SpitCollisionEnable(false);
	EnableBossCollision(true);
	spitDamageTimer = 0.0f;
	isSpitDamaged = false;
}

void ABossCharacter::TrackingPlayer()
{
	isTracking = true;
}

void ABossCharacter::StopTrackingPlayer()
{
	isTracking = false;
	//aiController->StopMovement();
}

void ABossCharacter::DashForward(float DeltaTime, FVector targetLocation,float speed)
{
	FHitResult HitResult;
	FVector direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), targetLocation);
	SetActorLocation(GetActorLocation() + (direction * speed * DeltaTime), false, &HitResult);
}

bool ABossCharacter::GetIsTracking()
{
	return isTracking;
}

float ABossCharacter::GetTempHealth()
{
	return tempHealth;
}

void ABossCharacter::ResetStanceRecovery()
{
	stanceRecoveryTimer = 0.0f;
	stanceRecoveryDuration = (maxHealth / currentHealth) * 3.0f;
	stanceRecoverySpeed = (currentHealth / maxHealth) * 10.0f;
}

void ABossCharacter::AddStance(float stance)
{
	if (bossState != BossState::Vulnerable)
	{
		currentStance += stance;
		if (currentStance >= maxStance)
		{
			SetVulnerable();
			currentStance = 0.0f;
		}
		ResetStanceRecovery();
	}	
}

void ABossCharacter::GroundPunch()
{
	//distance = FVector::Distance(GetActorLocation(), playerActor->GetActorLocation());
	//if (distance <= 700.0f) playerActor->Pushback(pushBackForce + 400.0f, GetActorLocation(), EPushbackTypeEnum::EP_Medium);
	AOEDamage(GetActorLocation(), 30.0f, 780.0f);
}

bool ABossCharacter::isDead()
{
	if (currentHealth <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ABossCharacter::ResetVulnerable()
{
	LookAtPlayer(rotSpeed);
	vulnerableTimer = 0.0f;
	bossState = BossState::Recovery;
	isAtk = true;	

	if (isInterrupted)
	{
		PlayBackJumpStart();		
	}
	else
	{
		CheckEngage();
	}
	isInterrupted = false;
}

void ABossCharacter::SetSpitRot(float r)
{
	FRotator temp = FRotator::ZeroRotator;
	float bossYaw = GetActorRotation().Yaw;
	temp.Yaw = bossYaw + r;
	spitRadiusRoot->SetWorldRotation(temp);
}

void ABossCharacter::JumpStab()
{
	isAtk = true;
	PlayArcJump();
	EnableBossCollision(false);
	bossState = BossState::Attack;
}

bool ABossCharacter::CheckRoundHouse()
{
	int randNum = FMath::RandRange(1, 100);
	if (randNum <= 40)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ABossCharacter::CheckYariPush()
{
	if (bossState == BossState::Vulnerable)
	{
		return false;
	}
	int randNum = FMath::RandRange(1, 100);
	int chance = 0;
	if (currentHealth > maxHealth * 0.5f)
	{
		chance = 20;
	}
	else
	{
		chance = 35;
	}

	if (randNum <= chance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float ABossCharacter::GetPlayerDistance()
{
	float distance = FVector::Distance(GetActorLocation(), playerActor->GetActorLocation());
	return distance;
}

bool ABossCharacter::CheckPercentage(int p)
{
	if (bossState == BossState::Vulnerable)
	{
		return false;
	}
	int randNum = FMath::RandRange(1, 100);

	if (randNum <= p)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ABossCharacter::CheckEngage()
{
	if (GetPlayerDistance() > 1800.0f) // Engage
	{
		CheckJumpStab();
	}
	else // Disengage
	{
		PlayBackJumpOnGround();		
		isDisengage = true;
	}
}

void ABossCharacter::CheckDisengageAction()
{
	isDisengage = false;
	if (bloodPoolList.Num() < 3)
	{
		PlayDrinking();
	}
	else
	{
		if (currentHealth < maxHealth * 0.2f)
		{
			if (healCount < maxHealCount)
			{
				PlayHealing();
				isAtk = true;
				isHealing = true;
				healCount += 1;
				bossState = BossState::Attack;
			}
			else
			{
				CheckJumpStab();
			}
		}
		else
		{
			CheckJumpStab();
		}
	}
}

void ABossCharacter::CheckJumpStab()
{
	if (jumpingStabCount >= 2)
	{
		PlayPortalDash();
		jumpingStabCount = 0;
	}
	else
	{
		ResetAttack();
		JumpStab();
		jumpingStabCount += 1;
	}
}

void ABossCharacter::ResetTempHealthTimer()
{
	tempHealthTimer = 0.0f;
}





