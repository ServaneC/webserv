/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 14:13:12 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/09 19:52:34 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parsing.hpp"

std::string parsingName(const std::string &conf)
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

// std::string parsingRoot(const std::string &conf)
// {
//     size_t root_index = conf.find("root");
//     if (root_index == std::string::npos)
//         return (getCurrentDirectory());
//     root_index = conf.find_first_not_of(" \t\n\r\v\f", root_index + 4);
//     size_t root_end = conf.find_first_of("; \t\n\r\v\f", root_index);
//     if (root_end == std::string::npos)
//         throw confInvalidRootException();
//     std::string root = getCurrentDirectory();
//     root.append(conf.substr(root_index, root_end - root_index));
//     // if (root[root.size() - 1] != '/')
//     //     root.append("/");
//     return (root);
// }

std::string parsingRoot(const std::string &loc_conf, const std::string &server_conf, const std::list<Location> &list)
{
    size_t root_index = loc_conf.find("root");
    if (root_index == std::string::npos)  // pas de directive root dans la loc
    {
        std::string root = parsingRoot(server_conf, server_conf, list);  // chercher hors des locations
        if (root.empty())  // pas de directive root dans le server non plus
            return (getCurrentDirectory());
        else
            return (root);
    }
    root_index = loc_conf.find_first_not_of(" \t\n\r\v\f", root_index + 4);
    size_t root_end = loc_conf.find_first_of("; \t\n\r\v\f", root_index);
    if (root_end == std::string::npos)
        throw confInvalidRootException();
    const Location *root = findRootLocation(list);
    if (root)
        return (root->getRoot().append(loc_conf.substr(root_index, root_end - root_index)));
    return (getCurrentDirectory().append(loc_conf.substr(root_index, root_end - root_index)));
}

unsigned int    convertIPAddress(const std::string &conf, size_t index)
{
    IPA_t   addr;
    
    for (int i = 3; i >= 0; i--)
        addr.block[i] = getValueBetweenPoints(conf, &index);
    return (addr.address);
}

void parsingIPAddress(const std::string &conf, unsigned int *ip, int *port)
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

std::list<Location> parsingLocations(const std::string &conf)
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
            Location *to_push = new Location(path, rules, trimLocations(conf), routes);
            routes.push_back(*to_push);
            //std::cout << "FOR LOCATION <" << path << ">, RULES ARE : " << rules << std::endl;
        }
        last_found = conf.find("location", last_found);
    }
    return (routes);
}

std::list<std::string> parsingIndexes(const std::string &loc_conf, const std::string &server_conf)
{
    std::list<std::string>  indexes;
    size_t                  index_i = loc_conf.find("index");

    while (loc_conf.find("autoindex") == index_i - 4)
        index_i = loc_conf.find("index", index_i + 1);
    if (index_i == std::string::npos)
    {
        // std::cout << "** SERVER CONF = " << server_conf << std::endl;
        if (loc_conf != server_conf)
            indexes = parsingIndexes(server_conf, server_conf);
        else
            indexes.push_front("index.html");
        return (indexes);
    }
    index_i += 6;
    while (index_i < loc_conf.find(";", index_i) && index_i < std::string::npos)
    {
        size_t path_index = loc_conf.find_first_not_of(" \t\n\r\v\f", index_i);
        size_t path_length = loc_conf.find_first_of(" \t\n\r\v\f;", path_index) - path_index;
        indexes.push_back(loc_conf.substr(path_index, path_length));
        // std::cout << "Added index <" << conf.substr(path_index, path_length) << ">" << std::endl;
        index_i = path_index + path_length;
    }
    return (indexes);
}

bool parsingAutoIndex(const std::string &loc_conf, const std::string &server_conf)
{
    size_t  index = loc_conf.find("autoindex");

    if (index == std::string::npos)
    {
        if (loc_conf != server_conf)
            return (parsingAutoIndex(server_conf, server_conf));
    }
        // return (server.getAutoIndex());
    index += 10;
    index = loc_conf.find_first_not_of(" \t\n\r\v\f", index);
    if (!loc_conf.compare(index, 2, "on"))
        return (true);
    return (false);
}

int     setMethodCode(const std::string &method_name)
{
    int code = METHOD_NOT_ALLOWED;
    code = !method_name.compare("GET") ? METHOD_GET : code;
    code = !method_name.compare("POST") ? METHOD_POST : code;
    code = !method_name.compare("DELETE") ? METHOD_DELETE : code;
    return (code);
}

std::vector<int> parseAcceptedMethods(const std::string &location_conf)
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

