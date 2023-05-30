#pragma once
#include <arpa/inet.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#define BUFFER_SIZE (1024 * 1024)
