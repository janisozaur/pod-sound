#ifndef AUTOCORRELATIONFILTER_H
#define AUTOCORRELATIONFILTER_H

#include "FilterInterface.h"

class AutoCorrelationFilter : public FilterInterface
{
	Q_OBJECT
public:
	explicit AutoCorrelationFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

private:
	QVector<QVector<qint16> > mSamples;
	WavDecoder mWav;
	int mStart;
	int mWindowSize;
	qreal mComparisonFactor;
};

#endif // AUTOCORRELATIONFILTER_H
