/////////////////////////////////////////////////////////////////////
// File: EEventManager.h
// 
// Purpose: Defines the E.N.D.G.I.N.E. core component event manager.
//          The event manager registers and process "events" that 
//          manipulate the model data.
//
// Author: Wolfe S. Greene
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// DLL Export Macros
//-------------------------------------------------------------------
#ifdef EVENTMANAGER_EXPORTS
	#define EVENTMANAGER_API __declspec(dllexport)
#else
	#define EVENTMANAGER_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////
// External Header Includes
//--------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////
// Class Definition
//-------------------------------------------------------------------
class EVENTMANAGER_API EEventManager 
{
public:
	EEventManager();
	~EEventManager();
};
