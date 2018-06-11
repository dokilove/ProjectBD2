// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BattleCharacter.generated.h"

UCLASS()
class PROJECTBD2_API ABattleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABattleCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_DeadProgress();
	void S2A_DeadProgress_Implementation();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	UFUNCTION(Server, Reliable, WithValidation)
		void C2S_Ironsight();
	bool C2S_Ironsight_Validate();
	void C2S_Ironsight_Implementation();

	void SetIronsightSpeed();
	void ReleaseIronsightSpeed();
	UFUNCTION()
		void TryProne();
	UFUNCTION(Server, Reliable, WithValidation)
		void C2S_Prone();
	bool C2S_Prone_Validate();
	void C2S_Prone_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", Replicated)
		bool bIsProne;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWeaponComponent* Weapon;

	FRotator GetAimoffset() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", Replicated)
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
	UFUNCTION(Server, Reliable, WithValidation)
		void C2S_Sprint();
	bool C2S_Sprint_Validate();
	void C2S_Sprint_Implementation();
	UFUNCTION()
		void UnSprint();
	UFUNCTION(Server, Reliable, WithValidation)
		void C2S_UnSprint();
	bool C2S_UnSprint_Validate();
	void C2S_UnSprint_Implementation();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", Replicated)
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

	UFUNCTION(Server, Reliable, WithValidation)
		void C2S_OnShot(FVector TraceStart, FVector TraceEnd);
	bool C2S_OnShot_Validate(FVector TraceStart, FVector TraceEnd);
	void C2S_OnShot_Implementation(FVector TraceStart, FVector TraceEnd);

	UFUNCTION(NetMulticast, Reliable)
		void S2A_MuzzleEffectAndSound();
	void S2A_MuzzleEffectAndSound_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void S2A_HitEffect(const FHitResult& OutHit);
	void S2A_HitEffect_Implementation(const FHitResult& OutHit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info", Replicated)
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* BloodEffect;

	FTimerHandle FireTimeHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info", Replicated)
		float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Info", Replicated)
		float MaxHP = 100.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimMontage* DeadAnim;

	TArray<class AMasterItem*> CanPickupList;

	void AddPickupItemList(class AMasterItem* Item);
	void RemovePickupItemList(class AMasterItem* Item);
	void ViewItemTooltip();
	AMasterItem * GetClosestItem();
	UFUNCTION()
		void GetItem();
	UFUNCTION()
		void Inventory();
	
};
