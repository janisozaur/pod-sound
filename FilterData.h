#ifndef FILTERDATA_H
#define FILTERDATA_H

#include "ComplexArray.h"

#include <QImage>
#include <QVector>

// this class is POD

class FilterData
{
public:
	ComplexArray transformData;
	QVector<QVector<qint16> > samples;
};


#endif // FILTERDATA_H