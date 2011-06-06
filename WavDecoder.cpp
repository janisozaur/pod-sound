#include "WavDecoder.h"

#include <QDataStream>
#include <QMessageBox>
#include <QFile>

WavDecoder::WavDecoder(QObject *parent) :
	QObject(parent)
{
}

WavDecoder::WavDecoder(const WavDecoder &other) :
	QObject(NULL),
	mNumChannels(other.mNumChannels),
	mSampleRate(other.mSampleRate),
	mByteRate(other.mByteRate),
	mBlockAlign(other.mBlockAlign),
	mBitsPerSample(other.mBitsPerSample),
	mNumSamples(other.mNumSamples),
	mSamples(other.mSamples)
{
}

WavDecoder &WavDecoder::operator=(const WavDecoder &other)
{
	this->mNumChannels = other.mNumChannels;
	this->mSampleRate = other.mSampleRate;
	this->mByteRate = other.mByteRate;
	this->mBlockAlign = other.mBlockAlign;
	this->mBitsPerSample = other.mBitsPerSample;
	this->mNumSamples = other.mNumSamples;
	this->mSamples = other.mSamples;
	return *this;
}

bool WavDecoder::open(QIODevice *dev)
{
	try {
		if (!dev->isOpen()) {
			throw QString("input device is not opened");
		}
		if (!dev->isReadable()) {
			throw QString("input device is not readable");
		}
		QDataStream stream(dev);
		stream.setByteOrder(QDataStream::LittleEndian);
		if (readString(&stream, 4) != "RIFF") {
			throw QString("input is not a RIFF file");
		}
		quint32 chunkSize;
		stream >> chunkSize;
		if (readString(&stream, 4) != "WAVE") {
			throw QString("input is not a WAVE file");
		}
		if (readString(&stream, 4) != "fmt ") {
			throw QString("fmt chunk is missing, malformed or misplaced");
		}
		stream >> chunkSize;
		if (chunkSize != 16) {
			throw QString("input is not PCM");
		}
		quint16 audioFormat;
		stream >> audioFormat;
		if (audioFormat != 1) {
			throw QString("input is compressed");
		}
		stream >> mNumChannels;
		stream >> mSampleRate;
		stream >> mByteRate;
		stream >> mBlockAlign;
		stream >> mBitsPerSample;
		if (mBitsPerSample > 16) {
			throw QString("this program only supports files with up to 16 bits per sample");
		}
		if ((mByteRate != (mSampleRate * mNumChannels * mBitsPerSample / 8)) ||
			(mBlockAlign != (mNumChannels * mBitsPerSample / 8)))
		{
			throw QString("some header fields don't match");
		}
		while (readString(&stream, 4) != "data") {
			stream >> chunkSize;
			stream.skipRawData(chunkSize);
		}
		stream >> chunkSize;
		mNumSamples = chunkSize / ((mBitsPerSample / 8) * mNumChannels);
		mSamples.resize(mNumChannels);
		for (int i = 0; i < mNumChannels; i++) {
			mSamples[i].reserve(mNumSamples);
		}
		for (unsigned int sample = 0; sample < mNumSamples; sample++) {
			for (quint16 channel = 0; channel < mNumChannels; channel++) {
				qint16 data;
				stream >> data;
				mSamples[channel] << data;
			}
		}
	} catch (QString &e) {
		QMessageBox::critical(qobject_cast<QWidget *>(this), "Error with WAV decoding", "There was an error with decoding the file: " + e);
		return false;
	}
	return true;
}

bool WavDecoder::open(QString fileName)
{
	QFile f(fileName);
	f.open(QIODevice::ReadOnly);
	bool result = open(&f);
	f.close();
	return result;
}

QVector<QVector<qint16> > WavDecoder::samples() const
{
	return mSamples;
}

QVector<QVector<qint16> > &WavDecoder::samples()
{
	return mSamples;
}

quint16 WavDecoder::channelCount() const
{
	return mNumChannels;
}

quint32 WavDecoder::sampleRate() const
{
	return mSampleRate;
}

quint32 WavDecoder::byteRate() const
{
	return mByteRate;
}

quint16 WavDecoder::blockAlign() const
{
	return mBlockAlign;
}

quint16 WavDecoder::bitsPerSample() const
{
	return mBitsPerSample;
}

unsigned int WavDecoder::samplesCount() const
{
	return mNumSamples;
}

QString WavDecoder::readString(QDataStream *stream, int length) const
{
	char *buf = new char[length + 1];
	int bytesRead = stream->readRawData(buf, length);
	if (bytesRead != length) {
		delete [] buf;
		return QString();
	}
	buf[length] = '\0';
	QString result(buf);
	delete [] buf;
	return result;
}
