// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SphereComponent.h>
#include "BaseBallActor.generated.h"

class APortalVRTCharacter;
class ARingActor;

UENUM(BlueprintType)
enum class EActorTagType : uint8
{
	Player,
	Ring,
	Unknown
};

EActorTagType GetTagType(const TArray<FName>& Tags);

UCLASS()
class PORTALVRT_API ABaseBallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseBallActor();

	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	USphereComponent* BallCollider;

	void SetLastPlayer(FString& playerName);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnSphereOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	FString lastPlayerName;

	UPROPERTY(EditInstanceOnly)
	int scorePoints = 1;

	APortalVRTCharacter* character;
	ARingActor* ring;
};
