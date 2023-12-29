#include "memory.h"

std::vector<BYTE> calculatehash( const BYTE* buffer , size_t bufferSize ) 
{
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;

    if ( !CryptAcquireContext( &hProv , nullptr , nullptr , PROV_RSA_AES , CRYPT_VERIFYCONTEXT ) ) {
        std::cerr << "Error acquiring cryptographic context." << std::endl;
        return {};
    }

    if ( !CryptCreateHash( hProv , CALG_SHA_256 , 0 , 0 , &hHash ) ) {
        std::cerr << "Error creating hash object." << std::endl;
        CryptReleaseContext( hProv , 0 );
        return {};
    }

    if ( !CryptHashData( hHash , buffer , static_cast< DWORD >( bufferSize ) , 0 ) ) {
        std::cerr << "Error hashing data." << std::endl;
        CryptDestroyHash( hHash );
        CryptReleaseContext( hProv , 0 );
        return {};
    }

    DWORD hashSize = 0;
    DWORD hashSizeSize = sizeof( DWORD );
    if ( !CryptGetHashParam( hHash , HP_HASHSIZE , reinterpret_cast< BYTE* >( &hashSize ) , &hashSizeSize , 0 ) ) {
        std::cerr << "Error getting hash size." << std::endl;
        CryptDestroyHash( hHash );
        CryptReleaseContext( hProv , 0 );
        return {};
    }

    std::vector<BYTE> hash( hashSize );
    if ( !CryptGetHashParam( hHash , HP_HASHVAL , hash.data( ) , &hashSize , 0 ) ) {
        std::cerr << "Error getting hash value." << std::endl;
        CryptDestroyHash( hHash );
        CryptReleaseContext( hProv , 0 );
        return {};
    }

    CryptDestroyHash( hHash );
    CryptReleaseContext( hProv , 0 );

    return hash;
}

std::vector<BYTE> readtextsection( )
{
	HMODULE baseproc = GetModuleHandleA( nullptr );
	MODULEINFO moduleinfo;

	if ( !GetModuleInformation( GetCurrentProcess( ) , baseproc , &moduleinfo , sizeof( moduleinfo ) ) )
	{
		MessageBoxA( 0 , "Cannot get module information" , "EZFN AC" , MB_OK | MB_ICONERROR );
		return { };
	}

	std::vector<BYTE> textsection( moduleinfo.SizeOfImage );
	memcpy( textsection.data( ) , moduleinfo.lpBaseOfDll , moduleinfo.SizeOfImage );
	return textsection;
}

auto memorychecking::checktextsection( ) -> int
{
	std::vector<BYTE> textsection = readtextsection( );
	if ( textsection.empty( ) ) return 1;

	std::vector< BYTE > calculatedhash = calculatehash( textsection.data( ), textsection.size( ) );
	if ( calculatedhash.empty( ) ) return 1;

    while ( true )
    {
        const std::vector<BYTE> properHash = calculatehash( readtextsection( ).data( ) , readtextsection( ).size( ) );
        if ( calculatedhash == properHash )
        {
            printf( ".text section is not patched\n" );
        }
        else
        {
            printf( ".text section is patched\n" );
        }

        Sleep( 2000 );
    }
	

	return 0;
}