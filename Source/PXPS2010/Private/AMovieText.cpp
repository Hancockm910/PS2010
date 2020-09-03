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
#include "FileHelpers.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "DrawDebugHelpers.h"

#include "Runtime/MediaAssets/Public/FileMediaSource.h"



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

	FString VideoPath = FPaths::ProjectContentDir();
	VideoPath += "Movies/waves.mp4";
	FText TextVariable = FText::AsCultureInvariant(VideoPath);
/*
	MediaSource = LoadObject<UMediaSource>(NULL, TextVariable, NULL, LOAD_None, NULL);*/
	//MediaSource = NewObject<UMediaSource>;


	MediaSource = NewObject<UFileMediaSource>();
	FString MediaDir = FPaths::ProjectContentDir();
	MediaDir += "Movies/MP_Base.uasset";
	//MediaSource->SetFilePath(FText::FromString(MediaDir));         
	//Me->OpenSource(Video);




	MediaPlayer = NewObject<UMediaPlayer>();



	// Assign properties to sphere component
	CollisionComp->InitSphereRadius(250.0f);

	//CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AAMovieText::OnOverlapBegin);       // set up a notification for when this component overlaps something
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AAMovieText::OnOverlapEnd);       // set up a notification for when this component overlaps something



	IdleParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticlesComponent"));
	IdleParticlesComponent->SetupAttachment(GetRootComponent());

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	TextRenderComponent->SetupAttachment(GetRootComponent());
	//TextRenderComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
	TextRenderComponent->SetHorizontalAlignment(EHTA_Center);
	TextRenderComponent->SetYScale(1.0f);
	TextRenderComponent->SetXScale(1.0f);
	TextRenderComponent->SetVisibility(false);
	TextRenderComponent->SetText(NSLOCTEXT("AnyNs", "Any", "HelloWorld"));




	bRotate = false;

	RotationRate = 30.f;

	TimerVariable = 3.f;



	FString projectDir = FPaths::ProjectContentDir();
	projectDir += "Sentences/Scary.txt";
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*projectDir))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("** Could not Find File **"));
		return;
	}
	FFileHelper::LoadANSITextFileToStrings(*(projectDir), NULL, StringArray);

	SIndex = 0;
}

// Called when the game starts or when spawned
void AAMovieText::BeginPlay()
{
	Super::BeginPlay();
	float InitialTimer = 2.0f;
	float RepeatingTimer = 3.0f;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AAMovieText::RepeatingFunction, RepeatingTimer, true, InitialTimer);

	//FVector ActorLoc = GetActorLocation();
	//DrawDebugSphere(GetWorld(), ActorLoc, 200, 26, FColor(181, 0, 0), true, -1, 0, 2);
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



void AAMovieText::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		InitialSoundTimer();
	}
}

void AAMovieText::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AAMovieText::InitialSoundTimer()
{
	if (OverlapSound)
	{
		UGameplayStatics::PlaySound2D(this, OverlapSound);
		
	}
	if (IdleParticlesComponent)
	{
		IdleParticlesComponent->Activate(false);
	}
	TextRenderComponent->SetText(FText::FromString("BOOM"));
}

// Called to rotate through sentences
void AAMovieText::RepeatingFunction()
{
	// Once we've called this function enough times, clear the Timer.
	if (++SIndex >= StringArray.Num())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		// MemberTimerHandle can now be reused for any other Timer.
	}
	else 
	{
		// Sets text to the next line.
		//TextRenderComponent->SetText(FText::FromString(StringArray[SIndex]));
	}
}

