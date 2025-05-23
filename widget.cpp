#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QMetaType>
// 操作符实现
QT_BEGIN_NAMESPACE
QDataStream &operator<<(QDataStream &out, const AccountRecord &v) {
    return out << v.isIncome << v.amount << v.date << v.note;
}
QDataStream &operator>>(QDataStream &in, AccountRecord &v) {
    return in >> v.isIncome >> v.amount >> v.date >> v.note;
}
QT_END_NAMESPACE
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
 qRegisterMetaType<AccountRecord>("AccountRecord");
    // ================= 初始化UI =================
    // 设置日期控件默认值
    ui->dateEdit->setDate(QDate::currentDate());
    ui->startDateEdit->setDate(QDate::currentDate().addMonths(-1));
    ui->endDateEdit->setDate(QDate::currentDate());

    // 配置表格
    QStringList headers = {"序号", "类型", "金额", "日期", "备注"};
    ui->recordTable->setColumnCount(5);
    ui->recordTable->setHorizontalHeaderLabels(headers);
    ui->recordTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch); // 备注列自动拉伸



    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // ================= 信号槽连接 =================
    connect(ui->clearFilterButton, &QPushButton::clicked, [this](){
        filteredRecords = allRecords;
        refreshTable();
    });
}

Widget::~Widget()
{
    delete ui;
}

// ================= 核心功能实现 =================

// 收入按钮点击
void Widget::on_incomeButton_clicked()
{
    if(!validateInput()) return;

    AccountRecord record;
    record.isIncome = true;
    record.amount = ui->amountEdit->text().toDouble();
    record.date = ui->dateEdit->date();
    record.note = ui->noteEdit->text().trimmed();

    allRecords.append(record);
    filteredRecords = allRecords;
    refreshTable();
    clearInputs();
}

// 支出按钮点击（与收入处理逻辑类似）
void Widget::on_expenseButton_clicked()
{
    if(!validateInput()) return;

    AccountRecord record;
    record.isIncome = false; // 唯一区别
    record.amount = ui->amountEdit->text().toDouble();
    record.date = ui->dateEdit->date();
    record.note = ui->noteEdit->text().trimmed();

    allRecords.append(record);
    filteredRecords = allRecords;
    refreshTable();
    clearInputs();
}

// 筛选按钮点击
void Widget::on_filterButton_clicked()
{
    QDate start = ui->startDateEdit->date();
    QDate end = ui->endDateEdit->date();

    filteredRecords.clear();
    for(const AccountRecord &record : allRecords){
        if(record.date >= start && record.date <= end){
            filteredRecords.append(record);
        }
    }
    refreshTable();
}

// 刷新表格显示
void Widget::refreshTable()
{
    ui->recordTable->setRowCount(0); // 清空表格

    double totalIncome = 0, totalExpense = 0;

    for(int i = 0; i < filteredRecords.size(); ++i){
        const AccountRecord &record = filteredRecords[i];

        // 统计计算
        if(record.isIncome) totalIncome += record.amount;
        else totalExpense += record.amount;

        // 插入行
        int row = ui->recordTable->rowCount();
        ui->recordTable->insertRow(row);

        // 填充数据
        ui->recordTable->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
        ui->recordTable->setItem(row, 1, new QTableWidgetItem(record.isIncome ? "收入" : "支出"));
        ui->recordTable->setItem(row, 2, new QTableWidgetItem(QString::number(record.amount, 'f', 2)));
        ui->recordTable->setItem(row, 3, new QTableWidgetItem(record.date.toString("yyyy-MM-dd")));
        ui->recordTable->setItem(row, 4, new QTableWidgetItem(record.note));
    }

    updateSummary();
}

// 更新统计信息
void Widget::updateSummary()
{
    double totalIncome = 0, totalExpense = 0;

    for(const AccountRecord &record : filteredRecords){
        if(record.isIncome) totalIncome += record.amount;
        else totalExpense += record.amount;
    }

    ui->totalIncomeLabel->setText(QString::number(totalIncome, 'f', 2));
    ui->totalExpenseLabel->setText(QString::number(totalExpense, 'f', 2));
    ui->balanceLabel->setText(QString::number(totalIncome - totalExpense, 'f', 2));
}

// 清空输入框
void Widget::clearInputs()
{
    ui->amountEdit->clear();
    ui->noteEdit->clear();
    ui->dateEdit->setDate(QDate::currentDate());
}

// 输入验证
bool Widget::validateInput()
{
    if(ui->amountEdit->text().isEmpty()){
        QMessageBox::warning(this, "输入错误", "金额不能为空");
        return false;
    }

    bool ok;
    double amount = ui->amountEdit->text().toDouble(&ok);
    if(!ok || amount <= 0){
        QMessageBox::warning(this, "输入错误", "请输入有效的金额");
        return false;
    }

    return true;
}

// ================= 文件操作 =================

// 保存数据
void Widget::on_saveButton_clicked()
{
    QFile file("records.dat");
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this, "错误", "无法创建保存文件");
        return;
    }

    QDataStream out(&file);
    out << allRecords; // 利用QDataStream序列化

    QMessageBox::information(this, "保存成功", "数据已保存到 records.dat");
}

// 加载数据
void Widget::on_loadButton_clicked()
{
    QFile file("records.dat");
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, "错误", "找不到数据文件");
        return;
    }

    QDataStream in(&file);
    in >> allRecords;

    filteredRecords = allRecords;
    refreshTable();
    QMessageBox::information(this, "加载成功", QString("已加载%1条记录").arg(allRecords.size()));
}
