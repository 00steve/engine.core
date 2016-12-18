#include "collision.h"

Collision::Collision() :
    other(NULL),
    contacts(NULL),
    contact_count(0){

}


Collision::Collision(Physical* other,dContactGeom* contacts,int contact_count) :
        other(other),
        contacts(contacts),
        contact_count(0)
        {
    //temp just to get the ray collision
    if(contact_count >2) return;

    contact_points = new double3[contact_count];
    while(this->contact_count < contact_count){
        contact_points[this->contact_count].x = contacts[this->contact_count].pos[0];
        contact_points[this->contact_count].y = contacts[this->contact_count].pos[1];
        contact_points[this->contact_count].z = contacts[this->contact_count].pos[2];
        ++this->contact_count;
    }
}

Physical* Collision::GetCollider(){
    return other;
}

int Collision::ContactCount(){ return contact_count; }

dContactGeom Collision::GetContact(int index){ return contacts[index]; }

dContactGeom* Collision::GetContacts(){ return contacts; }

Physical* Collision::OtherPhysical(){return other;}

double3 Collision::GetContactPoint(const int index){ return contact_points[index]; }



