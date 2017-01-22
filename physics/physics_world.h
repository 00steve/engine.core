#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H


#define dSINGLE
#include <ode/ode.h>


#include "../core/double3.h"
class PhysicsWorld;
#include "../physics/physical.h"
#include "../physics/physicsgroup.h"

class PhysicsWorld{
private:

    /*physics world stuff for the fps*/
    dWorldID worldID;
    dSpaceID spaceID;
    dJointGroupID contactGroup;
	double stepSize;

    void NearCallback (void *data, dGeomID o1, dGeomID o2);
    static void NearCallbackStatic(void *data,dGeomID o1, dGeomID o2);
    void Init(){
        cout << "Initialize physics\n";
        stepSize  = .01;
        worldID = dWorldCreate();
        spaceID = dHashSpaceCreate(0);
        contactGroup = dJointGroupCreate(0);
    }

public:


    PhysicsWorld();
    ~PhysicsWorld();


    void Step();


    void PhysicsSettings(VarMap settings);


    /*set the world settings, like bounciness, constant force mixing, gravity, etc.*/
    double3 SetGravity(const double3 newGravity);


    /*create new 3d shapes of coolness*/
    /**\brief Creates a new box in the current physics space and world.

    The box needs dimensions, so a length, width, and height. It also needs a density,
    and it should have a reference to the node that it is being created for added as
    the void* argument.*/
    void NewBox(dBodyID &body,dGeomID &geom,double3 dimensions,double density,void* data);

    /**\brief Creates new plane in the current physics space and world.

    The normal of the normal is used to determine the angle of the plane, the
    offset is used to determine how far away from the origin the plane sits.

    Since this plane does not have a body,
    it will be static by default, meaning that it has no mass and cannot be
    moved by any objects. It is an immovable object.*/
    void NewPlane(dGeomID &geom,double3 normal,double offset,void* data);

    /**\brief Create new sphere in the current physics space and world.

    It creates a sphere. It needs a radius. 'nuff said.*/
    void NewSphere(dBodyID &body,dGeomID &geom,double radius,double density,void* data);

    void NewCylinder(dBodyID &body,dGeomID &geom,double radius,double length,double density,void* data);


    /**\brief Generates a capsule with the given height and radius, with the mass
    that was argued.*/
    void NewCapsule(dBodyID &body,dGeomID &geom,double height,double radius,double density, void* data);


    void NewUniversalJoint(dJointID &joint,dBodyID &body1,dBodyID &body2,double3 anchor);
    void NewBallAndSocketJoint(dJointID &joint,dBodyID &body1,dBodyID &body2,double3 anchor);

    /**\brief Given a VarMap of different objects, which should be arranged as groups with their
    specific properties, generates physics objects and returns them in each group, with property names
    such as body and geom.

    The following is an example VarMap that will build a sphere and a box:
    {
        sphere1{
            radius 2
            position 0,4,0
        }
        box{
            lengths 1,1,2
            position -1,0,0
        }
    }

    The following objects will be returned:
    {
        sphere1{
            body (object)
            geom (object)
        }
        box{
            body (object)
            geom (object)
        }
    }
    */
    PhysicsGroup* BuildPhysicsGroup(VarMap* groupSettings,void* data);


    static void GlMatrix(dBodyID body,float *matrix);
    static void GlMatrix(dGeomID geom,float *matrix);


};
#endif // PHYSICS_WORLD_H




