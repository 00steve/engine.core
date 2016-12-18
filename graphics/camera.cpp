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

bool Camera::Init(){
    /*
    pipeline = newPipeline;
    _cam = h3dAddCameraNode( targetNode, "Camera", pipeline );
    h3dSetNodeParamI( _cam, H3DCamera::ViewportXI, 0 );
    h3dSetNodeParamI( _cam, H3DCamera::ViewportYI, 0 );
    h3dSetNodeParamI( _cam, H3DCamera::ViewportWidthI, 800 );
    h3dSetNodeParamI( _cam, H3DCamera::ViewportHeightI, 600 );
    h3dSetupCameraView( _cam, 45.0f, (float).75, 0.5f, 2048.0f );
    */
    //OnSetSettings();
    return true;
}
