// Fill out your copyright notice in the Description page of Project Settings.

#include "MyZombieAnimInstance.h"
#include "Enemy/MyZombie.h"


UMyZombieAnimInstance::UMyZombieAnimInstance()
{

}

void UMyZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMyZombie* Zombie = Cast<AMyZombie>(TryGetPawnOwner());

	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		CurrentState = Zombie->CurrentState;
		CurrentAnimState = Zombie->CurrentAnimState;
	}

}

void UMyZombieAnimInstance::AnimNotify_OnAttack(UAnimNotify* Notify)
{
	//UE_LOG(LogClass, Warning, TEXT("OnAttack"));

	AMyZombie* Zombie = Cast<AMyZombie>(TryGetPawnOwner());

	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		Zombie->OnAttack();
	}
}