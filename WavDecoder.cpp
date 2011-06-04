#include "WavDecoder.h"

#include <QDataStream>
#include <QMessageBox>
#include <QFile>

WavDecoder::WavDecoder(QObject *parent) :
	QObject(parent)
{
}

bool WavDecoder::open(QIODevice *dev)
{
	try {
		if (!dev->isOpen()) {
			throw QString("input device is not open");
		}
		if (!dev->isReadable()) {
			throw QString("input device is not readable");
		}
		QDataStream stream(dev);
		stream.setByteOrder(QDataStream::LittleEndian);
		if (string(&stream, 4) != "RIFF") {
			throw QString("input is not a RIFF file");
		}
		quint32 chunkSize;
		stream >> chunkSize;
		if (string(&stream, 4) != "WAVE") {
			throw QString("input is not a WAVE file");
		}
		if (string(&stream, 4) != "fmt ") {
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
		if ((mByteRate != (mSampleRate * mNumChannels * mBitsPerSample / 8)) ||
			(mBlockAlign != (mNumChannels * mBitsPerSample / 8)))
		{
			throw QString("some header fields don't match");
		}
		if (string(&stream, 4) != "data") {
			throw QString("the data chunk is invalid");
		}
		stream >> chunkSize;
		mNumSamples = chunkSize / ((mBitsPerSample / 8) * mNumChannels);
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

QString WavDecoder::string(QDataStream *stream, int length) const
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
