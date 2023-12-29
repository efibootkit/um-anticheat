#include "tpm.h"

auto tpmchecker::check_tpm( ) -> int
{
	std::vector<BYTE> ekData = Utils::GetEK( );
	if ( ekData.empty( ) )
	{
		printf( "Cannot get TPM data!\n" );
		return EXIT_FAILURE;
	}

	/*std::string tpm_md5 = Utils::GetKeyHash( ekData , CALG_MD5 ).c_str( );
	if ( tpm_md5 == "3592212fff796866d5dfa24705b4afd4" )
	{
		communication::send_request( "TPM_BLACKLISTED_MD5" );
		exit( 0 );
	}*/

	printf( "TPM INFOS:\n" );
	printf( "MD5:     %s\n" , Utils::GetKeyHash( ekData , CALG_MD5 ).c_str( ) );
	printf( "SHA1:    %s\n" , Utils::GetKeyHash( ekData , CALG_SHA1 ).c_str( ) );
	printf( "SHA256:  %s\n" , Utils::GetKeyHash( ekData , CALG_SHA_256 ).c_str( ) );

	communication::send_request( "TPM HASH IS NOT BLACKLISTED" );
	return 0;
}