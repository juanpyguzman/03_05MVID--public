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

void Body::init(Agent* thisAgent, const Role role, const Type type, Mind* mind, zonas zonasMapa, std::vector<doors>* doorsState) {
  thisAgent_ = thisAgent;
  type_ = type;
  role_ = role;
  mind_ = mind;
  zonas_ = zonasMapa;
  doors_ = doorsState;
  doorsClosed_ = *doorsState;
  soldierNumber_ = rand() % 100;

  switch(role) {
  //Inicialización de Soldier
  case Role::Soldier: {
      sprite_.loadFromFile(AGENT_SOLDIER);

      //Posicionamos de manera aleatoria a los soldados en sus bases en estado Idle
      int random = rand() % zonas_.base.size();
      state_.position = MathLib::Vec2(zonas_.base[random]*8);
      behaviour_status_ = Behaviour::SoldierIdle;
      hackingDoorNumber_=-1;
      break; 
  }
  //Inicialización de Guard
  case Role::Guard: {
      sprite_.loadFromFile(AGENT_GUARD); 
      int random = rand() % zonas_.interior.size();
      state_.position = MathLib::Vec2(zonas_.interior[random] * 8);
      behaviour_status_ = Behaviour::GuardStart;
      closingDoorNumber_ = -1;
      break;
  }
  //Inicialización de Slave
  case Role::Slave: {
      sprite_.loadFromFile(AGENT_SLAVE);
      int random = rand() % zonas_.rest.size();
      state_.position = MathLib::Vec2(zonas_.rest[random] * 8);
      behaviour_status_ = Behaviour::SlaveResting;
      break;
  }
    default: sprite_.loadFromFile(AGENT_SLAVE);
  }

  steering_mode_ = SteeringMode::Kinematic_Seek;

}

