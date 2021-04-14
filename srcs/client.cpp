/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:29:26 by schene            #+#    #+#             */
/*   Updated: 2021/04/14 11:43:59 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    struct stat info;
    char *buf;

    lstat("request.txt", &info);
    int size = info.st_size;
    int fd = open("request.txt", O_RDONLY);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    // send(sock , hello.c_str() , hello.size(), 0);
    if (!(buf = (char *)malloc(sizeof(char) * size + 1)))
    {
        perror("Malloc failed !");
        return -1;
    }
    int r = read(fd, buf, size);
    buf [r] = 0;
    close(fd);
    // std::string hello = "GET test.html HTTP/1.1\nHost: 127.0.0.1:8080\n\n";
    // std::string hello = "GET YoupiBanane/youpi.bla HTTP/1.1\nHost: 127.0.0.1:8080\n\n";
   
    std::cout << buf << std::endl;
    send(sock , buf , size, 0);
    free(buf);
    printf("Request sent\n");
    char buffer[1024] = {0};
    valread = read(sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
