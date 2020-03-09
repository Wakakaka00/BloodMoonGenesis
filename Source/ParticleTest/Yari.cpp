// Fill out your copyright notice in the Description page of Project Settings.


#include "Yari.h"
#include "Engine/World.h"
#include "MyAIController.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "BloodPool.h"
#include "TimerManager.h"

// Sets default values
AYari::AYari()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->bAutoActivate = false;

	//SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	//RootComponent = SceneRoot;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBox->InitBoxExtent(FVector(15.0f, 15.0f, 15.0f));
	TriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
	TriggerBox->AttachTo(RootComponent);
	TriggerBox->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AYari::OnOverlapBegin);

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(5.0f, 10.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("NoCollision"));
	TriggerCapsule->AttachTo(RootComponent);
	TriggerCapsule->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AYari::OnOverlapPlayer);
	OnCalculateCustomPhysics.BindUObject(this, &AYari::CustomPhysics);
}


// Called when the game starts or when spawned
void AYari::BeginPlay()
{
	Super::BeginPlay();
	bossActor = Cast<ABossCharacter>(GetParentActor());
	//bossAI = Cast<AMyAIController>(bossActor->GetController());
	
	playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerActor = Cast<ATPSCharacter>(playerCharacter);
	gravityScale = 1.5f;
	//initLocation = FVector(28.676769f, -12.031745f, 200.363602f);
	//initRotation = FRotator(0.000006f, -10.000103f, 0.000082f);
	//FName handSocketName = TEXT("WeaponSocket");
	//GetStaticMeshComponent()->AttachToComponent(bossActor->skeletalMesh, FAttachmentTransformRules::KeepWorldTransform, handSocketName);
	//K2_AttachRootComponentTo(bossActor->skeletalMesh, handSocketName, EAttachLocation::SnapToTarget, true);
	initLocation = FVector(-29.874313f, -14.592071f, 6.415133f);
	initRotation = FRotator(-0.000034f, 0.000024f, 0.000045f);
}

// Called every frame
void AYari::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetStaticMeshComponent()->GetBodyInstance() != NULL)
	{	
		GetStaticMeshComponent()->GetBodyInstance()->AddCustomPhysics(OnCalculateCustomPhysics);
	}

	if(isBack)
	{
		BackToHandSocket(DeltaTime);
	}
}

void AYari::PhysicsTick_Implementation(float SubstepDeltaTime)
{
	if (isThrowing)
	{
		ThrowOnThrone(SubstepDeltaTime);
	}
}

void AYari::CustomPhysics(float DeltaTime, FBodyInstance* BodyInstance)
{
	PhysicsTick(DeltaTime);
}

void AYari::ThrowOnGround(float speed,FRotator angle)
{
	//GetStaticMeshComponent()->SetSimulatePhysics(true);
	isThrowing = true;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(angle);	
	ProjectileMovement->ProjectileGravityScale = gravityScale;
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * speed ); 
	ProjectileMovement->Activate();
}

void AYari::ThrowOnThrone(float subDeltaTime)
{
	FVector direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), playerLocation);
	acceleration += direction * accelerationForce * subDeltaTime;
	currentVelocity += acceleration;
	if (currentVelocity.SizeSquared() > maxMagnitude * maxMagnitude)
	{
		FVector temp = currentVelocity;
		temp.GetSafeNormal(1.0f);
		temp.Normalize(1.0f);
		currentVelocity = temp * maxMagnitude;
	}

	FHitResult HitResult;
	SetActorLocation(GetActorLocation() + currentVelocity, true, &HitResult);
}

void AYari::DetachFromBoss()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (!GetStaticMeshComponent()->IsSimulatingPhysics())GetStaticMeshComponent()->SetSimulatePhysics(true);	
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetCollisionProfileName(TEXT("YariAttackPreset"));
}

