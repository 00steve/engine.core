#include "camera.h"



Camera::Camera()// :
        //_cam(0)
        {
}

Camera::~Camera(){
}


void Camera::OnSetSettings(){
    RegisterGlobal(this,Settings().Name());
}


bool Camera::TranslateView(){
	return false;
}

float* Camera::TranslationMatrix(){
    return NULL;
}

bool Camera::Init(){
    return true;
}
