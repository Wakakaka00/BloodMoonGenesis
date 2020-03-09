// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"


// Sets default values
ATPSCharacter::ATPSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	
{//

	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Default Number Set Ups
	//Maximum Health
	AttackCounts = 0;
	MaximumHealth = 200.0f;
	MaximumStamina = 100.0f;
	CurrentStamina = 100.0f;
	TargetStamina = 100.0f;
	CurrentHealth = MaximumHealth;
	
	CurrentStamina = MaximumStamina;
	enemyTag = TEXT("Enemy");

}
	









// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();

}
	


// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	
}




/*void ATPSCharacter::PushBack(float force, FVector pushLocation)
{
	CanMove = false;
	CanAttack = false;
	CanEvade = false;
	isPushingBack = true;
	EvadeAcceleration = EvadeAcceleration/2.0f;
	FVector direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), pushLocation);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	direction.Z = 0.0f;
	PushBackAnimation();
	//LaunchCharacter(-direction * force, true, true);
	pushBackTargetLocation = -direction * force;
}*/

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindInputs();
}



//Lock on functions





/*


void ATPSCharacter::SwitchRight_Implementation()
{
	if (TargetLocked)
	{
		FoundNewTarget = false;
		ClosestTargetDistance = MaximumDistance;
		//Get all enemies in the scene
		TArray<AActor*> FoundActors;
		FName enemyTag = TEXT("Enemy");
		ClosestTargetDistanceToTargetedEnemy = MaximumDistance;
		bool MaybeSwitchDown = false;
		bool MaybeSwitchUp = false;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), enemyTag, FoundActors);
		for (int i = 0; i < FoundActors.Num(); i++)
		{
			if (FoundActors[i] != NearestTarget)
			{
				AEnemy* enemy = Cast<AEnemy>(FoundActors[i]);
				if (enemy->CanBeTargeted)
				{
					EnemyElement = enemy;


					FRotator lookAtEnemyRotation = UKismetMathLibrary::FindLookAtRotation(NearestTarget->sphereLooker->GetComponentLocation(), EnemyElement->GetActorLocation());
					bool selectBool = lookAtEnemyRotation.Yaw < 0.0f;
					float selectFloat1 = UKismetMathLibrary::SelectFloat(lookAtEnemyRotation.Yaw + 360.0f, lookAtEnemyRotation.Yaw, selectBool);


					FRotator lookAtNearestEnemyRotation = UKismetMathLibrary::FindLookAtRotation(GetGlobalPlayer()->GetActorLocation(), NearestTarget->sphereLooker->GetComponentLocation());

					bool selectBool2 = lookAtNearestEnemyRotation.Yaw < 0.0f;
					float selectFloat2 = UKismetMathLibrary::SelectFloat(lookAtNearestEnemyRotation.Yaw + 360.0f, lookAtNearestEnemyRotation.Yaw, selectBool2);

					float outComeFloat = selectFloat1 - selectFloat2;

					bool selectBool3 = outComeFloat < 0.0f;

					float selectFloat3 = UKismetMathLibrary::SelectFloat(outComeFloat + 360.0f, outComeFloat, selectBool3);
					//0.0    180
					bool isInRange = UKismetMathLibrary::InRange_FloatFloat(selectFloat3, 30.0f, 145.0f);			
					float distanceBetweenEnemy = (EnemyElement->GetActorLocation() - NearestTarget->GetActorLocation()).Size();
					if(!MaybeSwitchDown)MaybeSwitchDown = UKismetMathLibrary::InRange_FloatFloat(selectFloat3, 145.0f ,180.0f);
					if (!MaybeSwitchUp)MaybeSwitchUp = UKismetMathLibrary::InRange_FloatFloat(selectFloat3, 0.0f, 30.0f);
					FString TheFloatStr = FString::SanitizeFloat(selectFloat3);
								

					/*if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Bool: %s"), MaybeSwitchDown ? TEXT("true") : TEXT("false")));
					}
						if (isInRange)
						{
							if (distanceBetweenEnemy < ClosestTargetDistanceToTargetedEnemy)
							{
								ClosestTargetDistanceToTargetedEnemy = distanceBetweenEnemy;				
								FoundNewTarget = true;
								NewTarget = EnemyElement;
							}
						}			
					
				}
			}
		


		}

		if (FoundNewTarget)
		{

			FOutputDeviceNull ar;
			NearestTarget->CallFunctionByNameWithArguments(TEXT("LockOnFlip"), ar, NULL, true);
			SwitchDone();
			SetLockOnToTarget();
		}
		else 
		{
			
			if (MaybeSwitchDown)
			{
				SwitchDown();
				
			}
			else if (MaybeSwitchUp)
			{
				SwitchUp();
			}	
			
		}
	}



}


void ATPSCharacter::SwitchDone_Implementation()
{
	NearestTarget = NewTarget;
	ClosestTargetDistance = NearestTarget->PlayerToEnemyDistance;
	
}
*/


//Melee Combat

