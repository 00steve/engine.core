#include "physics_world.h"


//dWorldID PhysicsWorld::worldID;
//dSpaceID PhysicsWorld::spaceID;
//dJointGroupID PhysicsWorld::contactGroup;
//double PhysicsWorld::stepSize  = .01;


PhysicsWorld::PhysicsWorld(){
    dInitODE();
    Init();
}

PhysicsWorld::~PhysicsWorld(){
    cout << "physics world > ";
    dCloseODE();
}

void PhysicsWorld::Step(){
    // remove all contact joints
    dJointGroupEmpty (contactGroup);
    // find collisions and add contact joints
    dSpaceCollide (spaceID,(void*)this,&PhysicsWorld::NearCallbackStatic);
    // step the simulation
    dWorldQuickStep(worldID,stepSize);
}

void PhysicsWorld::NearCallback (void *data, dGeomID o1, dGeomID o2) {

    /*check to see if there are two bodies and if they are connected
    by a joint already.*/
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);

    if(b1 && b2 && dAreConnected(b1,b2)) return;

    /*see if any of the objects that are colliding have info about
    them that can be checked.*/
    Physical* nA = (Physical*)dGeomGetData(o1);
    Physical* nB = (Physical*)dGeomGetData(o2);


    /*if both either of the geoms doesn't have a physical, or both
    physicals are static or both physicals are virtual, bail from
    the collision check.

    20150924 - If one of the geoms is static or virtual and the other
    geom is also static or virtual, don't count the collision*/
    if(!nA || !nB
        || (nA->Static() && nB->Static())
        //|| ((nA->isStatic() || nA->isVirtual()) && (nB->isStatic() || nB->isVirtual()))
    ){
        return;
    }

    /*check if either of the objects is not grouped, and if they are both
    grouped, make sure they are not in the same group*/
    if(!nA->Grouped() || !nB->Grouped() || nA->Group() != nB->Group()){

        /*check to see if the two physical objects contain contact information
        that should be used to determine how the contact point behaves*/
        dSurfaceParameters* csA = nA->SurfaceParameters(o1);
        dSurfaceParameters* csB = nB->SurfaceParameters(o2);
        dContact contact;
        if(!csA && !csB){ //if neither geom has surface properties
            contact.surface.mode = dContactBounce | dContactSoftCFM;// | dContactFDir1;//dContactSlip1  | dContactSlip2 ;
            contact.surface.mu = 2;
            contact.surface.bounce = 0.2;
            contact.surface.bounce_vel = 0.6;
            contact.surface.soft_cfm = 0.005;
        } else if(csA && !csB){ //if only geom A has surface properties
            contact.surface = *csA;
        } else if(csB && !csA){ //if only geom B has surface properties
            contact.surface = *csB;
        } else{  //if both geoms have surface properties
            cout << "warning, no logic implemented if both objects have surface properties. Fix this shit!\n";
        }
        dContactGeom contacts[8];
        if (int numc = dCollide (o1,o2,8,contacts,sizeof(dContactGeom))) {
            //dBodyID b1 = dGeomGetBody(o1);
            //dBodyID b2 = dGeomGetBody(o2);
            /*make sure that both physicals are not virtual before
            generating contact points*/
            if(!nA->Virtual() && !nB->Virtual()){
                for(int i=0;i<numc;i++){
                    contact.geom = contacts[i];
                    dJointID c = dJointCreateContact (worldID,contactGroup,&contact);
                    dJointAttach (c,b1,b2);
                }//end of looping through the collision points of the two bodies
            }//end of if both physicals are not virtual
            nA->AddCollision(new Collision(nB,contacts,numc));
            nB->AddCollision(new Collision(nA,contacts,numc));
        }//end of if both geoms have a physical that they are attached to
    }//end of if the two geometries collided
    else {
        cout << "neither geom is grouped\n";
    }
}

void PhysicsWorld::NearCallbackStatic(void *data,dGeomID o1, dGeomID o2){
    PhysicsWorld* world = (PhysicsWorld*)data;
    world->NearCallback(NULL,o1,o2);
}


