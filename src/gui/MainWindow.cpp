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
	connect(ui.actionCentrer, &QAction::triggered, [this]() { ui.widget->centerScene();  });
	connect(ui.actionFront, &QAction::triggered, [this]() { ui.widget->centerScene(glm::vec3(0, 0, -1));  });
	connect(ui.actionRight, &QAction::triggered, [this]() { ui.widget->centerScene(glm::vec3(-1, 0, 0));  });
	connect(ui.actionLeft, &QAction::triggered, [this]() { ui.widget->centerScene(glm::vec3(1, 0, 0));  });
	connect(ui.actionTop, &QAction::triggered, [this]() { ui.widget->centerScene(glm::vec3(0, -1, 0));  });
	connect(ui.actionBottom, &QAction::triggered, [this]() { ui.widget->centerScene(glm::vec3(0, 1, 0));  });
	connect(ui.actiobBack, &QAction::triggered, [this]() { ui.widget->centerScene(glm::vec3(0, 0, 1));  });
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
