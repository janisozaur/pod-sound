#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SoundWindow.h"

#include <QFileDialog>
#include <QFileInfo>

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
		QFileInfo fi(fileName);
		SoundWindow *s = new SoundWindow(fileName, fi.fileName(), this);
		s->show();
	}
}
