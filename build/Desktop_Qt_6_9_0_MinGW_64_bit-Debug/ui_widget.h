/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLineEdit *amountEdit;
    QDateEdit *dateEdit;
    QLineEdit *noteEdit;
    QPushButton *incomeButton;
    QPushButton *expenseButton;
    QHBoxLayout *horizontalLayout_2;
    QDateEdit *startDateEdit;
    QLabel *label;
    QDateEdit *endDateEdit;
    QPushButton *filterButton;
    QPushButton *clearFilterButton;
    QTableWidget *recordTable;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QLabel *totalIncomeLabel;
    QLabel *label_6;
    QLabel *totalExpenseLabel;
    QLabel *label_4;
    QLabel *balanceLabel;
    QVBoxLayout *verticalLayout_2;
    QPushButton *loadButton;
    QPushButton *saveButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(585, 378);
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(-1, 5, -1, 5);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        amountEdit = new QLineEdit(Widget);
        amountEdit->setObjectName("amountEdit");

        horizontalLayout->addWidget(amountEdit);

        dateEdit = new QDateEdit(Widget);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setDate(QDate(2025, 5, 22));

        horizontalLayout->addWidget(dateEdit);

        noteEdit = new QLineEdit(Widget);
        noteEdit->setObjectName("noteEdit");

        horizontalLayout->addWidget(noteEdit);

        incomeButton = new QPushButton(Widget);
        incomeButton->setObjectName("incomeButton");

        horizontalLayout->addWidget(incomeButton);

        expenseButton = new QPushButton(Widget);
        expenseButton->setObjectName("expenseButton");

        horizontalLayout->addWidget(expenseButton);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(-1, 5, -1, 5);
        startDateEdit = new QDateEdit(Widget);
        startDateEdit->setObjectName("startDateEdit");
        startDateEdit->setDate(QDate(2025, 5, 22));

        horizontalLayout_2->addWidget(startDateEdit);

        label = new QLabel(Widget);
        label->setObjectName("label");

        horizontalLayout_2->addWidget(label);

        endDateEdit = new QDateEdit(Widget);
        endDateEdit->setObjectName("endDateEdit");
        endDateEdit->setDate(QDate(2025, 5, 22));

        horizontalLayout_2->addWidget(endDateEdit);

        filterButton = new QPushButton(Widget);
        filterButton->setObjectName("filterButton");

        horizontalLayout_2->addWidget(filterButton);

        clearFilterButton = new QPushButton(Widget);
        clearFilterButton->setObjectName("clearFilterButton");
        clearFilterButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);

        horizontalLayout_2->addWidget(clearFilterButton);

        recordTable = new QTableWidget(Widget);
        if (recordTable->columnCount() < 5)
            recordTable->setColumnCount(5);
        QFont font;
        font.setPointSize(11);
        font.setBold(false);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font);
        recordTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        recordTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        recordTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        recordTable->setObjectName("recordTable");
        recordTable->setMinimumSize(QSize(500, 600));
        recordTable->setRowCount(0);
        recordTable->setColumnCount(5);

        horizontalLayout_2->addWidget(recordTable);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(5);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(-1, 5, -1, 5);
        label_2 = new QLabel(Widget);
        label_2->setObjectName("label_2");

        horizontalLayout_5->addWidget(label_2);

        totalIncomeLabel = new QLabel(Widget);
        totalIncomeLabel->setObjectName("totalIncomeLabel");
        totalIncomeLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_5->addWidget(totalIncomeLabel);

        label_6 = new QLabel(Widget);
        label_6->setObjectName("label_6");

        horizontalLayout_5->addWidget(label_6);

        totalExpenseLabel = new QLabel(Widget);
        totalExpenseLabel->setObjectName("totalExpenseLabel");
        totalExpenseLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_5->addWidget(totalExpenseLabel);

        label_4 = new QLabel(Widget);
        label_4->setObjectName("label_4");

        horizontalLayout_5->addWidget(label_4);

        balanceLabel = new QLabel(Widget);
        balanceLabel->setObjectName("balanceLabel");
        balanceLabel->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        balanceLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_5->addWidget(balanceLabel);


        horizontalLayout_2->addLayout(horizontalLayout_5);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(5);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(-1, 5, -1, 5);
        loadButton = new QPushButton(Widget);
        loadButton->setObjectName("loadButton");

        verticalLayout_2->addWidget(loadButton);

        saveButton = new QPushButton(Widget);
        saveButton->setObjectName("saveButton");

        verticalLayout_2->addWidget(saveButton);


        horizontalLayout_2->addLayout(verticalLayout_2);


        horizontalLayout->addLayout(horizontalLayout_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        amountEdit->setPlaceholderText(QCoreApplication::translate("Widget", "\351\207\221\351\242\235", nullptr));
        noteEdit->setPlaceholderText(QCoreApplication::translate("Widget", "\345\244\207\346\263\250", nullptr));
        incomeButton->setText(QCoreApplication::translate("Widget", "\346\224\266\345\205\245", nullptr));
        expenseButton->setText(QCoreApplication::translate("Widget", "\346\224\257\345\207\272", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\350\207\263", nullptr));
        filterButton->setText(QCoreApplication::translate("Widget", "\347\255\233\351\200\211", nullptr));
        clearFilterButton->setText(QCoreApplication::translate("Widget", "\346\270\205\351\231\244\347\255\233\351\200\211", nullptr));
        QTableWidgetItem *___qtablewidgetitem = recordTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Widget", "\345\272\217\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = recordTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Widget", "\347\261\273\345\236\213", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = recordTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Widget", "\351\207\221\351\242\235", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\346\200\273\346\224\266\345\205\245\357\274\232", nullptr));
        totalIncomeLabel->setText(QCoreApplication::translate("Widget", "0.00", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", "\346\200\273\346\224\257\345\207\272\357\274\232", nullptr));
        totalExpenseLabel->setText(QCoreApplication::translate("Widget", "0.00", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\347\273\223\344\275\231\357\274\232", nullptr));
        balanceLabel->setText(QCoreApplication::translate("Widget", "0.00", nullptr));
        loadButton->setText(QCoreApplication::translate("Widget", "\345\212\240\350\275\275\346\225\260\346\215\256", nullptr));
        saveButton->setText(QCoreApplication::translate("Widget", "\344\277\235\345\255\230\346\225\260\346\215\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
