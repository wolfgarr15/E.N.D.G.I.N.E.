/////////////////////////////////////////////////////////////////////
// File: EViewManager.h
// 
// Purpose: Defines the E.N.D.G.I.N.E. core component view manager.
//          The view manager encapsulates all rendering and display
//          functionality.
//
// Author: Wolfe S. Greene
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// DLL Export Macros
//-------------------------------------------------------------------
#ifdef VIEWMANAGER_EXPORTS
	#define VIEWMANAGER_API __declspec(dllexport)
#else
	#define VIEWMANAGER_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////
// External Header Includes
//--------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////
// Class Definition
//-------------------------------------------------------------------
class VIEWMANAGER_API EViewManager 
{
public:
	EViewManager();
	~EViewManager();

	bool Initialize();

	void Shutdown();
};
