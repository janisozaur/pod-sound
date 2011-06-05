#ifndef WAVDECODER_H
#define WAVDECODER_H

#include <QObject>
#include <QIODevice>
#include <QVector>

class WavDecoder : public QObject
{
	Q_OBJECT
public:
	WavDecoder(QObject *parent = 0);
	bool open(QIODevice *dev);
	bool open(QString fileName);
	QVector<QVector<qint16> > samples() const;
	QVector<QVector<qint16> > &samples();

signals:

public slots:

private:
	QString string(QDataStream *stream, int length) const;
	quint16 mNumChannels;
	quint32 mSampleRate;
	quint32 mByteRate;
	quint16 mBlockAlign;
	quint16 mBitsPerSample;
	unsigned int mNumSamples;
	QVector<QVector<qint16> > mSamples;
};

#endif // WAVDECODER_H