const Location &getRelevantLocation(const std::list<Location> &_routes, const std::string &target)
{
    std::list<Location>::const_iterator it = _routes.begin();
    const Location *mostRelevant = &(*it);
    // Location    &mostRelevant = *it;

    for (; it != _routes.end(); it++)
    {
        std::string     path = it->getPath();
        // if (path[0] == '/')
        // {
            // std::cout << "SLASH : on compare ->\n";
        // std::cout << "path.size = " << path.size() << std::endl;
        if (!target.compare(0, path.size(), path) && (mostRelevant->getPath().size() < path.size()))
            mostRelevant = &(*it);
        // }
    }
    return (*mostRelevant);
}

const Location &getRelevantExtension(const std::list<Location> &_routes, const std::string &target)
{
    std::list<Location>::const_iterator it = _routes.begin();

    while (it != _routes.end())
    {
        std::string path = it->getPath();
        // if (path[0] == '*')
        // {
            // std::cout << "ETOILE : on check -> " << std::endl;
        // std::cout << "path.size = " << path.size() << std::endl;
        // std::cout << "target.size = " << target.size() << std::endl;
        if (target.size() > path.size() && !target.compare(target.size() - path.size(), path.size(), path))
            return (*it);
        // }
        it++;
    }
    it = _routes.begin();
    while (it != _routes.end() && it->getPath().compare("/"))
        it++;
    return (*it);
}

std::string buildPath(Server &server, Request &request, const std::string &target)
{  
    const Location  &loc = getRelevantLocation(server.getRoutes(), target);
	const Location  &ext = getRelevantExtension(server.getRoutes(), target);
    std::string     path(target);

    std::cout << "BUILD PATH :" << std::endl;
    path.erase(path.find_last_of("/"), path.size() - path.find_last_of("/"));
    std::cout << "\tTarget au debut (moins object) -> [" << path << "]" << std::endl;
    if (!loc.isAcceptedMethod(request.getMethodCode()) || !ext.isAcceptedMethod(request.getMethodCode()))
        throw methodNotAllowedException();
    path.insert(0, loc.getRoot());
    std::cout << "Path-> [" << path << "]" << std::endl;
    std::cout << "Loc path-> [" << loc.getPath() << "]" << std::endl;
    
    // if (loc->getPath().size() > 1 && server.getRoot().compare(loc->getRoot()) != 0) // take only location w/ a root set
    if (loc.getPath().size() > 1)
    {
        if (path.find(loc.getPath()) != std::string::npos) // '/' at the end
            path.erase(path.find(loc.getPath()), loc.getPath().size());
        else if (request.getObject().find(&loc.getPath()[1]) != std::string::npos) // no '/' at the end
        {
            std::string tmp = request.getObject();
            tmp.erase(tmp.find(&loc.getPath()[1]), loc.getPath().size() - 1);
            request.setObject(tmp);
        }
    }
    // path.replace(0, loc->getPath().size() - 1, loc->getRoot()); // => insert instead
    // request.setObject("");  // ca marchera pas si c'est [/directory/resource] qui est demandé est pas juste [/directory]
    //      => on erase direct dans le path a la place
//}
    else
    {
        path.replace(0, 1, server.getRoot());
        path.push_back('/');
    }
    std::cout << "\tPath built-> [" << path << "]" << std::endl;
    return (path);
}

std::string    parsingCGIconf(const std::string &location_conf, const std::string &server_conf)
{
    size_t      index = location_conf.find("cgi_path");
    std::string cgi_path;
    struct stat statbuf;

    if (index == std::string::npos)
    {
        if (location_conf != server_conf)
            cgi_path = parsingCGIconf(server_conf, server_conf);
        return (cgi_path);
    }
    index += 9;
    index = location_conf.find_first_not_of(" \t\n\r\v\f", index);
    size_t end = location_conf.find(";", index);
    if (end == std::string::npos)
        throw confInvalidCGIException();
    cgi_path = location_conf.substr(index, end - index);
    if (lstat(cgi_path.c_str(), &statbuf) == - 1)
        throw confInvalidCGIException();
    return (cgi_path);
}

const std::string firstValidIndex(const std::list<std::string> &indexes)
{
    struct stat     statbuf;

    for (std::list<std::string>::const_iterator it = indexes.begin(); it != indexes.end(); it++)
    {
        if (lstat((*it).c_str(), &statbuf) != -1)
            return (*it);
    }
    return (std::string());
}

const Location *findRootLocation(const std::list<Location> &list)
{
    for (std::list<Location>::const_iterator it = list.begin(); it != list.end(); it++)
    {
        if (!it->getPath().compare("/"))
            return (&(*it));
    }
    return (NULL);
}