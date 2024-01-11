// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsCompletions.h"
#include "LearningAgentsHelpers.h"
#include "LearningAgentsRewards.h"
#include "LearningAgentsTrainer.h"
#include "Components/SplineComponent.h"
#include "LearningAgentsTrainerCar.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LA_TESTENV_API ULearningAgentsTrainerCar : public ULearningAgentsTrainer
{
	GENERATED_BODY()

public:
	ULearningAgentsTrainerCar();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupRewards_Implementation() override;
	virtual void SetRewards_Implementation(const TArray<int32>& AgentIds) override;

	virtual void SetupCompletions_Implementation() override;
	virtual void SetCompletions_Implementation(const TArray<int32>& AgentIds) override;

	virtual void ResetEpisodes_Implementation(const TArray<int32>& AgentIds) override;
	
	
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "Spline")
	AActor* SplineActor;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Spline")
	USplineComponent* TrackSpline;

	//Helpers
	UPROPERTY(VisibleDefaultsOnly, Category = "Spline")
	USplineComponentHelper* TrackSplineHelper;

	//Rewards
	UPROPERTY(VisibleDefaultsOnly, Category = "Rewards")
	UScalarVelocityReward* VelocityReward;

	UPROPERTY(VisibleDefaultsOnly, Category = "Rewards")
	UFloatReward* DistanceReward;

	//Completions
	UPROPERTY(VisibleDefaultsOnly, Category = "Completions")
	UPlanarPositionDifferenceCompletion* OffTrackCompletion;
};
