#include "worker.h"

auto worker::main( ) -> int
{
	AllocConsole( );
	freopen( "CONOUT$" , "w" , stdout );

	tpmchecker::check_tpm( );
	memorychecking::checktextsection( );
	
	/*while ( true )
	{
		Sleep( 5000 );
		if ( !protection_iat::check_iat_integrity( ) )
		{
			MessageBoxA( 0 , "DLL INJECTION DETECTED" , "EZFN-AC" , MB_OK | MB_ICONERROR | MB_TOPMOST );
		}
	}*/

	return 0;
}