#ifndef AUTOCORRELATIONSETUPDIALOG_H
#define AUTOCORRELATIONSETUPDIALOG_H

#include <QDialog>

class QSpinBox;

class AutoCorrelationSetupDialog : public QDialog
{
	Q_OBJECT
public:
	AutoCorrelationSetupDialog(QWidget *parent = 0);
	int windowSize() const;

signals:

public slots:

private:
	QSpinBox *mWindowSizeSpinBox;
};

#endif // AUTOCORRELATIONSETUPDIALOG_H
