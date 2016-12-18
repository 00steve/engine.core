#include "window.h"
void window_size_callback(GLFWwindow* window, int width, int height){
}


void window_close_callback(GLFWwindow* window){
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->RecalculateBounds();
}


Window::Window(VarMap settings) :
			Node(settings),
			windowHandle(NULL)
		{
    OnSetSettings();


}

Window::~Window(){
    if(windowHandle){
        glfwDestroyWindow(windowHandle);
    }
}

void Window::SetupOpenGL(){
    //glEnable(GL_SCISSOR_TEST);
    //glEnable(GL_STENCIL_TEST);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
	//glDepthFunc(GL_NOTEQUAL);// Set our depth function to overwrite if new value less than or equal to current value
	glLineWidth(2.0f);

    glEnable(GL_ALPHA);
    //glEnable(GL_CULL_FACE);
    //glDepthMask(GL_TRUE);
    //glCullFace(GL_BACK);
    //glEnable (GL_BLEND);
    //glEnable( GL_TEXTURE_2D );
    //glFrontFace(GL_CCW);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Ask for nicest perspective correction

}



void Window::SetupWindow(){
    if(windowHandle){
        //do something to kill the window context
        //build list of views that window contains and make them
        glfwDestroyWindow(windowHandle);
    }
    windowHandle = glfwCreateWindow(800,600,"Engine 2000xl",NULL,NULL);
    glfwSetWindowUserPointer(windowHandle,(void*)this);
    glfwMakeContextCurrent(windowHandle);
    //SetupOpenGL();
    //glfwSwapInterval(0); //let it render as fast a fucking possible, otherwise it is linked to the screen refresh rate
    //on a side note, if everything is still rendering at the same speed without slowing down, this is basically a free
    //fast pill that makes the program go as fast as it can, otherwise it sits back and does shit at the speed we need
    //it.
    glfwSetFramebufferSizeCallback(windowHandle, framebuffer_size_callback);
    glfwSetWindowSizeCallback(windowHandle, window_size_callback);
    glfwSetWindowCloseCallback(windowHandle, window_close_callback);
    RegisterGlobal(this,Settings().Name());
}


void Window::OnSetSettings(){

    bool rebuildWindow = false;
    VarMap settings = Settings();

    /*all of the functions that the window needs to be rebuilt, basically
    things that set window, framebuffer,etc. hints. These hints only take
    affect when the glfwCreateWindow function is called.*/
    if(settings.IsSet("multisampling")){
        int multisampling = settings.get<double>("multisampling");
        //cout << " - use multisampling " << multisampling << endl;
        glfwWindowHint(GLFW_SAMPLES,multisampling);
        rebuildWindow = true;
    }
    if(settings.IsSet("user-resizable")){
        bool resizable = settings.get<bool>("user-resizable");
        glfwWindowHint(GLFW_RESIZABLE,resizable ? GL_TRUE : GL_FALSE);
        rebuildWindow = true;
    }


    /*rebuild the window*/
    if(rebuildWindow || !windowHandle){
        Window::SetupWindow();
    }


	if(settings.IsSet("size")){
        List<double> winSize = settings.get<List<double> >("size");
        if(winSize.GetCount()==2){
            glfwSetWindowSize(windowHandle,winSize[0],winSize[1]);
        }
	}
	if(settings.IsSet("position")){
        List<double> position = settings.get<List<double> >("position");
        if(position.GetCount()==2){
            glfwSetWindowPos(windowHandle,position[0],position[1]);
        }
	}
	if(settings.IsSet("title")){
        glfwSetWindowTitle(windowHandle,settings.get<string>("title").c_str());
	}


    Node::OnSetSettings();
}

void Window::OnUnlink(Node* otherNode){
    if(views.Remove((View*)otherNode)){
        cout << " Window " << Id() << " removed reference to view " << otherNode->Id() << endl;
    }
}


void Window::Update(){
	glfwPollEvents();
    for(int i=0;i<views.GetCount();i++){
        views[i]->Update();
    }
	Node::Update();
}


void Window::Draw(){
    glfwMakeContextCurrent(windowHandle);
    for(int i=0;i<views.GetCount();i++){
        views[i]->Draw();
    }
	glfwSwapBuffers(windowHandle);
}

bool Window::ShouldClose(){
	return glfwWindowShouldClose(windowHandle);
}


bool Window::HandleMessage(NodeMessage message){
    switch(message.code){
    case MESSAGE_ADD_VIEW:
        AddView((View*)message.data);
        return true;
    case MESSAGE_GET_GLFW_WINDOW:
		CreateAndSendMessage(message.sender,MESSAGE_GET_GLFW_WINDOW,(void*)windowHandle);
		return true;
    }
	return Node::HandleMessage(message);
}

bool Window::AddView(View* newView){
    views.Push(newView);
    Link(newView);
    newView->RecalculateBounds(Size());
    return false;
}


void Window::RecalculateBounds(){
    int2 windowSize = Size();
    for(int i=0;i<views.GetCount();i++){
        views[i]->RecalculateBounds(windowSize);
    }
}

int2 Window::Size(){
    int2 windowSize;
    glfwGetWindowSize(windowHandle, &windowSize.x, &windowSize.y);
    return windowSize;
}

GLFWwindow* Window::Handle(){ return windowHandle; }
