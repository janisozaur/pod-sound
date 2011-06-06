#ifndef FFT_H
#define FFT_H

#include "Complex.h"

#include <QVector>

class FFT
{
public:
    FFT();
	void rearrange(QVector<Complex> &elements);
	void transform(QVector<Complex> &elements, bool inverse);
};

#endif // FFT_H
