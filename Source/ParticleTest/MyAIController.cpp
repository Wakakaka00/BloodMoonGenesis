// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Minion.h"
#include "Runtime/Core/Public/Math/Vector.h"


AMyAIController::AMyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	bossPawn = GetPawn();
	bossActor = Cast<ABossCharacter>(bossPawn);
	RandomizeRecovery();
	for (int i = 0; i < 3; i++)
	{
		AttackList.Add(AttackType(i));
	}
}

void AMyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!isEnterArena || bossActor->isDead())
	{
		return;
	}

	bossActor->distance = FVector::Distance(bossPawn->GetActorLocation(), playerLocation);
	//CheckNearestEnemy();

	if (!bossActor->isStart)
	{
		if (bossActor->currentHealth > 0)
		{
			if (bossActor->bossState == BossState::Recovery)
			{
				if (!bossActor->isAtk)
				{
					playerLocation = playerCharacter->GetActorLocation();
					if (!bossActor->isHit)
					{
						bossActor->LookAtPlayer(bossActor->rotSpeed);
						FollowPlayer();
					}

					recoveryTimer += DeltaSeconds;
					if (recoveryTimer >= recoveryDuration)
					{
						if (bossActor->distance <= bossActor->atkDistance)
						{
							bossActor->isHit = false;
							bossActor->bossState = BossState::Attack;
							bossActor->isAtk = true;
							recoveryTimer = 0.0f;
							RandomizeRecovery();
							StopMovement();

							int randAttack = FMath::RandRange(0, AttackList.Num() - 1);
							if (AttackList[randAttack] == AttackType::Lunge)
							{
								bossActor->PlayLungeStabAttack();
							}
							else if (AttackList[randAttack] == AttackType::Arc3)
							{
								bossActor->PlayArc3Hit();
							}
							else if (AttackList[randAttack] == AttackType::AOE)
							{
								bossActor->PlayYariSmash();
							}
							AttackList.RemoveAt(randAttack);
						}
					}

					/*if (bossActor->currentHealth < bossActor->maxHealth * 0.3f && bossActor->healCount < bossActor->maxHealCount)
					{
						bossActor->PlayHealing();
						bossActor->isAtk = true;
						bossActor->isHealing = true;
						bossActor->healCount += 1;
						bossActor->bossState = BossState::Attack;
					}*/

					/*if (bossActor->bloodPoolList.Num() < 3)
					{
						bossActor->isAtk = true;
						bossActor->isDrinking = true;
					}*/
				}

				// Jump Dash from throne
				if (bossActor->isJumpDash)
				{
					bossActor->JumpDash(DeltaSeconds);
				}
			}
			else if (bossActor->bossState == BossState::Attack)
			{
				if (bossActor->GetIsTracking())
				{
					playerLocation = playerCharacter->GetActorLocation();
					//bossActor->LookAtPlayer(bossActor->rotSpeed);
					//MoveToLocation(playerLocation, 200.0f, false);
				}

				if (bossActor->isLookAtPlayer)
				{
					bossActor->LookAtPlayer(bossActor->rotSpeed);
				}

				// Portal Dash
				if (bossActor->isPortalDash)
				{
					DashToPortalOrPlayer(DeltaSeconds);
				}

				//Spit blood
				if (bossActor->isSpitting)
				{
					spitRadiusYaw += 40.0f * DeltaSeconds;
					//with animation
					//bossActor->SetSpitRot(spitRadiusYaw);

					// without animation
					FRotator currentRot = bossOriginalRot;
					currentRot.Yaw += spitRadiusYaw;
					bossActor->SetActorRotation(currentRot);
				}

				// Drink Blood
				if (bossActor->isDrinking)
				{
					StopMovement();
					bossActor->LookAtPlayer(bossActor->rotSpeed);
					drinkBloodTimer += DeltaSeconds;
					if (drinkBloodTimer >= 1.0f)
					{
						drinkBloodDuration += 1.0f;
						drinkBloodTimer = 0.0f;
					}
					if (drinkBloodDuration >= 2.0f)
					{
						bossActor->isDrinking = false;
						bossActor->PlayDrinkingEnd();
						/*SpitBlood();
						bossActor->SpitCollisionEnable(true);*/
						recoveryTimer = 0.0f;
					}
				}

				// Heal Guord
				if (bossActor->isHealing)
				{
					StopMovement();
					bossActor->LookAtPlayer(bossActor->rotSpeed);
					//currentHealAmount += 3.0f * DeltaSeconds;
					//bossActor->currentHealth += 3.0f * DeltaSeconds;
					/*healTimer += DeltaSeconds;
					if (healTimer >= 2.0f)
					{
						currentHealAmount += 1;
						bossActor->currentHealth += (bossActor->maxHealth * 0.1f);
						healTimer = 0.0f;
						recoveryTimer = 0.0f;
					}
					if (currentHealAmount >= maxHealAmount)
					{
						bossActor->isAtk = false;
						bossActor->isHealing = false;
						currentHealAmount = 0.0f;
						healTimer = 0.0f;
						bossActor->bossState = BossState::Recovery;
					}*/
				}
			}
			else
			{
				StopMovement();
			}
		}		
	}
	else // isStart
	{
		bossActor->LookAtPlayer(bossActor->rotSpeed);
		//yariThrowTimer += DeltaSeconds;
		if (yariThrowCount >= 2)
		{
			if (!bossActor->isYariThrow)
			{
				yariThrowTimer = 0.0f;
				yariThrowCount = 0;
				bossActor->isStart = false;
				bossActor->isOnThrone = false;
				StopMovement();
				bossLastPos = bossActor->GetActorLocation();
				bossActor->isFromThrone = true;
				bossActor->JumpStab();
				bossActor->bossPositionState = BossPositionState::ThroneDash;		
				yari->SetYariCollisionSize(240.0f, 81.6f);
			}
		}
		else
		{
			if (!bossActor->isYariThrow) CheckYariThrowDuration(DeltaSeconds);
		}

		if (bossActor->isAiming)
		{
			yari->StickToLocation(bossActor->handSocketLocation);
			yari->RotateTowardsPlayer();
		}
	}
}


