// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "TimerManager.h"
#include "Basic/RifleCameraShake.h"
#include "Basic/BulletDamageType.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Item/MasterItem.h"
#include "Basic/BasicPC.h"
#include "UI/ItemTooltipWidgetBase.h"
#include "Components/TextBlock.h"
#include "Item/ItemDataTableComponent.h"
#include "UI/InventoryWidgetBase.h"
#include "UI/ItemSlotWidgetBase.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0, 30, 70));
	SpringArm->TargetArmLength = 150.0f;

	NormalSpringArmPosition = SpringArm->GetRelativeTransform().GetLocation();
	CrouchSpringArmPosition = FVector(NormalSpringArmPosition.X, NormalSpringArmPosition.Y, NormalSpringArmPosition.Z - 40);
	ProneSpringArmPosition = FVector(100, NormalSpringArmPosition.Y, NormalSpringArmPosition.Z - 100);

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
	//static ConstructorHelpers::FClassFinder<UAnimInstance> Anim_Class(TEXT("AnimBlueprint'/Game/Blueprints/Animation/ABP_Male.ABP_Male_C'"));
	//if (Anim_Class.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(Anim_Class.Class);
	//}

	FStringClassReference Anim_Male_Ref(TEXT("AnimBlueprint'/Game/Blueprints/Animation/ABP_Male.ABP_Male_C'"));
	if (UClass* Anim_Male_Class = Anim_Male_Ref.TryLoadClass<UAnimInstance>())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Male_Class);
	}

	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("RHandWeapon"));


	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	static ConstructorHelpers::FObjectFinder<USoundBase> S_RifleSound(TEXT("SoundCue'/Game/TPSData/Sound/Weapons/SMG_Thompson/Cue_Thompson_Shot.Cue_Thompson_Shot'"));
	if (S_RifleSound.Succeeded())
	{
		RifleSound = S_RifleSound.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_RifleMuzzle(TEXT("ParticleSystem'/Game/TPSData/Effects/P_AssaultRifle_MF.P_AssaultRifle_MF'"));
	if (P_RifleMuzzle.Succeeded())
	{
		RifleMuzzle = P_RifleMuzzle.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HitEffect(TEXT("ParticleSystem'/Game/TPSData/Effects/P_AssaultRifle_IH.P_AssaultRifle_IH'"));
	if (P_HitEffect.Succeeded())
	{
		HitEffect = P_HitEffect.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_BloodEffect(TEXT("ParticleSystem'/Game/TPSData/Effects/P_body_bullet_impact.P_body_bullet_impact'"));
	if (P_BloodEffect.Succeeded())
	{
		BloodEffect = P_BloodEffect.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Anim_Dead(TEXT("AnimMontage'/Game/TPSData/Male_Grunt/Animations/AnimMontage/Death_1_Montage.Death_1_Montage'"));
	if (Anim_Dead.Succeeded())
	{
		DeadAnim = Anim_Dead.Object;
	}

	Tags.Add(FName(TEXT("Player")));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkSpeed;

	CurrentHP = MaxHP;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogClass, Warning, TEXT("Crouch %f"), SpringArm->GetComponentLocation().Z);
	//UE_LOG(LogClass, Warning, TEXT("eyeheight %f"), CrouchedEyeHeight);

	if (GetCharacterMovement()->Velocity.Size() <= 0.0f)
	{
		UnSprint();
	}
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

	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::TryCrouch);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::TryIronsight);
	PlayerInputComponent->BindAction(TEXT("Prone"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::TryProne);
	PlayerInputComponent->BindAction(TEXT("GetItem"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::GetItem);
	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::Inventory);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this,
		&AMyCharacter::UnSprint);

	PlayerInputComponent->BindAction(TEXT("LookAround"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::LookAround);
	PlayerInputComponent->BindAction(TEXT("LookAround"), EInputEvent::IE_Released, this,
		&AMyCharacter::LookForward);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this,
		&AMyCharacter::StopFire);
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)&DamageEvent;
		UE_LOG(LogClass, Warning, TEXT("FRadialDamageEvent %f"), DamageAmount);
	}
	else if(DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		UE_LOG(LogClass, Warning, TEXT("FPointDamageEvent %f %s"), DamageAmount, *PointDamageEvent->HitInfo.BoneName.ToString());

		if (PointDamageEvent->HitInfo.BoneName.Compare(FName("head")) == 0)
		{
			CurrentHP = 0;
		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		UE_LOG(LogClass, Warning, TEXT("Damage %f"), DamageAmount);
	}


	CurrentHP -= DamageAmount;
	if (CurrentHP <= 0.0f)
	{
		CurrentHP = 0;
		//GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DisableInput(Cast<APlayerController>(GetController()));

		if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(DeadAnim))
		{
			PlayAnimMontage(DeadAnim);
		}
	}

	return 0.0f;
}

