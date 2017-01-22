#include "view.h"


void View::OnGlobalRequest(Node* globalNodeRef,string name){
    if(name == windowName){
        Node* window = Node::Global(windowName);
        if(window){
            Node::CreateAndSendMessage(window,MESSAGE_ADD_VIEW,(void*)this);
        }
        return;
    }
    if(name == renderName){
        RenderSubject(Node::Global(renderName));
        //if(renderSubject) cout << " - found render subject " << renderName << endl;
    }
}

void View::OnSetSettings(){


	//_pipeRes = h3dAddResource( H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0 );

    //try to set to window
    if(Settings().IsSet("window")){
        windowName = Settings().get<string>("window");
        GlobalRequest(windowName);
    }
    if(Settings().IsSet("render")){
        renderName = Settings().get<string>("render");
        GlobalRequest(renderName);
        //cout << " - view has render subject set = " << renderName << endl;

    }
    if(Settings().IsSet("size-method")){
        string sizeMethodString = Settings().get<string>("size-method");
        if(sizeMethodString == "pixel"){
            sizeMethod = VIEW_SIZEBY_PIXEL;
        } else if(sizeMethodString == "percentage"){
            sizeMethod = VIEW_SIZEBY_SCALE;
        } else {
            cout << "invalid size-method: must be \"pixel\" or \"percentage\"\n";
        }
    }
    if(Settings().IsSet("anchor")){
        string locationString = Settings().get<string>("anchor");
        if(locationString == "top-left"){
            anchor = VIEW_LOCATION_TOP_LEFT;
        } else if(locationString == "top"){
            anchor = VIEW_LOCATION_TOP;
        } else if(locationString == "top-right"){
            anchor = VIEW_LOCATION_TOP_RIGHT;
        } else if(locationString == "right"){
            anchor = VIEW_LOCATION_RIGHT;
        } else if(locationString == "bottom-right"){
            anchor = VIEW_LOCATION_BOTTOM_RIGHT;
        } else if(locationString == "bottom"){
            anchor = VIEW_LOCATION_BOTTOM;
        } else if(locationString == "bottom-left"){
            anchor = VIEW_LOCATION_BOTTOM_LEFT;
        } else if(locationString == "left"){
            anchor = VIEW_LOCATION_LEFT;
        } else if(locationString == "center"){
            anchor = VIEW_LOCATION_CENTER;
        }
    }
    if(Settings().IsSet("offset")){
        List<double> offsets = Settings().get<List<double> >("offset");
        if(offsets.GetCount() == 2){
            offset = int2(offsets[0],offsets[1]);
        } else {
            cout << "Invalid offset value. Offset should be 2d, i.e. offset 10,500\n";
        }
    }
    if(Settings().IsSet("scale")){
        List<double> scales = Settings().get<List<double> >("scale");
        if(scales.GetCount() == 2){
            scale = double2(scales[0],scales[1]);
        } else {
            cout << "Invalid view scale. View scale must be 2d value, i.e. 1,1 or .5,.25\n";
        }
    }



    Node::OnSetSettings();
}

bool View::HandleMessage(NodeMessage message){
    switch(message.code){
    case MESSAGE_SET_RENDER_SUBJECT:
        RenderSubject((Node*) message.data);
        return true;
    }
    return Node::HandleMessage(message);
}

void View::OnNodeUnlink(Node* otherNode){
    if(otherNode == renderSubject){
        renderSubject = NULL;
        cout << "Unlink view from render subject\n";
    }
}

View::View() :
    dimensions(int2(800,600)),
    rawDimensions(double2(800,600)),
    anchor(VIEW_LOCATION_CENTER),
    sizeMethod(VIEW_SIZEBY_SCALE),
    offset(int2(0,0)),
    calculatedOffset(int2(0,0)),
    scale(double2(1,1)),
    aspectRatio(1.33),
    bottomLeft(int2(0,0))
    {
}

View::~View(){
}


void View::Draw(){
	glScissor(bottomLeft.x,bottomLeft.y,dimensions.x,dimensions.y);
	glViewport(bottomLeft.x,bottomLeft.y,dimensions.x,dimensions.y);

    glClear(GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if(renderSubject) renderSubject->Draw();
	glPopAttrib();
}


Node* View::RenderSubject(Node* renderSubject){
    if(this->renderSubject){
        Unlink(this->renderSubject);
    }
    this->renderSubject = renderSubject;
    Link(renderSubject);
    return renderSubject;
}


void View::RecalculateBounds(int2 windowSize) {
    switch(sizeMethod){
        case VIEW_SIZEBY_PIXEL:
            calculatedOffset = offset;
            break;
        case VIEW_SIZEBY_SCALE:
            dimensions = int2(windowSize.x*scale.x,windowSize.y*scale.y);
            rawDimensions = double2((double)windowSize.x*scale.x,(double)windowSize.y*scale.y);
            calculatedOffset = int2(windowSize.x*offset.x,windowSize.y*offset.y);
            break;
    }
    //calculate aspect ratio
    aspectRatio = (double)dimensions.x / (double)dimensions.y;
    //find the bounds of the view based on its anchor setting
    switch(anchor) {
        case VIEW_LOCATION_TOP_LEFT:
            bottomLeft = int2(windowSize.x*calculatedOffset.x,windowSize.y-dimensions.y-windowSize.y*calculatedOffset.y);
            break;
        case VIEW_LOCATION_TOP:
            bottomLeft = int2((windowSize.x - dimensions.x) * .5f +windowSize.x*calculatedOffset.x , windowSize.y-dimensions.y-windowSize.y*calculatedOffset.y);
            break;
        case VIEW_LOCATION_TOP_RIGHT:
            bottomLeft = int2(windowSize.x-dimensions.x-windowSize.x*calculatedOffset.x,windowSize.y-dimensions.y-windowSize.y*calculatedOffset.y);
            break;
        case VIEW_LOCATION_LEFT:
            bottomLeft = int2( windowSize.x*calculatedOffset.x, (windowSize.y - dimensions.y) * .5f);
            break;
        case VIEW_LOCATION_CENTER:
            bottomLeft = int2((windowSize.x - dimensions.x) * .5f +windowSize.x*calculatedOffset.x , (windowSize.y - dimensions.y) * .5f);
            break;
        case VIEW_LOCATION_RIGHT:
            bottomLeft = int2( windowSize.x-dimensions.x-windowSize.x*calculatedOffset.x, (windowSize.y - dimensions.y) * .5f);
            break;
        case VIEW_LOCATION_BOTTOM_LEFT:
            bottomLeft = int2( windowSize.x*calculatedOffset.x, windowSize.y*calculatedOffset.y);
            break;
        case VIEW_LOCATION_BOTTOM:
            bottomLeft = int2((windowSize.x - dimensions.x) * .5f , windowSize.y*calculatedOffset.y);
            break;
        case VIEW_LOCATION_BOTTOM_RIGHT:
            bottomLeft = int2(windowSize.x-dimensions.x-windowSize.x*calculatedOffset.x, windowSize.y*calculatedOffset.y);
            break;
    }
}

