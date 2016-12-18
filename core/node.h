#ifndef ENGINE_NODE_H
#define ENGINE_NODE_H

#include <iostream>
#include <engine/core/nodemessage.h>
#include <engine/core/list.h>
#include <engine/core/map.h>
#include <engine/core/varmap.h>
#include <engine/core/noderequest.h>
#include <engine/core/timer.h>
#include <engine/assetlibrary/assetlibrary.h>

//using namespace std;


/**
About
----------------------------------------------------------------
This is the class that most all other engine classes are based on. It contains all of the basic expected functionality of
any nodes in the engine. It takes care of linking children to parents, reference counting, and various other boilerplate
things that would be very annoying to do over and over again.

It also provides a default communication method to allow any node with a pointer to any other node to send it a message of
any type, via the NodeMessage class.

Static member variables
----------------------------------------------------------------
Due to the fact that .dlls don't share the same memory space, or
something, by default, any static objects that are created for the
node class must be passed to any nodes loaded from dlls by reference.
Make sure the following is implemented whenever a static member
variable is added.
1.static variable [static int someVar]
2.add non-static reference to variable[int* dllSomeVar]
3.add static member function to return reference to static variable[static int* someVarRef(){ return &someVar; }]
4.add non-static member function to set reference to static variable[void someVarRef(int* someVarRef){ dllSomeVar = someVarRef; }]
5.add code anywhere a node is being loaded in the asset library to store the reference on creation.[node->someVarRef(Node::someVarRef());]
6.add member function to return whichever var should be used[int SomeVar(){if(dllSomeVar) return *dllSomeVar; return someVar;}]
7.whenever trying to use variable, always use the SomeVar() function, which will automatically use the correct one.
*/


class Node{
private:
    /**\brief List of nodes that should be accessible by any other node.

    They are added to this list using the RegisterGlobal(Node*) function. Only very
    important objects should be registered as global, to keep the list short and running
    like a champ. Each node must have a unique name, which is how they are returned.

    By default the following nodes are included in the global list...
    - "engine" - the main engine that runs everything. This is added by the engine when it is created.
    - "current engine control" - the current engine control. This is set by the engine.
    - "window" - the main window of the application. Don't know how extra windows should be handled yet.
    */
    static map<Node*> globals;

    /**\brief Reference to global list of nodes (static map<Node*> globals), needed by dynamically loaded
    dlls.

    Dlls cannot share the same static namespace something or another, so when a dll is loaded by the
    asset library, it needs to call the GlobalRef() function to set a reference to the static globals
    variable that exists in the .exe.

    When the Globals() function is called, Nodes are set to check if the dllGlobals variable is set, if
    it is, it will return that object, otherwise, it will return the default globals object. This depends
    on the asset library to set that dllGlobals function whenever it loads a Node from a dll. As long as
    that doesn't change, any dll loaded Nodes will always have access to the globals variable.*/
    map<Node*>* dllGlobals;

    static List<NodeRequest> globalRequests;
    List<NodeRequest> *dllGlobalRequests;


    /**\brief Node reference to the assets library.

    This is the instance that all nodes will share.*/
    static AssetLibrary assetLibrary;
    AssetLibrary* dllAssetLibrary;



	/**\brief A static counter that keeps track of the next new available node id.

	This is called and incremented when the GetNextFreeId() function is called, which
	either uses the next available recycled id or uses, then increments, the nextId
	value*/
	static long nextId;

	static List<long> recyledIds;

	long id;

	/**\brief A list of all nodes that are referencing the current node.

	Whenever a node calls the Link() function, both the caller node and
	the argument node have each other added as references. When the Unlink()
	function is called, the caller node and argument node are both removed
	as references from each other. */
	List<Node*> references;

	Node* parent;

	List<Node*> children;

	VarMap settings;

	List<NodeMessage> messageQueue;

	static long GetNextFreeId();

	void OnNodeUnlink(Node* otherNode);

	/**\brief Get the list of global requests.

	This function will return the actual original globalRequests object if the
	node instance calling it is not a dll, if the node instance is a dll, it will
	return the reference to globalRequests called dllGlobalRequests. Either way,
	this should always be called because no inheriting nodes should need this as
	it will only be called from the node base class when a new global is registered.*/
    List<NodeRequest> &GlobalRequests();

    /**\brief Get the percentage of a second since the last tick.

    A tick is the number of milliseconds that pass between each iteration of the
    game loop.*/


	/**\brief Keep track of the global time.

	The timer is a static class used to give the Node class reference to when
    the time is.*/
    double* timeStep;
protected:

    /**\brief Lets any node request a reference to any other node by name.

    If the global node already exists, it is found and sent back to the requester
    using the OnGlobalRequest() function. If it is not found, the request is added
    to a list that is checked whenever a new global is registered. If the global has
    been added, it calls the OnGlobalRequest() callback at that point.*/
    bool GlobalRequest(string name);

    /**\brief Returns the reference to a global when it has been registered.

    Should be implemented by any Nodes that wish to reference a global object.*/
    virtual void OnGlobalRequest(Node* globalNodeRef,string name);


    /**\brief Store a global node to the global node map.

    This is an exclusive list of nodes that should be able to be referenced from
    anywhere in the program. Every node will have a way to reference global variables
    by calling the Global(string name) function.*/
    bool RegisterGlobal(Node* globalNode,string name);

    bool CreateAndSendMessage(Node* receiver, int code, void* data);

