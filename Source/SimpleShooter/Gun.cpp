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


// Called to bind functionality to input
//void AGun::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
//{
//	// "Shoot" should use spacebar key
//	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AGun::PullTrigger);
//}

// What happens on trigger pull
void AGun::PullTrigger()
{
	if (MuzzleFlash) // Check for null values, avoid an unreal crash
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner()); // Pawn with ownership of this gun
	if (!OwnerPawn) return; // Return early if null, avoid an unreal crash

	AController* OwnerController = OwnerPawn->GetController(); // Controller responsible for this gun
	if (!OwnerController) return; // Return early if null, avoid an unreal crash

	// --- Get Player Viewpoint --- //
	FVector Location;	// Player viewpoint's location
	FRotator Rotation;	// Player viewpoint's rotation
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FHitResult HitResult; // Result of the gun's trajectory

	// End point for the gun trajectory line trace
	FVector End = Location + Rotation.Vector() * MaxRange;
	
	// If a hit actually occurs...
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECC_GameTraceChannel1))
	{
		// --- Apply Damage --- //

		// Where the shot's coming from
		FVector ShotDirection = -Rotation.Vector();

		// Object handling this event
		FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);

		// Tell the actor hit to take damage
		if (HitResult.GetActor())
		{
			HitResult.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}

		// Spawn an effect on impact
		if (ImpactEffect) // Check for null values, avoid an unreal crash
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDirection.Rotation());
		}

		// --- DEBUG CORNER --- //
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20, 16, FColor::Red, true);
		//DrawDebugCamera(GetWorld(), Location, Rotation, 90.f, 2,FColor::Red, true);

		UE_LOG(LogTemp, Warning, TEXT("You've been shot!"))
	}
}
