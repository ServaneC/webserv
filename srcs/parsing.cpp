/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemarabe <lemarabe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 14:13:12 by lemarabe          #+#    #+#             */
/*   Updated: 2021/07/18 19:59:30 by lemarabe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parsing.hpp"

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

std::string parsingRoot(const std::string &loc_conf, const Location &general)//, const std::string &server_conf, const std::list<Location> &list)
{
    std::string root;
    size_t root_index = loc_conf.find("root");
    
    if (root_index == std::string::npos)  // pas de directive root dans la loc
    {
        // std::string root;
        // const Location *tmp = findRootLocation(list);
        // if (!tmp)
        root = general.getRoot();
        //     root = parsingRoot(server_conf, server_conf, list);  // chercher hors des locations
        // else
        //     root = tmp->getRoot();  // la root du server a deja été trouvé
        if (root.empty())  // pas de directive root dans le server non plus
            return (getCurrentDirectory());
        return (root);
    }
    root_index = loc_conf.find_first_not_of(" \t\n\r\v\f", root_index + 4);
    size_t root_end = loc_conf.find_first_of("; \t\n\r\v\f", root_index);
    if (root_end == std::string::npos)
        throw confInvalidRootException();
    // const Location *root = findRootLocation(list);
    // if (root)
    //     return (root->getRoot().append(loc_conf.substr(root_index, root_end - root_index)));
    root = general.getRoot();
    if (!root.empty())
        return (root.append(loc_conf.substr(root_index, root_end - root_index)));
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

std::list<std::string> parsingIndexes(const std::string &loc_conf, const Location &general)//, const std::string &server_conf)
{
    std::list<std::string>  indexes;
    size_t                  index_i = loc_conf.find("index");

    while (loc_conf.find("autoindex") == index_i - 4)
        index_i = loc_conf.find("index", index_i + 1);
    if (index_i == std::string::npos)
    {
        // std::cout << "** SERVER CONF = " << server_conf << std::endl;
        if (general.getIndexes().empty())
            indexes.push_front("index.html");
        else
            indexes = general.getIndexes();
        // if (loc_conf != server_conf)
        //     indexes = parsingIndexes(server_conf, server_conf);
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

    for (; it != _routes.end(); it++)
    {
        std::string     path = it->getPath();
        if (!target.compare(0, path.size(), path) && (mostRelevant->getPath().size() < path.size()))
            mostRelevant = &(*it);
    }
    return (*mostRelevant);
}

const Location &getRelevantExtension(const std::list<Location> &_routes, const std::string &target)
{
    std::list<Location>::const_iterator it = _routes.begin();

    while (it != _routes.end())
    {
        std::string path = it->getPath();
        if (target.size() > path.size() && !target.compare(target.size() - path.size(), path.size(), path))
            return (*it);
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

    // std::cout << "BUILD PATH :" << std::endl;
    path.erase(path.find_last_of("/"), path.size() - path.find_last_of("/"));
    // std::cout << "\tTarget au debut (moins object) -> [" << path << "]" << std::endl;
    if (!loc.isAcceptedMethod(request.getMethodCode()) || !ext.isAcceptedMethod(request.getMethodCode()))
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
    // std::cout << "\tPath built-> [" << path << "]" << std::endl;
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

size_t parsingBodySize(const std::string &loc_conf, const std::string &server_conf)
{
    size_t  index = loc_conf.find("max_body_size");
    size_t  max_body_size = 0;

    if (index == std::string::npos)
    {
        if (loc_conf != server_conf)
            return (parsingBodySize(server_conf, server_conf));
        else
            return (--max_body_size);  //return ulong max si precisé nulle part
    }
    index += 14;
    index = loc_conf.find_first_not_of(" \t\n\r\v\f", index);
    size_t length = loc_conf.find_first_not_of("0123456789", index) - index;
    max_body_size = ft_atoul(loc_conf.substr(index, length));
    return (max_body_size);
}

// std::string    parsingErrorPage(const std::string &location_conf, const std::string &server_conf)//, const std::list<Location> &list)
// {
//     std::string page_path;
//     // struct stat statbuf;
//     size_t      index = location_conf.find("error_page");

//     if (index == std::string::npos || index == 0)
//     {
//         if (&location_conf == &server_conf)
//             return (std::string());
//         else
//             return (parsingErrorPage(server_conf, server_conf));
//     }
//     index += 11;
//     index = location_conf.find_first_not_of(" \t\n\r\v\f", index);
//     size_t end = location_conf.find(";", index);
//     if (end == std::string::npos)
//         throw confInvalidErrorPageException();
//     page_path = location_conf.substr(index, end - index);
//     return (page_path);
// }

std::string parsingErrorPage(const Location &loc, const std::list<Location> &list)
{
    std::string loc_conf = loc.getConf();
    size_t page_index = loc_conf.find("error_page");
    std::cout << "PAGE INDEX =" << page_index << std::endl;
    std::cout << "PATH =" << loc.getPath() << std::endl;
    std::cout << "CONF =" << loc_conf << std::endl;
    if (page_index == std::string::npos)  // pas de directive root dans la loc
    {
        //const Location *root = findRootLocation(list);
        const Location &gen_loc = list.front();
        if (&loc == &gen_loc) //&gen_loc == NULL || 
            return (std::string());  // pas de page d'erreur
        else
            return (parsingErrorPage(gen_loc, list));  // chercher hors des locations
    }
    page_index = loc_conf.find_first_not_of(" \t\n\r\v\f", page_index + 11);
    size_t page_end = loc_conf.find_first_of("; \t\n\r\v\f", page_index);
    if (page_end == std::string::npos)
        throw confInvalidRootException();
    // const Location *root = findRootLocation(list);
    // if (root)
    //     return (root->getRoot().append(loc_conf.substr(page_index, page_end - page_index)));
    return (loc.getRoot().append("/" + loc_conf.substr(page_index, page_end - page_index)));
}

// void checkErrorPages(std::list<Location> &list)
// {
//     const Location *root = findRootLocation(list);

//     for (std::list<Location>::iterator it = list.begin(); it != list.end(); it++)
//     {
//         std::cout << "PATH de la page d'erreur avant = " << it->getErrorPage() << std::endl;
//         if (!it->getErrorPage().empty())  // error page directive present
//         {
//             if (!it->getRoot().empty())
//                 it->addErrorPagePrefix(it->getRoot() + "/");
//             else
//                 it->addErrorPagePrefix(root->getRoot() + "/");
//             if (lstat(it->getErrorPage().c_str(), &statbuf) == - 1)
//                 throw confInvalidErrorPageException();
//         }
//     }
// }

// void            updateWithRootInfos(std::list<Location> &routes, const Location *root)
// {
//     for (std::list<Location>::iterator it = list.begin(); it != list.end(); it++)
//     {
//         Location &loc = *it;

//         if (loc.getRoot().empty() && !root->getRoot().empty())
//             loc.setRoot(root->getRoot());
//         if (loc.getIndexes().empty() && !root->getIndexes().empty())
//             loc.setIndexes(root->getIndexes());
//         if (loc.getCGIPath().empty() && !root->getCGIPath().empty())
//             loc.setCGIpath(root->getCGIPath());
//         if (loc.getMaxBodySize())
//     }
    
//     if (!page_path.empty())
//     {
//         page_path.insert(0, root_prefix);
//         std::cout << "PATH de la page d'erreur qu'on checke = " << page_path << std::endl;
//         if (lstat(page_path.c_str(), &statbuf) == - 1)
//             throw confInvalidErrorPageException();
//     }
// }
