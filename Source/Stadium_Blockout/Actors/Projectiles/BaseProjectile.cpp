// Fill out your copyright notice in the Description page of Project Settings.



#include "BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Stadium_Blockout/Interfaces/DamageInterface.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(15.f);
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComp;
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->bAutoActivate = false;
	ProjectileMovementComp->InitialSpeed = 0.0f;
	ProjectileMovementComp->MaxSpeed = HomingSpeed;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	ProjectileMovementComp->bIsHomingProjectile = true;
	ProjectileMovementComp->HomingAccelerationMagnitude = HomingAcceleration;
	
	InitialLifeSpan = 10.f;
}

void ABaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CollisionComp->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
}


// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	float Delay = FMath::RandRange(MinDelayRange, MaxDelayRange);
	GetWorld()->GetTimerManager().SetTimer(ActivationTimerHandle, this, &ABaseProjectile::ActivateProjectile, Delay, false);
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseProjectile::ActivateProjectile()
{
	if (!TargetActor)
	{
		TargetActor = FindTargetBySweep();
	}
	if (TargetActor)
	{
		ProjectileMovementComp->HomingTargetComponent = TargetActor->GetRootComponent();
		FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		ProjectileMovementComp->Velocity =  Direction * (HomingSpeed * .5f);
	}
	else
	{
		ProjectileMovementComp->bIsHomingProjectile = false;
		ProjectileMovementComp->Velocity = GetActorForwardVector() * HomingSpeed;
	}
	
	ProjectileMovementComp->MaxSpeed = HomingSpeed;
	ProjectileMovementComp->Activate();
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator()) return;
	
	if (OtherActor->Implements<UDamageInterface>())
	{
		IDamageInterface::Execute_TakeDamage(OtherActor, ProjectileDamage, GetInstigator());
	}
	
	Destroy();
}

AActor* ABaseProjectile::FindTargetBySweep()
{
	FVector Start = GetActorLocation();
	
	TArray<FOverlapResult> HitResults;
	
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(GetInstigator());
	
	bool bHit = GetWorld()->OverlapMultiByObjectType(HitResults, Start, FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(TraceRadius), Params);
	
	DrawDebugSphere(GetWorld(), Start, TraceRadius, 16, FColor::Cyan, false, 2.0f);
	
	if (bHit)
	{
		for (const FOverlapResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			
			if (HitActor && HitActor->Implements<UDamageInterface>())
			{
				return HitActor;
			}
		}
	}
	return nullptr;
}


