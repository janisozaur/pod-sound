#include "SoundWindow.h"
#include "FilterInterface.h"
#include "AutoCorrelationFilter.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>

#include <QDebug>

Q_DECLARE_METATYPE(QUuid)

SoundWindow::SoundWindow(QString fileName, QWidget *parent) :
	DisplayWindow(parent)
{
	constructorInternals();
	mWavDecoder = new WavDecoder(this);
	bool opened = mWavDecoder->open(fileName);
	qDebug() << opened;
}

SoundWindow::~SoundWindow()
{
	delete mWavDecoder;
}

void SoundWindow::constructorInternals()
{
	mFiltersMenu = menuBar()->addMenu("Filters");
	appendFilter(new AutoCorrelationFilter(this));
}

void SoundWindow::appendFilter(FilterInterface *filter)
{

	mFiltersHash.insert(filter->uuid(), filter);
	QAction *menuAction = new QAction(filter->name(), this);
	QVariant v;
	v.setValue(filter->uuid());
	menuAction->setData(v);
	mFiltersMenu->addAction(menuAction);
}
