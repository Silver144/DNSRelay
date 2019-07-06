#pragma once

#include <QTimer>

class Timer : public QTimer
{
	Q_OBJECT

public:
	explicit Timer(quint16 cnt, int num);
	int seq;
	quint16 count;

signals:
	void c_timeout(quint16);

public slots:
	void handle_timeout();
};
