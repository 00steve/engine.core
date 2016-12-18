#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <windows.h>
#include <GLFW/glfw3.h>

#include <engine/core/node.h>
#include <engine/core/int2.h>
#include <engine/graphics/view.h>

class Window : public Node{
private:

    GLFWmonitor* primary;
	GLFWwindow* windowHandle;

    /**\brief The list of views that are being rendered in this window.

    Views can be added directly by calling the AddView() function or passing
    the view as a NodeMessage using the Message() function with the code
    MESSAGE_ADD_VIEW.

    When a view is added, the view and window are now referencing each other.
    This is handled automatically, so if a view is deleted, it is automatically
    removed from the list of views that the window draws.*/
    List<View*> views;


    void SetupOpenGL();

    void SetupWindow();

protected:

    virtual void OnUnlink(Node* otherNode);

	virtual bool HandleMessage(NodeMessage message);

    /**\brief Sets up the window based on the Node settings that were loaded.

    Is called when the Settings(VarMap settings) function is called, or in the
    Window() constructor is called.

    First it sets the window hints that need to be set before GLFW builds the
    window, then if any hints have been set or the window does not yet exist,
    it calls the code to build the window. After the window has been built, it
    calls any functions that can only be called after the window exists.*/
    virtual void OnSetSettings();


public:

	Window(VarMap settings);
	~Window();

	virtual void Update();

	/**\brief Draws all of the views contained in the window.

	The Draw() function is called for every view in the window. Right now it
	is first stored, first drawn, so there is no good way to order overlapping
	windows. It's pretty janky, I know, if you don't like it go build your own
	engine.

	There are several ways that a view can be added to the window, if the view
	has the window property set to the name of a window in the config file, the
	view will be added when both the view and window exist. Views can also
	manually be added to the window calling AddView() function.*/
	virtual void Draw();

	bool ShouldClose();

	/**\breif Add a new View to the list of views that are rendered in the window.*/
	bool AddView(View* newView);

	void RecalculateBounds();

    GLFWwindow* Handle();

    int2 Size();


};



#endif // ENGINE_WINDOW_H
