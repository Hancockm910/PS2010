// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AMovieText.generated.h"

UCLASS()
class PXPS2010_API AAMovieText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAMovieText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EmOcean Sentence")
		class UTextRenderComponent* TextRenderComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Object")
		class USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Object")
		class UParticleSystemComponent* IdleParticlesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Object")
		class UStaticMeshComponent* SphereVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
		class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
		bool bRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
		float RotationRate;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EmOcean Sentence")
		FString OutputString;


	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, Category = "SoundDelay")
		float TimerVariable;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MyOnBeginOverlap(AActor* OtherActor);
	void MyOnEndOverlap(AActor* OtherActor);



	UFUNCTION()
		void InitialSoundTimer();
};
