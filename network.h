#pragma once
#ifndef __PACKET_HEADER_GUARD__
#define __PACKET_HEADER_GUARD__
#include <stdint.h>
#include <stddef.h>
#include "container.h"
#define PACKET_SIZE (8)

struct socket;
typedef struct socket socket_t;
typedef struct socket *socket_ptr;
struct router;
typedef struct router router_t;
typedef struct router* router_ptr;
struct packet;
typedef struct packet packet_t;
typedef struct packet *packet_ptr; 

struct packet {
    uint8_t dest,inst;
    uint8_t data[12];
};

struct socket{
    uint8_t dest,inst;
    uint8_t port,flags;
    bling_ptr buffer;//onwer
    router_ptr router;//shared
};

struct router {
    varray_ptr sockets;
};






#endif