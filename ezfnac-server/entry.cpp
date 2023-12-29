#include "workspace.h"

auto main( ) -> void
{
    ShowWindow( GetConsoleWindow( ) , SW_HIDE );

    std::cout << "\n\n";
    SetConsoleTitleA( "EZFNAC-SERVER" );

    WSADATA w_data;
    if ( WSAStartup( MAKEWORD( 2 , 2 ) , &w_data ) != 0 )
    std::cout << "  [-] WSAStartup failed." << std::endl;

    SOCKET serverSocket = socket( AF_INET , SOCK_STREAM , 0 );
    if ( serverSocket == INVALID_SOCKET )
    std::cout << "  [-] Failed to create socket." << std::endl;

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons( PROTOCOL_PORT );
    server_address.sin_addr.s_addr = INADDR_ANY;

    if ( bind( serverSocket , ( struct sockaddr* ) &server_address , sizeof( server_address ) ) == SOCKET_ERROR )
    {
        std::cout << "  [-] Bind failed." << std::endl;
        closesocket( serverSocket );
        WSACleanup( );
    }

    if ( listen( serverSocket , SOMAXCONN ) == SOCKET_ERROR )
    {
        std::cout << "  [-] Listen failed." << std::endl;
        closesocket( serverSocket );
        WSACleanup( );
    }

    std::cout << "  [~] Listening for requests..." << std::endl;

    while ( true )
    {
        SOCKET clientSocket = accept( serverSocket , nullptr , nullptr );
        if ( clientSocket == INVALID_SOCKET )
        {
            std::cout << "[-] Accept failed." << std::endl;
            continue;
        }

        char buffer[ 1024 ];
        int bytesRead = recv( clientSocket , buffer , sizeof( buffer ) , 0 );
        if ( bytesRead > 0 )
        {
            buffer[ bytesRead ] = '\0';

            std::string message( buffer );
            std::cout << "  [+] Received: " << message << std::endl;

            if ( message == "AC_DLL_DETECTION" )
            MessageBoxA( 0 , "DLL INJECTION DETECTED" , "EZFN-AC" , MB_OK | MB_ICONERROR | MB_TOPMOST );

            else if ( message == "TPM_BLACKLISTED_MD5" )
            MessageBoxA( 0 , "YOU HAVE BEEN BANNED FROM USING EZFN." , "EZFN-AC" , MB_OK | MB_ICONERROR | MB_TOPMOST );
        }

        closesocket( clientSocket );
    }

    closesocket( serverSocket );
    WSACleanup( );
}