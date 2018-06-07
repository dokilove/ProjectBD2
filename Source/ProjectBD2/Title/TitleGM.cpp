// Fill out your copyright notice in the Description page of Project Settings.

#include "TitleGM.h"
#include "Title/TitlePC.h"
#include "UObject/ConstructorHelpers.h"

ATitleGM::ATitleGM()
{
	static  ConstructorHelpers::FClassFinder<ATitlePC> PC_Class(TEXT("Blueprint'/Game/Blueprints/Title/BP_TitlePC.BP_TitlePC_C'"));
	if (PC_Class.Succeeded())
	{
		PlayerControllerClass = PC_Class.Class;
	}
}

