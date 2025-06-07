#include "widget.h"         // 包含主窗口头文件
#include "ui_widget.h"      // 包含UI设计器生成的头文件（自动生成，无需手动编写）
#include <QFile>          // 文件操作类
#include <QTextStream>    // 文本流类（用于文件读写）
#include <QMessageBox>    // 消息框类（用于提示用户）
#include <QDebug>         // 调试输出类（用于开发阶段打印信息）
#include <QMetaType>      // 元类型系统（注册自定义类型）

// ================= 操作符重载实现（用于文件序列化） =================
// 将AccountRecord对象写入数据流（二进制格式）
QT_BEGIN_NAMESPACE
QDataStream &operator<<(QDataStream &out, const AccountRecord &v) {
    // 按顺序写入结构体成员（必须与读取顺序一致！）
    return out << v.isIncome << v.amount << v.date << v.note;
}

// 从数据流读取AccountRecord对象（二进制格式）
QDataStream &operator>>(QDataStream &in, AccountRecord &v) {
    // 按顺序读取结构体成员（必须与写入顺序一致！）
    return in >> v.isIncome >> v.amount >> v.date >> v.note;
}
QT_END_NAMESPACE

// ================= 构造函数（初始化界面和数据） =================
Widget::Widget(QWidget *parent)
    : QWidget(parent)          // 调用父类构造函数
    , ui(new Ui::Widget)       // 创建UI对象
{
    ui->setupUi(this);         // 初始化UI（从widget.ui文件加载界面布局）

    // 注册自定义类型（否则信号槽无法传递AccountRecord对象）
    qRegisterMetaType<AccountRecord>("AccountRecord");

    // ================= UI初始化 =================
    // 设置日期选择框的默认值
    ui->dateEdit->setDate(QDate::currentDate());       // 当前日期
    ui->startDateEdit->setDate(QDate::currentDate().addMonths(-1)); // 一个月前
    ui->endDateEdit->setDate(QDate::currentDate());     // 当前日期

    // 配置表格控件（QTableWidget）
    QStringList headers = {"序号", "类型", "金额", "日期", "备注"};
    ui->recordTable->setColumnCount(5);                // 设置列数
    ui->recordTable->setHorizontalHeaderLabels(headers);// 设置表头文字
    // 让最后一列（备注）自动拉伸填充剩余空间
    ui->recordTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    // 设置窗口大小策略为“可扩展”（窗口可随内容自动缩放）
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // ================= 信号槽连接（手动连接） =================
    // 清除筛选按钮点击时，恢复所有记录并刷新表格
    // 使用Lambda表达式简化代码（C++11特性，需在.pro文件中添加CONFIG += c++17）
    connect(ui->clearFilterButton, &QPushButton::clicked, [this](){
        filteredRecords = allRecords; // 筛选记录 = 所有记录
        refreshTable();                // 刷新表格显示
    });
}

// 析构函数（释放UI资源）
Widget::~Widget()
{
    delete ui; // 释放UI设计器生成的对象（自动生成的组件会被递归删除）
}

// ================= 核心功能：收入按钮处理 =================
void Widget::on_incomeButton_clicked()
{
    if(!validateInput()) return; // 验证输入，失败则返回

    AccountRecord record;         // 创建收支记录对象
    record.isIncome = true;       // 标记为收入
    record.amount = ui->amountEdit->text().toDouble(); // 转换金额（字符串→double）
    record.date = ui->dateEdit->date(); // 获取日期
    record.note = ui->noteEdit->text().trimmed(); // 获取备注并去除首尾空格

    allRecords.append(record);     // 添加到所有记录列表
    filteredRecords = allRecords;  // 更新筛选列表（显示全部）
    refreshTable();                // 刷新表格
    clearInputs();                 // 清空输入框
}

// ================= 核心功能：支出按钮处理（逻辑与收入类似） =================
void Widget::on_expenseButton_clicked()
{
    if(!validateInput()) return;

    AccountRecord record;
    record.isIncome = false;      // 标记为支出
    record.amount = ui->amountEdit->text().toDouble();
    record.date = ui->dateEdit->date();
    record.note = ui->noteEdit->text().trimmed();

    allRecords.append(record);
    filteredRecords = allRecords;
    refreshTable();
    clearInputs();
}

// ================= 核心功能：筛选按钮处理 =================
void Widget::on_filterButton_clicked()
{
    QDate start = ui->startDateEdit->date(); // 获取开始日期
    QDate end = ui->endDateEdit->date();     // 获取结束日期

    filteredRecords.clear(); // 清空筛选列表

    // 遍历所有记录，筛选出日期在[start, end]之间的记录
    for(const AccountRecord &record : allRecords){
        if(record.date >= start && record.date <= end){
            filteredRecords.append(record); // 添加到筛选列表
        }
    }
    refreshTable(); // 刷新表格显示筛选结果
}

