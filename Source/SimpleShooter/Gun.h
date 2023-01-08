// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root; // Gun's root component

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh; // Gun's actual mesh

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash; // Particle effect that spawns on bullet firing

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound; // Sound effect that spawns on bullet firing

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect; // Particle effect that spawns on bullet impact

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound; // Sound effect that spawns on bullet impact

	UPROPERTY(EditAnywhere)
	float MaxRange = 10000.f; // Max distance this gun's bullet's will travel

	UPROPERTY(EditAnywhere)
	float Damage = 10.f; // Damage inflicted by this gun

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection); // perform a line trace to see if the gun should hit something

	AController* GetOwnerController() const; // get the controller responsible for the gun
};
