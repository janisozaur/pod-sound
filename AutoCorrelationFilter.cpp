#include "AutoCorrelationFilter.h"
#include "AutoCorrelationSetupDialog.h"
#include "SoundWindow.h"

#include <QVector>
#include <QElapsedTimer>

#include <QDebug>

AutoCorrelationFilter::AutoCorrelationFilter(QObject *parent) :
	FilterInterface(parent)
{
}

QString AutoCorrelationFilter::name() const
{
	return "Autocorrelation";
}

bool AutoCorrelationFilter::setup(const FilterData &data)
{
	// parent's parent should be mainwindow
	QWidget *dialogParent = qobject_cast<QWidget *>(parent()->parent());
	dialogParent = q_check_ptr(dialogParent);
	AutoCorrelationSetupDialog dialog(data.wav.samples().at(0).size(), dialogParent);
	if (dialog.exec() != QDialog::Accepted) {
		return false;
	}
	mSamples = data.wav.samples().mid(0, dialog.windowSize());
	mWindowSize = dialog.windowSize();
	mWav = data.wav;
	mStart = dialog.startM();
	mComparisonFactor = dialog.comparionFactor();
	return true;
}

DisplayWindow *AutoCorrelationFilter::apply(QString windowBaseName)
{
	qDebug() << "parent" << parent();
	qDebug() << "parents parent" << parent()->parent();
	const int windows = (mWav.samplesCount() + mWindowSize - 1) / mWindowSize;
	qreal phase = 0;
	QStringList fList;
	QVector<QString> fVector(windows);
	QElapsedTimer t;
	t.start();
	const int size = mWindowSize;
	#pragma omp parallel for
	for (int window = 0; window < windows; window++) {
		qreal maxM = -INFINITY;
		for (int channel = 0; channel < mSamples.size(); channel++) {
			bool crossed = false;
			int m, m2 = 0;
			double max = -INFINITY;
			for (m = mStart; m < size / 2; m++) {
				double sum = 0;
				int offset = window * size;
				for (int i = 0; i < size; i++) {
					int idx1 = (i + offset) % mWav.samplesCount();
					int idx2 = ((i + m) % size + offset) % mWav.samplesCount();
					qreal s1 = mSamples.at(channel).at(idx1);
					qreal s2 = mSamples.at(channel).at(idx2);
					s1 *= (0.54 - 0.46 * cos(2.0 * M_PI * idx1 / (mWindowSize - 1)));
					s2 *= (0.54 - 0.46 * cos(2.0 * M_PI * idx2 / (mWindowSize - 1)));
					sum += s1 * s2;
				}
				//qDebug() << "sum:" << sum << "max:" << max;
				if (!crossed && sum < 0) {
					crossed = true;
				}
				if (crossed && sum > max * mComparisonFactor) {
					max = sum;
					m2 = m;
				}
				//qDebug() << "m for channel" << m << ":" << sum / 100000;
				//qDebug() << sum / 100000;
			}
			if (m2 > maxM) {
				maxM = m2;
			}
		}
		if (maxM == 0) {
			qDebug() << "m for window" << window << "equals 0";
		} else {
			qDebug() << "m" << maxM;
			qreal f = (qreal)mWav.sampleRate() / maxM;
			fVector[window] = QString::number(f) + "Hz";
			phase = mWav.generateSine(window * size, size, f, phase);
		}

		/*int cnt;
		{
			cnt = ++done;
		}
		qDebug() << "window" << window << "(" << cnt << "of" << windows << ":" <<
					(int)((qreal)cnt * 100 / (qreal) windows) <<
					"%).";*/
	}
	int msecs = t.elapsed();
	qDebug() << "time taken:" << msecs << "miliseconds";
	fList = QStringList::fromVector(fVector);
	QString fString(fList.join(", "));
	qDebug() << fString;
	QWidget *newParent = q_check_ptr(qobject_cast<QWidget *>(parent()->parent()));
	return new SoundWindow(mWav, fString, windowBaseName + ", " + name(),
						   newParent);
}
