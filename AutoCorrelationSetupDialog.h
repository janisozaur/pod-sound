#ifndef AUTOCORRELATIONSETUPDIALOG_H
#define AUTOCORRELATIONSETUPDIALOG_H

#include <QDialog>

class QSpinBox;
class QDoubleSpinBox;

class AutoCorrelationSetupDialog : public QDialog
{
	Q_OBJECT
public:
	AutoCorrelationSetupDialog(int max, QWidget *parent = 0);
	int windowSize() const;
	int startM() const;
	qreal comparionFactor() const;

signals:

public slots:

private:
	QSpinBox *mWindowSizeSpinBox;
	QSpinBox *mStartMSpinBox;
	QDoubleSpinBox *mComparisonFactorSpinBox;
};

#endif // AUTOCORRELATIONSETUPDIALOG_H
