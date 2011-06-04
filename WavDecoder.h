#ifndef WAVDECODER_H
#define WAVDECODER_H

#include <QObject>
#include <QIODevice>

class WavDecoder : public QObject
{
	Q_OBJECT
public:
	WavDecoder(QObject *parent = 0);
	bool open(QIODevice *dev);
	bool open(QString fileName);

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
};

#endif // WAVDECODER_H