void AYari::StopYari()
{	
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	//ProjectileMovement->SetVelocityInLocalSpace(FVector::ZeroVector);
	ProjectileMovement->StopMovementImmediately();
	//GetStaticMeshComponent()->SetSimulatePhysics(false);
	isThrowing = false;
	bossActor->CallBackYari();
	TriggerCapsule->SetCollisionProfileName("NoCollision");
	if (GetStaticMeshComponent()->IsSimulatingPhysics())GetStaticMeshComponent()->SetSimulatePhysics(false);
}

void AYari::StickToLocation(FVector location)
{
	FHitResult HitResult;
	SetActorLocation(UKismetMathLibrary::VLerp(GetActorLocation(),location, GetWorld()->DeltaTimeSeconds * 3.0f), true, &HitResult);
}


void AYari::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Box Collider
	if (OtherComp->ComponentHasTag("Floor"))
	{	
		UE_LOG(LogTemp, Warning, TEXT("Floor"));
		
		//if (isFire)
		//{
		//	for (int i = 0; i < bossActor->bloodPoolList.Num(); i++)
		//	{
		//		if (FVector::Distance(TriggerBox->GetComponentLocation(), bossActor->bloodPoolList[i]->GetActorLocation()) <= pushBackRadius)
		//		{
		//			auto Minion = GetWorld()->SpawnActor<AMinion>(bossActor->MinionBlueprint->GetOwnerClass(), bossActor->bloodPoolList[i]->GetActorLocation(), FRotator::ZeroRotator);
		//			ABloodPool* bp = Cast<ABloodPool>(bossActor->bloodPoolList[i]);
		//			bp->destroySpeed = 2.0f;
		//			Minion->Initialize(isFire);
		//			bossAI->AddFireMinion(Minion);
		//			//delete bp;
		//		}
		//	}
		//}		
		//GetWorld()->GetTimerManager().SetTimer(StopTimer, this, &AYari::StopYari, 0.03f);
		StopYari();
		//Play rock impact PE
		
		
		SetFireEffectEnable(false);
		
		// AOE when hit ground
		//if (!bossActor->isDamaged)
		//{
		//	/*if (FVector::Distance(TriggerBox->GetComponentLocation(), playerCharacter->GetActorLocation()) <= pushBackRadius)
		//	{
		//		if (!playerActor->isPushingBack)
		//		{
		//			playerActor->Pushback(800.0f, TriggerBox->GetComponentLocation(), EPushbackTypeEnum::EP_Medium);
		//			bossActor->DamagePlayer(20.0f);
		//		}
		//	}*/

		
		bossActor->AOEDamage(TriggerBox->GetComponentLocation(),40.0f, pushBackRadius);
		PlayYariHitGroundParticles(1.0f);
		//}		
	}
}

void AYari::OnOverlapPlayer(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Capsule collider
	if (OtherComp->ComponentHasTag("Player"))
	{
		bossActor->SetIsDamaged(true);
		if (bossActor->isYariThrow)
		{		
			playerActor->Pushback(800.0f, GetActorLocation(), EPushbackTypeEnum::EP_Medium,true);
			bossActor->DamagePlayer(40.0f, false);
		}	
		else
		{
			// Yari Attack
			playerActor->Pushback(400.0f, bossActor->GetActorLocation(), EPushbackTypeEnum::EP_Short,true);
			bossActor->DamagePlayer(20.0f, false);
		}		
	}
}

void AYari::PlayYariCallBackParticle()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), callbackParticle, GetActorLocation(), FRotator::ZeroRotator, true, EPSCPoolMethod::None);
}

