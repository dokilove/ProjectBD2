// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_ZombieMove.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy/MyZombie.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotifyState_ZombieMove::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	AMyZombie* Zombie = Cast<AMyZombie>(MeshComp->GetOwner());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->WalkSpeed;
	}
}

void UAnimNotifyState_ZombieMove::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AMyZombie* Zombie = Cast<AMyZombie>(MeshComp->GetOwner());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = 3.0f;
	}
}
