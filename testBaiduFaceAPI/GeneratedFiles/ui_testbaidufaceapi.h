/********************************************************************************
** Form generated from reading UI file 'testbaidufaceapi.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTBAIDUFACEAPI_H
#define UI_TESTBAIDUFACEAPI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "DBTreeManager.h"

QT_BEGIN_NAMESPACE

class Ui_testBaiduFaceAPIClass
{
public:
    QAction *actionOpen_File;
    QAction *actionOpenDir;
    QAction *actionsaveAllImages;
    QAction *actionsaveAllText;
    QAction *actionexit;
    QAction *actionFaceSetting;
    QAction *actiondefault_setting;
    QAction *actionsave_setting;
    QAction *actionrecent_files;
    QAction *actionnew_repo;
    QAction *actionopen_test;
    QAction *actioncloseDB;
    QAction *actionlock;
    QAction *actionunlock;
    QAction *actionuseradd;
    QAction *actionuserdel;
    QAction *actiongroupadd;
    QAction *actiongroupdel;
    QAction *actionuserrename;
    QAction *actionmanage;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGraphicsView *pictureView;
    QTextEdit *FaceDetail;
    DBTreeManager *filetree;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *menusetting;
    QMenu *menuface_repo;
    QMenu *menuuser;
    QMenu *menugroup;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *testBaiduFaceAPIClass)
    {
        if (testBaiduFaceAPIClass->objectName().isEmpty())
            testBaiduFaceAPIClass->setObjectName(QStringLiteral("testBaiduFaceAPIClass"));
        testBaiduFaceAPIClass->resize(666, 555);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(testBaiduFaceAPIClass->sizePolicy().hasHeightForWidth());
        testBaiduFaceAPIClass->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/testBaiduFaceAPI/Resources/finder.png"), QSize(), QIcon::Normal, QIcon::Off);
        testBaiduFaceAPIClass->setWindowIcon(icon);
        actionOpen_File = new QAction(testBaiduFaceAPIClass);
        actionOpen_File->setObjectName(QStringLiteral("actionOpen_File"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/Resources/file.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_File->setIcon(icon1);
        actionOpenDir = new QAction(testBaiduFaceAPIClass);
        actionOpenDir->setObjectName(QStringLiteral("actionOpenDir"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/Resources/fileexplorer.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenDir->setIcon(icon2);
        actionsaveAllImages = new QAction(testBaiduFaceAPIClass);
        actionsaveAllImages->setObjectName(QStringLiteral("actionsaveAllImages"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/Resources/savepicture.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionsaveAllImages->setIcon(icon3);
        actionsaveAllText = new QAction(testBaiduFaceAPIClass);
        actionsaveAllText->setObjectName(QStringLiteral("actionsaveAllText"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/Resources/savetext.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionsaveAllText->setIcon(icon4);
        actionexit = new QAction(testBaiduFaceAPIClass);
        actionexit->setObjectName(QStringLiteral("actionexit"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/Resources/warning15.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionexit->setIcon(icon5);
        actionFaceSetting = new QAction(testBaiduFaceAPIClass);
        actionFaceSetting->setObjectName(QStringLiteral("actionFaceSetting"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/Resources/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFaceSetting->setIcon(icon6);
        actiondefault_setting = new QAction(testBaiduFaceAPIClass);
        actiondefault_setting->setObjectName(QStringLiteral("actiondefault_setting"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/Resources/defaultsetting.png"), QSize(), QIcon::Normal, QIcon::Off);
        actiondefault_setting->setIcon(icon7);
        actionsave_setting = new QAction(testBaiduFaceAPIClass);
        actionsave_setting->setObjectName(QStringLiteral("actionsave_setting"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/Resources/savesetting.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionsave_setting->setIcon(icon8);
        actionrecent_files = new QAction(testBaiduFaceAPIClass);
        actionrecent_files->setObjectName(QStringLiteral("actionrecent_files"));
        actionnew_repo = new QAction(testBaiduFaceAPIClass);
        actionnew_repo->setObjectName(QStringLiteral("actionnew_repo"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/Resources/database17.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionnew_repo->setIcon(icon9);
        actionopen_test = new QAction(testBaiduFaceAPIClass);
        actionopen_test->setObjectName(QStringLiteral("actionopen_test"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/Resources/database20.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionopen_test->setIcon(icon10);
        actioncloseDB = new QAction(testBaiduFaceAPIClass);
        actioncloseDB->setObjectName(QStringLiteral("actioncloseDB"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icons/Resources/database21.png"), QSize(), QIcon::Normal, QIcon::Off);
        actioncloseDB->setIcon(icon11);
        actionlock = new QAction(testBaiduFaceAPIClass);
        actionlock->setObjectName(QStringLiteral("actionlock"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/icons/Resources/database19.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionlock->setIcon(icon12);
        actionunlock = new QAction(testBaiduFaceAPIClass);
        actionunlock->setObjectName(QStringLiteral("actionunlock"));
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/icons/Resources/database16.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionunlock->setIcon(icon13);
        actionuseradd = new QAction(testBaiduFaceAPIClass);
        actionuseradd->setObjectName(QStringLiteral("actionuseradd"));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/icons/Resources/increase.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionuseradd->setIcon(icon14);
        actionuserdel = new QAction(testBaiduFaceAPIClass);
        actionuserdel->setObjectName(QStringLiteral("actionuserdel"));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/icons/Resources/Delete .png"), QSize(), QIcon::Normal, QIcon::Off);
        actionuserdel->setIcon(icon15);
        actiongroupadd = new QAction(testBaiduFaceAPIClass);
        actiongroupadd->setObjectName(QStringLiteral("actiongroupadd"));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/icons/Resources/plus77.png"), QSize(), QIcon::Normal, QIcon::Off);
        actiongroupadd->setIcon(icon16);
        actiongroupdel = new QAction(testBaiduFaceAPIClass);
        actiongroupdel->setObjectName(QStringLiteral("actiongroupdel"));
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/icons/Resources/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actiongroupdel->setIcon(icon17);
        actionuserrename = new QAction(testBaiduFaceAPIClass);
        actionuserrename->setObjectName(QStringLiteral("actionuserrename"));
        actionmanage = new QAction(testBaiduFaceAPIClass);
        actionmanage->setObjectName(QStringLiteral("actionmanage"));
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/icons/Resources/database15.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionmanage->setIcon(icon18);
        centralWidget = new QWidget(testBaiduFaceAPIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pictureView = new QGraphicsView(centralWidget);
        pictureView->setObjectName(QStringLiteral("pictureView"));

        gridLayout->addWidget(pictureView, 0, 1, 1, 1);

        FaceDetail = new QTextEdit(centralWidget);
        FaceDetail->setObjectName(QStringLiteral("FaceDetail"));
        FaceDetail->setReadOnly(true);

        gridLayout->addWidget(FaceDetail, 1, 1, 1, 1);

        filetree = new DBTreeManager(centralWidget);
        filetree->setObjectName(QStringLiteral("filetree"));

        gridLayout->addWidget(filetree, 0, 0, 2, 1);

        gridLayout->setRowStretch(0, 3);
        gridLayout->setColumnStretch(0, 1);
        testBaiduFaceAPIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(testBaiduFaceAPIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 666, 26));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(menuBar->sizePolicy().hasHeightForWidth());
        menuBar->setSizePolicy(sizePolicy1);
        fileMenu = new QMenu(menuBar);
        fileMenu->setObjectName(QStringLiteral("fileMenu"));
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/icons/Resources/fileexplorer (1).png"), QSize(), QIcon::Normal, QIcon::Off);
        fileMenu->setIcon(icon19);
        menusetting = new QMenu(menuBar);
        menusetting->setObjectName(QStringLiteral("menusetting"));
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/icons/Resources/settings (1).png"), QSize(), QIcon::Normal, QIcon::Off);
        menusetting->setIcon(icon20);
        menuface_repo = new QMenu(menuBar);
        menuface_repo->setObjectName(QStringLiteral("menuface_repo"));
        QIcon icon21;
        icon21.addFile(QStringLiteral(":/icons/Resources/database18.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuface_repo->setIcon(icon21);
        menuuser = new QMenu(menuBar);
        menuuser->setObjectName(QStringLiteral("menuuser"));
        QIcon icon22;
        icon22.addFile(QStringLiteral(":/icons/Resources/user.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuuser->setIcon(icon22);
        menugroup = new QMenu(menuBar);
        menugroup->setObjectName(QStringLiteral("menugroup"));
        QIcon icon23;
        icon23.addFile(QStringLiteral(":/icons/Resources/group.png"), QSize(), QIcon::Normal, QIcon::Off);
        menugroup->setIcon(icon23);
        testBaiduFaceAPIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(testBaiduFaceAPIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(mainToolBar->sizePolicy().hasHeightForWidth());
        mainToolBar->setSizePolicy(sizePolicy2);
        testBaiduFaceAPIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(testBaiduFaceAPIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        sizePolicy.setHeightForWidth(statusBar->sizePolicy().hasHeightForWidth());
        statusBar->setSizePolicy(sizePolicy);
        testBaiduFaceAPIClass->setStatusBar(statusBar);

        menuBar->addAction(fileMenu->menuAction());
        menuBar->addAction(menusetting->menuAction());
        menuBar->addAction(menuface_repo->menuAction());
        menuBar->addAction(menuuser->menuAction());
        menuBar->addAction(menugroup->menuAction());
        fileMenu->addAction(actionOpen_File);
        fileMenu->addAction(actionOpenDir);
        fileMenu->addSeparator();
        fileMenu->addAction(actionsaveAllImages);
        fileMenu->addAction(actionsaveAllText);
        fileMenu->addSeparator();
        fileMenu->addAction(actionexit);
        menusetting->addAction(actionFaceSetting);
        menusetting->addAction(actiondefault_setting);
        menusetting->addAction(actionsave_setting);
        menuface_repo->addAction(actionnew_repo);
        menuface_repo->addAction(actionopen_test);
        menuface_repo->addSeparator();
        menuface_repo->addAction(actionmanage);
        menuface_repo->addAction(actionlock);
        menuface_repo->addAction(actionunlock);
        menuface_repo->addAction(actioncloseDB);
        menuuser->addAction(actionuseradd);
        menuuser->addAction(actionuserdel);
        menugroup->addAction(actiongroupadd);
        menugroup->addAction(actiongroupdel);
        mainToolBar->addSeparator();

        retranslateUi(testBaiduFaceAPIClass);
        QObject::connect(actionOpen_File, SIGNAL(triggered()), testBaiduFaceAPIClass, SLOT(FileOpen()));
        QObject::connect(actionsaveAllText, SIGNAL(triggered()), testBaiduFaceAPIClass, SLOT(TextSave()));
        QObject::connect(actionsaveAllImages, SIGNAL(triggered()), testBaiduFaceAPIClass, SLOT(ImageSave()));
        QObject::connect(actionOpenDir, SIGNAL(triggered()), testBaiduFaceAPIClass, SLOT(DirOpen()));
        QObject::connect(actionFaceSetting, SIGNAL(triggered()), testBaiduFaceAPIClass, SLOT(openFaceSetting()));
        QObject::connect(actiondefault_setting, SIGNAL(triggered()), testBaiduFaceAPIClass, SLOT(readSetting()));
        QObject::connect(actionsave_setting, SIGNAL(triggered()), testBaiduFaceAPIClass, SLOT(saveSetting()));
        QObject::connect(actionnew_repo, SIGNAL(triggered()), testBaiduFaceAPIClass, SLOT(LoadFaceDB()));
        QObject::connect(actionmanage, SIGNAL(triggered()), testBaiduFaceAPIClass, SLOT(manageMode()));
        QObject::connect(actiongroupdel, SIGNAL(triggered()), testBaiduFaceAPIClass, SLOT(groupDelete()));

        QMetaObject::connectSlotsByName(testBaiduFaceAPIClass);
    } // setupUi

    void retranslateUi(QMainWindow *testBaiduFaceAPIClass)
    {
        testBaiduFaceAPIClass->setWindowTitle(QApplication::translate("testBaiduFaceAPIClass", "testBaiduFaceAPI", Q_NULLPTR));
        actionOpen_File->setText(QApplication::translate("testBaiduFaceAPIClass", "\346\211\223\345\274\200\346\226\207\344\273\266", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionOpen_File->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\346\211\223\345\274\200\346\226\207\344\273\266img\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionOpen_File->setShortcut(QApplication::translate("testBaiduFaceAPIClass", "O", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionOpenDir->setText(QApplication::translate("testBaiduFaceAPIClass", "\346\211\223\345\274\200\346\226\207\344\273\266\345\244\271", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionOpenDir->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\346\211\223\345\274\200\346\226\207\344\273\266\345\244\271\344\270\213\347\232\204jpg,png\345\233\276\347\211\207", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionOpenDir->setShortcut(QApplication::translate("testBaiduFaceAPIClass", "D", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionsaveAllImages->setText(QApplication::translate("testBaiduFaceAPIClass", "\344\277\235\345\255\230\345\275\223\345\211\215\345\233\276\345\203\217", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionsaveAllImages->setShortcut(QApplication::translate("testBaiduFaceAPIClass", "Ctrl+S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionsaveAllText->setText(QApplication::translate("testBaiduFaceAPIClass", "\344\277\235\345\255\230\345\233\276\345\203\217\350\257\246\346\203\205", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionsaveAllText->setShortcut(QApplication::translate("testBaiduFaceAPIClass", "Ctrl+T", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionexit->setText(QApplication::translate("testBaiduFaceAPIClass", "\351\200\200\345\207\272", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionexit->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\351\200\200\345\207\272\347\250\213\345\272\217", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionexit->setShortcut(QApplication::translate("testBaiduFaceAPIClass", "Ctrl+F4", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionFaceSetting->setText(QApplication::translate("testBaiduFaceAPIClass", "\344\272\272\350\204\270\350\257\206\345\210\253\350\256\276\347\275\256", Q_NULLPTR));
        actionFaceSetting->setIconText(QApplication::translate("testBaiduFaceAPIClass", "\344\272\272\350\204\270\350\257\206\345\210\253\350\256\276\347\275\256", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionFaceSetting->setShortcut(QApplication::translate("testBaiduFaceAPIClass", "Shift+F", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actiondefault_setting->setText(QApplication::translate("testBaiduFaceAPIClass", "\351\273\230\350\256\244\350\256\276\347\275\256", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actiondefault_setting->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\350\277\224\345\233\236\345\210\260\351\273\230\350\256\244\350\256\276\347\275\256", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actiondefault_setting->setShortcut(QApplication::translate("testBaiduFaceAPIClass", "Shift+D", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionsave_setting->setText(QApplication::translate("testBaiduFaceAPIClass", "\344\277\235\345\255\230\350\256\276\347\275\256", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionsave_setting->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\344\277\235\345\255\230\345\275\223\345\211\215\350\256\276\347\275\256", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionsave_setting->setShortcut(QApplication::translate("testBaiduFaceAPIClass", "Ctrl+Shift+S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionrecent_files->setText(QApplication::translate("testBaiduFaceAPIClass", "recent files", Q_NULLPTR));
        actionnew_repo->setText(QApplication::translate("testBaiduFaceAPIClass", "\346\226\260\344\272\272\347\261\273\345\272\223", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionnew_repo->setShortcut(QApplication::translate("testBaiduFaceAPIClass", "Ctrl+R", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionopen_test->setText(QApplication::translate("testBaiduFaceAPIClass", "\346\211\223\345\274\200\346\265\213\350\257\225", Q_NULLPTR));
        actioncloseDB->setText(QApplication::translate("testBaiduFaceAPIClass", "\345\205\263\351\227\255", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actioncloseDB->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\345\205\263\351\227\255\344\272\272\350\204\270\345\272\223", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionlock->setText(QApplication::translate("testBaiduFaceAPIClass", "\351\224\201\345\256\232", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionlock->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\351\224\201\345\256\232\344\272\272\350\204\270\345\272\223\357\274\214\347\246\201\346\255\242\344\277\256\346\224\271\343\200\202", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionunlock->setText(QApplication::translate("testBaiduFaceAPIClass", "\350\247\243\351\224\201", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionunlock->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\350\247\243\351\224\201\344\272\272\350\204\270\345\272\223\357\274\214\345\205\201\350\256\270\344\277\256\346\224\271\343\200\202", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionuseradd->setText(QApplication::translate("testBaiduFaceAPIClass", "\346\226\260\345\242\236", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionuseradd->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\345\242\236\345\212\240\347\224\250\346\210\267", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionuserdel->setText(QApplication::translate("testBaiduFaceAPIClass", "\345\210\240\351\231\244", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionuserdel->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\345\210\240\351\231\244\347\224\250\346\210\267", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actiongroupadd->setText(QApplication::translate("testBaiduFaceAPIClass", "\346\226\260\345\242\236", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actiongroupadd->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\346\226\260\345\242\236\347\276\244\347\273\204", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actiongroupdel->setText(QApplication::translate("testBaiduFaceAPIClass", "\345\210\240\351\231\244", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actiongroupdel->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\345\210\240\351\231\244\347\276\244\347\273\204", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionuserrename->setText(QApplication::translate("testBaiduFaceAPIClass", "userrename", Q_NULLPTR));
        actionmanage->setText(QApplication::translate("testBaiduFaceAPIClass", "\347\256\241\347\220\206", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionmanage->setToolTip(QApplication::translate("testBaiduFaceAPIClass", "\347\256\241\347\220\206\344\272\272\350\204\270\345\272\223", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        FaceDetail->setHtml(QApplication::translate("testBaiduFaceAPIClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\344\272\272\350\204\270\347\273\206\350\212\202\344\277\241\346\201\257</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", Q_NULLPTR));
        fileMenu->setTitle(QApplication::translate("testBaiduFaceAPIClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menusetting->setTitle(QApplication::translate("testBaiduFaceAPIClass", "\350\256\276\347\275\256", Q_NULLPTR));
        menuface_repo->setTitle(QApplication::translate("testBaiduFaceAPIClass", "\344\272\272\350\204\270\345\272\223", Q_NULLPTR));
        menuuser->setTitle(QApplication::translate("testBaiduFaceAPIClass", "user", Q_NULLPTR));
        menugroup->setTitle(QApplication::translate("testBaiduFaceAPIClass", "\347\276\244/\347\273\204", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class testBaiduFaceAPIClass: public Ui_testBaiduFaceAPIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTBAIDUFACEAPI_H
