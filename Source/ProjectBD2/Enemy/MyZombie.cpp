// Fill out your copyright notice in the Description page of Project Settings.

#include "MyZombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy/ZombieAIController.h"
#include "Enemy/PatrolPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Player/MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/UI/ZombieHPBarWidgetBase.h"

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

	AIControllerClass = AZombieAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BT_Zombie(TEXT("BehaviorTree'/Game/Blueprints/Enemy/AI/BT_Zombie.BT_Zombie'"));
	if (BT_Zombie.Succeeded())
	{
		BehaviorTree = BT_Zombie.Object;
	}

	CurrentState = EZombieState::Normal;
	CurrentAnimState = EZombieAnimState::Idle;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 1500.0f;
	PawnSensing->SetPeripheralVisionAngle(60.0f);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UZombieHPBarWidgetBase> WidgetClass(TEXT("WidgetBlueprint'/Game/Blueprints/Enemy/UI/ZombieHPBarWidget.ZombieHPBarWidget_C'"));
	if (WidgetClass.Succeeded())
	{
		Widget->SetWidgetClass(WidgetClass.Class);
	}
	Widget->SetDrawSize(FVector2D(100, 30));
	Widget->SetRelativeLocation(FVector(0, 0, 120));
}

// Called when the game starts or when spawned
void AMyZombie::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPoint::StaticClass(), OutActors);

	for (auto PatrolPoint : OutActors)
	{
		PatrolPoints.Add(Cast<APatrolPoint>(PatrolPoint));
	}

	PawnSensing->OnSeePawn.AddDynamic(this, &AMyZombie::OnSeePawn);
	PawnSensing->OnHearNoise.AddDynamic(this, &AMyZombie::OnHearNoise);
}

// Called every frame
void AMyZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CameraLocation;
	FRotator CameraRotation;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector Dir = CameraLocation - Widget->GetComponentLocation();
	Widget->SetWorldRotation(Dir.Rotation());

	UZombieHPBarWidgetBase* HPBar = Cast<UZombieHPBarWidgetBase>(Widget->GetUserWidgetObject());
	if (HPBar)
	{
		HPBar->CurrentHP = CurrentHP / MaxHP;
	}
}

// Called to bind functionality to input
void AMyZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMyZombie::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentState == EZombieState::Dead)
	{
		return 0.0f;
	}

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
		}
	}
	CurrentHP -= Damage;
	if (CurrentHP <= 0.0f)
	{
		CurrentHP = 0;

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetMesh()->SetSimulatePhysics(true);
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC && AIC->IsValidLowLevelFast())
		{
			CurrentState = EZombieState::Dead;
			AIC->BB_Zombie->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);
		}
		Widget->SetVisibility(false);
	}

	return 0.0f;
}

void AMyZombie::OnSeePawn(APawn* Pawn)
{
	//UE_LOG(LogClass, Warning, TEXT("see %s"), *Pawn->GetName());
	AMyCharacter* Player = Cast<AMyCharacter>(Pawn);
	if (Pawn && Pawn->IsValidLowLevelFast() && Player->CurrentHP > 0.0f && CurrentState == EZombieState::Normal)
	{
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC && AIC->IsValidLowLevelFast())
		{
			CurrentState = EZombieState::Chase;
			CurrentAnimState = EZombieAnimState::Run;

			AIC->BB_Zombie->SetValueAsObject(FName(TEXT("Target")), Player);
			AIC->BB_Zombie->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);
		}
	}

}

void AMyZombie::OnHearNoise(APawn* Pawn, const FVector& Location, float Volume)
{

}

void AMyZombie::OnAttack()
{
	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC && AIC->IsValidLowLevelFast())
	{
		AActor* Player = Cast<AActor>(AIC->BB_Zombie->GetValueAsObject(FName(TEXT("Target"))));

		UGameplayStatics::ApplyDamage(Player, AttackDamage, AIC, this, nullptr);
	}
}

