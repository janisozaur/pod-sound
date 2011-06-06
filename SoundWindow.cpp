#include "SoundWindow.h"
#include "FilterInterface.h"
#include "AutoCorrelationFilter.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>

#include <QDebug>

Q_DECLARE_METATYPE(QUuid)

SoundWindow::SoundWindow(QString fileName, QString title, QWidget *parent) :
	DisplayWindow(parent)
{
	constructorInternals(title);
	mWavDecoder = new WavDecoder(this);
	bool opened = mWavDecoder->open(fileName);
	qDebug() << opened;
}

SoundWindow::~SoundWindow()
{
	delete mWavDecoder;
}

void SoundWindow::constructorInternals(QString title)
{
	setWindowTitle(title);

	mFiltersMenu = menuBar()->addMenu("Filters");

	appendFilter(new AutoCorrelationFilter(this));

	connect(mFiltersMenu, SIGNAL(triggered(QAction*)), this, SLOT(applyFilter(QAction*)));
}

void SoundWindow::applyFilter(QAction *action)
{
	FilterInterface *filter = mFiltersHash[action->data().value<QUuid>()];
	qDebug() << "filter name:" << filter->name();
	FilterData fd;
	fd.wav = mWavDecoder;
	if (filter->setup(fd)) {
		DisplayWindow *dw = filter->apply(windowTitle());
		q_check_ptr(dw)->show();
	}
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
