#include <iostream>
#include <ctime>

// time_t now = time(NULL);

// struct tm* local = localtime(&now);   // ローカル時刻に変換
// struct tm* utc   = gmtime(&now);      // UTCに変換

// struct tm {
//     int tm_sec;   // 秒   [0-60]
//     int tm_min;   // 分   [0-59]
//     int tm_hour;  // 時   [0-23]
//     int tm_mday;  // 日   [1-31]
//     int tm_mon;   // 月   [0-11]  ←注意: 0が1月
//     int tm_year;  // 年   (1900からの年数)
//     int tm_wday;  // 曜日 [0-6]   (日曜=0)
//     int tm_yday;  // 年内通算日 [0-365]
//     int tm_isdst; // サマータイムフラグ
// };

// char buf[20];     //出力
// strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
// std::cout << buf << std::endl;

static void	_displayTimestamp( void )
{
	std::time_t	now = std::time(NULL);
	std::tm		*ltm = std::localtime(&now);

	char	buffer[20];
	std::strftime(buffer, sizeof(buffer), "[%Y%m%d_%H%M%S]", ltm);

	std::cout << now << std::endl;

	std::cout << ltm->tm_year + 1900 << std::endl << ltm->tm_mon + 1 << std::endl << ltm->tm_mday << std::endl;

	std::cout << buffer;
}

int	main()
{
	_displayTimestamp();
	return (0);
}
