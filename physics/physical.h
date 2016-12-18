/*the physical class provides an extension to any class
that wants to store properties about how it should collide
or interact with other physical objects in the ODE simulation.*/

#ifndef PHYSICAL_H
#define PHYSICAL_H

#include <engine/core/node.h>
#include <engine/physics/collision.h>
#include <engine/physics/physics_world.h>

#define PROJECTILE_GROUP 1
#define EXPLOSION_GROUP 2

#define GOOD_GUYS 10
#define BAD_GUYS 11
#define OTHER_GUYS 12


class Physical : public Node {
private:

    /*the maximum number of  contact points that can be created
    between this physical object's bodies and any other physical
    object's bodies.*/
    int maxContactPoints;

    /*if the object is set as virtual, it will not generate
    collision contact points between it and any other physicals
    that it is colliding with. Making something virtual is great
    when you want to see if things intersect without colliding
    them.*/
    bool virt;

    bool stat;

	bool grouped;

	bool selected;
	bool selectable;

    /*if the int is in a group of like bodies, they should not
    generate collision data. This is useful because you could
    set all projectiles in the engine to one group, so they
    will not interact with each other, or set all of the bodies
    in a vehicle to one group so that they won't interact with
    each other. Each physical that is created is automatically
    given its own group (it's unique node id is used).*/
    unsigned int group;

    unsigned int contact_mode;

	bool shouldDefaultEnabled;

	/*easy helper properties to store collision groups so they
	don't have to be a compiler something or another.*/
	static map<int> collision_groups;
	static int next_collision_group_id;

	PhysicsWorld* world;

protected:

    /*store a list of nodes that this node has collided with.
    This will be performed by the physics engine.*/
    List<Collision*>  collisions;

	static int GetNextCollisionGroupId(string name);

	virtual void OnSetSettings();

	/**\brief Default Physical override of the Node::OnSetParent() virtual
	callback.

	When a parent is set, it checks to see if there is a PhysicsWorld class
	in the node tree above this class. If it finds one, it calls the
	OnSetWorld() function.**/
	virtual void OnSetParent();

	/**\brief Callback for when a PhysicsWorld is set.

	This is called when the Physical has parent, or grandparent, etc. that
	inherits from the PhysicsWorld class. If it does, the World() function
	can be used to return a pointer to that object, and that pointer can
	be used to create bodies, shapes, joints, etc. for physical simulation.

	By default this callback does nothing*/
	virtual void OnSetWorld();

	virtual bool HandleMessage(NodeMessage message);
public:

    Physical();

    ~Physical();

    /**\brief returns the physics world object, used for creating
    new physics bodies.

    This will return the nearest parent that inherits from the physics
    world class. This could possibly a very costly function, so should
    only be used
    **/

    /**\brief sets the contact parameters to use for contact
    points between this body and other bodies in the physics
    engine.**/
    unsigned int ContactMode(unsigned int mode);


    /**\brief updates the physical object. By default, this really
    just clears out the collisions list. Any classes that override
    this should also call collisions.clear(), or they will keep
    accumulating, which is probably a bad thing.
    */
    virtual void Update();

    void AddCollision(Collision* c);

    int MaxContactPoints();
    /*check if an object is virtual*/
    bool Virtual();

    /*check if an object is static*/
    bool Static();

	/**\brief check if a physical object is enabled or not.

	This is a general type function, so it is left up to things implementing it to decide if they are enabled or not.**/
	virtual bool Enabled();

    /**\brief if an object is virtual it will not generate contact hinges that apply forces to it or what is colliding with it.**/
    bool Virtual(bool setVirt);

    /**\brief set if the object should not collide with itself or other objects of its type.**/
    bool Static(bool setStat);

	/**\brief "should" set the object and any bodies it is comprised of to have infinite mass.**/
	virtual void Kinematic();

	/**\brief should remove the infinite mass from any bodies that
	the object is comprised of:

	basically, set them back to what they were before they went
	kinematic**/
	virtual void Dynamic();

	virtual bool Enabled(const bool enabled);

	virtual void LinearDampening(double scale);
	virtual void AngularDampening(double scale);

    virtual double3 Position();

	/**\brief implementable function that lets an inheriting class
	determine how it should work. It should shift the position of
	the object by the amount provided in world space.*/
	virtual void ShiftPosition(const double3 offset);

    /**\brief removes the geom or geoms from a physical
    and attaches them to a body.*/
    virtual void GlueTo(dBodyID body,double3 offset);

    /**\brief takes two bodies and attaches them together
    with a joint. If too much force is applied to this
    joint, it will break.*/
    virtual void AttachTo(dBodyID body,double3 offset,double strength);

    /**\brief add forces to any bodies in the physical*/
    virtual void AddForce(const double3 force);

	/**\brief ignoring what the current velocity or other
	forces are acting on the object, set its velocity however
	the inheriting class wishes to the amount set in
	world space*/
	virtual void Velocity(const double3 velocity);

    /**\brief returns a body that is being updated in the physics
    engine that the physical object is using.

    By default, no body is returned. It is up to the inheriting
    class to determine if a body can be returned or what index
    any bodies should have.**/
    virtual dBodyID Body(unsigned int index);

	/**\brief returns the number of bodies that the physical object
	has.

	It defaults to zero so that the program doesn't die, but should
	be overwritten by any inheriting classes.**/
	virtual unsigned int BodyCount();

    /**\brief returns a joint that is being updated in the physics
    engine that the physical object is using.

    By default, no joint is returned. It is up to the inheriting
    class to determine if a joint can be returned or what index
    any joints should have.**/
    virtual dJointID Joint(unsigned int index);

	bool Grouped();

	/*if the object can be selected. This allows an object to exist
	in the physics engine, that can be selected by the selectionVector,
	but can be discarded if the user should not be able to select it.*/
	virtual bool Selectable();

    unsigned int Group();

    unsigned int Group(unsigned int newGroup);

    /**\brief return the contact settings of a particular geom that
    is part of this physical object.

    By default nothing is returned. This can be overridden so that
    it returns any settings based on the geom id that is passed to
    it. This allows the programmer to let physicals return a different
    contact_setting for each contact part of it. */
    virtual dSurfaceParameters* SurfaceParameters(dGeomID);


	virtual void DisableBodies();

	/**\brief this is a generic function that returns if the physical
	object is enabled or not.

	If it is, things should collide against it and its bodies should still
	be active in the simulation. If this is set to false, none of the bodies
	that constitutes the physical should be enabled.*/
	virtual bool DefaultEnabled();

	virtual void DefaultEnabled(const bool enabled);

    PhysicsWorld* World();


};

#endif // PHYSICAL_H
