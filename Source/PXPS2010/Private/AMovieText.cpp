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
#include "MediaPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MediaSoundComponent.h"
#include "MediaTexture.h"
#include "MediaSource.h"

// Sets default values
AAMovieText::AAMovieText()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	// Our root component will be a sphere component that will inform us of overlaps and collisions
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = CollisionComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->AttachTo(CollisionComp);


	MediaSource = LoadObject<UMediaSource>(NULL, TEXT("/Content/Movies/waves.mp4"), NULL, LOAD_None, NULL);

	MediaPlayer = NewObject<UMediaPlayer>();
	FString VideoPath = "/PXPS2010/Content/Movies/waves.mp4";
	MediaPlayer->OpenSource(MediaSource);
	MediaPlayer->AddToRoot();


	// Assign properties to sphere component
	CollisionComp->InitSphereRadius(220.0f);

	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));


	IdleParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticlesComponent"));
	IdleParticlesComponent->SetupAttachment(GetRootComponent());

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	TextRenderComponent->SetupAttachment(GetRootComponent());
	//TextRenderComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
	TextRenderComponent->SetHorizontalAlignment(EHTA_Center);
	TextRenderComponent->SetYScale(1.0f);
	TextRenderComponent->SetXScale(1.0f);
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

	MediaPlayer->Pause();

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
		OutputString = FString::SanitizeFloat(DeltaTime);
		TextRenderComponent->SetText(FText::FromString(OutputString));
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
		//UGameplayStatics::PlaySound2D(this, OverlapSound);
		OutputString = "Sound has started";
		TextRenderComponent->SetText(FText::FromString(OutputString));
	}
}

