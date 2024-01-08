/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QLabel *mainTitleLabel;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *titleLabelAddress;
    QLabel *titleLabelLocal;
    QLabel *titleLabelPort;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QLabel *ipAddressLabel;
    QLabel *localAddressLabel;
    QLabel *portNumberLabel;
    QSpacerItem *horizontalSpacer_2;
    QTextBrowser *textBrowser;
    QLineEdit *lineEdit;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(640, 421);
        verticalLayout_3 = new QVBoxLayout(Dialog);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        mainTitleLabel = new QLabel(Dialog);
        mainTitleLabel->setObjectName(QString::fromUtf8("mainTitleLabel"));

        verticalLayout_3->addWidget(mainTitleLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        titleLabelAddress = new QLabel(Dialog);
        titleLabelAddress->setObjectName(QString::fromUtf8("titleLabelAddress"));

        verticalLayout_2->addWidget(titleLabelAddress);

        titleLabelLocal = new QLabel(Dialog);
        titleLabelLocal->setObjectName(QString::fromUtf8("titleLabelLocal"));

        verticalLayout_2->addWidget(titleLabelLocal);

        titleLabelPort = new QLabel(Dialog);
        titleLabelPort->setObjectName(QString::fromUtf8("titleLabelPort"));

        verticalLayout_2->addWidget(titleLabelPort);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ipAddressLabel = new QLabel(Dialog);
        ipAddressLabel->setObjectName(QString::fromUtf8("ipAddressLabel"));

        verticalLayout->addWidget(ipAddressLabel);

        localAddressLabel = new QLabel(Dialog);
        localAddressLabel->setObjectName(QString::fromUtf8("localAddressLabel"));

        verticalLayout->addWidget(localAddressLabel);

        portNumberLabel = new QLabel(Dialog);
        portNumberLabel->setObjectName(QString::fromUtf8("portNumberLabel"));

        verticalLayout->addWidget(portNumberLabel);


        horizontalLayout->addLayout(verticalLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout);

        textBrowser = new QTextBrowser(Dialog);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout_3->addWidget(textBrowser);

        lineEdit = new QLineEdit(Dialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout_3->addWidget(lineEdit);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
        mainTitleLabel->setText(QApplication::translate("Dialog", "C&C Server Module", nullptr));
        titleLabelAddress->setText(QApplication::translate("Dialog", "IP Address:", nullptr));
        titleLabelLocal->setText(QApplication::translate("Dialog", "LOCAL:", nullptr));
        titleLabelPort->setText(QApplication::translate("Dialog", "Port:", nullptr));
        ipAddressLabel->setText(QApplication::translate("Dialog", "TextLabel", nullptr));
        localAddressLabel->setText(QApplication::translate("Dialog", "TextLabel", nullptr));
        portNumberLabel->setText(QApplication::translate("Dialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
