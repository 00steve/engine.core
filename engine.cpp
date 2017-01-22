#include "engine.h"

Engine::Engine()
    {
	glfwInit();


}

Engine::~Engine(){

    if(engineControlStack.GetCount()){
        cout << "delete engine control\n";
        delete engineControlStack.Pop();
    }

	glfwTerminate();
	cout << "Kill engine\n";
}

/**\brief Initializes the engine and loads anything that is defined in the default.est
configuration file. It loads all of that stuff right away so it is available to anything
that wants to reference it.*/
bool Engine::Init(){
    //set this node's time step reference
    TimeStepRef(timer.TimeStepRef());
    //cout << "Engien::TimeStep = " << TimeStepRef() << endl;
    //load asset library

	//load default settings
	VarMap settings = AssetLibrary::LoadSettings("default.est");
	settings = settings.GetGroup("default");

    if(settings.IsSet("engine-control.type")){
		EngineControl* ec = (EngineControl*) Load("control",settings.GetGroup("engine-control"));
		engineControlStack.Push(ec);
		Node::RegisterGlobal((Node*)ec,"engine-control");
    }

    //load any windows
    VarMap windowsSettings =settings.GetGroup("windows");
    if(windowsSettings.GetCount()){
        List<string> windowNames = windowsSettings.GroupNames();
        for(int i=0;i<windowNames.GetCount();i++){
            VarMap windowSettings = windowsSettings.GetGroup(windowNames[i]);
            Window* window = new Window(windowSettings);
            if(window->Handle()){
                windows.Push(window);
            } else {
                cout << "Couldn't create the window: " << windowNames[i] << "\n";
            }
        }
    }
    if(windows.GetCount() == 0){
        cout << "engine can't continue initialization without a window\n";
        return false;
    }

    //load any cameras
    VarMap cameras = settings.GetGroup("cameras");
    List<string> cameraNames = cameras.GroupNames();
    for(int i=0;i<cameraNames.GetCount();i++){
        Load("camera",cameras.GetGroup(cameraNames[i]+""));
    }
    cout << "loaded camera(s)\n";

	//load any views
    VarMap views = settings.GetGroup("views");
    List<string> viewNames = views.GroupNames();
    for(int i=0;i<viewNames.GetCount();i++){
        cout << " view name : " << viewNames[i] << endl;
        Load("view",views.GetGroup(viewNames[i]+""));
    }
    cout << "loaded view(s)\n";
/*
	//try to load any misc. objects that have a name and a type and in the
	//  custom {
	//      object {
	// 		    type objectType
	//      }
	//  }
	//This should load the .dll with the name engine.object.objectType.dll
	//from the engine/assets/extensions folder
	VarMap customNodes = settings.GetGroup("custom");
	List<string> customNodeNames = customNodes.GroupNames();
	for(int i=0;i<customNodeNames.GetCount();i++){
		AssetLibrary::LoadCustom(customNodeNames[i],customNodes.GetGroup(customNodeNames[i]));
	}
    */



    Node::RegisterGlobal((Node*)this,"engine");
	return true;
}

void Engine::Run(){
    cout << "made it to run\n";
	while(windows.GetCount()){
        timer.Update();
        glfwPollEvents();

        if(engineControlStack.GetCount()){
            engineControlStack.Last()->Update();
        }

        for(int i=0;i<windows.GetCount();i++){
            Window* window = windows[i];
            window->Update();
            window->Draw();
            if(window->ShouldClose()){
                cout << "closed window\n";
                windows.Cut(i);
                delete window;
            }
        }
	}
}
