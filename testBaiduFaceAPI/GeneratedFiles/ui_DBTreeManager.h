/********************************************************************************
** Form generated from reading UI file 'DBTreeManager.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBTREEMANAGER_H
#define UI_DBTREEMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DBManager
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_1;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit_3;
    QTreeWidget *filetree;
    QProgressBar *syncProgress;

    void setupUi(QWidget *DBManager)
    {
        if (DBManager->objectName().isEmpty())
            DBManager->setObjectName(QStringLiteral("DBManager"));
        DBManager->resize(320, 533);
        verticalLayout = new QVBoxLayout(DBManager);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout_1 = new QGridLayout();
        gridLayout_1->setObjectName(QStringLiteral("gridLayout_1"));
        label = new QLabel(DBManager);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_1->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(DBManager);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_1->addWidget(label_2, 1, 0, 1, 1);

        pushButton = new QPushButton(DBManager);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/Resources/fileexplorer.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon);

        gridLayout_1->addWidget(pushButton, 0, 2, 1, 1);

        lineEdit_2 = new QLineEdit(DBManager);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        gridLayout_1->addWidget(lineEdit_2, 1, 1, 1, 1);

        lineEdit = new QLineEdit(DBManager);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout_1->addWidget(lineEdit, 0, 1, 1, 1);

        label_3 = new QLabel(DBManager);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_1->addWidget(label_3, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(DBManager);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/Resources/approval.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon1);

        gridLayout_1->addWidget(pushButton_2, 2, 2, 1, 1);

        lineEdit_3 = new QLineEdit(DBManager);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        gridLayout_1->addWidget(lineEdit_3, 2, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_1);

        filetree = new QTreeWidget(DBManager);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("\346\226\207\344\273\266\345\210\227\350\241\250"));
        filetree->setHeaderItem(__qtreewidgetitem);
        filetree->setObjectName(QStringLiteral("filetree"));
        filetree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        filetree->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        verticalLayout->addWidget(filetree);

        syncProgress = new QProgressBar(DBManager);
        syncProgress->setObjectName(QStringLiteral("syncProgress"));
        syncProgress->setMaximum(100);
        syncProgress->setValue(100);
        syncProgress->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        syncProgress->setInvertedAppearance(false);
        syncProgress->setTextDirection(QProgressBar::TopToBottom);

        verticalLayout->addWidget(syncProgress);


        retranslateUi(DBManager);

        QMetaObject::connectSlotsByName(DBManager);
    } // setupUi

    void retranslateUi(QWidget *DBManager)
    {
        DBManager->setWindowTitle(QApplication::translate("DBManager", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("DBManager", "\346\226\207\344\273\266\345\244\271", Q_NULLPTR));
        label_2->setText(QApplication::translate("DBManager", "\347\276\244\347\273\204", Q_NULLPTR));
        pushButton->setText(QString());
        label_3->setText(QApplication::translate("DBManager", "\347\224\250\346\210\267", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("DBManager", "\346\237\245\346\211\276", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = filetree->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("DBManager", "\346\240\207\350\257\206\347\254\246", Q_NULLPTR));
        ___qtreewidgetitem->setText(1, QApplication::translate("DBManager", "\347\261\273\345\236\213", Q_NULLPTR));
        syncProgress->setFormat(QApplication::translate("DBManager", "\345\220\214\346\255\245-%p%", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DBManager: public Ui_DBManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBTREEMANAGER_H
