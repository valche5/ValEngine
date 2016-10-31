/********************************************************************************
** Form generated from reading UI file 'enginetest.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENGINETEST_H
#define UI_ENGINETEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_EngineTestClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    GLWidget *widget;
    QMenuBar *menuBar;
    QMenu *menuFichier;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EngineTestClass)
    {
        if (EngineTestClass->objectName().isEmpty())
            EngineTestClass->setObjectName(QStringLiteral("EngineTestClass"));
        EngineTestClass->resize(800, 641);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EngineTestClass->sizePolicy().hasHeightForWidth());
        EngineTestClass->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(EngineTestClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new GLWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(800, 600));
        widget->setMaximumSize(QSize(800, 600));

        horizontalLayout->addWidget(widget);

        EngineTestClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EngineTestClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        menuFichier = new QMenu(menuBar);
        menuFichier->setObjectName(QStringLiteral("menuFichier"));
        EngineTestClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(EngineTestClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EngineTestClass->setStatusBar(statusBar);

        menuBar->addAction(menuFichier->menuAction());

        retranslateUi(EngineTestClass);

        QMetaObject::connectSlotsByName(EngineTestClass);
    } // setupUi

    void retranslateUi(QMainWindow *EngineTestClass)
    {
        EngineTestClass->setWindowTitle(QApplication::translate("EngineTestClass", "EngineTest", 0));
        menuFichier->setTitle(QApplication::translate("EngineTestClass", "Fichier", 0));
    } // retranslateUi

};

namespace Ui {
    class EngineTestClass: public Ui_EngineTestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENGINETEST_H
