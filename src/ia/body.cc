//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/body.h"
#include "ia/mind.h"
#include "engine/debug_draw.h"
#include "ia/agent.h"
#include "ia/defines.h"

void Body::init(const Role role, const Type type, Mind* mind) {
  type_ = type;
  role_ = role;
  mind_ = mind;

  switch(role) {
    case Role::Soldier: sprite_.loadFromFile(AGENT_SOLDIER); break;
    case Role::Guard: sprite_.loadFromFile(AGENT_GUARD); break;
    case Role::Slave: sprite_.loadFromFile(AGENT_SLAVE); break;
    default: sprite_.loadFromFile(AGENT_SLAVE);
  }

  steering_mode_ = SteeringMode::Kinematic_Seek;
  behaviour_status_ = Behaviour::Idle;

}

void Body::update(const uint32_t dt) {

    KinematicStatus* target = nullptr;
    
    switch (behaviour_status_) {
    case Behaviour::Path: {
        KinematicSteering steer;
        setOrientation(state_.velocity);
        if ((nextPoint_ - state_.position).length() <= ((previousPoint_ - state_.position).length()))
        {
            mind_->getNextIter();
            previousPoint_ = nextPoint_;
        }
        target = new KinematicStatus();
        target->position = nextPoint_;
        k_seek_.calculate(state_, target, &steer);
        updateKinematic(dt, steer);
        break; }
    case Behaviour::Idle: {
        KinematicSteering steer;
        steer.velocity = MathLib::Vec2(0.0f, 0.0f);
        break; }
    }


  sprite_.setPosition(state_.position.x(), state_.position.y());
  sprite_.setRotation(state_.orientation);
}

void Body::render() const {
  sprite_.render();

  DebugDraw::drawVector(dd.red.pos, dd.red.v, 0xFF, 0x00, 0x00, 0xFF);
  DebugDraw::drawVector(dd.green.pos, dd.green.v, 0x00, 0x50, 0x00, 0xFF);
  DebugDraw::drawVector(dd.blue.pos, dd.blue.v, 0x00, 0x00, 0xFF, 0xFF);
  DebugDraw::drawPositionHist(state_.position);
}

void Body::setTarget(Agent* target) {
  target_ = target;
}

void Body::setNextPoint(MathLib::Vec2 nextPoint)
{
    nextPoint_ = nextPoint;
}

void Body::updateKinematic(const uint32_t dt, const KinematicSteering& steering) {
  const float time = dt * 0.001f;             //dt comes in miliseconds

  state_.velocity = steering.velocity;
  state_.position += steering.velocity * time;

  state_.rotation = steering.rotation;
  state_.orientation += steering.rotation * time;

  keepInBounds();

  dd.green.pos = state_.position;
  dd.green.v = state_.velocity;
}

void Body::updateSteering(const uint32_t dt, const Steering& steering) {
  const float time = dt * 0.001f;             //dt comes in miliseconds

  state_.velocity += steering.linear;
  state_.position += state_.velocity * time;

  state_.rotation += steering.angular;
  state_.orientation += state_.rotation * time;

  keepInSpeed();
  keepInBounds();

  dd.green.pos = state_.position;
  dd.green.v = state_.velocity;
}

void Body::updateManual(const uint32_t dt) {
  const float time = dt * 0.001f;             //dt comes in miliseconds

  MathLib::Vec2 orientation;
  orientation.fromPolar(1.0f, state_.orientation);
  state_.velocity = orientation.normalized() * state_.speed;
  state_.position += state_.velocity * time;

  keepInSpeed();
  keepInBounds();

  dd.green.pos = state_.position;
  dd.green.v = state_.velocity;
}

void Body::setOrientation(const Vec2& velocity) {
  if (velocity.length2() > 0) {
    state_.orientation = atan2(velocity.y(), velocity.x());
  }
}

void Body::keepInBounds() {
  if (state_.position.x() > WINDOW_WIDTH) state_.position.x() = 0.0f;
  if (state_.position.x() < 0.0f) state_.position.x() = WINDOW_WIDTH;
  if (state_.position.y() > WINDOW_HEIGHT) state_.position.y() = 0.0f;
  if (state_.position.y() < 0.0f) state_.position.y() = WINDOW_HEIGHT;
}

void Body::keepInSpeed() {
  if (state_.velocity.length() > max_speed_) {
    state_.velocity = state_.velocity.normalized() * max_speed_;
  }
}