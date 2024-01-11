// Fill out your copyright notice in the Description page of Project Settings.
#include "LearningAgentsInteractorCar.h"

#include "ChaosVehicleMovementComponent.h"
#include "LearningAgentsActions.h"
#include "LearningAgentsHelpers.h"
#include "LearningAgentsObservations.h"


// Sets default values for this component's properties
ULearningAgentsInteractorCar::ULearningAgentsInteractorCar(): TrackSpline(nullptr),
                                                              CarPositionOnTrackObservation(nullptr),
                                                              CarDirectionOnTrackObservation(nullptr),
                                                              TrackGlobalAngleObservation(nullptr),
                                                              CarVelocityObservation(nullptr),
                                                              CarDistanceToTrackObservation(nullptr),
                                                              CarThrottleAction(nullptr), CarBrakeAction(nullptr),
                                                              SteeringAction(nullptr),
                                                              TrackSplineHelper(nullptr),
																CarActions()

{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULearningAgentsInteractorCar::BeginPlay()
{
	Super::BeginPlay();

	check(SplineActor->IsValidLowLevel())

	TrackSpline = CastChecked<USplineComponent>(SplineActor->GetComponentByClass(USplineComponent::StaticClass()));
	check(TrackSpline->IsValidLowLevel())
}


// Called every frame
void ULearningAgentsInteractorCar::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULearningAgentsInteractorCar::SetupObservations_Implementation()
{
	Super::SetupObservations_Implementation();

	//Planar Observations
	CarPositionOnTrackObservation = UPlanarPositionObservation::AddPlanarPositionObservation(this, TEXT("Car Position On Track"));
	CarDirectionOnTrackObservation = UPlanarDirectionObservation::AddPlanarDirectionObservation(this, TEXT("Car Direction On Track"));
	TrackGlobalAngleObservation = UAngleObservation::AddAngleObservation(this, TEXT("Track Global Angle"));
	CarVelocityObservation = UPlanarVelocityObservation::AddPlanarVelocityObservation(this, TEXT("Car Velocity"));
	CarDistanceToTrackObservation = UFloatObservation::AddFloatObservation(this, TEXT("Car Distance To Track"),1000.f);

	//Helper Observations
	TrackSplineHelper = USplineComponentHelper::AddSplineComponentHelper(this, TEXT("Track Spline Helper"));


	check(TrackSplineHelper->IsValidLowLevel())
	check(CarPositionOnTrackObservation->IsValidLowLevel())
    check(CarDirectionOnTrackObservation->IsValidLowLevel())
	check(TrackGlobalAngleObservation->IsValidLowLevel())
	check(CarVelocityObservation->IsValidLowLevel())
	check(CarDistanceToTrackObservation->IsValidLowLevel())
	
}

void ULearningAgentsInteractorCar::SetObservations_Implementation(const TArray<int32>& AgentIds)
{
	Super::SetObservations_Implementation(AgentIds);
	verify(TrackSpline->IsValidLowLevel())
	
	for (const int32 AgentId : AgentIds)
	{
		//Get the car and check if its valid
		const AActor* carAgent =  CastChecked<AActor>(GetAgent(AgentId));
		check(carAgent->IsValidLowLevel())
		if(!carAgent->IsValidLowLevel()) continue;

		//get data of the car
		const FVector carLocation = carAgent->GetActorLocation();
		const FRotator carRotation = carAgent->GetActorRotation();
		const FVector carVelocity = carAgent->GetVelocity();

		//get data of the spline
		const float distanceAlongSplineAtPosition = TrackSplineHelper->GetDistanceAlongSplineAtPosition(AgentId, TrackSpline, carLocation);
		const FVector splineLocationAtDistance = TrackSplineHelper->GetPositionAtDistanceAlongSpline(AgentId, TrackSpline, distanceAlongSplineAtPosition);
		
		const FVector splineDirectionAtDistance = TrackSplineHelper->GetDirectionAtDistanceAlongSpline(AgentId, TrackSpline, distanceAlongSplineAtPosition);
		
		const float proportionAlongSplineAsAngle = TrackSplineHelper->GetProportionAlongSplineAsAngle(AgentId, TrackSpline, distanceAlongSplineAtPosition);
		const FVector nearestSplineLocation = TrackSplineHelper->GetNearestPositionOnSpline(AgentId, TrackSpline, carLocation);

		
		//Set The Actual Observations
		CarPositionOnTrackObservation->SetPlanarPositionObservation(AgentId, splineLocationAtDistance, carLocation, carRotation);
		CarDirectionOnTrackObservation->SetPlanarDirectionObservation(AgentId, splineDirectionAtDistance, carRotation);
		TrackGlobalAngleObservation->SetAngleObservation(AgentId, proportionAlongSplineAsAngle);
		CarVelocityObservation->SetPlanarVelocityObservation(AgentId, carVelocity);
		CarDistanceToTrackObservation->SetFloatObservation(AgentId, FVector::Dist(carLocation, nearestSplineLocation));
	}
}

void ULearningAgentsInteractorCar::SetupActions_Implementation()
{
	Super::SetupActions_Implementation();

	CarThrottleAction = UFloatAction::AddFloatAction(this, TEXT("Throttle"), 2.f);
	CarBrakeAction = UFloatAction::AddFloatAction(this, TEXT("Brake"), 0.1f);
	SteeringAction = UFloatAction::AddFloatAction(this, TEXT("Steering"), 2.f);

	check(CarThrottleAction->IsValidLowLevel())
	check(CarBrakeAction->IsValidLowLevel())
	check(SteeringAction->IsValidLowLevel())
}

void ULearningAgentsInteractorCar::GetActions_Implementation(const TArray<int32>& AgentIds)
{
	Super::GetActions_Implementation(AgentIds);
	for (const int32 AgentId : AgentIds)
	{
		const AActor* carAgent =  CastChecked<AActor>(GetAgent(AgentId));
		check(carAgent->IsValidLowLevel())
		if(!carAgent->IsValidLowLevel()) continue;

		const float throttleValue = CarThrottleAction->GetFloatAction(AgentId);
		const float brakeValue = CarBrakeAction->GetFloatAction(AgentId);
		const float steeringValue = SteeringAction->GetFloatAction(AgentId);
		
		CarActions.Throttle = throttleValue;
		CarActions.Brake = brakeValue;
		CarActions.Steering = steeringValue;
		
		if(bApplyDirectlyToCar)
		{
			//Apply the value's to the movement component of the actor
			 UChaosVehicleMovementComponent* vehMovementComponent = carAgent->FindComponentByClass<UChaosVehicleMovementComponent>();
			 check(vehMovementComponent->IsValidLowLevel())
			 if(!vehMovementComponent->IsValidLowLevel()) continue;

			vehMovementComponent->SetThrottleInput(CarActions.Throttle);
			vehMovementComponent->SetBrakeInput(CarActions.Brake);
			vehMovementComponent->SetSteeringInput(CarActions.Steering);
		}
	}
}
