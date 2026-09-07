#include "PollEventManager.hpp"
#include <poll.h>

PollEventManager::PollEventManager()
{
}

PollEventManager::~PollEventManager()
{
}

void	PollEventManager::add(int fd, int interest)
{
	_interest[fd] = interest;
}

void	PollEventManager::modify(int fd, int interest)
{
	std::map<int, int>::iterator	it = _interest.find(fd);

	if (it == _interest.end())
		return ;
	it->second = interest;
}

void	PollEventManager::remove(int fd)
{
	_interest.erase(fd);
}

static short	toPollEvents(int interest)
{
	int	events = 0;

	if (interest & EVENT_READ)
		events |= POLLIN;
	if (interest & EVENT_WRITE)
		events |= POLLOUT;
	return (static_cast<short>(events));
}

static int	toEventTypes(short revents)
{
	int	types = 0;

	if (revents & POLLIN)
		types |= EVENT_READ;
	if (revents & POLLOUT)
		types |= EVENT_WRITE;
	if (revents & (POLLERR | POLLHUP | POLLNVAL))
		types |= EVENT_ERROR;
	return (types);
}

int	PollEventManager::wait(std::vector<Event> &out, int timeoutMs)
{
	std::vector<struct pollfd>			fds;
	std::map<int, int>::const_iterator	it;

	out.clear();
	fds.reserve(_interest.size());
	for (it = _interest.begin(); it != _interest.end(); ++it)
	{
		struct pollfd	pfd;

		pfd.fd = it->first;
		pfd.events = toPollEvents(it->second);
		pfd.revents = 0;
		fds.push_back(pfd);
	}

	if (fds.empty())
		return (0);

	int	ready = poll(&fds[0], fds.size(), timeoutMs);

	if (ready <= 0)
		return (ready);

	for (std::size_t i = 0; i < fds.size(); ++i)
	{
		if (fds[i].revents == 0)
			continue ;

		Event	event;

		event.fd = fds[i].fd;
		event.types = toEventTypes(fds[i].revents);
		out.push_back(event);
	}
	return (ready);
}