void PhysicsWorld::PhysicsSettings(VarMap settings){
    cout << " - set physics settings\n";
    if(settings.IsSet("physics.gravity")){
        List<double> gravity = settings.get<List<double> >("physics.gravity");
        if(gravity.GetCount() == 3){
            SetGravity(double3(gravity[0],gravity[1],gravity[2]));
            //dWorldSetGravity(worldID,gravity[0],gravity[1],gravity[2]);
        } else if (gravity.GetCount() == 1){
            //dWorldSetGravity(worldID,0,gravity[0],0);
        } else {
            cout << "Incorrect number of physics.gravity values. Should be 1d or 3d i.e. gravity -9.81 or gravity 0,-9.81,0\n";
        }
    }


    if(settings.IsSet("physics.step-size")){
        //dReal stepSize = (float)settings.get<double>("physics.step-size");
    }

    /*
    dWorldSetERP (worldID, 0.10);	//the springiness of everything
    dWorldSetCFM (worldID, 0.02);//3e-5 (old) //the softness of everything
    dWorldSetAutoDisableFlag(worldID,true);
    dWorldSetQuickStepNumIterations (worldID, 10);
    dWorldSetContactMaxCorrectingVel (worldID, 5);
    dWorldSetAutoDisableLinearThreshold(worldID,0.000001f);
    dWorldSetAutoDisableAngularThreshold(worldID,0.0000001f);
    dWorldSetAutoDisableTime(worldID,.25);

*/


}


double3 PhysicsWorld::SetGravity(const double3 newGravity){
    dWorldSetGravity(worldID,newGravity.x,newGravity.y,newGravity.z);
    return newGravity;
}


/**\brief Builds a box of a set size with a set density
and adds the body to the world and the geom
to the space, then returns a reference to that
body.

The last variable is a reference to the node that the
box will be a part of. This is required so that when
collisions are performed, the engine can determine what
to do with the body. If it is set to null, no callback
will be performed.*/
void PhysicsWorld::NewBox(dBodyID &body,dGeomID &geom,double3 dimensions,double density,void* data){
    body = dBodyCreate(worldID);
    dMass m;
    dMassSetZero(&m);
    dMassSetBox(&m, density, dimensions.x, dimensions.y, dimensions.z);
    dBodySetMass(body, &m);
    geom = dCreateBox(spaceID,dimensions.x,dimensions.y,dimensions.z);
    dGeomSetBody (geom, body);
    dGeomSetData(geom,data);
}
/*PhysicsWorld::NewPlane(dBodyID &body,dGeomID &geom,double3 normal,void* data){
    double magnitude = normal.length();
    normal.normalize();
    geom = dCreatePlane (spaceID,normal.x,normal.y,normal.z,magnitude);
    dGeomSetData(geom,data);
}*/
void PhysicsWorld::NewPlane(dGeomID &geom,double3 normal,double offset,void* data){
    normal.normalize();
    geom = dCreatePlane (spaceID,normal.x,normal.y,normal.z,offset);
    dGeomSetData(geom,data);
}
void PhysicsWorld::NewSphere(dBodyID &body,dGeomID &geom,double radius,double density,void* data){
    body = dBodyCreate(worldID);
    dMass m;
    dMassSetZero(&m);
    dMassSetSphere(&m, density, radius);
    dBodySetMass(body, &m);
    geom = dCreateSphere(spaceID,radius);
    dGeomSetBody (geom, body);
    dGeomSetData(geom,data);
}
void PhysicsWorld::NewCylinder(dBodyID &body,dGeomID &geom,double radius,double length,double density, void* data){
    body = dBodyCreate(worldID);
    dMass m;
    dMassSetZero(&m);
    dMassSetCylinder(&m,density,3,radius,length);
    geom = dCreateCylinder(spaceID,radius,length);
    dGeomSetBody(geom,body);
    dMatrix3 rot;
    dRSetIdentity(rot);
    dRFromAxisAndAngle(rot,1,0,0,1.57f);
    dGeomSetRotation(geom,rot);
    dGeomSetData(geom,data);
}


