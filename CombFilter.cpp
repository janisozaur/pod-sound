#include "CombFilter.h"
#include "FFT.h"
#include "SoundWindow.h"
#include "CombFilterSetupDialog.h"

#include <QStringList>
#include <QElapsedTimer>

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
	// parent's parent should be mainwindow
	QWidget *dialogParent = qobject_cast<QWidget *>(parent()->parent());
	CombFilterSetupDialog d(data.wav.samples().at(0).size(), data.wav.sampleRate(),
							dialogParent);
	if (d.exec() != QDialog::Accepted) {
		return false;
	}
	mWav = data.wav;
	mStart = d.start();
	mStop = d.stop();
	mStep = d.step();
	mWindowSize = d.windowSize();
	return true;
}

DisplayWindow *CombFilter::apply(QString windowBaseName)
{
	QVector<Complex> complexData;
	int size = mWindowSize;
	qDebug() << "size:" << size;
	complexData.reserve(size);
	const int windows = (mWav.samplesCount() + mWindowSize - 1) / mWindowSize;
	QStringList fList;
	qreal phase = 0;
	for (int window = 0; window < windows; window++) {
		complexData.resize(0);
		for (int i = 0; i < size; i++) {
			Complex c(mWav.samples().at(0).at(i + window * mWindowSize), 0);
			complexData << c;
		}
		{
			FFT fft;
			fft.rearrange(complexData);
			fft.transform(complexData, false);
		}
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
		const int steps = (mStop - mStart) / mStep;
		quint32 sr = mWav.sampleRate();
		qreal sumMax = -INFINITY;
		qreal fMax = -INFINITY;
		QElapsedTimer t;
		t.start();
		#pragma omp parallel for
		for (int w = 0; w < steps; w++) {
			qreal f = mStart + mStep * w;
			QVector<Complex> c = generateTriangle(size, f, sr);
			/*QVector<Complex> c2;
			c2.reserve(size);
			for (int i = 0; i < size; i++) {
				c2 << Complex(c.at(i), 0);
			}*/
			FFT fft;
			fft.rearrange(c);
			fft.transform(c, false);
			qreal sum = 0;
			for (int i = 0; i < size; i++) {
				sum += c.at(i).real() * realData.at(i);
			}
			if (sum > sumMax) {
				sumMax = sum;
				fMax = f;
			}
		}
		int msecs = t.elapsed();
		qDebug() << "window" << window << "of" << windows << "(" <<
					(int)((qreal)window * 100 / (qreal) windows) <<
					"%). time taken:" << msecs << "miliseconds";
		qDebug() << "found f:" << fMax;
		fList << QString::number(fMax) + "Hz";
		phase = mWav.generateSine(window * mWindowSize,
								  (window + 1) * mWindowSize, fMax, phase);
	}
	QWidget *newParent = q_check_ptr(qobject_cast<QWidget *>(parent()->parent()));
	QString fString(fList.join(", "));
	qDebug() << fString;
	return new SoundWindow(mWav, fString, windowBaseName + ", " + name(),
						   newParent);
}

QVector<Complex> CombFilter::generateTriangle(int duration, qreal freq, qreal sampleRate)
{
	QVector<Complex> samples;
	const qreal omega = sampleRate / (freq * 2);
	//qDebug() << "omega:" << omega;
	//QStringList str;
	for (qreal i = 0; i < duration; i += 1) {
		qreal val;
		// http://en.wikipedia.org/wiki/Triangle_wave
		val = (2.0 / omega) * (i - omega * (qreal)((int)(i / omega + 0.5))) * (((int)(i / omega + 0.5)) % 2 == 1 ? -1 : 1);
		samples << Complex(val, 0);
		//str << QString::number(val);
	}
	//qDebug() << str.join(",");
	return samples;
}