void AMyAIController::RecoverFromStun()
{
	bossActor->isAtk = true;
	playerLastPos = playerLocation;
}

void AMyAIController::AddMinion(AMinion* minion)
{
	MinionList.Add(minion);
}

void AMyAIController::AddFireMinion(AMinion* minion)
{
	FireMinionList.Add(minion);
}

void AMyAIController::SummonMinion()
{
	for (int i = 0; i < bossActor->bloodPoolList.Num(); i++)
	{
		if (FVector::Distance(bossActor->bloodPoolList[i]->GetActorLocation(), bossActor->GetActorLocation()) <= 700.0f)
		{
			int randNum = FMath::RandRange(0, 1);
			for (int j = 0; j < randNum + 1; j++)
			{
				float x = bloodPoolRadius * cos(FMath::RandRange(0.0f, 360.0f));
				float y = bloodPoolRadius * sin(FMath::RandRange(0.0f, 360.0f));
				FVector spawnLocation = bossActor->bloodPoolList[i]->GetActorLocation() + FVector(x, y, 100.0f);
				auto Minion = GetWorld()->SpawnActor<AMinion>(bossActor->MinionBlueprint->GetOwnerClass(), spawnLocation, FRotator::ZeroRotator);
				AddMinion(Minion);
			}
		}
	}
}

void AMyAIController::ThrowYari()
{
	yari->SetYariCollisionSize(240.0f,110.0f);
	bossActor->isAiming = false;
	yariThrowTimer = 0.0f;
	/*FVector OutLaunchVelocity;
	FVector targetLocation = playerCharacter->GetActorLocation() - (bossActor->GetActorUpVector() * 20.0f);
	FVector spawnLocation = bossActor->GetActorLocation() + (bossActor->GetActorForwardVector() * 30.0f) + (bossActor->GetActorUpVector() * 90.0f);
	if (UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, spawnLocation, targetLocation, bossActor->yariLaunchSpeed * 0.8f, false, 0.0f, 0, ESuggestProjVelocityTraceOption::DoNotTrace))
	{
		bossActor->AimDirection = OutLaunchVelocity.GetSafeNormal();
	}

	FRotator AimAsRotator = bossActor->AimDirection.Rotation();*/
	
	if (bossActor->isOnThrone)
	{
		//yari->ThrowOnThrone();
		//yari->DetachFromBoss();
		yari->isThrowing = true;	
		
	}
	/*else
	{
		yari->ThrowOnGround(bossActor->yariLaunchSpeed, AimAsRotator);
	}*/
	
	yariThrowCount += 1;
}

