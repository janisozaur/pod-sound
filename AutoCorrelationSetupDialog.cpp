#include "AutoCorrelationSetupDialog.h"

#include <QVBoxLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>

AutoCorrelationSetupDialog::AutoCorrelationSetupDialog(int max,
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

	mStartMSpinBox = new QSpinBox(inputWidget);
	mStartMSpinBox->setMaximum(max);
	mStartMSpinBox->setMinimum(1);
	mStartMSpinBox->setValue(qMin(10, mStartMSpinBox->maximum()));
	form->addRow("Starting m", mStartMSpinBox);

	mComparisonFactorSpinBox = new QDoubleSpinBox(inputWidget);
	mComparisonFactorSpinBox->setMaximum(2);
	mComparisonFactorSpinBox->setMinimum(0.01);
	mComparisonFactorSpinBox->setValue(1.05);
	form->addRow("Comparison factor", mComparisonFactorSpinBox);

	vbl->addWidget(inputWidget);
	QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(dbb, SIGNAL(accepted()), this, SLOT(accept()));
	connect(dbb, SIGNAL(rejected()), this, SLOT(reject()));
	vbl->addWidget(inputWidget);
	vbl->addWidget(dbb);
}

int AutoCorrelationSetupDialog::windowSize() const
{
	return mWindowSizeSpinBox->value();
}

int AutoCorrelationSetupDialog::startM() const
{
	return mStartMSpinBox->value();
}

qreal AutoCorrelationSetupDialog::comparionFactor() const
{
	return mComparisonFactorSpinBox->value();
}