void PhysicsWorld::NewUniversalJoint(dJointID &joint,dBodyID &body1,dBodyID &body2,double3 anchor){
    joint = dJointCreateUniversal(worldID, 0);
    dJointAttach(joint, body1, body2);
    dJointSetUniversalAnchor(joint, anchor.x, anchor.y, anchor.z);
}


void PhysicsWorld::NewBallAndSocketJoint(dJointID &joint,dBodyID &body1,dBodyID &body2,double3 anchor){
    joint = dJointCreateBall(worldID,0);
    dJointAttach(joint,body1,body2);
    dJointSetBallAnchor(joint,anchor.x,anchor.y,anchor.z);
}


    /*generates a capsule with the given height and radius, with the mass
    that was argued.*/
void PhysicsWorld::NewCapsule(dBodyID &body,dGeomID &geom,double radius,double height,double density, void* data){
    body = dBodyCreate(worldID);
    dMass m;
    //dMassSetZero(&m);
    dMassSetCapsule(&m,density,2,radius,height);
    geom = dCreateCapsule(spaceID,radius,height);
    dGeomSetBody(geom,body);
    dMatrix3 rot;
    dRSetIdentity(rot);
    dRFromAxisAndAngle(rot,1,0,0,1.57f);
    dGeomSetRotation(geom,rot);
    dGeomSetData(geom,data);
}



