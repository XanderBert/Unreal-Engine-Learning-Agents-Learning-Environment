# Unreal Engine 5.3 Learning Agents: How to setup Machine Learning with C++ in Unreal Engine

Unreal Engine released a plugin called Learning Agents. Its still in beta and there arent' many recoures on it yet. So here i am adding my ... to the pot????

Unreal Engine released a [Basic Introduction Tutorial](https://dev.epicgames.com/community/learning/tutorials/qj2O/unreal-engine-learning-to-drive) .This project is building the same project but with C++ and then is exapanding upon this. The tutorial only goes over basic Reinforcement learning with blueprints.

In this write up i will explain how to setup

* Basic Reinforcement learning with c++
* Imitation learning
  * Recording
  * Inference
  * Deploying the recorded network
* Combine the neural networks with a behavior tree  to combine best of both worlds

## Initial Setup

First we need to enable the plugin and add private and public DependencyModuleNames

### Enable the plugin

![EnablePlugin.gif](Gifs/EnablePlugin.gif)

After Enabeling the plugin you will be prompted to restart Unreal Engine, You should do so.



```
PrivateDependencyModuleNames
```

## Basic Reinforcement Learning

What components do we need for a basic Reinforcement Learning setup?

1. Policy
2. Interactor
3. Trainer

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
