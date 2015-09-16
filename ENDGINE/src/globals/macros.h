///////////////////////////////////////////////////////////////////////
// Filenam: macros.h
//
// Purpose: Includes helper C-macro definitions used throughout the
//          E.N.D.G.I.N.E. project. 
///////////////////////////////////////////////////////////////////////

#pragma once

///////////////////////////////////////////////////////////////////////
// Macro Definitions
//---------------------------------------------------------------------
#define RETURN_IF_FAILS(hResult) if(FAILED(hResult)) { return FALSE; }
#define RETURN_IF_FALSE(bResult) if(bResult == FALSE) { return FALSE; }