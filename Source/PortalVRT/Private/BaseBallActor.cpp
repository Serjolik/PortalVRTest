// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallActor.h"
#include <PortalVRT/PortalVRTCharacter.h>
#include <RingActor.h>

// Sets default values
ABaseBallActor::ABaseBallActor()
{
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
    SetRootComponent(BallMesh);

    BallCollider = CreateDefaultSubobject<USphereComponent>(TEXT("BallCollider"));
    BallCollider->SetupAttachment(BallMesh);

}

// Called when the game starts or when spawned
void ABaseBallActor::BeginPlay()
{
    Super::BeginPlay();
    BallCollider->OnComponentBeginOverlap.AddDynamic(this, &ABaseBallActor::OnSphereOverlapBegin);

}

void ABaseBallActor::SetLastPlayer(FString& playerName)
{
    lastPlayerName = playerName;
}

void ABaseBallActor::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor) {
        return;
    }

    EActorTagType TagType = GetTagType(OtherComp->ComponentTags);

    switch (TagType)
    {
    case EActorTagType::Player:
        character = Cast<APortalVRTCharacter>(OtherActor);

        if (character) {
            character->PickupBall(this);
        }
        break;

    case EActorTagType::Ring:
        ring = Cast<ARingActor>(OtherActor);

        if (ring) {
            ring->AddScore(lastPlayerName, scorePoints);
        }
        break;

    default:
        break;
    }
}

EActorTagType GetTagType(const TArray<FName>& Tags)
{
    if (Tags.Contains("Player")) return EActorTagType::Player;
    if (Tags.Contains("Ring")) return EActorTagType::Ring;
    return EActorTagType::Unknown;
}
