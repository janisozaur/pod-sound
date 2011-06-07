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
	mWindowSize = dialog.windowSize();
	mWav = data.wav;
	mStart = dialog.startM();
	return true;
}

DisplayWindow *AutoCorrelationFilter::apply(QString windowBaseName)
{
	qDebug() << "parent" << parent();
	qDebug() << "parents parent" << parent()->parent();
	const int windows = (mWav.samplesCount() + mWindowSize - 1) / mWindowSize;
	qreal phase = 0;
	QStringList fList;
	for (int window = 0; window < windows; window++) {
		qreal maxM = -INFINITY;
		for (int channel = 0; channel < mSamples.size(); channel++) {
			int m;
			double max = -INFINITY;
			for (m = mStart; m < mWindowSize; m++) {
				double sum = 0;
				int offset = window * mWindowSize;
				for (int i = 0; i < mWindowSize; i++) {
					int idx1 = qMin(mWav.samplesCount(), i + offset);
					int idx2 = qMin(mWav.samplesCount(), (i + m) % mWindowSize + offset);
					sum += mSamples.at(channel).at(idx1) * mSamples.at(channel).at(idx2);
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
		fList << QString::number(f) + "Hz";
		phase = mWav.generateSine(window * mWindowSize, mWindowSize, f, phase);
		qDebug() << "window" << window << "of" << windows << "(" <<
					(int)((qreal)window * 100 / (qreal) windows) <<
					"%).";
	}
	QString fString(fList.join(", "));
	qDebug() << fString;
	QWidget *newParent = q_check_ptr(qobject_cast<QWidget *>(parent()->parent()));
	return new SoundWindow(mWav, fString, windowBaseName + ", " + name(),
						   newParent);
}
