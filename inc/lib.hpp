#pragma once

#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <set>
#include <poll.h>


#define MAX_CLIENTS 5