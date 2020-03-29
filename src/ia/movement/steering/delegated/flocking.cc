#include "ia/world.h"

void Flocking::calculate(Agent* thisAgent, World* world, const KinematicStatus* target, Steering* steering) {
	world_ = world;

	separation_.calculate(thisAgent, world, steering, separationComponent_);
	cohesion_.calculate(thisAgent, world, steering, cohesionComponent_);
	alignment_.calculate(thisAgent, world, steering, alignmentComponent_);

	//Seek component
	steering->linear = (target->position - thisAgent->getKinematic()->position).normalized() *seekComponent_ * max_acceleration_;

}