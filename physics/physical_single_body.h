#ifndef SINGLE_BODY_OBJECT_H
#define SINGLE_BODY_OBJECT_H

#include "../physics/physical.h"
#include "../physics/physics_world.h"


class PhysicalSingleBody : public Physical{
protected:

    dBodyID body;
    dGeomID geom;
    float gl_matrix[16];
	double3 position;
	double3 velocity;
    bool active;
    double density;

	/*variables to tell the object the angle of the
	floor. 0 radians is flat (facing directly upwards). 1.57
	radians is a wall, and then everything in between.*/
	bool trackFloor;
	double floorAngle;

    virtual void OnSetSettings();
    virtual bool HandleMessage(NodeMessage message);

public:

	PhysicalSingleBody();

	~PhysicalSingleBody();


	void Update();



	virtual void Kinematic();
	virtual void Dynamic();

	void LinearDampening(double scale);

	void AngularDampening(double scale);

	void DisableBodies();

    double3 Position();

	void Position(double3 newPosition);

	void ShiftPosition(const double3 offset);

	/**\brief adds a force at the COG of an object.

	The force is added at the COG(center of gravity)
	of the object. This force causes acceleration, but
	not rotational change because it is acting on the
	center of gravity. If you want to simulate push or
	some type of force acting on an object that could
	cause velocity or rotational change, use the
	addForceAtPoint() function**/
	void AddForce(const double3 force);

	/**\brief adds a force to the object at a specific point.

	This is useful for a collision or simulating a force acting
	on an object somewhere on the surface.**/
	void AddForceAtPoint(const double3 force,const double3 origin);

	void Velocity(const double3 velocity);

	bool Enabled();
	bool Enabled(const bool enabled);

};

#endif // SINGLE_BODY_OBJECT_H
