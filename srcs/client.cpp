/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:29:26 by schene            #+#    #+#             */
/*   Updated: 2021/05/06 14:20:50 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Client side C/C++ program to demonstrate Socket programming
#include <arpa/inet.h>
#include <sys/stat.h>
#include "../webserv.hpp"

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
    if (!(buf = (char *)malloc(sizeof(char) * size + 3)))
    {
        perror("Malloc failed !");
        return -1;
    }
    int r = read(fd, buf, size);
    // buf [r] = '\r';
    // buf [r + 1] = '\n';
    // buf [r + 2] = 0;
    buf [r] = 0;
    close(fd);
    // char		*line = NULL;
    // char        *crlf = (char *)"\r\n";
    // char        *tmp = NULL;
    

	// if (get_next_line(fd, &line) > 0)
    // {
    //     buf = ft_strjoin(line, crlf);
    //     free(line);
    //     line = NULL;
    // }
	// while (get_next_line(fd, &line) > 0)
	// {
    //     if (line)
    //     {
    //         tmp = ft_strjoin(line, crlf);
    //         free(line);
    //         line = ft_strjoin(buf, tmp);
    //         free(buf);
    //         buf = ft_strdup(line);
    //         free(line);
    //         free(tmp);
    //         tmp = NULL;
    //     }
    //     else
    //         break;
    //     line = NULL;
    // }
    // if (line)
    //     free(line);
    // line = NULL;
    // tmp = ft_strjoin(buf, crlf);
    // free(buf);
    // buf = ft_strdup(tmp);
    // free(tmp);
    // while (get_next_line(fd, &line) > 0) // parse request line
	// {
    //     if (line)
    //     {
    //         tmp = ft_strjoin(line, (char *)"\n");
    //         free(line);
    //         line = ft_strjoin(buf, tmp);
    //         free(buf);
    //         buf = ft_strdup(line);
    //         free(line);
    //         free(tmp);
    //         tmp = NULL;
    //     }
    //     else
    //         break;
    //     line = NULL;
    // }
    // if (line)
    // {
    //     tmp = ft_strjoin(buf, line);
    //     free(buf);
    //     buf = ft_strdup(tmp);
    //     free(tmp);
    //     free(line);
    //     line = NULL;
    // }
    close(fd);
    std::cout << "-----" << std::endl;
    std::cout << buf;
    std::cout << "-----" << std::endl;


    // std::cout << buf << std::endl;
    send(sock , buf , size, 0);
    free(buf);
    printf("Request sent\n");
    char buffer[1024] = {0};
    valread = read(sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