void AMyAIController::CallBackYari()
{
	yari->isBack = true;
	yari->PlayYariCallBackParticle();
	bossActor->PlayCallBackYari();
}

void AMyAIController::SpitBlood()
{
	bossActor->isAtk = true;
	bossActor->isSpitting = true;
	recoveryTimer = 0.0f;

	// rotate without animation
	FVector spawnLocation = bossActor->GetActorLocation() + (bossActor->GetActorForwardVector() * 600.0f);

	// rotate using animation
	//FVector spawnLocation = bossActor->spitRadiusRoot->GetComponentLocation() + (bossActor->spitRadiusRoot->GetForwardVector() * 600.0f);

	auto Projectile = GetWorld()->SpawnActor<ABloodProjectile>(bossActor->BloodProjectileBlueprint->GetOwnerClass(), spawnLocation, bossActor->GetActorRotation());
	/*AActor * projectileActor = Projectile;
	FRotator AimAsRotator = bossActor->AimDirection.Rotation();

	projectileActor->SetActorRotation(AimAsRotator);

	Projectile->LaunchProjectile(bossActor->bloodLaunchSpeed);*/

	if (bossActor->bossState != BossState::Vulnerable)
	{
		if (drinkBloodDuration > 0)
		{
			drinkBloodDuration -= bossActor->spitDelay;
			if (drinkBloodDuration > 0) GetWorld()->GetTimerManager().SetTimer(SpitTimer, this, &AMyAIController::SpitBlood, bossActor->spitDelay);
			else
			{
				bossActor->PlayStopSpitting();
				drinkBloodDuration = 0.0f;
				bossActor->isSpitting = false;
				bossActor->SpitCollisionEnable(false);
				spitRadiusYaw = -40.0f;
			}
		}
	}	
	//delete projectileActor;
}

void AMyAIController::CheckNearestEnemy()
{
	for (int i = 0; i < MinionList.Num(); i++)
	{	
		if (nearestMinionList.Num() < 3 && !nearestMinionList.Contains(MinionList[i]))
		{
			float distance = FVector::Distance(MinionList[i]->GetActorLocation(), playerCharacter->GetActorLocation());
			if (distance <= minionMaxDistance)
			{
				nearestMinionList.Add(MinionList[i]);
				MinionList[i]->enemyType = EnemyType::Melee;
			}
		}	
	}

	//updateDelay += 1;

	//if (updateDelay >= 3)
	//{
		for (int i = 0; i < nearestMinionList.Num(); i++)
		{
			float distance = FVector::Distance(nearestMinionList[i]->GetActorLocation(), playerCharacter->GetActorLocation());
			if (distance > minionMaxDistance)
			{
				nearestMinionList[i]->enemyType = EnemyType::Range;
				nearestMinionList.RemoveAt(i);
			}
		}
	//	updateDelay = 0;
	//}
}

void AMyAIController::DashToPortalOrPlayer(float DeltaSeconds)
{
	if (bossActor->dashCount < bossActor->maxDashCount)
	{
		acceleration = directionMoving * accelerationForce * DeltaSeconds;
		acceleration.Z = 0.0f;

		currentVelocity += acceleration;
		currentVelocity = currentVelocity * 0.99f;

		if (currentVelocity.SizeSquared() > maxMagnitude * maxMagnitude)
		{
			FVector temp = currentVelocity;
			temp.GetSafeNormal(1.0f);
			temp.Normalize(1.0f);
			currentVelocity = temp * maxMagnitude;
		}

		FHitResult HitResult;
		bossActor->SetActorLocation(bossActor->GetActorLocation() + currentVelocity, false, &HitResult);

		LookAtPortal(bossActor->rotSpeed);	
	}
	else
	{
		findPlayerTimer += DeltaSeconds;
		if (findPlayerTimer >= findPlayerDuration)
		{
		//	directionMoving = UKismetMathLibrary::GetDirectionUnitVector(bossActor->GetActorLocation(), playerCharacter->GetActorLocation());
		//	directionMoving.Z = 0.0f;
		//	findPlayerTimer = 0.0f;
			
		//	//acceleration = FVector::ZeroVector;
		//	//currentVelocity = currentVelocity / 3.0f;
		}
		//acceleration = directionMoving * accelerationForce;
		//acceleration.Z = 0.0f;
		playerLastPos = playerCharacter->GetActorLocation();
		//currentVelocity += acceleration;
		////currentVelocity = currentVelocity * 0.99f;

		//if (currentVelocity.SizeSquared() > maxMagnitude * maxMagnitude)
		//{
		//	FVector temp = currentVelocity;
		//	temp.GetSafeNormal(1.0f);
		//	temp.Normalize(1.0f);
		//	currentVelocity = temp * maxMagnitude;
		//}

		//FHitResult HitResult;
		//bossActor->SetActorLocation(bossActor->GetActorLocation() + currentVelocity, false, &HitResult);
		////bossActor->LookAtPlayer();
		//LookAtVelocity();
		//bossActor->LookAtPlayer(bossActor->rotSpeed);
		//MoveToLocation(playerCharacter->GetActorLocation(), 200.0f, false);
		FRotator Rot = FRotationMatrix::MakeFromX(playerLastPos - bossActor->GetActorLocation()).Rotator();
		bossActor->SetActorRotation(Rot);
		MoveToLocation(FVector(playerLastPos.X,playerLastPos.Y,bossActor->GetActorLocation().Z), 200.0f, false);

		if (FVector::Distance(bossActor->GetActorLocation(), playerCharacter->GetActorLocation()) <= 600.0f)
		{
			bossActor->StopPortalDash();
			bossActor->isPortalDash = false;
			StopMovement();
			//bossActor->GetCapsuleComponent()->SetCollisionProfileName("Pawn");
			//bossActor->isAtk = false;
			bossActor->dashCount = 0;
			ResetVelocity();
			//bossActor->ResetAttack();
		}
	}
}

