#include "CombFilter.h"
#include "FFT.h"
#include "DisplayWindow.h"

#include <QStringList>

#include <QDebug>

CombFilter::CombFilter(QObject *parent) :
	FilterInterface(parent)
{
}

QString CombFilter::name() const
{
	return "Comb filter";
}

bool CombFilter::setup(const FilterData &data)
{
	mWav = data.wav;
	return true;
}

DisplayWindow *CombFilter::apply(QString windowBaseName)
{
	FFT fft;
	QVector<Complex> complexData;
	int size = mWav.samples().at(0).size();
	size = 4096;
	qDebug() << "size:" << size;
	complexData.reserve(size);
	for (int i = 0; i < size; i++) {
		complexData << Complex(mWav.samples().at(0).at(i), 0);
	}
	complexData.resize(4096);
	fft.rearrange(complexData);
	fft.transform(complexData, false);
	/*QStringList str;
	for (int i = 0; i < complexData.size(); i++) {
		str << QString::number(complexData.at(i).real());
	}
	qDebug() << str.join(",");*/
	QVector<qreal> realData;
	realData.reserve(complexData.size());
	for (int i = 0; i < size; i++) {
		realData << complexData.at(i).real();
	}
	qreal fStart = 1;
	qreal fStop = 10000;
	qreal fStep = 1;
	quint32 sr = mWav.sampleRate();
	qreal sumMax = -INFINITY;
	qreal fMax = -INFINITY;
	for (qreal f = fStart; f < fStop; f += fStep) {
		QVector<qreal> c = generateTriangle(size, f, sr);
		QVector<Complex> c2;
		c2.reserve(size);
		for (int i = 0; i < size; i++) {
			c2 << Complex(c.at(i), 0);
		}
		fft.rearrange(c2);
		fft.transform(c2, false);
		qreal sum = 0;
		for (int i = 0; i < size; i++) {
			sum += c2.at(i).real() * realData.at(i);
		}
		if (sum > sumMax) {
			sumMax = sum;
			fMax = f;
		}
	}
	qDebug() << "found f:" << fMax;
	return new DisplayWindow(q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));;
}

QVector<qreal> CombFilter::generateTriangle(int duration, qreal freq, qreal sampleRate)
{
	QVector<qreal> samples;
	const qreal omega = sampleRate / (freq * 2);
	//qDebug() << "omega:" << omega;
	//QStringList str;
	for (qreal i = 0; i < duration; i += 1) {
		qreal val;
		// http://en.wikipedia.org/wiki/Triangle_wave
		val = (2.0 / omega) * (i - omega * (qreal)((int)(i / omega + 0.5))) * (((int)(i / omega + 0.5)) % 2 == 1 ? -1 : 1);
		samples << val;
		//str << QString::number(val);
	}
	//qDebug() << str.join(",");
	return samples;
}
