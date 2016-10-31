#ifndef ENGINETEST_H
#define ENGINETEST_H

#include <QtWidgets/QMainWindow>
#include "ui_enginetest.h"

class GLWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *event);
	void readSettings();
private:
	Ui::EngineTestClass ui;
};

#endif // ENGINETEST_H