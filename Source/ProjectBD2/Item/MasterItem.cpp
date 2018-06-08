// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Item/ItemDataTableComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/StaticMesh.h"
#include "Player/MyCharacter.h"
#include "UnrealNetwork.h"

AMasterItem::AMasterItem()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetStaticMeshComponent());
	Sphere->SetSphereRadius(200.0f);
	Sphere->bGenerateOverlapEvents = true;

	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

	ItemDataTable = CreateDefaultSubobject<UItemDataTableComponent>(TEXT("ItemDataTable"));

	bReplicates = true;
}

void AMasterItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemDataTable && ItemDataTable->DataTable)
	{
		if (HasAuthority())
		{
			ItemIndex = FMath::RandRange(1, 6) * 10;
		}
		FItemDataTable Data = ItemDataTable->GetItemData(ItemIndex);
		ItemCount = Data.ItemCount;

		FStreamableManager AssetLoader;
		GetStaticMeshComponent()->SetStaticMesh(AssetLoader.LoadSynchronous<UStaticMesh>(Data.ItemMesh));

		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMasterItem::OnBeginOverlap);
		Sphere->OnComponentEndOverlap.AddDynamic(this, &AMasterItem::OnEndOverlap);
	}
}

void AMasterItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMasterItem, ItemIndex);
}

void AMasterItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//UE_LOG(LogClass, Warning, TEXT("OnBeginOverlap : %s"), *this->GetName());

	if (OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		AMyCharacter* Pawn = Cast<AMyCharacter>(OtherActor);
		if (Pawn)
		{
			Pawn->AddPickupItemList(this);
		}
	}

}

void AMasterItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	//UE_LOG(LogClass, Warning, TEXT("OnEndOverlap : %s"), *this->GetName());
	if (OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		AMyCharacter* Pawn = Cast<AMyCharacter>(OtherActor);
		if (Pawn)
		{
			Pawn->RemovePickupItemList(this);
		}
	}
}
