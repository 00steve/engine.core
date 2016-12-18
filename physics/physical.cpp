#include "physical.h"

map<int> Physical::collision_groups = map<int>();
int Physical::next_collision_group_id = 0;


int Physical::GetNextCollisionGroupId(string name){
    collision_groups.push(next_collision_group_id,name);
    return next_collision_group_id++;
}

void Physical::OnSetSettings(){
    Node::OnSetSettings();
}

void Physical::OnSetParent(){
    world = Closest<PhysicsWorld>();
    if(world) OnSetWorld();
}

void Physical::OnSetWorld(){}

Physical::Physical() :
        maxContactPoints(4),
        virt(false),
        stat(false),
        grouped(false),
        selected(false),
        selectable(true),
        group(0),
        contact_mode(dContactBounce | dContactSoftCFM),
        shouldDefaultEnabled(false),
        world(NULL) {
    }

Physical::~Physical(){
    collisions.Clear();
}

unsigned int Physical::ContactMode(unsigned int mode){
    return contact_mode = mode;
}

void Physical::Update(){
    Node::Update();
    collisions.Clear();
}

void Physical::AddCollision(Collision* c){
    collisions.Push(c);
}

int Physical::MaxContactPoints(){
    return maxContactPoints;
}

bool Physical::Virtual(){ return virt; }

bool Physical::Static(){ return stat; }

bool Physical::Enabled(){return false; }

bool Physical::Virtual(bool setVirt){ return virt = setVirt; }

bool Physical::Static(bool setStat){ return stat = setStat; }

void Physical::Kinematic(){}

void Physical::Dynamic(){}

bool Physical::Enabled(const bool enabled){ return false; };

void Physical::LinearDampening(double scale){}

void Physical::AngularDampening(double scale){}

double3 Physical::Position(){return double3(0,0,0);}

void Physical::ShiftPosition(const double3 offset){}

void Physical::GlueTo(dBodyID body,double3 offset){}

void Physical::AttachTo(dBodyID body,double3 offset,double strength){}

void Physical::AddForce(const double3 force){};

void Physical::Velocity(const double3 velocity){};

dBodyID Physical::Body(unsigned int index){
    return NULL;
}

unsigned int Physical::BodyCount(){
    return 0;
}

dJointID Physical::Joint(unsigned int index){
    return NULL;
}

bool Physical::Grouped(){
    return grouped;
}

bool Physical::Selectable(){
    return selectable;
}

unsigned int Physical::Group(){
    return group;
}

unsigned int Physical::Group(unsigned int newGroup){
    return group = newGroup;
}

dSurfaceParameters* Physical::SurfaceParameters(dGeomID){
    return NULL;
}

bool Physical::HandleMessage(NodeMessage message){
    switch(message.code){
    case MESSAGE_SELECTED:
        selected = true;
        selectable = false;
        return true;
    case MESSAGE_DESELECTED:
        selected = false;
        selectable = true;
        return true;
    case MESSAGE_SET_VELOCITY:
        return true;
    case MESSAGE_SET_ORIENTATION:
        return true;
    case MESSAGE_SET_ROTATION:
        return true;
    }
    return Node::HandleMessage(message);
}

void Physical::DisableBodies(){}

bool Physical::DefaultEnabled(){ return shouldDefaultEnabled; };

void Physical::DefaultEnabled(const bool enabled){
    shouldDefaultEnabled = enabled;
}

PhysicsWorld* Physical::World(){ return world; }


