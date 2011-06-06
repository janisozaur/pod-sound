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
	WavDecoder(const WavDecoder &other);
	WavDecoder &operator=(const WavDecoder &other);
	bool open(QIODevice *dev);
	bool open(QString fileName);
	QVector<QVector<qint16> > samples() const;
	QVector<QVector<qint16> > &samples();
	quint16 channelCount() const;
	quint32 sampleRate() const;
	quint32 byteRate() const;
	quint16 blockAlign() const;
	quint16 bitsPerSample() const;
	unsigned int samplesCount() const;

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
