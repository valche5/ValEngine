#include "MainWindow.h"

#include <QSettings>

#include "GLWidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	readSettings();

	connect(ui.actionOuvrir, &QAction::triggered, ui.widget, &GLWidget::openScene);
	connect(ui.actionFermer, &QAction::triggered, ui.widget, &GLWidget::closeScene);
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
