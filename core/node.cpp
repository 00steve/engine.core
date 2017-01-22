#include "node.h"
#include "../assetlibrary/assetlibrary.h"



/*Generate static node variables.*/
long Node::nextId = 0;
List<long> Node::recyledIds = List<long>();
map<Node*> Node::globals = map<Node*>();
List<NodeRequest> Node::globalRequests = List<NodeRequest>();
AssetLibrary Node::assetLibrary;


void Node::OnSetParent(){}

Node* Node::Child(Node* newChild){
	//make sure that the new child being added is not already the
	//parent. A node cannot be a parent and a child at the same time.
	//That turns into some crazy I'm-my-own-grandpa the stupids shit.
	if(newChild == parent) return NULL;
	if(children.PushIfNew(newChild)){
		Link(newChild);
		newChild->parent = this;
        newChild->OnSetParent();
	}
	return newChild;
}


void Node::Draw(){
	for(int i=0;i<children.GetCount();i++){
		children[i]->Draw();
	}
}


long Node::GetNextFreeId(){
	return (recyledIds.GetSize() > 0 ? recyledIds.Pop() : nextId++);
}


long Node::Id(){
	return id;
}


bool Node::Link(Node* otherNode){
	return references.PushIfNew(otherNode) && otherNode->references.PushIfNew(this);
}


Node::Node(VarMap settings):
    dllGlobals(NULL),
    dllGlobalRequests(NULL),
    dllAssetLibrary(NULL),
	id(GetNextFreeId()),
	parent(NULL),
	settings(settings)
	{
}


Node::Node():
    dllGlobals(NULL),
    dllGlobalRequests(NULL),
    dllAssetLibrary(NULL),
	id(GetNextFreeId()),
	parent(NULL)
		{
}


Node::~Node(){
    cout << "node\n";
	while(references.GetCount()){
		Unlink(references[0]);
	}
	while(children.GetCount()){
        cout << " - delete child\n";
        delete children.Pop();
	}
}


void Node::OnGlobalRequest(Node* globalNodeRef,string name){
    //do nothing, currently
}


bool Node::GlobalRequest(string name){
    //cout << " - request resource " << name << endl;
    Node* request = Global(name);
    if(request){
        //cout << "   > already loaded, being returned\n";
        OnGlobalRequest(request,name);
        return true;
    }
    NodeRequest n;
    n.Requester = this;
    n.RequestName = name;
    if(dllGlobalRequests){
        dllGlobalRequests->Push(n);
    } else {
        globalRequests.Push(n);
    }
    return false;
}


void Node::OnNodeUnlink(Node* otherNode){
	if(parent == otherNode){
		parent = NULL;
	}
	if(children.Remove(otherNode)){
	}
}


void Node::OnUnlink(Node* otherNode){
	return;
}


Node* Node::Parent(Node* newParent){
	//make sure that the new parent isn't a child of the current
	//node. If it is, break from this function and return false.
	if(children.Exists(newParent)) return NULL;
	//if there is an existing parent, and the new parent is not
	//the same node as the existing parent, unlink the existing
	//parent from the node.
	if(parent && parent != newParent){
		Unlink(parent);
	}
	//if there is a new parent (it is not NULL), and the new parent
	//is not the same node as the existing parent, link the new
	//parent to the current node, and add the current node as one
	//of the parent's children.
	if(newParent && parent != newParent){
		Link(newParent);
		newParent->children.PushIfNew(this);
	}
	//call this so any inheriting classes can have their own custom
	//callback
	OnSetParent();
	//no matter what, set the new parent as the current parent. If
	//it is the same node, nothing will change. If it is a new node,
	//the new node will be set. If it is NULL, the parent will be set
	//as null which is also OK.
	return parent = newParent;
}
Node* Node::Parent(){
	return parent;
}



void Node::CopyGlobalRefsTo(Node* node){
	if(!node) return;
    node->dllGlobals = dllGlobals;
    node->dllGlobalRequests = dllGlobalRequests;
    node->dllAssetLibrary = dllAssetLibrary;
	node->timeStep = timeStep;
	//cout << "timeStep*  = " << timeStep << endl;
}

bool Node::Unlink(Node* otherNode){
	if(references.Remove(otherNode) && otherNode->references.Remove(this)){
		OnUnlink(otherNode);
		otherNode->OnUnlink(this);
		OnNodeUnlink(otherNode);
		otherNode->OnNodeUnlink(this);
		return true;
	};
	return false;
}


