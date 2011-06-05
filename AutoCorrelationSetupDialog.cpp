#include "AutoCorrelationSetupDialog.h"

#include <QVBoxLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>

AutoCorrelationSetupDialog::AutoCorrelationSetupDialog(QWidget *parent) :
	QDialog(parent)
{
	QVBoxLayout *vbl = new QVBoxLayout(this);
	QWidget *inputWidget = new QWidget(this);
	QFormLayout *form = new QFormLayout(inputWidget);
	mWindowSizeSpinBox = new QSpinBox(inputWidget);
	mWindowSizeSpinBox->setMaximum(4096);
	mWindowSizeSpinBox->setMinimum(1);
	mWindowSizeSpinBox->setValue(mWindowSizeSpinBox->maximum());
	form->addRow("Window size", mWindowSizeSpinBox);
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
