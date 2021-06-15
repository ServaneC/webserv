void	Cluster::run(void)
{
	std::string	dot[3] = {".  ", ".. ", "..."};
	int			n = 0;

	while (1)
	{
		fd_set		reading_set;
		fd_set		writing_set;
		struct timeval	timeout;
		int				ret = 0;

		while (ret == 0)
		{
			timeout.tv_sec  = 1;
			timeout.tv_usec = 0;
			ft_memcpy(&reading_set, &_fd_set, sizeof(_fd_set));
			FD_ZERO(&writing_set);
			for (std::vector<int>::iterator it = _ready.begin() ; it != _ready.end() ; it++)
				FD_SET(*it, &writing_set);

			std::cout << "\rWaiting on a connection" << dot[n++] << std::flush;
			if (n == 3)
				n = 0;

			ret = select(_max_fd + 1, &reading_set, &writing_set, NULL, &timeout);
		}

		if (ret > 0)
		{
			for (std::vector<int>::iterator it = _ready.begin() ; ret && it != _ready.end() ; it++)
			{
				if (FD_ISSET(*it, &writing_set))
				{
					long	ret = _sockets[*it]->send(*it);

					if (ret == 0)
						_ready.erase(it);
					else if (ret == -1)
					{
						FD_CLR(*it, &_fd_set);
						FD_CLR(*it, &reading_set);
						_sockets.erase(*it);
						_ready.erase(it);
					}

					ret = 0;
					break;
				}
			}

			if (ret)
				std::cout << "\rReceived a connection !   " << std::flush;

			for (std::map<long, Server *>::iterator it = _sockets.begin() ; ret && it != _sockets.end() ; it++)
			{
				long	socket = it->first;

				if (FD_ISSET(socket, &reading_set))
				{
					long	ret = it->second->recv(socket);

					if (ret == 0)
					{
						it->second->process(socket, _config);
						_ready.push_back(socket);
					}
					else if (ret == -1)
					{
						FD_CLR(socket, &_fd_set);
						FD_CLR(socket, &reading_set);
						_sockets.erase(socket);
						it = _sockets.begin();
					}
					ret = 0;
					break;
				}
			}

			for (std::map<long, Server>::iterator it = _servers.begin() ; ret && it != _servers.end() ; it++)
			{
				long	fd = it->first;

				if (FD_ISSET(fd, &reading_set))
				{
					long	socket = it->second.accept();

					if (socket != -1)
					{
						FD_SET(socket, &_fd_set);
						_sockets.insert(std::make_pair(socket, &(it->second)));
						if (socket > _max_fd)
							_max_fd = socket;
					}
					ret = 0;
					break;
				}
			}
		}
		else
		{
			std::cerr << RED << "Problem with select !" << RESET << std::endl;
			for (std::map<long, Server *>::iterator it = _sockets.begin() ; it != _sockets.end() ; it++)
				it->second->close(it->first);
			_sockets.clear();
			_ready.clear();
			FD_ZERO(&_fd_set);
			for (std::map<long, Server>::iterator it = _servers.begin() ; it != _servers.end() ; it++)
				FD_SET(it->first, &_fd_set);
		}

		n = 0;
	}
}