#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#define MAX_SIZE 255
#define OCTET 0
#define NETASCII 1
#define DATA_PACKET_SIZE 516
#define ACK_PACKET_SIZE 4
#define MAX_TRIES 65536
int connectTo(char *,char *);
void sendRRQ(char*,int,int);
void sendWRQ(char*,int,int);
void receive_data(int,char*);
void send_data(int,char*);

