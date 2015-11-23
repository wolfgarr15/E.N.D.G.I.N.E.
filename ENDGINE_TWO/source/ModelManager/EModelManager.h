//////////////////////////////////////////////////////////////////////
// File: EModelManager.h
// 
// Purpose: Defines the E.N.D.G.I.N.E. core component model manager.
//          The model manager encapsulates 3D model and texture 
//          resource registration, allocation, and processing.
//
// Author: Wolfe S. Greene
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// DLL Export Macros
//--------------------------------------------------------------------
#ifdef MODELMANAGER_EXPORTS
	#define MODELMANAGER_API __declspec(dllexport)
#else
	#define MODELMANAGER_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////
// External Header Includes
//--------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// Class Definition
//--------------------------------------------------------------------
class MODELMANAGER_API EModelManager 
{
public:
	EModelManager();
	~EModelManager();
};