void AMyAIController::FindPortalDirection()
{
	directionMoving = UKismetMathLibrary::GetDirectionUnitVector(bossActor->GetActorLocation(), bossActor->targetPortal->GetActorLocation());
	directionMoving.Z = 0.0f;
}

void AMyAIController::FindPlayerDirection()
{
	directionMoving = UKismetMathLibrary::GetDirectionUnitVector(bossActor->GetActorLocation(), playerCharacter->GetActorLocation());
	directionMoving.Z = 0.0f;
	playerLastPos = playerCharacter->GetActorLocation();
}

void AMyAIController::LookAtVelocity()
{
	FRotator Rot = FRotationMatrix::MakeFromX(currentVelocity).Rotator();
	bossActor->SetActorRotation(Rot);
}

void AMyAIController::CheckYariThrowDuration(float DeltaSeconds)
{
	yariThrowTimer += DeltaSeconds;
	if (yariThrowTimer >= yariThrowDuration)
	{
		//ThrowYari();
		bossActor->PlayThrowYari();
		bossActor->isYariThrow = true;
		yari->DetachFromBoss();
		bossActor->isAiming = true;
	}
}

void AMyAIController::LookAtPortal(float speed)
{
	FRotator Rot = FRotationMatrix::MakeFromX(directionMoving).Rotator();
	if (speed == -1) bossActor->SetActorRotation(Rot);
	else bossActor->SetActorRotation(UKismetMathLibrary::RLerp(bossActor->GetActorRotation(), Rot, GetWorld()->DeltaTimeSeconds * speed, true));	
}

void AMyAIController::ResetVelocity()
{
	currentVelocity = FVector::ZeroVector;
	acceleration = FVector::ZeroVector;
}

void AMyAIController::RandomizeRecovery()
{
	recoveryDuration = FMath::RandRange(3.0f, 5.0f);
}

void AMyAIController::Heal()
{
	bossActor->currentHealth += (bossActor->maxHealth * 0.1f);
	recoveryTimer = 0.0f;
}

void AMyAIController::StopHeal()
{
	bossActor->isAtk = false;
	bossActor->isHealing = false;
	bossActor->bossState = BossState::Attack;
	bossActor->PlayBackJumpStart();
}

void AMyAIController::FollowPlayer()
{
	MoveToLocation(FVector(playerLocation.X,playerLocation.Y, bossActor->GetActorLocation().Z), 200.0f, false);
}

void AMyAIController::ResetTimer()
{
	drinkBloodDuration = 0.0f;
	drinkBloodTimer = 0.0f;
}

void AMyAIController::CheckAttackList()
{
	if (AttackList.Num() == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			AttackList.Add(AttackType(i));
		}
		bossActor->PlayBackJumpStart();
		bossActor->bossState = BossState::Attack;
		recoveryTimer = 0.0f;
	}
}

