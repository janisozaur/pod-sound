#ifndef SOUNDWINDOW_H
#define SOUNDWINDOW_H

#include "DisplayWindow.h"
#include "FilterInterface.h"
#include "WavDecoder.h"

#include <QUuid>
#include <QHash>

class SoundWindow : public DisplayWindow
{
	Q_OBJECT
public:
	explicit SoundWindow(QString fileName, QString title, QWidget *parent = 0);
	virtual ~SoundWindow();

signals:

public slots:
	void applyFilter(QAction *action);

private:
	void appendFilter(FilterInterface *filter);
	void constructorInternals(QString title);

	QHash<QUuid, FilterInterface *> mFiltersHash;
	QMenu *mFiltersMenu;
	WavDecoder *mWavDecoder;
};

#endif // SOUNDWINDOW_H
