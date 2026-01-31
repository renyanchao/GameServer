// #include"Net.h"
// #include"GameDefine.h"
// #include"Session.h"
// #include"protobuf_deserializer.h"

// // 设置文件描述符为非阻塞模式
// int set_nonblocking(int fd) {
//     int flags = fcntl(fd, F_GETFL, 0);
//     if (flags == -1) return -1;
//     return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
// }

// int NetManager::Listen()
// {
//     //int server_fd, epoll_fd;
//         struct sockaddr_in server_addr;
//         //struct epoll_event ev, events[MAX_EVENTS];

//         // 1. 创建服务器 socket
//         if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//             perror("socket creation failed");
//             exit(EXIT_FAILURE);
//         }
//             // 2. 设置服务器 socket 为非阻塞模式 (重要修复！)
//             if (set_nonblocking(server_fd) < 0) {
//                 perror("set_nonblocking server_fd");
//                 close(server_fd);
//                 exit(EXIT_FAILURE);
//             }

//         // 设置 SO_REUSEADDR 选项，避免重启时 "Address already in use" 错误
//         int opt = 1;
//         if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
//             perror("setsockopt failed");
//             close(server_fd);
//             exit(EXIT_FAILURE);
//         }

//         // 2. 绑定服务器地址和端口
//         memset(&server_addr, 0, sizeof(server_addr));
//         server_addr.sin_family = AF_INET;
//         server_addr.sin_addr.s_addr = INADDR_ANY;
//         server_addr.sin_port = htons(PORT);

//         if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
//             perror("bind failed");
//             close(server_fd);
//             exit(EXIT_FAILURE);
//         }

//         // 3. 开始监听
//         //if (listen(server_fd, SOMAXCONN) < 0) {
//         if(listen(server_fd, LISTEN_QUENE_SIZE)){
//             perror("listen failed");
//             close(server_fd);
//             exit(EXIT_FAILURE);
//         }

//         // 4. 创建 epoll 实例
//         if ((epoll_fd = epoll_create1(0)) < 0) {
//             perror("epoll_create1 failed");
//             close(server_fd);
//             exit(EXIT_FAILURE);
//         }

//         // 5. 将服务器 socket 添加到 epoll 监听列表中，监听可读事件（新连接）
//         ev.events = EPOLLIN; // 监听可读事件
//         ev.data.fd = server_fd;
//         if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) < 0) {
//             perror("epoll_ctl: server_fd");
//             close(server_fd);
//             close(epoll_fd);
//             exit(EXIT_FAILURE);
//         }

//         printf("Epoll server started successfully.\n");
//         return server_fd;
// }

// void NetManager::Loop()
// {
//      printf("Epoll server started begin.\n");
//      //sleep_ms(500000);
//         // 主事件循环
//         while (1) {
//             // 6. 等待事件发生，超时时间 -1 表示永久阻塞
//             int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
//             if (nfds < 0) {
//                 perror("epoll_wait");
//                 break;
//             }
//             printf("Epoll server epoll_wait finish.\n");
//             // 7. 处理所有就绪的事件
//             for (int i = 0; i < nfds; i++) {
//                 // 7.1 如果是服务器 socket 可读，表示有新连接到来
//                 if (events[i].data.fd == server_fd) {
//                     printf("Epoll server epoll_new connect \n");
//                     struct sockaddr_in client_addr;
//                     socklen_t addr_len = sizeof(client_addr);
                    
//                     // 接受新连接。使用非阻塞 accept，循环直到耗尽全连接队列
//                     while (1) {
//                         int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);


//                         if (client_fd < 0) {
//                             // 如果错误是 EAGAIN 或 EWOULDBLOCK，说明队列已空
//                             if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                                 break;
//                             } else {
//                                 perror("accept");
//                                 break;
//                             }
//                         }

//                         // 设置客户端 socket 为非阻塞模式
//                         if (set_nonblocking(client_fd) < 0) {
//                             perror("set_nonblocking client");
//                             close(client_fd);
//                             continue;
//                         }

//                         // 打印新连接信息
//                         printf("New connection from %s:%d, fd: %d\n",
//                             inet_ntoa(client_addr.sin_addr),
//                             ntohs(client_addr.sin_port),
//                             client_fd);

//                         // 将新的客户端 socket 添加到 epoll 监听，监听可读事件
//                         ev.events = EPOLLIN | EPOLLET; // 边缘触发模式
//                         ev.data.fd = client_fd;
//                         if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
//                             perror("epoll_ctl: client_fd");
//                             close(client_fd);
//                         }
//                     }
//                 }
//                 // 7.2 否则是客户端 socket 可读，表示有数据到来
//                 else {
//                     int client_fd = events[i].data.fd;
//                     char buffer[BUFFER_SIZE];
                    
//                     // 循环读取数据，直到读完（对于边缘触发模式尤其重要）
//                     while (1) {
//                         ssize_t count = read(client_fd, buffer, BUFFER_SIZE - 1);
//                         if (count > 0) {
//                             buffer[count] = '\0'; // 添加字符串结束符
//                             printf("Received from fd %d: %s", client_fd, buffer);
                            
//                             // 回显数据给客户端
//                             write(client_fd, buffer, count);
//                         } 
//                         else if (count == 0) {
//                             // EOF，客户端关闭了连接
//                             printf("Client fd %d disconnected.\n", client_fd);
//                             epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
//                             close(client_fd);
//                             break;
//                         } 
//                         else {
//                             // 错误处理
//                             if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                                 // 在非阻塞模式下，没有更多数据可读了
//                                 break;
//                             } else {
//                                 perror("read");
//                                 epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
//                                 close(client_fd);
//                                 break;
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//         // 清理资源（实际上上面的循环是无限的，这里只是为了规范）
//         close(server_fd);
//         close(epoll_fd);
// }