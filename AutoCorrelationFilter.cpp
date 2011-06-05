#include "AutoCorrelationFilter.h"
#include "AutoCorrelationSetupDialog.h"
#include "DisplayWindow.h"

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
	AutoCorrelationSetupDialog dialog(data.samples.at(0).size(), dialogParent);
	if (dialog.exec() != QDialog::Accepted) {
		return false;
	}
	mSamples = data.samples.mid(0, dialog.windowSize());
	mStart = dialog.startM();
	return true;
}

DisplayWindow *AutoCorrelationFilter::apply(QString /*windowBaseName*/)
{
	qDebug() << "parent" << parent();
	qDebug() << "parents parent" << parent()->parent();
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
	}
	return new DisplayWindow(q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}
