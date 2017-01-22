#ifndef ENGINE_H
#define ENGINE_H

#define GLFW_INCLUDE_GLU
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "core/node.h"
#include "core/enginecontrol.h"
#include "graphics/window.h"

/**\brief This is the class that you add to your main() function. It is the engine.

It will load any objects in the default{} group of the default.est file. It should be
given a default engine-control object.

After declaring the instance of Engine(), call the Init() function to set everything
up. Then call Run() and you're good to. The following example is all you should need
in the main.cpp file to get up and running.

int main()
{

	Engine engine;
	if(!engine.Init()){
		cout << "engine failed to initialize\n";
		return 0;
	}
	engine.Run();
    return 0;
}*/
class Engine : public Node{
private:

	List<EngineControl*> engineControlStack;
	List<Window*> windows;

	/**\brief Keep track of the global time.

	The timer is updated by the engine and set in a static variables for
	all of the other nodes to use. It is updated once every loop in the
	Run() function.*/
    Timer timer;
public:

    Engine();
    ~Engine();

    /**\brief Sets up all of the default objects in the engine.

    Builds any default windows, cameras, loads the default engine-control

    In order for the game loop to run in the Run() function, at least one
    window must be created. If one or more windows is not created in the
    Init() funciton, it will stop loading and return false. If everything
    loaded correctly, it will return true.*/
	bool Init();

    /**\brief The main loop of the engine. Here various components are updated,
    such as input, keeping the game timer updated, etc. Also, the current
    engine-control is updated.

    To exit the game loop, remove all windows. The loop will continue until there
    are none.*/
	void Run();

};




#endif // ENGINE_H
