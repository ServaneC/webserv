/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 14:13:12 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/07 14:34:33 by schene           ###   ########.fr       */
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

std::list<std::string> parsingIndexes(std::string conf)
{
    std::list<std::string>  indexes;
    size_t                  index_i = conf.find("index");

    while (conf.find("autoindex") == index_i - 4)
        index_i = conf.find("index", index_i + 1);
    if (index_i == std::string::npos)
    {
        indexes.push_front("index.html");
        return (indexes);
    }
    index_i += 6;
    while (index_i < conf.find(";", index_i) && index_i < std::string::npos)
    {
        size_t path_index = conf.find_first_not_of(" \t\n\r\v\f", index_i);
        size_t path_length = conf.find_first_of(" \t\n\r\v\f;", path_index) - path_index;
        indexes.push_back(conf.substr(path_index, path_length));
        // std::cout << "Added index <" << conf.substr(path_index, path_length) << ">" << std::endl;
        index_i = path_index + path_length;
    }
    return (indexes);
}

bool parsingAutoIndex(Server &server, std::string conf)
{
    size_t  index = conf.find("autoindex");

    if (index == std::string::npos)
        return (server.getAutoIndex());
    index += 10;
    index = conf.find_first_not_of(" \t\n\r\v\f", index);
    if (!conf.compare(index, 2, "on"))
        return (true);
    return (false);
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

const Location *getRelevantLocation(const std::list<Location> &_routes, std::string target)
{
    const Location *mostRelevant = NULL;

    for (std::list<Location>::const_iterator it = _routes.begin(); it != _routes.end(); it++)
    {
        std::string path = it->getPath();
        // if (path[0] == '/')
        // {
            // std::cout << "SLASH : on compare ->\n";
            // int i = target.compare(0, path.size(), path);
            // std::cout << "ret de compare = " << i << std::endl;
        if (!target.compare(0, path.size(), path) && (!mostRelevant || mostRelevant->getPath().size() < path.size()))
            mostRelevant = &(*it);
        // std::cout << "path.size = " << path.size() << std::endl;
        // }
    }
    return (mostRelevant);
}

const Location *getRelevantExtension(const std::list<Location> &_routes, std::string target)
{
    for (std::list<Location>::const_iterator it = _routes.begin(); it != _routes.end(); it++)
    {
        std::string path = it->getPath();
        // if (path[0] == '*')
        // {
            // path.erase(path.begin());
            // std::cout << "ETOILE : on check -> " << std::endl;
            // std::cout << "|" << target.substr(target.size() - extension_length, extension_length) << "|" << std::endl;
        // std::cout << "path.size = " << path.size() << std::endl;
        // std::cout << "target.size = " << target.size() << std::endl;
        // if (target.size() > path.size() && !path.compare(target.size() - path.size(), path.size(), target))
        if (target.size() > path.size() && !target.compare(target.size() - path.size(), path.size(), path))
            return (&(*it));
        // }
    }
    return (NULL);
}

std::string setPathInfo(Server &server, Request &request, std::string target)
{
    const Location *loc = getRelevantLocation(server.getRoutes(), target);
	const Location *ext = getRelevantExtension(server.getRoutes(), target);
	std::string path = server.getRoot();
    std::cout << "loc -> "<< loc << std::endl;
	if (loc)
    {
        path = loc->getRoot();
        if (loc->getPath().size() > 1)
        target.erase(1, loc->getPath().size() - 1);
        // std::cout << "-> " <<  << std::endl;
    }
    if ((loc && !loc->isAcceptedMethod(request.getMethodCode()))
        || (ext && !ext->isAcceptedMethod(request.getMethodCode())))
        throw methodNotAllowedException();
	// if (chdir(path.c_str()) == -1)
	// {
	// 	// this->_env["STATUS_CODE"] = "500";  ??
	// 	return ;
	// }
    std::cout << "PATH = <<" << path << ">>" << std::endl;
    if (target.compare("/") != 0)
        // path = "/YoupiBanane" + target.substr(0, target.find_first_of("?=;"));
        path += target.substr(0, target.find_first_of("?=;"));
    std::cout << "PATH = <<" << path << ">>" << std::endl;
    return (path);
}