// ================= 核心功能：刷新表格显示 =================
void Widget::refreshTable()
{
    ui->recordTable->setRowCount(0); // 清空表格所有行

    double totalIncome = 0, totalExpense = 0; // 统计变量

    // 遍历筛选后的记录，填充表格并计算总计
    for(int i = 0; i < filteredRecords.size(); ++i){
        const AccountRecord &record = filteredRecords[i];

        // 统计收入和支出总额
        if(record.isIncome) totalIncome += record.amount;
        else totalExpense += record.amount;

        // 在表格中插入新行（从0开始计数）
        int row = ui->recordTable->rowCount(); // 获取当前行数（即新行索引）
        ui->recordTable->insertRow(row);       // 插入空行

        // 填充每行数据（QTableWidgetItem是表格中的单元格）
        ui->recordTable->setItem(row, 0, new QTableWidgetItem(QString::number(row+1))); // 序号
        ui->recordTable->setItem(row, 1, new QTableWidgetItem(record.isIncome ? "收入" : "支出")); // 类型
        // 'f'表示固定小数点格式，2表示保留两位小数
        ui->recordTable->setItem(row, 2, new QTableWidgetItem(QString::number(record.amount, 'f', 2))); // 金额
        ui->recordTable->setItem(row, 3, new QTableWidgetItem(record.date.toString("yyyy-MM-dd"))); // 日期格式化为字符串
        ui->recordTable->setItem(row, 4, new QTableWidgetItem(record.note)); // 备注
    }

    updateSummary(); // 更新底部的统计信息（总收入/支出/余额）
}

// ================= 核心功能：更新统计信息 =================
void Widget::updateSummary()
{
    double totalIncome = 0, totalExpense = 0;

    // 重新计算筛选后的总计（避免依赖缓存数据）
    for(const AccountRecord &record : filteredRecords){
        if(record.isIncome) totalIncome += record.amount;
        else totalExpense += record.amount;
    }

    // 更新界面上的标签文本（显示两位小数）
    ui->totalIncomeLabel->setText(QString::number(totalIncome, 'f', 2));
    ui->totalExpenseLabel->setText(QString::number(totalExpense, 'f', 2));
    ui->balanceLabel->setText(QString::number(totalIncome - totalExpense, 'f', 2));
}

// ================= 辅助功能：清空输入框 =================
void Widget::clearInputs()
{
    ui->amountEdit->clear();     // 清空金额输入框
    ui->noteEdit->clear();       // 清空备注输入框
    ui->dateEdit->setDate(QDate::currentDate()); // 恢复日期为当前日期
}

// ================= 辅助功能：输入验证 =================
bool Widget::validateInput()
{
    // 检查金额是否为空
    if(ui->amountEdit->text().isEmpty()){
        // 显示警告消息框（父窗口为this，即主窗口）
        QMessageBox::warning(this, "输入错误", "金额不能为空");
        return false; // 验证失败
    }

    // 尝试将字符串转换为double
    bool ok;
    double amount = ui->amountEdit->text().toDouble(&ok); // ok参数接收转换是否成功
    if(!ok || amount <= 0){ // 转换失败或金额≤0
        QMessageBox::warning(this, "输入错误", "请输入有效的金额（>0）");
        return false;
    }

    return true; // 验证通过
}

// ================= 文件操作：保存数据到文件 =================
void Widget::on_saveButton_clicked()
{
    QFile file("records.dat"); // 创建文件对象（文件名：records.dat）
    // 以二进制写入模式打开文件（QIODevice::WriteOnly）
    if(!file.open(QIODevice::WriteOnly)){
        // 显示错误消息框（QMessageBox::critical用于严重错误）
        QMessageBox::critical(this, "错误", "无法创建保存文件");
        return;
    }

    QDataStream out(&file); // 创建数据流对象，关联到文件
    out << allRecords;      // 使用重载的<<操作符写入所有记录（自动序列化）

    file.close(); // 关闭文件（建议显式关闭，虽然析构时会自动关闭）
    QMessageBox::information(this, "保存成功", "数据已保存到 records.dat");
}

// ================= 文件操作：从文件加载数据 =================
void Widget::on_loadButton_clicked()
{
    QFile file("records.dat"); // 创建文件对象
    // 以二进制读取模式打开文件（QIODevice::ReadOnly）
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, "错误", "找不到数据文件（records.dat）");
        return;
    }

    QDataStream in(&file); // 创建数据流对象，关联到文件
    in >> allRecords;      // 使用重载的>>操作符读取所有记录（自动反序列化）

    file.close();
    filteredRecords = allRecords; // 加载后显示全部记录
    refreshTable(); // 刷新表格
    // 使用QString的arg()函数动态填充消息文本
    QMessageBox::information(this, "加载成功",
                             QString("已加载%1条记录").arg(allRecords.size())
                             );
}
