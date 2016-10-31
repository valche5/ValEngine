#include "MainWindow.h"

#include <QSettings>

#include "GLWidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	readSettings();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent * event)
{
	QSettings settings("ValCompany", "ValRenderer");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(event);
}

void MainWindow::readSettings()
{
	QSettings settings("ValCompany", "ValRenderer");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
}
