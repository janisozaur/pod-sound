#ifndef FILTERDATA_H
#define FILTERDATA_H

#include "ComplexArray.h"
#include "WavDecoder.h"

#include <QImage>
#include <QVector>

// this class is POD

class FilterData
{
public:
	ComplexArray transformData;
	WavDecoder wav;
};


#endif // FILTERDATA_H
