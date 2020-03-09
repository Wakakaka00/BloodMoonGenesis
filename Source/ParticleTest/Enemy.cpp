// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "TPSCharacter.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	playerPawn = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerActor = Cast<ATPSCharacter>(playerPawn);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}





// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::SetIsDamaged(bool b)
{
	isDamaged = b;
}

bool AEnemy::GetIsDamaged()
{
	return isDamaged;
}

