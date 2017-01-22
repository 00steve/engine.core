/*the dll class is an interface that allows an operating system to load dlls*/
#ifndef SYNENGINE_DLL_H
#define SYNENGINE_DLL_H
#include <windows.h>

class eDLL {
private:

	HINSTANCE hInstLibrary;//the windows reference to the dll
	char* filename;
	bool multiClass;

public:
	eDLL(const char* filename) :
            multiClass(false){
		this->filename = (char*)filename;//store the name of the dll file
		hInstLibrary = LoadLibrary(filename);//attempt to load the current .dll
		if(!hInstLibrary) cout << "Failed to load " << filename << "\n";
    }


	~eDLL() {
		FreeLibrary(hInstLibrary);
		delete filename;
    }
	bool IsLoaded(){
        return (bool) GetModuleHandle(filename);
    }
	HINSTANCE Lib(){
	    return hInstLibrary;
    }
	const char* FileName(){
	    return filename;
    }

    bool MultiClass(bool isMultiClass){
        return multiClass = isMultiClass;
    }

    bool MultiClass(){
        return multiClass;
    }
};


template <typename classX> static classX GetDllValue(eDLL* dll,const char* func) {
    typedef classX (*GetClass)();
    GetClass newClass = (GetClass)::GetProcAddress(dll->Lib(),func);
    classX temp = newClass();
    if(temp) return temp;
    return NULL;
};

#endif
