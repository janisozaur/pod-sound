#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "WavDecoder.h"

#include <QFileDialog>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
	QString fileName(QFileDialog::getOpenFileName(this, "Select WAV file", QDir::currentPath(), "WAV files (*.wav)"));
	if (!fileName.isEmpty()) {
		WavDecoder dec(this);
		bool open = dec.open(fileName);
		qDebug() << open;
	}
}