void Node::Update(){
	for(int i=0;i<children.GetCount();i++){
		children[i]->Update();
	}
	HandleMessages();
}


void Node::OnSetSettings(){
}


VarMap Node::Settings(VarMap newSettings){
    settings = newSettings;
    OnSetSettings();
	return settings;
}


VarMap Node::Settings(){
    return settings;
}


bool Node::CreateAndSendMessage(Node* receiver, int code, void* data){
    NodeMessage message;
    message.receiver = receiver;
    message.data = data;
    message.code = code;
    return Message(message);
}


void Node::ReceiveMessage(NodeMessage message){
    messageQueue.Push(message);
}


bool Node::HandleMessage(NodeMessage message){
	return false;
}


void Node::HandleMessages(){
	while(messageQueue.GetCount()){
		HandleMessage(messageQueue.Pop());
	}
}


bool Node::Message(NodeMessage message){
	if(!message.receiver) return false;
	message.sender = this;
	message.receiver->ReceiveMessage(message);
	return true;

}


Node* Node::operator [](string name){
    if(globals.exists(name)){
        return globals.getLastCheck();
    }
    return NULL;
}


Node* Node::Global(string name){
    if(dllGlobals){
        if(dllGlobals->isSet(name)){
            return dllGlobals->getLastCheck();
        }
        return NULL;
    }
    if(globals.isSet(name)){
        return globals.getLastCheck();
    }
    return NULL;
}


map<Node*> Node::StaticGlobals(){
    return Node::globals;
}


map<Node*> &Node::Globals(){
    if(dllGlobals) return *dllGlobals;
    return globals;
}


bool Node::RegisterGlobal(Node* globalNode,string name){
    //cout << " - [register global] " << name << endl;
    if(Globals().isSet(name)){
        Globals()[name] = globalNode;
    } else {
        Globals().push(globalNode,name);
    }
    NodeRequest *nr;
    int i=0;
    while(i<GlobalRequests().GetCount()){
        nr = GlobalRequests().RefAt(i);
        if(nr->RequestName != name){
            ++i;
            continue;
        }
        nr->Requester->OnGlobalRequest(globalNode,name);
        GlobalRequests().RemoveAt(i);
    }
    return true;
}


map<Node*>* Node::GlobalsRef(){
    return &globals;
}


void Node::GlobalsRef(map<Node*>* globals){
    dllGlobals = globals;
}


List<NodeRequest> &Node::GlobalRequests(){
    if(dllGlobalRequests) return *dllGlobalRequests;
    return globalRequests;
}


List<NodeRequest>* Node::GlobalRequestsRef(){
    return &globalRequests;
}
void Node::GlobalRequestsRef(List<NodeRequest>* globalRequests){
    dllGlobalRequests = globalRequests;
}

AssetLibrary* Node::AssetLibraryRef(){
    if(dllAssetLibrary) return dllAssetLibrary;
    return &assetLibrary;
}
void Node::AssetLibraryRef(AssetLibrary* assetLibrary){
    //cout << " - store static asset library ref : " << assetLibrary << endl;
    dllAssetLibrary = assetLibrary;
}
AssetLibrary &Node::Assets(){
    if(dllAssetLibrary) return *dllAssetLibrary;
    return assetLibrary;
}


List<Node*> Node::Children(){
    return children;
}


double Node::TimeStep(){
    return timeStep ? *timeStep : 0;
}

double* Node::TimeStepRef(double* timeStepReference){
    return timeStep = timeStepReference;
}

double* Node::TimeStepRef(){
    return timeStep;
}


Node* Node::Load(string settingsName,VarMap settings){
    string type = settings.get<string>("type");
    int firstP = static_cast<int>(type.find('.'));
    bool sub = firstP != -1;
    string fileName = "extensions/engine." + (sub ? type.substr(0,firstP) : settingsName + "." + settings.get<string>("type")) + ".dll";
    //cout << "Load filename : " << fileName << endl;
    Node* newNode = AssetLibrary::Load<Node*>(fileName,settings);
    if(!newNode) return NULL;
	newNode->GlobalsRef(Node::GlobalsRef());
	newNode->GlobalRequestsRef(GlobalRequestsRef());
	newNode->AssetLibraryRef(AssetLibraryRef());
	newNode->TimeStepRef(TimeStepRef());
    newNode->Settings(settings);
	return newNode;
}

