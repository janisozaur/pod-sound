#ifndef COMBFILTER_H
#define COMBFILTER_H

#include "FilterInterface.h"

class CombFilter : public FilterInterface
{
	Q_OBJECT
public:
	explicit CombFilter(QObject *parent = 0);
	virtual QString name() const;
	QVector<Complex> generateTriangle(int duration, qreal freq, qreal sampleRate);

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

private:
	WavDecoder mWav;
	qreal mStart;
	qreal mStop;
	qreal mStep;
	int mWindowSize;
};

#endif // COMBFILTER_H
