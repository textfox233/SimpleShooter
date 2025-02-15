// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Spawn a gun
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	// Delete default gun from mesh
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);
	// attach new gun
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	// set owner to this character (multiplayer & dmg)
	Gun->SetOwner(this);

	// initialise health property
	Health = MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// process damage in parent
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// update health property
	Health -= DamageToApply;
	
	if (Health < 0.f) // clamp it above minus numbers
	{
		Health = 0.f; 
	}

	UE_LOG(LogTemp, Warning, TEXT("Health is now: %f"), Health);

	if (IsDead())
	{
		// get the game mode
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();

		// if GameMode's not a nullptr let it know the pawn's been killed
		if (GameMode)
		{
			GameMode->PawnKilled(this);
		}

		// detach controller so that the pawn stays dead
		DetachFromControllerPendingDestroy();

		// remove the capsule so it can be walked over and not consume block any shots
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
	// return true if health is equal to 0 (roughly equal; float)
	return (Health > -0.1f && Health < 0.1f);
}

// Informs the health bar
float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

// --- Player Input --- //

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// --- AXIS EVENTS --- //

	// "MoveForward" should use W/S keys OR Controller Leftstick Y-Axis
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);

	// "MoveRight" should use A/D keys OR Controller Leftstick X-Axis
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);

	// "LookUp" should use mouse Y input
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	
	// "LookUpRate" should use Controller Rightstick Y-Axis
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);

	// "Turn" should use mouse X input
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);

	// "TurnRate" should use Controller Rightstick X-Axis
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AShooterCharacter::TurnRate);


	// --- ACTION EVENTS --- //

	// "Jump" should use spacebar key
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	// "Shoot" should use spacebar key
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}

// Positive input is forward
void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

// Positive input is right
void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::TurnRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}