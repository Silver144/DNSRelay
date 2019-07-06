#include "clock_ctl.h"

clock_ctl::clock_ctl()
{
	n_count_timerseq_table.clear();
	memset(is_used, 0, sizeof(is_used));
	memset(timerList, 0, sizeof(timerList));
	cur_pointer = 0;
}

int clock_ctl::search_for_next_valid()
{
	int counter = 0;
	while (counter < MAXTIMER)
	{
		if (!is_used[counter])
		{
			is_used[counter] = true;
			return counter;
		}
		counter++;
	}
	return -1;
}

bool clock_ctl::alloc_timer(quint16 n_count)
{
	int timer_seq = search_for_next_valid();
	if (timer_seq == -1)
		return false;
	n_count_timerseq_table.insert(std::map<quint16, int>::value_type(n_count, timer_seq));
	timerList[timer_seq] = new Timer(n_count, timer_seq);
	timerList[timer_seq]->setSingleShot(true);
	timerList[timer_seq]->start(COUNT_DOWN);
	connect(timerList[timer_seq], SIGNAL(c_timeout(quint16)), this, SLOT(alert_clock(quint16)));
	return true;
}

bool clock_ctl::check_timer(quint16 n_count)
{
	return n_count_timerseq_table.find(n_count) != n_count_timerseq_table.end();
}

void clock_ctl::alert_clock(quint16 count)
{
	printf("0x%04x packet timeout\n", count);
	delete_timer(count);
}

bool clock_ctl::delete_timer(quint16 n_count)
{
	int timer_seq = n_count_timerseq_table[n_count];
	if (timerList[timer_seq] == Q_NULLPTR)
		return false;
	delete timerList[timer_seq];
	timerList[timer_seq] = Q_NULLPTR;
	return n_count_timerseq_table.erase(n_count);
}