// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create root scene & mesh components
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	
	// Set up attachments
	SetRootComponent(Root);
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// get the controller responsible for the gun
AController* AGun::GetOwnerController() const 
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner()); // Pawn with ownership of this gun
	if (!OwnerPawn) // Return early if null, avoid an unreal crash
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPawn Null - GetOwnerController"))
		return nullptr;
	}
	return OwnerPawn->GetController(); // Controller responsible for this gun
}

// perform a line trace to see if the gun should hit something
bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	//APawn* OwnerPawn = Cast<APawn>(GetOwner()); // Pawn with ownership of this gun
	//if (!OwnerPawn) return; // Return early if null, avoid an unreal crash
	//
	//AController* OwnerController = OwnerPawn->GetController(); // Controller responsible for this gun
	//if (!OwnerController) return; // Return early if null, avoid an unreal crash

	// get the owner controller
	AController* OwnerController = GetOwnerController();
	if (!OwnerController) // null check
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerController Null - GunTrace"))
		return false;
	}

	// --- Get Player Viewpoint --- //
	FVector Location;	// Player viewpoint's location
	FRotator Rotation;	// Player viewpoint's rotation
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	// End point for the gun trajectory line trace
	FVector End = Location + Rotation.Vector() * MaxRange;

	// Add exceptions to the query parameters (don't hit self, etc)
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	ShotDirection = -Rotation.Vector();

	// If a hit actually occurs...
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECC_GameTraceChannel1, Params);
}

// What happens on trigger pull
void AGun::PullTrigger()
{
	// spawn muzzle flash visual
	if (MuzzleFlash) // null check
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	}
	// spawn muzzle flash sound
	if (MuzzleSound) // null check
	{
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	}

	// get the controller responsible for the gun
	AController* OwnerController = GetOwnerController();
	if (!OwnerController) // null check
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerController Null - PullTrigger"))
		return;
	}

	FHitResult HitResult;	// Result of the gun's trajectory
	FVector ShotDirection;	// Where the shot's coming from

	// perform a line trace to see if the gun should hit something
	bool bSuccess = GunTrace(HitResult, ShotDirection);
	
	// If a hit actually occurs apply damage
	if (bSuccess)
	{
		// Object handling this event
		FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);

		// Tell the actor hit to take damage
		if (HitResult.GetActor())
		{
			HitResult.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}

		// Spawn impact visual
		if (ImpactEffect) // Check for null values, avoid an unreal crash
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDirection.Rotation());
		}
		// spawn impact sound
		if (ImpactSound) // null check
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, HitResult.Location, ShotDirection.Rotation());
		}
		// --- DEBUG CORNER --- //
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20, 16, FColor::Yellow, true);
		//DrawDebugCamera(GetWorld(), Location, Rotation, 90.f, 2,FColor::Red, true);
		//UE_LOG(LogTemp, Warning, TEXT("You've been shot!"))
	}
}
