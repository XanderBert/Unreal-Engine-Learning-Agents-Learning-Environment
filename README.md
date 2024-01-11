# Unreal Engine 5.3 Learning Agents: How to setup Machine Learning with C++ in Unreal Engine

Unreal Engine released a plugin called Learning Agents. Its still in beta and there arent' many recoures on it yet.

Unreal Engine released a [Basic Introduction Tutorial](https://dev.epicgames.com/community/learning/tutorials/qj2O/unreal-engine-learning-to-drive) .This project is building the same project but with C++ and then is exapanding upon this. The tutorial only goes over basic Reinforcement learning with blueprints.

In this write up i will explain how to setup

* Basic Reinforcement learning with c++
* Imitation learning
  * Recording
  * Inference
  * Deploying the recorded network
* Combine the neural networks with a behavior tree  to combine best of both worlds

## Initial Setup

First create a new Unrea Engine project with version 5.3 or highter. You should make a C++ project to be able to follow this write up.

Then we need to enable the plugin and add private and public DependencyModuleNames

### Enable the plugin

![EnablePlugin.gif](Gifs/EnablePlugin.gif)

After Enabeling the plugin you will be prompted to restart Unreal Engine, You should do so.

### Adding Private and Public Dependencies to the project

If we want to work with C++ we should add the Dependencies to the YourProjectName.Build.cs file. This gets generated when you solution is made.

Open up the solution with your prefered IDE and navigate to the Build.cs file. This should be located in the root of your project.

Here we add the dependencies.

```
PrivateDependencyModuleNames.AddRange(new string[] { "LearningAgentsTraining", "ChaosVehicles" });
PublicDependencyModuleNames.AddRange(new string[] {  "LearningAgents", "LearningAgentsTraining"  });
```

Note that i also added ChaosVehicles this is used to give the cars input trough c++. If you are not working with the Car template or not using a Vehicle Movement Componet you should not add this.

## Manager Component Layout

This plugin uses a component layout.

You will create a manager for each type of learning or actions you want to do.

The base class of each manager is a ```ALearningAgentsManager``` which is derived from a ```AActor```

Our Manager basicly can hold our ```ULearningAgentsManagerComponent```

Note: These components are derived from a ```UActorComponent``` and not ```USceneComponent```

Altough our components need to be attached to the manager to be able to work (because or manager does not really store the components but our components hold a pointer to our Manager).

```cpp
void ULearningAgentsManagerComponent::PostInitProperties()
{
	Super::PostInitProperties();

	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		return;
	}

	Manager = GetOwner<ALearningAgentsManager>();

	if (!Manager)
	{
		UE_LOG(LogLearning, Error, TEXT("%s: Must be attached to a LearningAgentsManager Actor."), *GetName());
	}
}
```

For this reason i decided to make the managers in blueprint as they are basicly emtpy vesels  with minimal setup. That way our C++ componets are attached to it without any big problems.

## Basic Reinforcement Learning

What components do we need for a basic Reinforcement Learning setup?

### Interactor

The interactor Component will be used to:

* Setup Observations
* Set those Observations
* Setup Actions
* Apply those Actions

#### Create the Interactor Component

![AddInteractor.gif](Gifs/AddInteractor.gif)


#### Implementing our Interactor

When our component is added to the project. Open it up in your favourite IDE and we will start with overriding the needed functions in the header file.


#### Trainer

#### Policy

My learning environment for testing out Unreal Learning Agents
As of now i am exploring Reinforcement learning and Imitation Learning.

Explain how to implement the system in c++

Explain what each component does and why you would need it

We have our manager which contains our own implementation of a Interactor component

We need to setup the observations for our interactor. (Show the classes in LearningAgentsObservations.h) They each hold a static function to add a observation

The same for Actions, Show the Static function sin LearningAgentsActions.h

Same for LearningAgentsRewards.h

Same for class ULearningAgentsCompletion

We should have visual Loging?

```
#if UE_LEARNING_AGENTS_ENABLE_VISUAL_LOG
	/** Color used to draw this action in the visual log */
	FLinearColor VisualLogColor = FColor::Blue;

	/** Describes this action to the visual logger for debugging purposes. */
	virtual void VisualLog(const UE::Learning::FIndexSet Instances) const {}
#endif
```

Then show the learning agents helper (LearningAgentsHelpser.h) here are the helper functions stored and they also use a static function.

Explain why you would use Imitation learning for certain types of games

Explain why you would learn Reinforcement learning for other types of games

Add Behavior tree support.

Why would we add behaviour tree support?

Behaviour trees are good at a high level support for giving directives

Some sub behaviours would be silly to train the neural network for. This mostly applies for actions that have a hard "When to fire this behaiour".

A good way would be to implement the trained network further down the behaviour tree, For example.

train a neural network for enemies to fight with a human like behaviour, And when the enemy should fight in the behaviour tree then use the neural network
