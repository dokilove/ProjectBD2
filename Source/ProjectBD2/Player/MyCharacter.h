// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class PROJECTBD2_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void LookUp(float Value);
	UFUNCTION()
		void Turn(float Value);
	UFUNCTION()
		void TryCrouch();
	UFUNCTION()
		void TryIronsight();
	void SetIronsightSpeed();
	void ReleaseIronsightSpeed();
	UFUNCTION()
		void TryProne();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		bool bIsProne;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UWeaponComponent* Weapon;
		
	FRotator GetAimoffset() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		bool bIsIronsight = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		float JogSpeed = 360.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		float WalkSpeed = 160.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		float SprintSpeed = 700.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		float ProneSpeed = 50.0f;

	UFUNCTION()
		void Sprint();
	UFUNCTION()
		void UnSprint();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
		bool bIsSprint;
	UFUNCTION()
		void LookAround();
	UFUNCTION()
		void LookForward();
	UFUNCTION()
		void StartFire();
	UFUNCTION()
		void StopFire();
	void OnShot();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		bool bIsFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		bool bIsMovingLocked = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		FVector NormalSpringArmPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		FVector CrouchSpringArmPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		FVector ProneSpringArmPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class USoundBase* RifleSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* RifleMuzzle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* HitEffect;

	FTimerHandle FireTimeHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info")
		float MaxHP = 100.0f;
};
