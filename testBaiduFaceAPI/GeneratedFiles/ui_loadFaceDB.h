/********************************************************************************
** Form generated from reading UI file 'loadFaceDB.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADFACEDB_H
#define UI_LOADFACEDB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *openDir;
    QLineEdit *dirName;
    QLabel *label_2;
    QLineEdit *groupName;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(400, 120);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(Dialog);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        openDir = new QPushButton(Dialog);
        openDir->setObjectName(QStringLiteral("openDir"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/Resources/fileexplorer.png"), QSize(), QIcon::Normal, QIcon::Off);
        openDir->setIcon(icon);

        gridLayout->addWidget(openDir, 1, 2, 1, 1);

        dirName = new QLineEdit(Dialog);
        dirName->setObjectName(QStringLiteral("dirName"));

        gridLayout->addWidget(dirName, 1, 1, 1, 1);

        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        groupName = new QLineEdit(Dialog);
        groupName->setObjectName(QStringLiteral("groupName"));

        gridLayout->addWidget(groupName, 3, 1, 1, 1);

        okButton = new QPushButton(Dialog);
        okButton->setObjectName(QStringLiteral("okButton"));

        gridLayout->addWidget(okButton, 0, 3, 1, 1);

        cancelButton = new QPushButton(Dialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        gridLayout->addWidget(cancelButton, 1, 3, 1, 1);


        retranslateUi(Dialog);
        QObject::connect(okButton, SIGNAL(clicked()), Dialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), Dialog, SLOT(reject()));
        QObject::connect(openDir, SIGNAL(clicked()), Dialog, SLOT(openDir()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("Dialog", "\345\272\223\346\226\207\344\273\266\345\244\271", Q_NULLPTR));
        openDir->setText(QString());
        label_2->setText(QApplication::translate("Dialog", "\347\276\244/\347\273\204\345\220\215\357\274\232", Q_NULLPTR));
        okButton->setText(QApplication::translate("Dialog", "\347\241\256\345\256\232", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("Dialog", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADFACEDB_H
