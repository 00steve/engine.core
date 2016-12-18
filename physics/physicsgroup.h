#ifndef PHYSICS_GROUP_H
#define PHYSICS_GROUP_H


#include <engine/core/map.h>
#include <ode/ode.h>


struct PhysicsGroup{
    map<dGeomID> geom;
    map<dBodyID> body;
    map<dJointID> joint;
};




#endif // PHYSICS_GROUP_H