    /**\brief Send a NodeMessage to another Node.

    This class takes the NodeMessage class as an argument. That argument
    should already have the property .receiver set. This is basically a
    helper function fill in some of the NodeMessage properties, then send
    the message to the receiver. */
	bool Message(NodeMessage message);

	void ReceiveMessage(NodeMessage message);

	/**\brief Loops through and calls the HandleMessage for each message in the
	messageQueue.

	This is called automatically in the Update() function. If you do not override the
	default Node::Update() function, you do not need to worry about calling this.**/
	void HandleMessages();

	/**\brief Check to see if a specific message contains a message_code that has any
	meaning to this class.

	If the message is used, true should be returned, otherwise the object should give
	its parent classes a chance to handle it by returning Node::HandleMessage(message),
	or whatever parent the implementing class directly inherits from.**/
	virtual bool HandleMessage(NodeMessage message);

	/**\brief Set a reference to the parent of the current node.

	The only rule is that the parent cannot also be one of the current
	node's children. That would cause an infinite recursion (and we don't
	want that, do we now?).*/
    Node* Parent(Node* newParent);

    /**\brief Return the current parent node reference.*/
    Node* Parent();

	/**\brief Adds a node to the list of children in this node.

	The only rule is that the new child can't already be a child of the
	current node. If it is, false is returned. If it is successfully added,
	true is returned.*/
    virtual Node* Child(Node* newChild);

	/**\brief Adds the caller node and the argument node as references to each other.

	This does not need to be called if the SetParent or AddChild() functions are being
	called. It will be called implicitly by those functions.*/
	bool Link(Node* otherNode);

	/**\brief Removes the caller node and the argument node from each others reference
	list if they are in them. This does not need to be called if the RemoveChild() or
	SetParent() changes a parent from one parent node to another parent node. The
	Unlink() function will be called implicitly.*/
	bool Unlink(Node* otherNode);

	/**\brief Placeholder function that programmer can use to clean up any internal
	references to a specific node.

	Called for both the calling node (passing the other node as the argument)
	and the other node (passing the calling node as the argument) when two nodes are
	unlinked from each other using the Unlink() function.

	This should be implemented by any classes inheriting from the Node class that employ
	their own internal lists of references to Nodes. Any time a node is removed, it will
	be unlinked from everything to which it is linked. This provides the programmer an
	opportunity to make sure that they are not referencing it in any of the mentioned
	internal lists.*/
	virtual void OnUnlink(Node* otherNode);

    /**\brief By default this function does nothing. It is called when the SetSetting()
    function is called. By default, node don't care if anything has changed with settings,
    but this is provided because we recognize that people will want to do things when
    the settings are changed.

    It can be overwritten to do things specific to the implementation. It is called at the
    end of the SetSettings() function. Do with it what you wish.*/
    virtual void OnSetSettings();


    /**\brief Called when the parent of a node is set.

    This can be overwritten by any inheriting class to do things that you know need
    to be done when a parent is set. It does nothing by default. It is only called when
    a child is added through the Child(the_child) function or the childs OnSetParent()
    function.**/
    virtual void OnSetParent();


    Node* Load(string settingsName,VarMap settings);


    template <class T>
    T* Closest(){
        if(!parent) return NULL;
        T* castParent = dynamic_cast<T*>(parent);
        if(castParent) return castParent;
        return parent->Closest<T>();
    }



public:

    Node();
    Node(VarMap settings);
    virtual ~Node();

	/**\brief Where the node is able to draw itself.

	If using the base implementation, the node will just render all of it's children
	nodes. If implementing, which I assume most nodes will do, you can just call
	Node::Draw() to have it draw all of the child nodes, otherwise it is up to you
	to make sure children are rendered.*/
	virtual void Draw();

	virtual void Update();


	long Id();

	VarMap Settings(VarMap newSettings);
    VarMap Settings();

	Node* operator [](string name);
    Node* Global(string name);
    static map<Node*> StaticGlobals();
    map<Node*> &Globals();
    static map<Node*>* GlobalsRef();
    void GlobalsRef(map<Node*>* globals);

    static List<NodeRequest>* GlobalRequestsRef();
    void GlobalRequestsRef(List<NodeRequest>* globalRequests);

    /**\brief Gets a reference to the asset library.

    This is needed by any .dlls that have a different memory space
    that are loaded by this node. Its asset library reference is
    passed to the newly loaded object so it is looking at the same
    asset library as everything else.*/
    AssetLibrary* AssetLibraryRef();

    /**\brief Sets the reference to the asset library.

    This is used by the asset library to copy the pointer to itself
    from the node that has loaded this node from a .dll. The asset
    library, with a reference to the node that is loading this node,
    and a reference to this node, calls dllNode->AssetLibraryRef(creatorNode->AssetLibraryRef());
    to store things.*/
    void AssetLibraryRef(AssetLibrary* assetLibrary);

    /**\brief Allows access to the static Asset() class that is included
    in every node.

    This just allows things outside of the class to access the private
    member of the Node() class. Yeah, it's probably dangerous, just don't
    do anything stupid, guy.*/
    AssetLibrary& Assets();

    /**\brief Return the list of children pointers*/
    List<Node*> Children();

    /**\brief The time that has occurred since the last step.

    Specifically, this is the time between the start of the current game loop
    and the start of the previous game loop.*/
    double TimeStep();

    double* TimeStepRef(double* timeStepReference);
    double* TimeStepRef();


	void CopyGlobalRefsTo(Node* node);
};






#endif // ENGINE_NODE_H
