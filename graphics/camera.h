#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include "../core/node.h"

class Camera : public Node{
private:


protected:

    virtual void OnSetSettings();

public:


    Camera();
    ~Camera();

    virtual bool TranslateView();
    virtual float* TranslationMatrix();

    bool Init();

};





#endif // ENGINE_CAMERA_H
