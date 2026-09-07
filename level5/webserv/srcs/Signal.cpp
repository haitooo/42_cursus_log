#include "Signal.hpp"

volatile	sig_atomic_t g_signalReceived = 0;

static void	handleShutdownSignal(int sig)
{
	g_signalReceived = sig;
}

void	setupSignalHandlers()
{
	signal(SIGINT, handleShutdownSignal);
	signal(SIGTERM, handleShutdownSignal);
	signal(SIGQUIT, handleShutdownSignal);
	signal(SIGPIPE, SIG_IGN);
}
