// Fill out your copyright notice in the Description page of Project Settings.

#include "MyZombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AMyZombie::AMyZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("SkeletalMesh'/Game/Zombie/Mesh/Zombie.Zombie'"));
	if (SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mesh.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0,
		-GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	FStringClassReference AnimRef(TEXT("AnimBlueprint'/Game/Blueprints/Enemy/ABP_Zombie.ABP_Zombie_C'"));
	if (UClass* AnimInstance = AnimRef.TryLoadClass<UAnimInstance>())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstance);
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

}

// Called when the game starts or when spawned
void AMyZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

