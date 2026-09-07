#ifndef SESSIONSTORE_HPP
# define SESSIONSTORE_HPP

# include <cstddef>
# include <ctime>
# include <map>
# include <string>

# define SESSION_TTL_SEC 1800
# define SESSION_MAX_COUNT 1024
# define SESSION_ID_BYTES 12

struct Session
{
	std::time_t	createdAt;
	std::time_t	expiresAt;
	std::size_t	visits;

	Session() : createdAt(0), expiresAt(0), visits(0) {}
};

class SessionStore
{
	public:
		SessionStore(std::size_t ttlSec, std::size_t maxCount);
		~SessionStore();

		const Session	*find(const std::string &id, std::time_t now) const;
		std::string		create(std::time_t now);
		void			recordVisit(const std::string &id, std::time_t now);
		void			sweep(std::time_t now);

		std::size_t		size() const;
		std::size_t		ttlSeconds() const;

	private:
		std::map<std::string, Session>	_sessions;
		std::size_t						_ttlSec;
		std::size_t						_maxCount;

		void	evictOldest();

		SessionStore();
		SessionStore(const SessionStore &other);
		SessionStore	&operator=(const SessionStore &other);
};

#endif