void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && !bIsMovingLocked)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && !bIsSprint && !bIsMovingLocked)
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

void AMyCharacter::TryCrouch()
{
	if (CanCrouch() && !bIsSprint && !bIsProne)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AMyCharacter::TryIronsight()
{
	if (bIsSprint)
	{
		return;
	}

	bIsIronsight = bIsIronsight ? false : true;

	if (bIsProne)
	{
		return;
	}

	if (bIsIronsight)
	{
		SetIronsightSpeed();
	}
	else
	{
		ReleaseIronsightSpeed();
	}
}

void AMyCharacter::SetIronsightSpeed()
{

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkSpeed / 2;
}

void AMyCharacter::ReleaseIronsightSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkSpeed;
}

void AMyCharacter::TryProne()
{
	if (!bIsProne)
	{
		if (bIsSprint)
		{
			return;
		}

		bIsProne = true;
		GetCharacterMovement()->MaxWalkSpeed = ProneSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = ProneSpeed;
	}
	else
	{
		bIsProne = false;
		if (bIsIronsight)
		{
			SetIronsightSpeed();
		}
		else
		{
			ReleaseIronsightSpeed();
		}
	}
	UnCrouch();
}

FRotator AMyCharacter::GetAimoffset() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();
	return AimRotLS;
}

void AMyCharacter::Sprint()
{
	if (!bIsCrouched && !bIsIronsight)
	{
		bIsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AMyCharacter::UnSprint()
{
	bIsSprint = false;
	if (!bIsProne)
	{
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
}

void AMyCharacter::LookAround()
{
	if (!bIsIronsight)
	{
		bUseControllerRotationYaw = false;
	}
}

void AMyCharacter::LookForward()
{
	bUseControllerRotationYaw = true;
}

void AMyCharacter::StartFire()
{
	if (!bIsSprint && !bIsMovingLocked)
	{
		bIsFire = true;
		OnShot();
	}
}

void AMyCharacter::StopFire()
{
	bIsFire = false;
}

void AMyCharacter::OnShot()
{
	if (!bIsFire)
	{
		return;
	}

	FVector CameraLocation;
	FRotator CameraRotation;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(CameraLocation, CameraRotation);

	int SizeX;
	int SizeY;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewportSize(SizeX, SizeY);

	FVector WorldLocation;
	FVector WorldDirection;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(SizeX / 2, SizeY / 2, WorldLocation, WorldDirection);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (WorldDirection * 80000.0f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreObject;
	FHitResult OutHit;


	FTransform MuzzleTransform = Weapon->GetSocketTransform(TEXT("MuzzleFlash"));

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	IgnoreObject.Add(this);

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd,
		ObjectTypes, false, IgnoreObject, EDrawDebugTrace::None,
		OutHit, true, FLinearColor::Blue, FLinearColor::Blue, 5.0f);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RifleMuzzle, MuzzleTransform);

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), RifleSound, MuzzleTransform.GetLocation(),
		MuzzleTransform.GetRotation().Rotator());

	if (Result)
	{
		TraceStart = MuzzleTransform.GetLocation();
		TraceEnd = TraceStart + (OutHit.Location - TraceStart) * 2.0f;
		
		bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd,
			ObjectTypes, false, IgnoreObject, EDrawDebugTrace::None,
			OutHit, true, FLinearColor::Green, FLinearColor::Red, 5.0f);
		if (Result)
		{
			UGameplayStatics::ApplyDamage(OutHit.GetActor(), 0.0f, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, UBulletDamageType::StaticClass());

			UGameplayStatics::ApplyRadialDamage(GetWorld(), 0.0f, OutHit.ImpactPoint, 300.0f, UBulletDamageType::StaticClass(), IgnoreObject, this, UGameplayStatics::GetPlayerController(GetWorld(), 0), false);

			UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), 30.0f, OutHit.ImpactPoint - TraceStart, OutHit,
				UGameplayStatics::GetPlayerController(GetWorld(), 0), this, UBulletDamageType::StaticClass());

			APawn* Pawn = Cast<APawn>(OutHit.GetActor());
			if (Pawn)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, OutHit.Location,
					OutHit.ImpactNormal.Rotation());
			}
			else
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, OutHit.Location,
					OutHit.ImpactNormal.Rotation());
			}
		}
	}

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(URifleCameraShake::StaticClass());
	FRotator CurrentRotation = GetControlRotation();
	CurrentRotation.Pitch += 1.0f;
	CurrentRotation.Yaw += FMath::FRandRange(-0.05f, 0.05f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(CurrentRotation);

	if (bIsFire)
	{
		GetWorldTimerManager().SetTimer(FireTimeHandle, this, &AMyCharacter::OnShot, 0.1f);
	}
}

