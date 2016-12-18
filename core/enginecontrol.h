#ifndef ENGINE_CONTROL_H
#define ENGINE_CONTROL_H

#include "node.h"
//#include "../physics/physics_world.h"

using namespace std;

class EngineControl : public Node{
protected:
    virtual void OnSetSettings();
    virtual void OnGlobalRequest(Node* globalNodeRef,string name);

public:
    ~EngineControl();

	virtual void Update();

};


#endif // ENGINE_CONTROL_H
