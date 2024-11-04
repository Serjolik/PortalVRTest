// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "RingActor.generated.h"

UCLASS()
class PORTALVRT_API ARingActor : public AActor
{
	GENERATED_BODY()
	
public:
    // Sets default values for this actor's properties
    ARingActor();

    UPROPERTY(EditInstanceOnly)
    UStaticMeshComponent* RingMesh;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
    UBoxComponent* RingScoreCollider;

    UFUNCTION(BlueprintImplementableEvent)
    void AddScore(const FString& playerName, int points);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TMap<FString, int32> PlayerScores;

    UFUNCTION(BlueprintImplementableEvent)
    void OnUpdatePointsEvent();

};