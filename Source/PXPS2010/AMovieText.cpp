// Fill out your copyright notice in the Description page of Project Settings.


#include "AMovieText.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound\SoundCue.h"
#include "Kismet\GameplayStatics.h"
#include "TimerManager.h"



// Sets default values
AAMovieText::AAMovieText()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	// Our root component will be a sphere component that will inform us of overlaps and collisions
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));

	RootComponent = CollisionComp;

	// Assign properties to sphere component
	CollisionComp->InitSphereRadius(220.0f);

	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereVisual->SetupAttachment(GetRootComponent());

	IdleParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticlesComponent"));
	IdleParticlesComponent->SetupAttachment(GetRootComponent());

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	TextRenderComponent->SetupAttachment(GetRootComponent());
	//TextRenderComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
	TextRenderComponent->SetHorizontalAlignment(EHTA_Center);
	TextRenderComponent->SetYScale(5.0f);
	TextRenderComponent->SetXScale(5.0f);
	TextRenderComponent->SetVisibility(true);
	TextRenderComponent->SetText(NSLOCTEXT("AnyNs", "Any", "HelloWorld"));

	OutputString = "No sound?!?!";
	TextRenderComponent->SetText(FText::FromString(OutputString));


	bRotate = false;

	RotationRate = 30.f;

	TimerVariable = 3.f;
}

// Called when the game starts or when spawned
void AAMovieText::BeginPlay()
{
	Super::BeginPlay();



	GetWorldTimerManager().SetTimer(TimerHandle, this, &AAMovieText::InitialSoundTimer, TimerVariable);

}

// Called every frame
void AAMovieText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}
}

void AAMovieText::MyOnBeginOverlap(AActor* OtherActor)
{
	OutputString = "Hello " + OtherActor->GetName() + "!";
	TextRenderComponent->SetText(FText::FromString(OutputString));
}

void AAMovieText::MyOnEndOverlap(AActor* OtherActor)
{
	TextRenderComponent->SetText(NSLOCTEXT("AnyNs", "Any", "HelloWorld"));
}


void AAMovieText::InitialSoundTimer()
{
	if (OverlapSound)
	{
		UGameplayStatics::PlaySound2D(this, OverlapSound);
		OutputString = "Sound has started";
		TextRenderComponent->SetText(FText::FromString(OutputString));
	}
}