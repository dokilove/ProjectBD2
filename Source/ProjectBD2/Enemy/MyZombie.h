// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyZombie.generated.h"

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Normal UMETA(Display = "Normal"),
	Chase UMETA(Display = "Chase"),
	Battle UMETA(Display = "Battle"),
	Dead UMETA(Display = "Dead"),
};

UENUM(BlueprintType)
enum class EZombieAnimState : uint8
{
	Idle UMETA(Display = "Idle"),
	Walk UMETA(Display = "Walk"),
	Run UMETA(Display = "Run"),
	Scream UMETA(Display = "Scream"),
	Death UMETA(Display = "Death"),
	Attack01 UMETA(Display = "Attack01"),
	Attack02 UMETA(Display = "Attack02"),
};

UCLASS()
class PROJECTBD2_API AMyZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieAnimState CurrentAnimState;

};
