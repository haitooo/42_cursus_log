#include "SessionStore.hpp"

#include <utility>

#include "Random.hpp"

SessionStore::SessionStore(std::size_t ttlSec, std::size_t maxCount)
	: _sessions(),
	  _ttlSec(ttlSec),
	  _maxCount(maxCount)
{
}

SessionStore::~SessionStore()
{
}

const Session	*SessionStore::find(const std::string &id, std::time_t now) const
{
	std::map<std::string, Session>::const_iterator	it = _sessions.find(id);

	if (it == _sessions.end())
		return (NULL);
	if (now < it->second.expiresAt)
		return (&it->second);
	return (NULL);
}

std::string	SessionStore::create(std::time_t now)
{
	if (_maxCount == 0)
		return (std::string());

	const std::string	id = randomHexToken(SESSION_ID_BYTES);

	if (id.empty())
		return (std::string());
	if (_sessions.size() >= _maxCount)
		evictOldest();

	Session	session;

	session.createdAt = now;
	session.expiresAt = now + static_cast<std::time_t>(_ttlSec);
	session.visits = 1;

	if (!_sessions.insert(std::make_pair(id, session)).second)
		return (std::string());
	return (id);
}

void	SessionStore::recordVisit(const std::string &id, std::time_t now)
{
	std::map<std::string, Session>::iterator	it = _sessions.find(id);

	if (it == _sessions.end())
		return ;
	if (now < it->second.expiresAt)
		it->second.visits++;
}

void	SessionStore::sweep(std::time_t now)
{
	std::map<std::string, Session>::iterator	it = _sessions.begin();

	while (it != _sessions.end())
	{
		if (it->second.expiresAt <= now)
			_sessions.erase(it++);
		else
			++it;
	}
}

std::size_t	SessionStore::size() const
{
	return (_sessions.size());
}

std::size_t	SessionStore::ttlSeconds() const
{
	return (_ttlSec);
}

void	SessionStore::evictOldest()
{
	if (_sessions.empty())
		return ;

	std::map<std::string, Session>::iterator	oldest = _sessions.begin();
	std::map<std::string, Session>::iterator	it;

	for (it = _sessions.begin(); it != _sessions.end(); ++it)
	{
		if (it->second.createdAt < oldest->second.createdAt)
			oldest = it;
	}
	_sessions.erase(oldest);
}
