#include "FilterInterface.h"

FilterInterface::FilterInterface(QObject *parent) :
	QObject(parent),
	mUuid(QUuid::createUuid())
{
}

QUuid FilterInterface::uuid() const
{
	return mUuid;
}

FilterInterface::~FilterInterface()
{
}
