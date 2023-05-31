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

#define WRITE_BUFFER_SIZE 1024 * 1024
// use for a faster server
// #define WRITE_BUFFER_SIZE 1024 * 1024 * 32
#define READ_BUFFER_SIZE 1024 * 32
