/* 
 * The following class makes use of lower position objects in order
 * to model a different N joints robotic arm.
 *
 */

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include "RoboticArm.h"
#include "RoboticArm_Config.h"


RoboticJoint::RoboticJoint(const int &id) : _id(id)
{
#ifndef VISUAL_ENCODER
    Position = new QuadratureEncoder(config::quad_enc_pins[_id][0],
                                     config::quad_enc_pins[_id][1]);
#endif
    Movement = new Motor(config::dc_motor_pins[_id][0],
                         config::dc_motor_pins[_id][1]);
}



RoboticJoint::~RoboticJoint(void)
{
    delete Position;
    delete Movement;
}


RoboticArm::RoboticArm(void) : _joints_nr(config::joints_nr)
{
    //_joints_nr = config.joints_nr;
    
    /* Initialize each joint objects */
    for(auto id = 0; id < _joints_nr; id++) {
        joints.push_back(new RoboticJoint(id));
    }

    std::cout << "INFO: Created a " << _joints_nr << " joints arm object" << std::endl;
    
}


RoboticArm::~RoboticArm(void)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    /* Call each of the joints destructors */
    for(auto id = 0; id < _joints_nr; id++) {
        delete joints[id];
    }
}


void RoboticArm::Init(void)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    /* Perform the initialization for each of the joints */
    for(auto id = 0; id < _joints_nr; id++) {
        /* Get the rotors to a known position */
        joints[id]->Movement->SetSpeed(50);
        joints[id]->Movement->Start();
        joints[id]->Movement->Stop();
    }
}


void RoboticArm::UpdatePosition(void)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    /* Print all of the joints positions relative to themselves for now */
    for(auto id = 0; id < _joints_nr; id++) {

        joints[id]->Movement->Start();
        usleep(100E03);
        joints[id]->Movement->Stop();
        usleep(100E03);
        
        joints[id]->Position->GetPosition();
        joints[id]->Position->GetPeriod();
        joints[id]->Position->PrintStats();

        std::cout << std::endl;
    }
}

