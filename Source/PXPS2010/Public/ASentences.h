// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASentences.generated.h"

UCLASS()
class PXPS2010_API AASentences : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASentences();

	// Pointers for setting the visual and audio objects.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sentence")
		class UTextRenderComponent* TextRenderComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sentence")
		FString OutputString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sentence")
		TArray<FString> StringArray;

	int32 SIndex;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, Category = "Change Delay")
		float TimerVariable;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		void RepeatingFunction();
};
