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
	qDebug() << "size:" << size;
	complexData.reserve(size);
	for (int i = 0; i < size; i++) {
		complexData << Complex(mWav.samples().at(0).at(i), 0);
	}
	complexData.resize(4096);
	fft.rearrange(complexData);
	fft.transform(complexData, false);
	QStringList str;
	for (int i = 0; i < complexData.size(); i++) {
		str << QString::number(complexData.at(i).real());
	}
	qDebug() << str.join(",");
	return new DisplayWindow(q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));;
}
