// Fill out your copyright notice in the Description page of Project Settings.

#pragma once





#include "CoreMinimal.h"
#include "LearningAgentsActions.h"
#include "LearningAgentsHelpers.h"
#include "LearningAgentsInteractor.h"
#include "LearningAgentsObservations.h"
#include "LearningAgentsInteractorCar.generated.h"
class USplineComponent;

USTRUCT(BlueprintType, Blueprintable )
struct FCarActions
{
	GENERATED_BODY()

	FCarActions() : Throttle(0.f), Brake(0.f), Steering(0.f) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	float Throttle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	float Brake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	float Steering;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LA_TESTENV_API ULearningAgentsInteractorCar : public ULearningAgentsInteractor
{
	GENERATED_BODY()

public:
	ULearningAgentsInteractorCar();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;


	virtual void SetupObservations_Implementation() override;
	virtual void SetObservations_Implementation(const TArray<int32>& AgentIds) override;
	
	virtual void SetupActions_Implementation() override;
	virtual void GetActions_Implementation(const TArray<int32>& AgentIds) override;
		


	UFUNCTION(BlueprintCallable, Category = "Learning Agents")
	USplineComponentHelper* GetTrackSplineHelper() const { return TrackSplineHelper; }

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SetActions(TSubclassOf<UObject> agentClass);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	FCarActions& GetCarActions() { return CarActions; }

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UFloatAction* GetCarThrottleAction() const { return CarThrottleAction; }

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UFloatAction* GetCarBrakeAction() const { return CarBrakeAction; }

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UFloatAction* GetSteeringAction() const { return SteeringAction; }
	
	UFUNCTION(BlueprintCallable, Category = "Learning Agents")
	bool GetApplyDirectlyToCar() const { return bApplyDirectlyToCar; }

	UFUNCTION(BlueprintCallable, Category = "Learning Agents")
	void SetApplyDirectlyToCar(bool bNewApplyDirectlyToCar) { bApplyDirectlyToCar = bNewApplyDirectlyToCar; }	

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Spline")
	AActor* SplineActor;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Spline")
	USplineComponent* TrackSpline;
	
//Observations
	UPROPERTY(VisibleDefaultsOnly, Category = "Observations")
	UPlanarPositionObservation* CarPositionOnTrackObservation;

	UPROPERTY(VisibleDefaultsOnly, Category = "Observations")
	UPlanarDirectionObservation* CarDirectionOnTrackObservation;

	UPROPERTY(VisibleDefaultsOnly, Category = "Observations")
	UAngleObservation* TrackGlobalAngleObservation;

	UPROPERTY(VisibleDefaultsOnly, Category = "Observations")
	UPlanarVelocityObservation* CarVelocityObservation;

	UPROPERTY(VisibleDefaultsOnly, Category = "Observations")
	UFloatObservation* CarDistanceToTrackObservation;

	//Actions
	UPROPERTY(VisibleDefaultsOnly, Category = "Actions")
	UFloatAction* CarThrottleAction;

	UPROPERTY(VisibleDefaultsOnly, Category = "Actions")
	UFloatAction* CarBrakeAction;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Actions")
	UFloatAction* SteeringAction;

	//Helpers	
	UPROPERTY(VisibleDefaultsOnly, Category = "Observations")
	USplineComponentHelper* TrackSplineHelper;

	UPROPERTY(VisibleAnywhere, Category = "Actions")
	FCarActions CarActions{};

	UPROPERTY(EditAnywhere, Category = "Actions")
	bool bApplyDirectlyToCar = true;
};
