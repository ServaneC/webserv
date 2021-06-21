/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 17:06:46 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/21 11:26:26 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_EXCEPTIONS_H
# define WEBSERV_EXCEPTIONS_H

# include "../../webserv.hpp"

class confIPAddrException : public std::exception {
    public :
        const char* what() const throw() {
            return ("Configuration file: Format error in IP Address : must be \"X.X.X.X:PORT;\""); }
};

class confNoListenException : public std::exception {
    public :
        const char* what() const throw() {
            return ("Configuration file: No IP or port to listen to"); }
};

class confInvalidPortException : public std::exception {
    public :
        const char* what() const throw() {
            return ("Configuration file: Invalid port number"); }
};

class confBadServerNameException : public std::exception {
    public :
        const char* what() const throw() {
            return ("Configuration file: Invalid server name"); }
};

class confInvalidRootException : public std::exception {
    public :
        const char* what() const throw() {
            return ("Configuration file: Invalid root path"); }
};


#endif