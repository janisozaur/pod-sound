#ifndef FILTERINTERFACE_H
#define FILTERINTERFACE_H

#include "FilterData.h"

#include <QObject>
#include <QUuid>

class DisplayWindow;

class FilterInterface : public QObject
{
	Q_OBJECT
public:
	explicit FilterInterface(QObject *parent = 0);
	virtual ~FilterInterface();					// dummy
	virtual QString name() const = 0;
	QUuid uuid() const;

signals:

public slots:
	virtual bool setup(const FilterData &data) = 0;
	virtual DisplayWindow *apply(QString windowBaseName) = 0;

private:
	QUuid mUuid;
};

#endif // FILTERINTERFACE_H
