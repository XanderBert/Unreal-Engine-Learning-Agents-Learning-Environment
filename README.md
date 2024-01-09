# Unreal Engine 5.3 Learning Agents Testing Environment

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
