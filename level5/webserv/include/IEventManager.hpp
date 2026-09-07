#ifndef IEVENTMANAGER_HPP
# define IEVENTMANAGER_HPP

# include <vector>

enum EventType
{
	EVENT_READ	= 1,
	EVENT_WRITE	= 2,
	EVENT_ERROR	= 4
};

struct Event
{
	int	fd;
	int	types;

	Event() : fd(-1), types(0) {}
};

class IEventManager
{
	public:
		virtual ~IEventManager() {}

		virtual void	add(int fd, int interest) = 0;
		virtual void	modify(int fd, int interest) = 0;
		virtual void	remove(int fd) = 0;
		virtual int		wait(std::vector<Event> &out, int timeoutMs) = 0;
};

#endif
