#ifndef ENGINE_VIEW_H
#define ENGINE_VIEW_H

#include <GL/gl.h>
#define GLFW_INCLUDE_GLU
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "../core/node.h"
#include "../core/int2.h"
#include "../core/double2.h"
#include "../graphics/shader.h"

#define VIEW_SIZEBY_PIXEL 0
#define VIEW_SIZEBY_SCALE 1

#define VIEW_LOCATION_TOP_LEFT 0
#define VIEW_LOCATION_TOP 1
#define VIEW_LOCATION_TOP_RIGHT 2
#define VIEW_LOCATION_LEFT 3
#define VIEW_LOCATION_CENTER 4
#define VIEW_LOCATION_RIGHT 5
#define VIEW_LOCATION_BOTTOM_LEFT 6
#define VIEW_LOCATION_BOTTOM 7
#define VIEW_LOCATION_BOTTOM_RIGHT 8





class View : public Node{
private:


protected:

    string windowName;
    string renderName;

    int2 dimensions;
    double2 rawDimensions;
    int anchor;
    int sizeMethod;
    int2 offset;
    int2 calculatedOffset;
    double2 scale;
    double aspectRatio;
    int2 bottomLeft;



//protected:

    virtual void OnGlobalRequest(Node* globalNodeRef,string name);

    virtual void OnSetSettings();

    virtual bool HandleMessage(NodeMessage message);

    virtual void OnNodeUnlink(Node* otherNode);

    /**\brief The node that should be drawn by the view each pass.
    If this node reference is NULL, nothing will be drawn. It can be
    set by sending the NodeMessage with code MESSAGE_SET_RENDER_SUBJECT
    to the view node, passing the new subject Node reference as the data.*/
    Node* renderSubject;

public:

    View();

    ~View();

    virtual void Draw();

    virtual Node* RenderSubject(Node* renderSubject);

    void RecalculateBounds(int2 windowSize);


};



#endif // ENGINE_VIEW_H
