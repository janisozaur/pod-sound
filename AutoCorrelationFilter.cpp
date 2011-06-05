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

bool AutoCorrelationFilter::setup(const FilterData &/*data*/)
{
	AutoCorrelationSetupDialog dialog(q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
	if (dialog.exec() != QDialog::Accepted) {
		return false;
	}
	//int windowSize = dialog.windowSize();
	return true;
}

DisplayWindow *AutoCorrelationFilter::apply(QString /*windowBaseName*/)
{
	qDebug() << "parent" << parent();
	qDebug() << "parents parent" << parent()->parent();
	return new DisplayWindow(q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}
