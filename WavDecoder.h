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
	void save(QIODevice *dev);
	void save(QString fileName);
	QVector<QVector<qint16> > samples() const;
	QVector<QVector<qint16> > &samples();
	quint16 channelCount() const;
	quint32 sampleRate() const;
	quint32 byteRate() const;
	quint16 blockAlign() const;
	quint16 bitsPerSample() const;
	int samplesCount() const;
	void setSamplesCount(int count);
	qreal generateSine(int start, int count, qreal freq, qreal phase);

signals:

public slots:

private:
	QString readString(QDataStream *stream, int length) const;
	void writeString(QDataStream *stream, QString string) const;
	quint16 mNumChannels;
	quint32 mSampleRate;
	quint32 mByteRate;
	quint16 mBlockAlign;
	quint16 mBitsPerSample;
	QVector<QVector<qint16> > mSamples;
};

#endif // WAVDECODER_H
