// Fill out your copyright notice in the Description page of Project Settings.


#include "RingActor.h"

// Sets default values
ARingActor::ARingActor()
{
    RingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring"));
    SetRootComponent(RingMesh);

    RingScoreCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("RingScoreCollider"));
    RingScoreCollider->SetupAttachment(RingMesh);

}

// Called when the game starts or when spawned
void ARingActor::BeginPlay()
{
    Super::BeginPlay();

}
