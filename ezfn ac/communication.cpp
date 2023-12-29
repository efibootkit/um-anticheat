#include "communication.h"

auto communication::send_request( std::string content ) -> int
{
    WSADATA w_data;
    if ( WSAStartup( MAKEWORD( 2 , 2 ) , &w_data ) != 0 )
    IFH( MessageBoxA ).get( )( 0 , xorstr( "Could not start WSA" ) , xorstr( "EZFN-AC ERROR" ) , 0 );

    SOCKET clientSocket = socket( AF_INET , SOCK_STREAM , 0 );
    if ( clientSocket == INVALID_SOCKET )
    {
        IFH( MessageBoxA ).get( )( 0 , xorstr( "Failed to create socket" ) , xorstr( "EZFN-AC ERROR" ) , 0 );
        WSACleanup( );
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons( SERVER_PORT );
    serverAddr.sin_addr.s_addr = inet_addr( xorstr( "127.0.0.1" ) );

    if ( connect( clientSocket , ( struct sockaddr* ) &serverAddr , sizeof( serverAddr ) ) == SOCKET_ERROR )
    {
        IFH( MessageBoxA ).get( )( 0 , xorstr( "Connect failed" ) , xorstr( "EZFN-AC ERROR" ) , 0 );
        closesocket( clientSocket );
        WSACleanup( );
    }

    send( clientSocket , content.c_str( ) , content.size( ) , 0 );
    closesocket( clientSocket );
    WSACleanup( );

    return 0;
}