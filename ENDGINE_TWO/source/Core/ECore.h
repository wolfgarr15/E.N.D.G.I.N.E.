//////////////////////////////////////////////////////////////////////
// File: ECore.h
// 
// Purpose: Defines the E.N.D.G.I.N.E. API entry point. The user 
//          accesses all engine features through this class.
//
// Author: Wolfe S. Greene
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// DLL Export Macros
//--------------------------------------------------------------------
#ifdef CORE_EXPORTS
	#define CORE_API __declspec(dllexport)
#else
	#define CORE_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////
// External Header Includes
//--------------------------------------------------------------------
#include "EModelManager.h"
#include "EViewManager.h"
#include "EEventManager.h"

//////////////////////////////////////////////////////////////////////
// Class Definition
//--------------------------------------------------------------------
class CORE_API ECore 
{
	EModelManager m_ModelManager;
	EViewManager m_ViewManager;
	EEventManager m_EventManager;

public:
	ECore();
	~ECore();

	bool Initialize();
	bool Run();
	void Shutdown();
};
