#include "ia/world.h"

void Flocking::calculate(Agent* thisAgent, World* world, const KinematicStatus* target, Steering* steering) {
	world_ = world;

	separation_.calculate(thisAgent, world, steering);
	cohesion_.calculate(thisAgent, world, steering);

	alignment_.calculate(thisAgent, world, steering);
	seek_ = (target->position - thisAgent->getKinematic()->position).normalized() * max_acceleration_;
	

	steering->linear = seek_*seekComponent_ 
						+ cohesion_.getSteeringLinear() * cohesionComponent_
						+ separation_.getSteeringLinear() * separationComponent_;

	steering->angular = alignment_.getSteeringAngular();
}