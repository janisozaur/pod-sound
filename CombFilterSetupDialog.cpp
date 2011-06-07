#include "CombFilterSetupDialog.h"

#include <QVBoxLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDoubleSpinBox>

CombFilterSetupDialog::CombFilterSetupDialog(int max, double fmax,
											 QWidget *parent) :
	QDialog(parent)
{
	QVBoxLayout *vbl = new QVBoxLayout(this);
	QWidget *inputWidget = new QWidget(this);
	QFormLayout *form = new QFormLayout(inputWidget);

	mWindowSizeSpinBox = new QSpinBox(inputWidget);
	mWindowSizeSpinBox->setMaximum(max);
	mWindowSizeSpinBox->setMinimum(1);
	mWindowSizeSpinBox->setValue(qMin(4096, mWindowSizeSpinBox->maximum()));
	form->addRow("Window size", mWindowSizeSpinBox);

	mStartSpinBox = new QDoubleSpinBox(inputWidget);
	mStartSpinBox->setMaximum(fmax);
	mStartSpinBox->setMinimum(1);
	mStartSpinBox->setValue(1);
	form->addRow("Starting frequency", mStartSpinBox);

	mStopSpinBox = new QDoubleSpinBox(inputWidget);
	mStopSpinBox->setMaximum(fmax);
	mStopSpinBox->setMinimum(1);
	mStopSpinBox->setValue(qMin(2000.0, mStopSpinBox->maximum()));
	form->addRow("Ending frequency", mStopSpinBox);

	mStepSpinBox = new QDoubleSpinBox(inputWidget);
	mStepSpinBox->setMaximum(fmax);
	mStepSpinBox->setMinimum(1);
	mStepSpinBox->setValue(1);
	form->addRow("Frequency step", mStepSpinBox);

	vbl->addWidget(inputWidget);
	QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(dbb, SIGNAL(accepted()), this, SLOT(accept()));
	connect(dbb, SIGNAL(rejected()), this, SLOT(reject()));
	vbl->addWidget(inputWidget);
	vbl->addWidget(dbb);
}

int CombFilterSetupDialog::windowSize() const
{
	return mWindowSizeSpinBox->value();
}

double CombFilterSetupDialog::start() const
{
	return mStartSpinBox->value();
}

double CombFilterSetupDialog::stop() const
{
	return mStopSpinBox->value();
}

double CombFilterSetupDialog::step() const
{
	return mStepSpinBox->value();
}
