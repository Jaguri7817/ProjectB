// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PBCharacterBase.h"
#include "PBCharacterBase.h"


// Sets default values
APBCharacterBase::APBCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	

}

void APBCharacterBase::Die()
{
	SetDead();
}

// Called when the game starts or when spawned
void APBCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APBCharacterBase::SetDead()
{

}

void APBCharacterBase::SpawnCharacter()
{

}


