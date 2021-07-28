/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schene <schene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 14:13:12 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/28 12:30:01 by schene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parsing.hpp"

// **************************************** //
// ***********  PARSING SERVER  *********** //
// **************************************** //

std::string parsingName(const std::string &conf)
{
    size_t name_index = conf.find("server_name");
    if (name_index == std::string::npos)
        return (std::string());
    name_index = conf.find_first_not_of(" \t\n\r\v\f", name_index + 12);
    size_t name_length = conf.find(";", name_index);
    if (name_length == std::string::npos)
        throw confBadServerNameException();
    return (conf.substr(name_index, name_length - name_index));
}

std::string parsingRoot(const std::string &loc_conf, const Location &general)//, const std::string &server_conf, const std::list<Location*> &list)
{
    std::string root;
    size_t root_index = loc_conf.find("root");
    
    if (root_index == std::string::npos)  // pas de directive root dans la loc
    {
        if (general.getRoot().empty())  // pas de directive root dans le server non plus
            return (getCurrentDirectory());
        return (general.getRoot());
    }
    root_index = loc_conf.find_first_not_of(" \t\n\r\v\f", root_index + 4);
    size_t root_end = loc_conf.find_first_of("; \t\n\r\v\f", root_index);
    if (root_end == std::string::npos)
        throw confInvalidRootException();
    return (getCurrentDirectory().append(loc_conf.substr(root_index, root_end - root_index)));
}

std::string  parsingIPAddress(const std::string &conf, unsigned int *ip, int *port)
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
    return (conf.substr(addr_index, conf.find(':', addr_index) - addr_index));
}

void parsingLocations(std::list<Location*> &routes, const std::string &conf)
{
    size_t              last_found = conf.find("location");

    while (last_found < conf.size() && last_found < std::string::npos)
    {
        last_found += 9;
        size_t path_index = conf.find_first_not_of(" \t\n\r\v\f", last_found);
        size_t path_length = conf.find_first_of(" \t\n\r\v\f", path_index) - path_index;
        std::string path = conf.substr(path_index, path_length);
        std::string rules = getScope(conf, path_index + path_length);
        if (!rules.empty())
        {
            Location *to_push = new Location(path, rules, *routes.front());
            routes.push_back(to_push);
        }
        if ((last_found = conf.find("}", last_found)) != std::string::npos)
            last_found = conf.find("location", last_found);
    }
}

// ******************************************* //
// ***********  PARSING LOCATIONS  *********** //
// ******************************************* //

std::vector<int> parsingAcceptedMethods(const std::string &location_conf)
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

std::list<std::string> parsingIndexes(const std::string &loc_conf, const Location &general)//, const std::string &server_conf)
{
    std::list<std::string>  indexes;
    size_t                  index_i = loc_conf.find("index");

    while (loc_conf.find("autoindex") == index_i - 4)
        index_i = loc_conf.find("index", index_i + 1);
    if (index_i == std::string::npos)
    {
        if (general.getIndexes().empty())
            indexes.push_front("index.html");
        else
            indexes = general.getIndexes();
        return (indexes);
    }
    index_i += 6;
    while (index_i < loc_conf.find(";", index_i) && index_i < std::string::npos)
    {
        size_t path_index = loc_conf.find_first_not_of(" \t\n\r\v\f", index_i);
        size_t path_length = loc_conf.find_first_of(" \t\n\r\v\f;", path_index) - path_index;
        indexes.push_back(loc_conf.substr(path_index, path_length));
        index_i = path_index + path_length;
    }
    return (indexes);
}

bool parsingAutoIndex(const std::string &loc_conf, const Location &general)//const std::string &server_conf)
{
    size_t  index = loc_conf.find("autoindex");

    if (index == std::string::npos)
        return (general.getAutoIndex());
    index += 10;
    index = loc_conf.find_first_not_of(" \t\n\r\v\f", index);
    if (!loc_conf.compare(index, 2, "on"))
        return (true);
    return (false);
}

