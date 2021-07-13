/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 17:06:46 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/13 21:45:12 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_EXCEPTIONS_H
# define WEBSERV_EXCEPTIONS_H

# include "../../webserv.hpp"

class methodNotAllowedException : public std::exception {
    public :
        const char* what() const throw() {
            return ("405 Method Not Allowed"); }
};

class targetNotFoundException : public std::exception {
    public :
        const char* what() const throw() {
            return ("404 Not Found"); }
};

class chdirFailException : public std::exception {
    public :
        const char* what() const throw() {
            return ("500 Internal Server Error (chdir failed)"); }
};

//////////////////////////
//  CONFIGURATION FILE  //
//////////////////////////

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

class confInvalidCGIException : public std::exception {
    public :
        const char* what() const throw() {
            return ("Configuration file: Invalid cgi path"); }
};

class confInvalidErrorPageException : public std::exception {
    public :
        const char* what() const throw() {
            return ("Configuration file: Invalid error_page path"); }
};

#endif