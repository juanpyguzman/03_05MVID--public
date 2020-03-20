//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/body.h"
#include "engine/debug_draw.h"
#include "ia/agent.h"
#include "ia/defines.h"

void Body::init(const Color color, const Type type) {
  type_ = type;
  color_ = color;

  switch(color) {
    case Color::Green: sprite_.loadFromFile(AGENT_GREEN_PATH); break;
    case Color::Blue: sprite_.loadFromFile(AGENT_BLUE_PATH); break;
    case Color::Purple: sprite_.loadFromFile(AGENT_PURPLE_PATH); break;
    case Color::Red: sprite_.loadFromFile(AGENT_RED_PATH); break;
    default: sprite_.loadFromFile(AGENT_GREEN_PATH);
  }

  steering_mode_ = SteeringMode::Kinematic_Seek;
}

void Body::update(const uint32_t dt) {
  if (type_ == Type::Autonomous) {
    switch (steering_mode_) {
      case SteeringMode::Kinematic_Seek: {
        KinematicSteering steer;
        k_seek_.calculate(state_, target_->getKinematic(), &steer);
        updateKinematic(dt, steer);
        setOrientation(state_.velocity);
        break; }
      case SteeringMode::Kinematic_Flee: {
        KinematicSteering steer;
        k_flee_.calculate(state_, target_->getKinematic(), &steer);
        updateKinematic(dt, steer);
        setOrientation(state_.velocity);
        break; }
      case SteeringMode::Kinematic_Arrive: {
        KinematicSteering steer;
        k_arrive_.calculate(state_, target_->getKinematic(), &steer);
        updateKinematic(dt, steer);
        setOrientation(state_.velocity);
        break; }
      case SteeringMode::Kinematic_Wander: {
        KinematicSteering steer;
        k_wander_.calculate(state_, target_->getKinematic(), &steer);
        updateKinematic(dt, steer);
        break; }
      case SteeringMode::Seek: {
        Steering steer;
        seek_.calculate(state_, target_->getKinematic(), &steer);
        updateSteering(dt, steer);
        setOrientation(state_.velocity);
        break; }
      case SteeringMode::Flee: {
        Steering steer;
        flee_.calculate(state_, target_->getKinematic(), &steer);
        updateSteering(dt, steer);
        setOrientation(state_.velocity);
        break; }
      case SteeringMode::Arrive: {
        Steering steer;
        arrive_.calculate(state_, target_->getKinematic(), &steer);
        updateSteering(dt, steer);
        setOrientation(state_.velocity);
        break; }
      case SteeringMode::Align: {
        Steering steer;
        align_.calculate(state_, target_->getKinematic(), &steer);
        updateSteering(dt, steer);
        //setOrientation(state_.velocity);
        break; }
      case SteeringMode::Velocity_Matching: {
        Steering steer;
        vel_matching_.calculate(state_, target_->getKinematic(), &steer);
        updateSteering(dt, steer);
        setOrientation(state_.velocity);
        break; }
      case SteeringMode::Pursue: {
        Steering steer;
        pursue_.calculate(state_, target_->getKinematic(), &steer);
        updateSteering(dt, steer);
        setOrientation(state_.velocity);
        break; }
      case SteeringMode::Face: {
        Steering steer;
        face_.calculate(state_, target_->getKinematic(), &steer);
        updateSteering(dt, steer);
        break; }
      case SteeringMode::LookGoing: {
        Steering steer;
        look_going_.calculate(state_, target_->getKinematic(), &steer);
        updateSteering(dt, steer);
        break; }
      case SteeringMode::Wander: {
        Steering steer;
        wander_.calculate(state_, target_->getKinematic(), &steer);
        updateSteering(dt, steer);
        break; }
    }
  } else {
    updateManual(dt);
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