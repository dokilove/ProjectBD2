// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Male(TEXT("SkeletalMesh'/Game/TPSData/Male_Grunt/Mesh/male_grunt.male_grunt'"));
	if (SK_Male.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Male.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0,
		-GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	bUseControllerRotationPitch = false;
	SpringArm->bUsePawnControlRotation = true;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// _C중요
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim_Class(TEXT("AnimBlueprint'/Game/Blueprints/Animation/ABP_Male.ABP_Male_C'"));
	if (Anim_Class.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Class.Class);
	}

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this,
		&AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this,
		&AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this,
		&AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this,
		&AMyCharacter::Turn);
}

void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}
void AMyCharacter::LookUp(float Value)
{
	if (Value != 0.0f)
	{		
		AddControllerPitchInput(Value);
		//UE_LOG(LogClass, Warning, TEXT("Look Up %f"), Value);
	}
}
void AMyCharacter::Turn(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value);
		//UE_LOG(LogClass, Warning, TEXT("Turn %f"), Value);
	}
}

