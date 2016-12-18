#ifndef TD_COLLISION_H
#define TD_COLLISION_H

#define dSINGLE
#include <ode/ode.h>
#include <engine/core/double3.h>

class Physical;

class Collision {
private:
    Physical *other;
    dContactGeom *contacts;
    double3 *contact_points;
    int contact_count;
public:

    Collision();
    Collision(Physical* other,dContactGeom* contacts,int contact_count);

    Physical* GetCollider();

    int ContactCount();

    dContactGeom GetContact(int index);

    dContactGeom* GetContacts();

    Physical* OtherPhysical();

    double3 GetContactPoint(const int index);

};


#endif // TD_COLLISION_H
