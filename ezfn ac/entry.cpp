#include "workspace.h"

auto DllMain( HMODULE hModule , DWORD  ul_reason_for_call , LPVOID lpReserved ) -> BOOL APIENTRY
{
    UNREFERENCED_PARAMETER( hModule );
    UNREFERENCED_PARAMETER( lpReserved );

    if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
    worker::main( );

    return true;
}