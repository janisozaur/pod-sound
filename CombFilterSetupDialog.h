#ifndef COMBFILTERSETUPDIALOG_H
#define COMBFILTERSETUPDIALOG_H

#include <QDialog>

class QSpinBox;
class QDoubleSpinBox;

class CombFilterSetupDialog : public QDialog
{
	Q_OBJECT
public:
	CombFilterSetupDialog(int max, double fmax, QWidget *parent = 0);
	int windowSize() const;
	double start() const;
	double stop() const;
	double step() const;

signals:

public slots:

private:
	QSpinBox *mWindowSizeSpinBox;
	QDoubleSpinBox *mStartSpinBox;
	QDoubleSpinBox *mStopSpinBox;
	QDoubleSpinBox *mStepSpinBox;
};

#endif // COMBFILTERSETUPDIALOG_H