void ATPSCharacter::DoAttacks(bool DoCharge)
{
	

	switch (SwitchMode)
	{
		case ESwitchModeEnum::ES_Light:
		{
			
				if (AttackCounts == 0)
				{
					//PlayAnimMontage(montages[0]);
					GetMesh()->GetAnimInstance()->Montage_Play(montages[0], 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
					AttackCounts = 1;
				}
				else if (AttackCounts == 1)
				{
					//PlayAnimMontage(montages[1]);
					GetMesh()->GetAnimInstance()->Montage_Play(montages[1], 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
					AttackCounts = 2;
				}
				else if (AttackCounts == 2)
				{
					//PlayAnimMontage(montages[2]);
					GetMesh()->GetAnimInstance()->Montage_Play(montages[2], 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
					AttackCounts = 0;
					if(GetCharacterMovement()->IsFalling())CanAttackOnAir = false;
					
				}
				
				
				

				
			
			
			break;
		}
		case ESwitchModeEnum::ES_Heavy:
		{
			if (AttackCounts == 0)
			{
				GetMesh()->GetAnimInstance()->Montage_Play(montages[4],1.0f,EMontagePlayReturnType::MontageLength,0.0f,true);
				//PlayAnimMontage(montages[4], 1.0f);
				AttackCounts = 1;
			}
			else if (AttackCounts == 1)
			{
				GetMesh()->GetAnimInstance()->Montage_Play(montages[5], 0.7f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				//PlayAnimMontage(montages[5], 0.7f);
				AttackCounts = 2;
			}
			else if (AttackCounts == 2)
			{
				GetMesh()->GetAnimInstance()->Montage_Play(montages[6], 0.7f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				//PlayAnimMontage(montages[6], 0.7f);
				AttackCounts = 0;
			}
			
			 break;
		}

	}
}

void ATPSCharacter::ResetCombo_Implementation()
{
	CanContinueCombo = false;
	AttackCounts = 0;
	inAttackAnimation = false;
	PostCanContinueCombo = false;
	HeavyChargeAttackCounterOrder = 0;
	CanAttack = true;
	if (GetCharacterMovement()->IsFalling())CanAttackOnAir = false;

}

void ATPSCharacter::ContinueCombo()
{
	
	if (!inAttackAnimation) return;
	if (CanContinueCombo )//Player clicked before post attack and it continue combo
	{
		CanContinueCombo = false;
		DoAttacks(false);
			
	}
	
	
	
	else if (!CanContinueCombo)// Player didnt hold neither click so it give another oppotunity
	{
		
		PostCanContinueCombo = true;
		CanContinueCombo = false;
		
	}
	

	// NEXT

	
}

void ATPSCharacter::SetCanContinueCombo(bool b)
{
	CanContinueCombo = b;
}

void ATPSCharacter::PlayerAttackedLight()
{
	if (CanAttack == false)return;
	
	else if (inAttackAnimation && !PostCanContinueCombo && !CanContinueCombo) // Second attack before postcancontinueattack
	{
		
		CanContinueCombo = true;
		
		
	}
	 else if(!inAttackAnimation && !CanContinueCombo) // First attack
	{
		
		inAttackAnimation = true;	
		DoAttacks(false);
		
	}
	 else if (inAttackAnimation && PostCanContinueCombo && !CanContinueCombo)//Last phase after can combo event for any last chance
	{
		
		DoAttacks(false);
		PostCanContinueCombo = false;
		
		
	}
	
	

}




ACharacter * ATPSCharacter::GetGlobalPlayer()
{
	
	 ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	 return player;
	
	
	

}

void ATPSCharacter::Movement(FVector direction,float v)
{
	
	
	GetGlobalPlayer()->AddMovementInput(direction, v);
	
	
	
}

//Inputs
void ATPSCharacter::MoveForward(float value)
{
	
	if (CanMove)
	{
		if (!ShouldRotateLockOn)
		{
			FRotator playerRotation = GetGlobalPlayer()->GetControlRotation();
			FRotator desiredRotation = FRotator(0.0f, playerRotation.Yaw, 0.0f);
			FVector desiredDir = FRotationMatrix(desiredRotation).GetScaledAxis(EAxis::X);
			targetForwardVector = desiredDir;
		}
		if (TargetLocked )
		{
			Movement(targetForwardVector, value);
		}
		else 
		{
			FRotator playerRotation = GetGlobalPlayer()->GetControlRotation();
			FRotator desiredRotation = FRotator(0.0f, playerRotation.Yaw, 0.0f);
			FVector desiredDir = FRotationMatrix(desiredRotation).GetScaledAxis(EAxis::X);
			Movement(desiredDir, value);
		}		
	}
}
void ATPSCharacter::MoveRight(float value)
{
	if (CanMove)
	{
		if (!ShouldRotateLockOn)
		{
			
			FRotator playerRotation = GetGlobalPlayer()->GetControlRotation();
			FRotator desiredRotation = FRotator(0.0f, playerRotation.Yaw, 0.0f);
			FVector desiredDir = FRotationMatrix(desiredRotation).GetScaledAxis(EAxis::Y);
			targetRightVector = desiredDir;
		}
		if (TargetLocked)
		{
			Movement(targetRightVector, value);
		}
		else 
		{
			FRotator playerRotation = GetGlobalPlayer()->GetControlRotation();
			FRotator desiredRotation = FRotator(0.0f, playerRotation.Yaw, 0.0f);
			desiredDir = FRotationMatrix(desiredRotation).GetScaledAxis(EAxis::Y);
			Movement(desiredDir, value);
		}
	
	}
		
}

void ATPSCharacter::BeginEvadeAcceleration(FVector direction)
{
	UMyCharacterMovementComponent* mc = Cast<UMyCharacterMovementComponent>(GetCharacterMovement());
	if (mc)
	{
		mc->BeginEvadeAccelerate(direction);
	}
}

void ATPSCharacter::BeginPushbackAcceleration(FVector direction)
{
	UMyCharacterMovementComponent* mc = Cast<UMyCharacterMovementComponent>(GetCharacterMovement());
	if (mc)
	{
		mc->beginingPushAccelerationDirection=direction;
	}
}


//Get Player Actor

void ATPSCharacter::BindInputs()
{
		InputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);
	
	
}