void Body::update(const uint32_t dt) {

    KinematicStatus* target = nullptr;
    
    //Comportamiento del Soldado
    if (role_ == Role::Soldier) {
        
        
        switch (behaviour_status_) {
        //Buscando puertas
        case Behaviour::SoldierSearch: {

            thisAgent_->getKinematic()->speed = 20.0f;
            if (!mind_->pathfinding_.isPath)
            {
                std::cout << "Soldado " << soldierNumber_ << " sin camino " << std::endl;
                setBehaviour(Body::Behaviour::SoldierIdle);
            }
            else {
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
                if (mind_->endPath)
                {
                    std::cout << "Soldado " << soldierNumber_ << " camino terminado " << std::endl;
                    setBehaviour(Body::Behaviour::SoldierIdle);
                }

                //Comprobación de distancia a Puerta
                for (int i = 0; i < 4; i++)
                {
                    if ((MathLib::Vec2(doors_->at(i).outsideX * 8, doors_->at(i).outsideY * 8) - state_.position).length() <= 200.0f) {
                        //Comprobamos si estamos pasando por delante
                        if ((doors_->at(i).startX * 8 < state_.position.x() && doors_->at(i).endX * 8 > state_.position.x())
                            || (doors_->at(i).startY * 8 < state_.position.y() && doors_->at(i).endY * 8 > state_.position.y())) {
                            //Si la puerta está cerrada
                            if (doors_->at(i).open == false) {
                                //Generamos pathfinding hacia la puerta
                                //mind_->stopPath();
                                if (hackingDoorNumber_ == i)
                                {
                                    setBehaviour(Body::Behaviour::SoldierHacking);
                                    break;
                                }
                                std::cout << "Problema step 1. Soldado " << soldierNumber_ << std::endl;
                                mind_->setStartPoints(state_.position.x(), state_.position.y());
                                std::cout << "Problema step 2. Soldado " << soldierNumber_ << std::endl;
                                mind_->setDoors(doorsClosed_);
                                std::cout << "Problema step 3. Soldado " << soldierNumber_ << std::endl;
                                mind_->setEndPoints(doors_->at(i).outsideX * 8, doors_->at(i).outsideY * 8);
                                std::cout << "Problema step 4. Soldado " << soldierNumber_ << std::endl;
                                hackingDoorNumber_ = i;
                                std::cout << "Problema step 5. Soldado " << soldierNumber_ << std::endl;
                                checkingDoor_ = i;
                                std::cout << "Problema step 6. Soldado " << soldierNumber_ << std::endl;
                                std::cout << "Soldado " << soldierNumber_ << " yendo a abrir puerta " << hackingDoorNumber_ << std::endl;
                                setBehaviour(Body::Behaviour::SoldierHacking);
                            }

                        }
                    }
                }

            }

            break; }
        //Estado de reposo
        case Behaviour::SoldierIdle: {
            KinematicSteering steer;
            steer.velocity = MathLib::Vec2(0.0f, 0.0f);
            int delay = rand() % 100;
            if (delay == 99)
            {
                int buscar = rand() % 2;
                //Búsqueda aleatoria en el mapa
                //Si buscar es 0, genera un aleatorio a las puertas y si dicha puerta ha sido encontrada, manda un Pathfinding a esa puerta, en caso contrario, manda buscar
                if (buscar == 0) {
                    int randomDoor = rand() % 4;
                    if (doors_->at(randomDoor).discovered == true) {
                        //Compruebo que el Pathfinding no manda al agente al mismo punto
                        if (checkingDoor_ == randomDoor) {
                            setBehaviour(Body::Behaviour::SoldierIdle);
                            break;
                        }

                        else {
                            //Compruebo que el Pathfinding no manda al agente al mismo punto
                            if (abs((state_.position.x() - doors_->at(randomDoor).outsideX * 8)) > 10 && abs((state_.position.y() - doors_->at(randomDoor).outsideY * 8)) > 10) {
                                mind_->setStartPoints(state_.position.x(), state_.position.y());
                                mind_->setDoors(doorsClosed_);
                                std::cout << "Nuevo pathfinding de soldado " << soldierNumber_ << " hacia puerta " << randomDoor << std::endl;
                                mind_->setEndPoints(doors_->at(randomDoor).outsideX * 8, doors_->at(randomDoor).outsideY * 8);
                                checkingDoor_ = randomDoor;
                                hackingDoorNumber_ = randomDoor;
                                std::cout << "Soldado " << soldierNumber_ << " yendo a abrir puerta ya descubierta " << hackingDoorNumber_ << std::endl;
                                setBehaviour(Body::Behaviour::SoldierHacking);
                            }

                            else {
                                break;
                            }
                        }
                       

                    }
                    else {
                        int random = rand() % zonas_.exterior.size();
                        //Compruebo que el Pathfinding no manda al agente al mismo punto
                        if (abs((state_.position.x() - zonas_.exterior[random].x() * 8)) > 10 && abs((state_.position.y() - zonas_.exterior[random].y() * 8)) > 10) {
                            mind_->setStartPoints(state_.position.x(), state_.position.y());
                            mind_->setDoors(doorsClosed_);
                            std::cout << "Nuevo pathfinding de Soldado" << std::endl;
                            mind_->setEndPoints(zonas_.exterior[random].x() * 8, zonas_.exterior[random].y() * 8);
                            setBehaviour(Body::Behaviour::SoldierSearch);
                        }

                        else {
                            break;
                        }


                    }
                }
                // Si buscar es 1, manda Pathfinding aleatorio de búsqueda en el mapa
                else {
                    int random = rand() % zonas_.exterior.size();
                    //Compruebo que el Pathfinding no manda al agente al mismo punto
                    if (abs((state_.position.x() - zonas_.exterior[random].x() * 8)) > 10 && abs((state_.position.y() - zonas_.exterior[random].y() * 8)) > 10) {
                        mind_->setStartPoints(state_.position.x(), state_.position.y());
                        mind_->setDoors(doorsClosed_);
                        std::cout << "Nuevo pathfinding de Soldado " << soldierNumber_ << "X: " << state_.position.x() << "    Y: " << state_.position.y() << std::endl;  
                       

                        
                        mind_->setEndPoints(zonas_.exterior[random].x() * 8, zonas_.exterior[random].y() * 8);

                        std::cout << "Nuevo pathfinding de Soldado " << soldierNumber_ << "X: " << zonas_.exterior[random].x() * 8 << "    Y: " << zonas_.exterior[random].y() * 8 << std::endl;
                        setBehaviour(Body::Behaviour::SoldierSearch);
                    }

                    else {
                        break;
                    }
                }
            }

            break; }
        
        //Abriendo puerta
        case Behaviour::SoldierHacking: {
            thisAgent_->getKinematic()->speed = 20.0f;
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
            if (mind_->endPath)
            {
                if (doors_->at(hackingDoorNumber_).open == false) {
                    doors_->at(hackingDoorNumber_).open = true;
                }
                if (doors_->at(hackingDoorNumber_).discovered == false)
                {
                    doors_->at(hackingDoorNumber_).discovered = true;
                }
                mind_->changeDoor(doors_->at(hackingDoorNumber_));
                
                //Pathfinding para volver a la base
                mind_->setStartPoints(state_.position.x(), state_.position.y());
                mind_->setDoors(doorsClosed_);

                int random = rand() % zonas_.base.size();
                mind_->setEndPoints(MathLib::Vec2(zonas_.base[random]).x() * 8, MathLib::Vec2(zonas_.base[random]).y() * 8);
                hackingDoorNumber_ = -1;
                checkingDoor_ = -1;
                setBehaviour(Body::Behaviour::SoldierBack);
            }

            break; }

        //Volviendo a base
        case Behaviour::SoldierBack: {
            thisAgent_->getKinematic()->speed = 20.0f;
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
            if (mind_->endPath)
            {
                setBehaviour(Body::Behaviour::SoldierIdle);
            }

            break; }
        }
    }


    //Comportamiento del Guard
    if (role_ == Role::Guard) {


        switch (behaviour_status_) {

        //Buscando puertas
        case Behaviour::GuardPatrol: {

            thisAgent_->getKinematic()->speed = 20.0f;
            if (!mind_->pathfinding_.isPath)
            {
                setBehaviour(Body::Behaviour::GuardStart);
            }
            else {
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
                if (mind_->endPath)
                {
                    setBehaviour(Body::Behaviour::GuardStart);
                }

                //Comprobación de distancia a Puerta
                for (int i = 0; i < 4; i++)
                {
                    if ((MathLib::Vec2(doors_->at(i).insideX * 8, doors_->at(i).insideY * 8) - state_.position).length() <= 200.0f) {
                        if ((abs(state_.position.x() - doors_->at(i).insideX * 8) > 20) && (abs(state_.position.y() - doors_->at(i).insideY * 8) > 20)) {
                            if ((doors_->at(i).startX * 8 < state_.position.x() && doors_->at(i).endX * 8 > state_.position.x())
                                || (doors_->at(i).startY * 8 < state_.position.y() && doors_->at(i).endY * 8 > state_.position.y())) {
                                //Si la puerta está cerrada
                                if (doors_->at(i).open == true) {
                                    //Generamos pathfinding hacia la puerta
                                    mind_->stopPath();
                                    mind_->setStartPoints(state_.position.x(), state_.position.y());
                                    mind_->setDoors(doorsClosed_);
                                    mind_->setEndPoints(doors_->at(i).insideX * 8, doors_->at(i).insideY * 8);
                                    closingDoorNumber_ = i;
                                    checkingDoor_ = i;
                                    setBehaviour(Body::Behaviour::GuardClosing);
                                }

                            }
                        }
                        
                        //else if ((abs(state_.position.x() - doors_->at(i).insideX * 8) < 20) && (abs(state_.position.y() - doors_->at(i).insideY * 8) < 20)) {
                            //setBehaviour(Body::Behaviour::GuardStart);
                            //break;
                        //}
                    }
                }

            }

            break; }

        //Estado de reposo
        case Behaviour::GuardStart: {
            KinematicSteering steer;
            steer.velocity = MathLib::Vec2(0.0f, 0.0f);
         
            int random = rand() % zonas_.interior.size();

            //Compruebo que el Pathfinding no manda al agente al mismo punto
            if (abs((state_.position.x() - zonas_.interior[random].x() * 8)) > 10 && abs((state_.position.y() - zonas_.interior[random].y() * 8)) > 10) {
                //Búsqueda aleatoria en el interior
                mind_->setStartPoints(state_.position.x(), state_.position.y());
                mind_->setDoors(doorsClosed_);
                std::cout << "Nuevo pathfinding de Guardia " << soldierNumber_ << "X: " << state_.position.x() << "    Y: " << state_.position.y()  << std::endl;
                mind_->setEndPoints(MathLib::Vec2(zonas_.interior[random]).x() * 8, MathLib::Vec2(zonas_.interior[random]).y() * 8);
                std::cout << "Nuevo pathfinding de Guardia " << soldierNumber_ << "X: " << zonas_.interior[random].x() * 8 << "    Y: " << zonas_.interior[random].y() * 8 << std::endl;
                setBehaviour(Body::Behaviour::GuardPatrol);
            }
            else {
                break;
            }

            break; }

        //Cerrando puerta
        case Behaviour::GuardClosing: {
            thisAgent_->getKinematic()->speed = 20.0f;

            if (!mind_->pathfinding_.isPath)
            {
                setBehaviour(Body::Behaviour::GuardStart);
            }

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
            if (mind_->endPath)
            {
                if (doors_->at(closingDoorNumber_).open == true) {
                    doors_->at(closingDoorNumber_).open = false;
                }
                mind_->changeDoor(doors_->at(closingDoorNumber_));

                setBehaviour(Body::Behaviour::GuardStart);
            }

            break; }
        }
    }

    //Comportamiento del Slave
    if (role_ == Role::Slave) {

        switch (behaviour_status_) {

        case Behaviour::SlaveResting: {

            KinematicSteering steer;
            KinematicStatus new_target;
            k_wander_.calculate(state_, &new_target, &steer);
            updateKinematic(dt, steer);

            break;  }

        //Buscando puertas
        case Behaviour::SoldierSearch: {

            thisAgent_->getKinematic()->speed = 20.0f;
            if (!mind_->pathfinding_.isPath)
            {
                setBehaviour(Body::Behaviour::SoldierIdle);
            }
            else {
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
                if (mind_->endPath)
                {
                    setBehaviour(Body::Behaviour::SoldierIdle);
                }

                //Comprobación de distancia a Puerta
                for (int i = 0; i < 4; i++)
                {
                    //doors_->at(i).
                    if ((MathLib::Vec2(doors_->at(i).outsideX * 8, doors_->at(i).outsideY * 8) - state_.position).length() <= 200.0f) {
                        //Comprobamos si estamos pasando por delante
                        if ((doors_->at(i).startX * 8 < state_.position.x() && doors_->at(i).endX * 8 > state_.position.x())
                            || (doors_->at(i).startY * 8 < state_.position.y() && doors_->at(i).endY * 8 > state_.position.y())) {
                            //Si la puerta está cerrada
                            if (doors_->at(i).open == false) {
                                //Generamos pathfinding hacia la puerta
                                mind_->setStartPoints(state_.position.x(), state_.position.y());
                                mind_->setDoors(doorsClosed_);
                                mind_->setEndPoints(doors_->at(i).outsideX * 8, doors_->at(i).outsideY * 8);
                                hackingDoorNumber_ = i;
                                setBehaviour(Body::Behaviour::SoldierHacking);
                            }

                        }
                    }
                }

            }

            break; }

        //Estado de reposo
        case Behaviour::SoldierIdle: {
            KinematicSteering steer;
            steer.velocity = MathLib::Vec2(0.0f, 0.0f);
            int delay = rand() % 100;
            if (delay == 99)
            {
                int buscar = rand() % 2;
                //Búsqueda aleatoria en el mapa
                //Si buscar es 0, genera un aleatorio a las puertas y si dicha puerta ha sido encontrada, manda un Pathfinding a esa puerta, en caso contrario, manda buscar
                if (buscar == 0) {
                    int randomDoor = rand() % 4;
                    if (doors_->at(randomDoor).discovered == true) {
                        mind_->setStartPoints(state_.position.x(), state_.position.y());
                        mind_->setDoors(doorsClosed_);
                        mind_->setEndPoints(doors_->at(randomDoor).outsideX * 8, doors_->at(randomDoor).outsideY * 8);
                        setBehaviour(Body::Behaviour::SoldierSearch);
                    }
                    else {
                        mind_->setStartPoints(state_.position.x(), state_.position.y());
                        mind_->setDoors(doorsClosed_);
                        int random = rand() % zonas_.exterior.size();
                        mind_->setEndPoints(MathLib::Vec2(zonas_.exterior[random]).x() * 8, MathLib::Vec2(zonas_.exterior[random]).y() * 8);

                        setBehaviour(Body::Behaviour::SoldierSearch);
                    }
                }
                // Si buscar es 1, manda Pathfinding aleatorio de búsqueda en el mapa
                else {
                    mind_->setStartPoints(state_.position.x(), state_.position.y());
                    mind_->setDoors(doorsClosed_);
                    int random = rand() % zonas_.exterior.size();
                    mind_->setEndPoints(MathLib::Vec2(zonas_.exterior[random]).x() * 8, MathLib::Vec2(zonas_.exterior[random]).y() * 8);

                    setBehaviour(Body::Behaviour::SoldierSearch);
                }
            }

            break; }

        //Abriendo puerta
        case Behaviour::SoldierHacking: {
            thisAgent_->getKinematic()->speed = 20.0f;
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
            if (mind_->endPath)
            {
                if (doors_->at(hackingDoorNumber_).open == false) {
                    doors_->at(hackingDoorNumber_).open = true;
                }
                if (doors_->at(hackingDoorNumber_).discovered == false)
                {
                    doors_->at(hackingDoorNumber_).discovered = true;
                }
                mind_->changeDoor(doors_->at(hackingDoorNumber_));

                //Pathfinding para volver a la base
                mind_->setStartPoints(state_.position.x(), state_.position.y());
                mind_->setDoors(doorsClosed_);

                int random = rand() % zonas_.base.size();
                mind_->setEndPoints(MathLib::Vec2(zonas_.base[random]).x() * 8, MathLib::Vec2(zonas_.base[random]).y() * 8);

                setBehaviour(Body::Behaviour::SoldierBack);
            }

            break; }

        //Volviendo a base
        case Behaviour::SoldierBack: {
            thisAgent_->getKinematic()->speed = 20.0f;
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
            if (mind_->endPath)
            {
                setBehaviour(Body::Behaviour::SoldierIdle);
            }

            break; }
        }
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

  if (role_ == Role::Slave && behaviour_status_ == Behaviour::SlaveResting) {
      keepInRestArea();
  }

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

void Body::keepInRestArea() {
    if (state_.position.x() > 230) state_.orientation = -M_PI;
    if (state_.position.x() < 44) state_.orientation = 0.0f;
    if (state_.position.y() > 896) state_.orientation = -M_PI/2.0f;
    if (state_.position.y() < 469) state_.orientation = M_PI/2.0f;
}

void Body::keepInSpeed() {
  if (state_.velocity.length() > max_speed_) {
    state_.velocity = state_.velocity.normalized() * max_speed_;
  }
}