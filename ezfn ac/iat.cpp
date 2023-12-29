#include "iat.h"

auto protection_iat::check_iat_integrity( ) -> bool
{
	HMODULE module = GetModuleHandle( nullptr );

	IMAGE_DOS_HEADER* dosHeader = reinterpret_cast< IMAGE_DOS_HEADER* >( module );
	IMAGE_NT_HEADERS* ntHeaders = reinterpret_cast< IMAGE_NT_HEADERS* >( reinterpret_cast< char* >( module ) + dosHeader->e_lfanew );
	IMAGE_IMPORT_DESCRIPTOR* importDescriptor = reinterpret_cast< IMAGE_IMPORT_DESCRIPTOR* >( reinterpret_cast< char* >( module ) + ntHeaders->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress );

	while ( importDescriptor->Name )
	{
		char* moduleName = reinterpret_cast< char* >( module ) + importDescriptor->Name;
		IMAGE_THUNK_DATA* iatThunk = reinterpret_cast< IMAGE_THUNK_DATA* >( reinterpret_cast< char* >( module ) + importDescriptor->FirstThunk );

		while ( iatThunk->u1.Function )
		{
			DWORD functionNameOrOrdinal = iatThunk->u1.Ordinal;
			if ( !( functionNameOrOrdinal & IMAGE_ORDINAL_FLAG ) )
			{
				IMAGE_IMPORT_BY_NAME* importByName = reinterpret_cast< IMAGE_IMPORT_BY_NAME* >( reinterpret_cast< char* >( module ) + functionNameOrOrdinal );
				FARPROC expectedAddress = GetProcAddress( GetModuleHandle( L"kernel32.dll" ) , importByName->Name );

				if ( ( ULONG ) ( iatThunk->u1.Function ) != reinterpret_cast< ULONGLONG >( expectedAddress ) )
				{
					return false;
				}
			}

			++iatThunk;
		}

		++importDescriptor;
	}

	return true;
}