void AMyCharacter::AddPickupItemList(AMasterItem * Item)
{
	if (Item && !Item->IsPendingKill())
	{
		CanPickupList.Add(Item);
	}

	ViewItemTooltip();
}

void AMyCharacter::RemovePickupItemList(AMasterItem * Item)
{
	if (Item)
	{
		CanPickupList.Remove(Item);
	}

	ViewItemTooltip();
}

void AMyCharacter::ViewItemTooltip()
{
	ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}

	if (CanPickupList.Num() == 0)
	{
		PC->ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	AMasterItem* ClosestItem = GetClosestItem();
	if (ClosestItem)
	{
		PC->ItemTooltip->ItemName->SetText(FText::FromString(ClosestItem->ItemDataTable->GetItemData(ClosestItem->ItemIndex).ItemName));
		PC->ItemTooltip->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PC->ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

AMasterItem* AMyCharacter::GetClosestItem()
{
	AMasterItem* ClosestItem = nullptr;

	float Min = 9999999999.9f;
	for (auto Item : CanPickupList)
	{
		float Distance = FVector::Dist(Item->GetActorLocation(), GetActorLocation());
		if (Min > Distance)
		{
			Min = Distance;
			ClosestItem = Item;
		}
	}

	return ClosestItem;
}

void AMyCharacter::GetItem()
{
	AMasterItem* PickupItem = GetClosestItem();
	if (PickupItem && !PickupItem->IsPendingKill())
	{
		ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC)
		{
			UItemSlotWidgetBase* Slot = PC->Inventory->GetSameIDSlot(PickupItem->ItemIndex);
			if (Slot)
			{
				Slot->AddItemCount(PickupItem->ItemCount);
				PickupItem->Destroy();
				ViewItemTooltip();
			}
			else
			{
				UItemSlotWidgetBase* NewSlot = PC->Inventory->GetEmptySlot();
				if (NewSlot)
				{
					NewSlot->SetItem(PickupItem->ItemIndex);
					PickupItem->Destroy();
					ViewItemTooltip();
				}
				else
				{
					UE_LOG(LogClass, Warning, TEXT("Inventory full"));
				}
			}
		}
	}
}

void AMyCharacter::Inventory()
{
	ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->ToggleInventory();
	}
}