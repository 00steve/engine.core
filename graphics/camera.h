#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <engine/core/node.h>

class Camera : public Node{
private:


protected:

    virtual void OnSetSettings();

public:


    Camera();
    ~Camera();

    virtual bool TranslateView();

    bool Init();

};





#endif // ENGINE_CAMERA_H
