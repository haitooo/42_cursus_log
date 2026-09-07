#ifndef POLLEVENTMANAGER_HPP
# define POLLEVENTMANAGER_HPP

# include <map>
# include "IEventManager.hpp"

class PollEventManager : public IEventManager
{
	private:
		std::map<int, int>	_interest;

	public:
		PollEventManager();
		~PollEventManager();

		void	add(int fd, int interest);
		void	modify(int fd, int interest);
		void	remove(int fd);
		int		wait(std::vector<Event> &out, int timeoutMs);
};

#endif