PhysicsGroup* PhysicsWorld::BuildPhysicsGroup(VarMap* groupSettings,void* data){
        //cout << "build physics group\n";
        bool containsJoints = false;

        PhysicsGroup* pGroup = new PhysicsGroup();

        List<string> groupNames = groupSettings->GroupNames();
        for(int i=0;i<groupNames.GetCount();i++){
            //cout << " - build object " << groupNames[i] << endl;
            if(groupNames[i] == "joints"){
                //cout << "   - skip\n";
                containsJoints = true;
                continue;
            }
            VarMap props = groupSettings->GetGroup(groupNames[i]);
            dGeomID geom;
            dBodyID body;

            double density;
            if(props.IsSet("density")){
                density = props.get<double>("density");
                if(density == 0){
                    cout << "   - cannot be set to zero, being set to 1.\n";
                    density = 1;
                }
            } else {
                density = 1;
            }

            if(props.IsSet("geometry")){
                string shape = props.get<string>("geometry");

                if(shape == "box"){
                    double3 dimensions;
                    if(props.IsSet("lengths")){
                        dimensions = props.get<double3>("lengths");
                    } else {
                        dimensions = double3(1,1,1);
                    }
                    NewBox(body,geom,dimensions,density,data);
                }//end of if is a box

                else if(shape == "plane"){
                    double3 normal;
                    double offset;
                    if(props.IsSet("normal")){
                        normal = props.get<double3>("normal");
                    } else {
                        normal = double3(0,1,0);
                    }
                    if(props.IsSet("offset")){
                        offset = props.get<double>("offset");
                    } else {
                        offset = 0;
                    }
                    NewPlane(geom,normal,offset,data);
                }//end of if is plane

                else if(shape == "sphere"){
                    double radius;
                    if(props.IsSet("radius")){
                        radius = props.get<double>("radius");
                    } else {
                        radius = 1;
                    }
                    NewSphere(body,geom,radius,density,data);
                }//end of if is sphere

                else if(shape == "cylinder"){
                    double radius;
                    double length;
                    if(props.IsSet("radius")){
                        radius = props.get<double>("radius");
                    } else {
                        radius = .5;
                    }
                    if(props.IsSet("length")){
                        length = props.get<double>("length");
                    } else {
                        length = 1;
                    }
                    NewCylinder(body,geom,radius,length,density,data);
                }

                else if(shape == "capsule"){
                    double radius;
                    double length;
                    if(props.IsSet("radius")){
                        radius = props.get<double>("radius");
                    } else {
                        radius = .5;
                    }
                    if(props.IsSet("length")){
                        length = props.get<double>("length");
                    } else {
                        length = 2;
                    }
                    NewCapsule(body,geom,radius,length-radius*2,density,data);
                    //cout << "- build capsule\n";
                }


                else {
                    cout << "NO GEOMETRY TYPE IS SET\n";
                }
            }


            if(body){
                if(props.IsSet("position")){
                    double3 pos = props.get<double3>("position");
                    //cout << "set position for body " << groupNames[i] << " at " << pos.x << "," << pos.y << "," << pos.z << endl;
                    dBodySetPosition(body,pos.x,pos.y,pos.z);
                }

                if(props.IsSet("velocity")){
                    double3 velocity = props.get<double3>("velocity");
                    dBodySetLinearVel(body,velocity.x,velocity.y,velocity.z);
                }
                pGroup->body.push(body,groupNames[i]);

            }

            if(geom){
                pGroup->geom.push(geom,groupNames[i]);
            }
        }


        //build any joints
        if(containsJoints){
            VarMap jointsGroup = groupSettings->GetGroup("joints");
            groupNames = jointsGroup.GroupNames();
            for(int i=0;i<groupNames.GetCount();i++){
                //cout << " - build joint " << groupNames[i] << endl;
                VarMap jProps = jointsGroup.GetGroup(groupNames[i]);
                string type = "ball-and-socket";
                if(jProps.IsSet("type")){
                    type = jProps.get<string>("type");
                }
                dJointID joint;
                dBodyID body1 = pGroup->body[jProps.get<string>("body1")];
                dBodyID body2 = pGroup->body[jProps.get<string>("body2")];
                //cout << "   - type : " << type << endl;
                if(type == "ball-and-socket"){
                    double3 anchor = jProps.get<double3>("anchor");
                    NewBallAndSocketJoint(joint,body1,body2,anchor);
                    pGroup->joint.push(joint,groupNames[i]);
                } else if(type == "universal"){
                    double3 anchor = jProps.get<double3>("anchor");
                    NewUniversalJoint(joint,body1,body2,anchor);
                    dJointSetUniversalAxis1(joint, 1, 0, 0);
                    dJointSetUniversalAxis2(joint, 0, 0, 1);
                    pGroup->joint.push(joint,groupNames[i]);

                } else {
                    cout << "   - [joint type not implimented]\n";
                }

            }
        }




        return pGroup;
    }


void PhysicsWorld::GlMatrix(dBodyID body,float *matrix){
    const dReal *R = dBodyGetRotation(body);
    const dReal *pos = dBodyGetPosition(body);
    matrix[0]=R[0];
    matrix[1]=R[4];
    matrix[2]=R[8];
    matrix[3]=0;
    matrix[4]=R[1];
    matrix[5]=R[5];
    matrix[6]=R[9];
    matrix[7]=0;
    matrix[8]=R[2];
    matrix[9]=R[6];
    matrix[10]=R[10];
    matrix[11]=0;
    matrix[12]=pos[0];
    matrix[13]=pos[1];
    matrix[14]=pos[2];
    matrix[15]=1;
}

void PhysicsWorld::GlMatrix(dGeomID geom,float *matrix){
    const dReal *R = dGeomGetRotation(geom);
    const dReal *pos = dGeomGetPosition(geom);
    matrix[0]=R[0];
    matrix[1]=R[4];
    matrix[2]=R[8];
    matrix[3]=0;
    matrix[4]=R[1];
    matrix[5]=R[5];
    matrix[6]=R[9];
    matrix[7]=0;
    matrix[8]=R[2];
    matrix[9]=R[6];
    matrix[10]=R[10];
    matrix[11]=0;
    matrix[12]=pos[0];
    matrix[13]=pos[1];
    matrix[14]=pos[2];
    matrix[15]=1;
}

