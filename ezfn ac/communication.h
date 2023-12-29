#pragma once
#include "workspace.h"

#define SERVER_PORT 1337

class communication
{
public:
	static auto send_request( std::string content ) -> int;
};