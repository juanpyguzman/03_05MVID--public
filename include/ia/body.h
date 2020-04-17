//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __BODY_H__
#define __BODY_H__ 1

#include "engine/sprite.h"
#include "ia/defines.h"
#include "ia/mind.h"
#include "ia/movement/kinematic/kinematicarrive.h"
#include "ia/movement/kinematic/kinematicflee.h"
#include "ia/movement/kinematic/kinematicseek.h"
#include "ia/movement/kinematic/kinematicwander.h"
#include "ia/movement/steering/align.h"
#include "ia/movement/steering/arrive.h"
#include "ia/movement/steering/delegated/face.h"
#include "ia/movement/steering/delegated/lookgoing.h"
#include "ia/movement/steering/delegated/pursue.h"
#include "ia/movement/steering/delegated/wander.h"
#include "ia/movement/steering/delegated/flocking.h"
#include "ia/movement/steering/flee.h"
#include "ia/movement/steering/seek.h"
#include "ia/movement/steering/seekPath.h"
#include "ia/movement/steering/seekPath.h"
#include "ia/movement/steering/velocity_matching.h"
#include "mathlib/vec2.h"
#include <list>

class Agent;

class Body {
  public:
    enum class Color {
      Green,
      Blue,
      Purple,
      Red,
    };

    enum class Type {
      Autonomous,
      Manual,
    };

    enum class Role {
        Slave,
        Guard,
        Soldier,
    };

    enum class SteeringMode {
      Kinematic_Seek,         //1       Kinematics
      Kinematic_Flee,         //2
      Kinematic_Arrive,       //3
      Kinematic_Wander,       //4
      Seek,                   //q       Steering Basics
      SeekPath,
      Flee,                   //w
      Arrive,                 //e
      Align,                  //r
      Velocity_Matching,      //t
      Pursue,                 //a       Steering Delegated
      Face,                   //s
      LookGoing,              //d
      Wander,                 //f
    };

    enum class Behaviour {
        SoldierSearch,
        SoldierIdle,
        SoldierHacking,
        SoldierBack,
        GuardStart,
        GuardPatrol,
        GuardClosing,
        SlaveStarting,
        SlaveMovingToWork_Rest,
        SlaveWorking,
        SlaveLoading,
        SlaveUnloading,
        SlaveResting,
    };

    Body() {};
    ~Body() {};

    void init(Agent* thisAgent, Role role, Type type, Mind* mind, zonas zonasMapa, std::vector<doors>* doorsState);
    void update(uint32_t dt);
    void render() const;

    void setTarget(Agent* target);
    void setNextPoint(MathLib::Vec2 nextPoint);
    void setSteering(const SteeringMode mode) { steering_mode_ = mode; };
    void setBehaviour(const Behaviour behaviour) { behaviour_status_ = behaviour; };
    const KinematicStatus* getKinematic() const { return &state_; }
    KinematicStatus* getKinematic() { return &state_; }
  private:
    void updateKinematic(uint32_t dt, const KinematicSteering& steering);
    void updateSteering(uint32_t dt, const Steering& steering);
    void updateManual(uint32_t);
    void setOrientation(const MathLib::Vec2& velocity);
    void keepInSpeed();
    void keepInBounds();
    void keepInRestArea();

    Sprite sprite_;
    Type type_;
    Role role_;
    SteeringMode steering_mode_;
    Behaviour behaviour_status_;
    Agent* target_;
    Agent* thisAgent_;
    Mind* mind_ = nullptr;
    zonas zonas_;
    std::vector<doors>* doors_;
    std::vector<doors> doorsClosed_;

    const float max_speed_ = 100.0f;

    struct {
      struct {
        MathLib::Vec2 pos;
        MathLib::Vec2 v;
      } green, red, blue;
    } dd;

    KinematicStatus state_;

    KinematicSeek k_seek_;
    //KinematicFlee k_flee_;
    //KinematicArrive k_arrive_;
    KinematicWander k_wander_;

    //Seek seek_;
    //SeekPath seekPath_;
    //Flee flee_;
    //Arrive arrive_;
    //Align align_;
    //VelocityMatching vel_matching_;

    //Pursue pursue_;
    //Face face_;
    //LookGoing look_going_;
    //Wander wander_;

    int hackingDoorNumber_, closingDoorNumber_, checkingDoor_;
    float rest_work_time_;
    float MAX_REST_WORK_TIME = 30.0f;
    float MAX_ALERT_TIME = 5.0f;
    int alertCounter_;
    bool wasWorking = false;
    bool wasLoading = false;

    MathLib::Vec2 nextPoint_, previousPoint_;
    int soldierNumber_;
};

#endif