std::string    parsingCGIconf(const std::string &location_conf, const Location &general)//, const std::string &server_conf)
{
    size_t      index = location_conf.find("cgi_path");
    std::string cgi_path;
    struct stat statbuf;

    if (index == std::string::npos)
        return (general.getCGIPath());
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

size_t parsingBodySize(const std::string &loc_conf, const Location &general)//const std::string &server_conf)
{
    size_t  index = loc_conf.find("max_body_size");
    size_t  max_body_size = 0;

    if (index == std::string::npos)
        return (general.getMaxBodySize());
    index += 14;
    index = loc_conf.find_first_not_of(" \t\n\r\v\f", index);
    size_t length = loc_conf.find_first_not_of("0123456789", index) - index;
    max_body_size = ft_atoul(loc_conf.substr(index, length));
    return (max_body_size);
}

std::string    parsingErrorPage(const Location &location, const Location &general)//, const std::list<Location*> &list)
{
    struct stat statbuf;
    std::string page_path;
    std::string location_conf = location.getConf();
    size_t      index = location_conf.find("error_page");

    if (index == std::string::npos)
        return (general.getErrorPage());
    index += 11;
    index = location_conf.find_first_not_of(" \t\n\r\v\f", index);
    size_t end = location_conf.find(";", index);
    if (end == std::string::npos)
        throw confInvalidErrorPageException();
    page_path = location_conf.substr(index, end - index);
    page_path.insert(0, location.getRoot() + "/");
    if (lstat(page_path.c_str(), &statbuf) == - 1)
        throw confInvalidErrorPageException();
    return (page_path);
}

std::string parsingRedirection(const std::string &loc_conf, const Location &general)
{
    size_t          index = loc_conf.find("return");
    int             code;
    std::string     redirect_url;

    if (index == std::string::npos)
        return (general.getRedirectURL());
    index += 7;
    index = loc_conf.find_first_not_of(" \t\n\r\v\f", index);
    size_t length = loc_conf.find_first_not_of("0123456789", index) - index;
    code = std::atoi(loc_conf.substr(index, length).c_str());
    index += length;
    index = loc_conf.find_first_not_of(" \t\n\r\v\f", index);
    if (code == 301)
    {
        length = loc_conf.find(';', index) - index;
        return (loc_conf.substr(index, length));
    }
    return (general.getRedirectURL());
}

std::string parsingUploadPath(const std::string &loc_conf, const Location &general)
{
    size_t      index = loc_conf.find("upload_path");
    std::string upload_path;

    if (index == std::string::npos)
        return (general.getUploadPath());
    index += 12;
    index = loc_conf.find_first_not_of(" \t\n\r\v\f", index);
    size_t end = loc_conf.find(";", index);
    if (end == std::string::npos)
        throw confInvalidCGIException();
    upload_path = loc_conf.substr(index, end - index);
    return (upload_path);
}

// ******************************************** //
// ***********  HANDLING LOCATIONS  *********** //
// ******************************************** //


const Location &getRelevantLocation(const std::list<Location*> &_routes, const std::string &target)
{
    std::list<Location*>::const_iterator it = ++_routes.begin();
    std::string    loc = target;
    const Location *mostRelevant = (*(it));

    if (target.size() > 1 && target.find('/', 1) != std::string::npos)
        loc = loc.erase(target.find('/', 1));
    for (; it != _routes.end(); it++)
    {
        std::string     path = (*it)->getPath();
        size_t          size = path.size() > loc.size() ? path.size() : loc.size();
        if (!loc.compare(0, size, path) && (mostRelevant->getPath().size() < path.size()))
            mostRelevant = (*it);
    }
    return (*mostRelevant);
}

std::string buildPath(Server &server, Request &request, const std::string &target)
{  
    const Location  &loc = getRelevantLocation(server.getRoutes(), target);
    std::string     path(target);

    if (loc.getRootInConf() && loc.getPath().size() > 1)
    {
        if (path.find(loc.getPath()) != std::string::npos)
            path.erase(path.find(loc.getPath()), loc.getPath().size());
        if (path.empty())
            path = "/";
    }
    path.erase(path.find_last_of("/"), path.size() - path.find_last_of("/"));
    if (!loc.isAcceptedMethod(request.getMethodCode()))
        throw methodNotAllowedException();
    path.insert(0, loc.getRoot());
    if (loc.getPath().size() > 1)
    {
        if (path.find(loc.getPath()) != std::string::npos &&
            loc.getRoot().compare(server.getRoot()) != 0)
                path.erase(path.find(loc.getPath()), loc.getPath().size());
        if (request.getObject().find(&loc.getPath()[1]) != std::string::npos) // no '/' at the end
        {
            std::string tmp = request.getObject();
            if (loc.getRoot().compare(server.getRoot()) != 0)
                tmp.erase(tmp.find(&loc.getPath()[1]), loc.getPath().size() - 1);
            request.setObject(tmp);
        }
    }
    return (path);
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

const Location *findRootLocation(const std::list<Location*> &list)
{
    for (std::list<Location*>::const_iterator it = list.begin(); it != list.end(); it++)
    {
        if (!(*it)->getPath().compare("/"))
            return (*it);
    }
    return (NULL);
}
