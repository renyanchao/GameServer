#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_EVENTS 1024
#define BUFFER_SIZE 4096
#define PORT 1234
#define LISTEN_QUENE_SIZE 2

class NetManager
{
public:


    int Listen();
    int InitEpool();
    void Connect();
    void Loop();
private:
    int server_fd;
    int epoll_fd;
    struct epoll_event ev, events[MAX_EVENTS];
};