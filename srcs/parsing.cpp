/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 14:13:12 by lemarabe          #+#    #+#             */
/*   Updated: 2021/06/28 14:34:15 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parsing.hpp"

std::string parsingName(std::string conf)
{
    size_t name_index = conf.find("server_name");
    if (name_index == std::string::npos)
        return (std::string());
        // throw confNoServerNameException();  // a voir, je sais pas si un serveur doit forcement avoir un nom donné en config
    name_index = conf.find_first_not_of(" \t\n\r\v\f", name_index + 12);
    size_t name_length = conf.find(";", name_index);
    if (name_length == std::string::npos)
        throw confBadServerNameException();
    return (conf.substr(name_index, name_length - name_index));
}

std::string parsingRoot(std::string conf)
{
    size_t root_index = conf.find("root");
    if (root_index == std::string::npos)
        return (getCurrentDirectory());
    root_index = conf.find_first_not_of(" \t\n\r\v\f", root_index + 4);
    size_t root_end = conf.find_first_of("; \t\n\r\v\f", root_index);
    if (root_end == std::string::npos)
        throw confInvalidRootException();
    std::string root = getCurrentDirectory();
    root.append(conf.substr(root_index, root_end - root_index));
    // if (root[root.size() - 1] != '/')
    //     root.append("/");
    return (root);
}

std::string parsingLocalRoot(std::string server_root, std::string conf)
{
    size_t root_index = conf.find("root");
    if (root_index == std::string::npos)
        return (server_root);
    root_index = conf.find_first_not_of(" \t\n\r\v\f", root_index + 4);
    size_t root_end = conf.find_first_of("; \t\n\r\v\f", root_index);
    if (root_end == std::string::npos)
        throw confInvalidRootException();
    std::string root(server_root);
    root.append(conf.substr(root_index, root_end - root_index));
    return (root);
}

unsigned int    convertIPAddress(std::string conf, size_t index)
{
    IPA_t   addr;
    
    for (int i = 3; i >= 0; i--)
        addr.block[i] = getValueBetweenPoints(conf, &index);
    return (addr.address);
}

void parsingIPAddress(std::string conf, unsigned int *ip, int *port)
{
    size_t  addr_index = conf.find("listen");
    if (addr_index == std::string::npos)
        throw confNoListenException();
    addr_index = conf.find_first_not_of(" \t\n\r\v\f", addr_index + 7);
    *ip = convertIPAddress(conf, addr_index);
    size_t port_index = conf.find(":", addr_index);
    if (port_index == std::string::npos)
        throw confIPAddrException();
    port_index++;
    size_t port_length = conf.find(";", port_index);
    if (port_length == std::string::npos)
        throw confIPAddrException();
    *port = std::atoi(conf.substr(port_index, port_length - port_index).c_str());
    if (*port < 0 || *port > 65535)
        throw confInvalidPortException();
}

std::list<Location> parsingLocations(Server &server, std::string conf)
{
    size_t              last_found = conf.find("location");
    std::list<Location> routes;

    while (last_found < conf.size() && last_found < std::string::npos)
    {
        last_found += 9;
        size_t path_index = conf.find_first_not_of(" \t\n\r\v\f", last_found);
        size_t path_length = conf.find_first_of(" \t\n\r\v\f", path_index) - path_index;
        std::string path = conf.substr(path_index, path_length);
        std::string rules = getScope(conf, path_index + path_length);
        if (!rules.empty())
        {
            Location *to_push = new Location(&server, path, rules);
            routes.push_back(*to_push);
            //std::cout << "FOR LOCATION <" << path << ">, RULES ARE : " << rules << std::endl;
        }
        last_found = conf.find("location", last_found);
    }
    return (routes);
}

int     setMethodCode(std::string method_name)
{
    int code = METHOD_NOT_ALLOWED;
    code = !method_name.compare("GET") ? METHOD_GET : code;
    code = !method_name.compare("POST") ? METHOD_POST : code;
    code = !method_name.compare("DELETE") ? METHOD_DELETE : code;
    return (code);
}

std::vector<int> parseAcceptedMethods(std::string location_conf)
{
    std::vector<int>    methods;
    size_t              index = location_conf.find("accepted_method");
    size_t              end = location_conf.find(';', index);

    if (index == std::string::npos || end == std::string::npos)
        return (methods);
    index += 15;
    while (index != end)
        methods.push_back(parseMethod(location_conf, &index));
    return (methods);
}