#pragma once

#include "timer.h"
#include <map>

#define MAXTIMER 1024

#define COUNT_DOWN 1000

#define inc(k) if(k < MAXTIMER) k = k + 1; else k = 0;

class clock_ctl : public QObject
{
	Q_OBJECT

public:
	clock_ctl();
	/* 返回下一个可用的timer编号，无可用返回-1 */
	int search_for_next_valid();
	bool alloc_timer(quint16 n_count);
	bool delete_timer(quint16 n_count);
	bool check_timer(quint16 n_count);

private:
	Timer* timerList[MAXTIMER];
	bool is_used[MAXTIMER];
	/* 指示当前查找未使用的timer的起点 */
	int cur_pointer;
	/* n_count <-> timer_seq */
	std::map<quint16, int> n_count_timerseq_table;
	const int TIMEOUT = 600;

private slots:
	void alert_clock(quint16 count);
};