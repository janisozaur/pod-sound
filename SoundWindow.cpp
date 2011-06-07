#include "SoundWindow.h"
#include "FilterInterface.h"
#include "AutoCorrelationFilter.h"
#include "CombFilter.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QLabel>

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

SoundWindow::SoundWindow(WavDecoder wav, QString display, QString title,
						 QWidget *parent) :
	DisplayWindow(parent)
{
	constructorInternals(title);
	mWavDecoder = new WavDecoder(wav);
	mWavDecoder->setParent(this);
	mDisplayLabel->setText(display);
}

SoundWindow::~SoundWindow()
{
	delete mWavDecoder;
}

void SoundWindow::constructorInternals(QString title)
{
	setWindowTitle(title);

	QMenu *fileMenu = menuBar()->addMenu("File");

	QAction *saveAction = new QAction("Save", this);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	fileMenu->addAction(saveAction);

	QAction *playAction = new QAction("Play", this);
	connect(playAction, SIGNAL(triggered()), this, SLOT(play()));
	fileMenu->addAction(playAction);

	mDisplayLabel = new QLabel(this);
	this->setCentralWidget(mDisplayLabel);

	mFiltersMenu = menuBar()->addMenu("Filters");

	appendFilter(new AutoCorrelationFilter(this));
	appendFilter(new CombFilter(this));

	connect(mFiltersMenu, SIGNAL(triggered(QAction*)), this, SLOT(applyFilter(QAction*)));
}

void SoundWindow::applyFilter(QAction *action)
{
	FilterInterface *filter = mFiltersHash[action->data().value<QUuid>()];
	qDebug() << "filter name:" << filter->name();
	FilterData fd;
	fd.wav = *mWavDecoder;
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

void SoundWindow::save()
{
	QString fileName(QFileDialog::getSaveFileName(this, "Where do you want to save WAV", QDir::currentPath(), "WAV file (*.wav)"));
	if (!fileName.isEmpty()) {
		mWavDecoder->save(fileName);
	}
}

void SoundWindow::play()
{
	mWavDecoder->play();
}
