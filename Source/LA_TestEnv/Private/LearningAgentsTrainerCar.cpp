// Fill out your copyright notice in the Description page of Project Settings.


#include "LearningAgentsTrainerCar.h"
#include "Kismet/KismetMathLibrary.h"
#include "LearningAgentsRewards.h"
#include "LearningRewardObject.h"
#include "Misc/OutputDeviceNull.h"


// Sets default values for this component's properties
ULearningAgentsTrainerCar::ULearningAgentsTrainerCar()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULearningAgentsTrainerCar::BeginPlay()
{
	Super::BeginPlay();

	check(SplineActor->IsValidLowLevel())

	TrackSpline = CastChecked<USplineComponent>(SplineActor->GetComponentByClass(USplineComponent::StaticClass()));
	check(TrackSpline->IsValidLowLevel())
}


// Called every frame
void ULearningAgentsTrainerCar::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULearningAgentsTrainerCar::SetupRewards_Implementation()
{
	Super::SetupRewards_Implementation();

	TrackSplineHelper = USplineComponentHelper::AddSplineComponentHelper(this, TEXT("Track Spline Helper"));

	//Rewards
	VelocityReward = UScalarVelocityReward::AddScalarVelocityReward(this, TEXT("Velocity Reward"));
	DistanceReward = UFloatReward::AddFloatReward(this, TEXT("Distance Reward"), 2.f);

	check(TrackSplineHelper->IsValidLowLevel())
	check(VelocityReward->IsValidLowLevel())
	check(DistanceReward->IsValidLowLevel())
	
}

void ULearningAgentsTrainerCar::SetRewards_Implementation(const TArray<int32>& AgentIds)
{
	Super::SetRewards_Implementation(AgentIds);

	for (const int32 AgentId : AgentIds)
	{
		const AActor* carActor = CastChecked<AActor>(GetAgent(AgentId));
		check(carActor->IsValidLowLevel())

		const FVector carVelocity = carActor->GetVelocity();
		const FVector carLocation = carActor->GetActorLocation();
		
		const float velocityAlongSpline = TrackSplineHelper->GetVelocityAlongSpline(AgentId, TrackSpline, carLocation, carVelocity);
		const FVector closestPointOnSpline = TrackSplineHelper->GetNearestPositionOnSpline(AgentId, TrackSpline, carLocation);
		const float distanceToSpline = FVector::Distance(carLocation, closestPointOnSpline);

		//clamp to give a more meaningfull gradient and penalize even more when going further of track
		const float aVal = UKismetMathLibrary::MapRangeClamped(distanceToSpline, 0.f, 100.f, 2.f, 0.f);
		const float bVal = UKismetMathLibrary::MapRangeClamped(distanceToSpline, 101.f, 1800.f, 0.f, -4.f);
		
		const float selectedFloat = UKismetMathLibrary::SelectFloat(aVal, bVal, distanceToSpline >= 101.f);
		
		//Set Rewards
		DistanceReward->SetFloatReward(AgentId, selectedFloat);
		//UE_LOG(LogTemp, Warning, TEXT("Distance Reward: %f"), selectedFloat);

		VelocityReward->SetScalarVelocityReward(AgentId, velocityAlongSpline);
		//UE_LOG(LogTemp, Warning, TEXT("Velocity Reward: %f"), velocityAlongSpline);
		
	}
}

void ULearningAgentsTrainerCar::SetupCompletions_Implementation()
{
	Super::SetupCompletions_Implementation();
	OffTrackCompletion = UPlanarPositionDifferenceCompletion::AddPlanarPositionDifferenceCompletion(this, TEXT("Off Track Completion"),1200.f);
	check(OffTrackCompletion->IsValidLowLevel())
}

void ULearningAgentsTrainerCar::SetCompletions_Implementation(const TArray<int32>& AgentIds)
{
	Super::SetCompletions_Implementation(AgentIds);

	for (const int32 AgentId : AgentIds)
	{
		const AActor* carActor = CastChecked<AActor>(GetAgent(AgentId));
		check(carActor->IsValidLowLevel())

		const FVector carLocation = carActor->GetActorLocation();
		const FVector closestPointOnSpline = TrackSplineHelper->GetNearestPositionOnSpline(AgentId, TrackSpline, carLocation);
		
		//Set Completions
		OffTrackCompletion->SetPlanarPositionDifferenceCompletion(AgentId, closestPointOnSpline, carLocation);
	}
}

void ULearningAgentsTrainerCar::ResetEpisodes_Implementation(const TArray<int32>& AgentIds)
{
	Super::ResetEpisodes_Implementation(AgentIds);

	for (const int32 AgentId : AgentIds)
	{
		AActor* carActor = CastChecked<AActor>(GetAgent(AgentId));
		check(carActor->IsValidLowLevel())

		

		//Set the car on a random point on the spline.
		//Why a random point? Because if we always start at the same point, the agent will learn the way the track goes and the learned data will not be flexible enough.
		FOutputDeviceNull OutputDevice;
		carActor->CallFunctionByNameWithArguments(TEXT("ResetToRandomPointOnSpline"), OutputDevice, NULL, true);
	}
}

