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
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	AController* OwnerController = OwnerPawn->GetController();
	if (!OwnerController) return;

	// Get Player Viewpoint
	FVector Location;	// OUT parameter
	FRotator Rotation; // OUT parameter
	OwnerController->GetPlayerViewPoint(Location, Rotation);


	FHitResult HitResult; // OUT parameter
	// Get an end point for line tracing
	FVector End = Location + Rotation.Vector() * MaxRange;
		
	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECC_GameTraceChannel1)) return; // return if no hit is found

	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20, 16, FColor::Red, true);

	//DrawDebugCamera(GetWorld(), Location, Rotation, 90.f, 2,FColor::Red, true);

	UE_LOG(LogTemp,Warning, TEXT("You've been shot!"))
}
