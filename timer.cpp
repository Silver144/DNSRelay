#include "timer.h"

Timer::Timer(quint16 cnt, std::int32_t num) : seq(num), count(cnt)
{
	connect(this, &Timer::timeout, this, &Timer::handle_timeout);
}

void Timer::handle_timeout()
{
	emit c_timeout(count);
}
