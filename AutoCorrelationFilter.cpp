#include "AutoCorrelationFilter.h"
#include "AutoCorrelationSetupDialog.h"
#include "SoundWindow.h"

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
	mWav = data.wav;
	mStart = dialog.startM();
	return true;
}

DisplayWindow *AutoCorrelationFilter::apply(QString windowBaseName)
{
	qDebug() << "parent" << parent();
	qDebug() << "parents parent" << parent()->parent();
	qreal maxM = -INFINITY;
	for (int channel = 0; channel < mSamples.size(); channel++) {
		int m;
		double max = -INFINITY;
		const int N = mSamples.at(channel).size();
		for (m = mStart; m < N; m++) {
			double sum = 0;
			for (int i = 0; i < N; i++) {
				sum += mSamples.at(channel).at(i) * mSamples.at(channel).at((i + m) % N);
			}
			qDebug() << "sum:" << sum << "max:" << max;
			if (sum > max) {
				max = sum;
			} else {
				break;
			}
		}
		qDebug() << "m for channel" << channel << ":" << m;
		if (m > maxM) {
			maxM = m;
		}
	}
	qreal f = (qreal)mWav.sampleRate() / maxM;
	mWav.generateSine(0, mWav.samplesCount(), f, 0);
	QString fString(QString::number(f) + "Hz");
	QWidget *newParent = q_check_ptr(qobject_cast<QWidget *>(parent()->parent()));
	return new SoundWindow(mWav, fString, windowBaseName + ", " + name(),
						   newParent);
}