void AYari::PlayYariHitGroundParticles(float scale)
{	
	//groundcrack
	FHitResult groundHit;
	
	FVector spawnLocation;
	FVector startPos = GetActorLocation();
	USphereComponent* AOECollision= nullptr;
	TArray<class USphereComponent*> bossComponentsArray;
	bossActor->GetComponents<class USphereComponent>(bossComponentsArray);
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), startPos, startPos - FVector(0, 0, 300), ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, groundHit, true))
	{
		for (auto& component : bossComponentsArray)
		{
			if (component->GetFName() == TEXT("AOECollision"))
			{
				AOECollision = component;	
			}
		}
		if (AOECollision)
		{
			
			FVector aoeLocation = AOECollision->K2_GetComponentLocation();
			spawnLocation = FVector(aoeLocation.X, aoeLocation.Y, groundHit.Location.Z + 5);
			
		}
		else
		{
			return;
		}
			
		
		
	}
	FTransform tempTransform = FTransform(FRotator::ZeroRotator, spawnLocation, FVector(scale, scale, 1.0f));
	//ground impact
	UParticleSystemComponent* ground = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), groundImpactParticle, spawnLocation,FRotator::ZeroRotator, true, EPSCPoolMethod::None);
	
	//shockwave
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), shockwaveImpactParticle, tempTransform, true, EPSCPoolMethod::None);

	FRotator crackRotation = GetActorRotation();
	FTransform crackTransform = FTransform(crackRotation, spawnLocation, FVector(scale, scale, 1.0f));
	UParticleSystemComponent* groundCrack= UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), groundCrackParticle, crackTransform, true, EPSCPoolMethod::None);
	
	UParticleSystemComponent* sparksParticle=UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), groundImpactSparkParticle, spawnLocation,FRotator::ZeroRotator, true, EPSCPoolMethod::None);
	sparksParticle->SetFloatParameter("respawnRadius", scale*300);
	sparksParticle->SetFloatParameter("burstScale", scale);
	
	//groundCrack->SetFloatParameter("CrackRotation", crackRotation.Yaw);
}

void AYari::SetInitPos(FVector location, FRotator rot)
{
	initLocation = location;
	initRotation = rot;
}

void AYari::SetLocalPos()
{
	SetActorRelativeLocation(initLocation, false, nullptr, ETeleportType::None);
	SetActorRelativeRotation(initRotation, false, nullptr, ETeleportType::None);
}

void AYari::BackToHandSocket(float DeltaSeconds)
{
	TriggerCapsule->SetCollisionProfileName("YariAttackPreset");
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), bossActor->handSocketLocation, DeltaSeconds, 3.0f));
	float distance = FVector::Distance(GetActorLocation(), bossActor->handSocketLocation);
	if (distance <= 100.0f)
	{
		acceleration = FVector::ZeroVector;
		currentVelocity = FVector::ZeroVector;
		bossActor->SetIsDamaged(false);
		TriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
		TriggerCapsule->SetCollisionProfileName("NoCollision");
		FName handSocketName = TEXT("WeaponSocket");
		if(GetStaticMeshComponent()->IsSimulatingPhysics())GetStaticMeshComponent()->SetSimulatePhysics(false);
		K2_AttachRootComponentTo(bossActor->skeletalMesh, handSocketName, EAttachLocation::SnapToTarget,false);
		//GetStaticMeshComponent()->AttachToComponent(bossActor->skeletalMesh, FAttachmentTransformRules::KeepWorldTransform, handSocketName);
		isBack = false;
		SetActorRelativeLocation(initLocation, false, nullptr, ETeleportType::ResetPhysics);
		SetActorRelativeRotation(initRotation, false, nullptr, ETeleportType::ResetPhysics);
		bossActor->PlayGrabYari();
	}
}

void AYari::SetYariCollision(bool b)
{
	if(b) TriggerCapsule->SetCollisionProfileName("YariAttackPreset");
	else TriggerCapsule->SetCollisionProfileName("NoCollision");
}

void AYari::RotateTowardsPlayer()
{
	playerLocation = playerCharacter->GetActorLocation();
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), playerLocation);
	rot.Pitch = rot.Pitch - 90.0f;
	SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), rot, GetWorld()->DeltaTimeSeconds * 2.0f, true));
}

void AYari::SetYariCollisionSize(float halfHeight, float radius)
{
	//TriggerCapsule->InitCapsuleSize(radius, halfHeight);
	TriggerCapsule->SetCapsuleSize(radius, halfHeight, false);
}
