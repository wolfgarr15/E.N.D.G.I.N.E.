#pragma once

#define RETURN_IF_FAILS(hResult) if(FAILED(hResult)) { return FALSE; }
#define RETURN_IF_FALSE(bResult) if(bResult == FALSE) { return FALSE; }
