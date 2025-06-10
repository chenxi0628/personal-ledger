#include "widget.h"
#include "ui_widget.h"

// 操作符重载实现
QDataStream &operator<<(QDataStream &out, const AccountRecord &v) {
    return out << v.isIncome
               << v.amount
               << v.date
               << v.note;
}

QDataStream &operator>>(QDataStream &in, AccountRecord &v) {
    return in >> v.isIncome
           >> v.amount
           >> v.date
           >> v.note;
}

// ================ 扩展功能：操作日志 ================
void Widget::logOperation(const QString &action)
{
    // 打开日志文件
    QFile logFile("operations.log");

    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        // 写入日志条目
        QTextStream out(&logFile);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
            << " - " << action << "\n";

        // 关闭文件
        logFile.close();
    }
}

// 自动备份加载函数实现
void Widget::loadAutoBackup()
{
    // 检查自动备份文件是否存在
    QFile file("autobackup.dat");

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        // 从备份文件加载数据
        QDataStream in(&file);
        in >> allRecords;
        file.close();

        // 更新筛选记录并刷新表格
        filteredRecords = allRecords;
        refreshTable();

        // 记录操作日志
        logOperation("加载自动备份数据");
    }
}

// 自动备份创建函数实现
void Widget::createAutoBackup()
{
    // 如果没有记录则不备份
    if (allRecords.isEmpty()) {
        return;
    }

    // 创建自动备份文件
    QFile file("autobackup.dat");

    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << allRecords;
        file.close();
    }
}

// 构造函数实现
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    // 设置UI
    ui->setupUi(this);

    // 注册自定义类型用于信号槽传递
    qRegisterMetaType<AccountRecord>("AccountRecord");

    // ================ UI初始化 ================

    // 设置默认日期
    ui->dateEdit->setDate(QDate::currentDate());

    // 设置筛选日期范围（默认为近一个月）
    ui->startDateEdit->setDate(QDate::currentDate().addMonths(-1));
    ui->endDateEdit->setDate(QDate::currentDate());

    // 配置表格控件
    QStringList headers;
    headers << "序号"
            << "类型"
            << "金额"
            << "日期"
            << "备注";

    ui->recordTable->setColumnCount(5);
    ui->recordTable->setHorizontalHeaderLabels(headers);

    // 设置最后一列自动拉伸以填充剩余空间
    ui->recordTable->horizontalHeader()->setSectionResizeMode(
        4,
        QHeaderView::Stretch
        );

    // 启用右键菜单功能
    ui->recordTable->setContextMenuPolicy(Qt::CustomContextMenu);

    // 连接右键菜单请求信号到槽函数
    connect(
        ui->recordTable,
        &QTableWidget::customContextMenuRequested,
        this,
        &Widget::showRecordContextMenu
        );

    // 启用双击编辑功能
    connect(
        ui->recordTable,
        &QTableWidget::cellDoubleClicked,
        this,
        &Widget::editRecord
        );

    // 设置窗口大小策略
    this->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );

    // ================ 信号槽连接 ================

    // 连接清除筛选按钮点击事件
    connect(
        ui->clearFilterButton,
        &QPushButton::clicked,
        [this]() {
            // 清除筛选，显示所有记录
            filteredRecords = allRecords;
            refreshTable();

            // 记录操作日志
            logOperation("清除筛选条件，显示全部记录");
        }
        );

    // 程序启动时加载自动备份数据
    loadAutoBackup();
}



// 析构函数实现（添加自动备份）
Widget::~Widget()
{
    // 程序退出前自动备份数据
    createAutoBackup();

    // 释放UI资源
    delete ui;
}

// ================ 收入按钮处理 ================
void Widget::on_incomeButton_clicked()
{
    // 验证输入有效性
    if (!validateInput()) {
        return;
    }

    // 创建新的收入记录
    AccountRecord record;
    record.isIncome = true;
    record.amount = ui->amountEdit->text().toDouble();
    record.date = ui->dateEdit->date();
    record.note = ui->noteEdit->text().trimmed();

    // 添加记录到数据列表
    allRecords.append(record);
    filteredRecords = allRecords;

    // 刷新表格显示
    refreshTable();

    // 清空输入框
    clearInputs();

    // 记录操作日志
    logOperation(
        QString("添加收入记录: 金额 %1, 备注: %2")
            .arg(record.amount)
            .arg(record.note)
        );
}

// ================ 支出按钮处理 ================
void Widget::on_expenseButton_clicked()
{
    // 验证输入有效性
    if (!validateInput()) {
        return;
    }

    // 创建新的支出记录
    AccountRecord record;
    record.isIncome = false;
    record.amount = ui->amountEdit->text().toDouble();
    record.date = ui->dateEdit->date();
    record.note = ui->noteEdit->text().trimmed();

    // 添加记录到数据列表
    allRecords.append(record);
    filteredRecords = allRecords;

    // 刷新表格显示
    refreshTable();

    // 清空输入框
    clearInputs();

    // 记录操作日志
    logOperation(
        QString("添加支出记录: 金额 %1, 备注: %2")
            .arg(record.amount)
            .arg(record.note)
        );
}

// ================ 筛选按钮处理 ================
void Widget::on_filterButton_clicked()
{
    // 获取筛选日期范围
    QDate start = ui->startDateEdit->date();
    QDate end = ui->endDateEdit->date();

    // 日期有效性检查
    if (start > end) {
        QMessageBox::warning(
            this,
            "日期错误",
            "开始日期不能晚于结束日期"
            );
        return;
    }

    // 执行筛选操作
    filteredRecords.clear();

    for (const AccountRecord &record : allRecords) {
        if (record.date >= start && record.date <= end) {
            filteredRecords.append(record);
        }
    }

    // 刷新表格显示筛选结果
    refreshTable();

    // 记录操作日志
    logOperation(
        QString("筛选记录: %1 至 %2, 共 %3 条记录")
            .arg(start.toString("yyyy-MM-dd"))
            .arg(end.toString("yyyy-MM-dd"))
            .arg(filteredRecords.size())
        );
}

// ================ 刷新表格显示 ================
void Widget::refreshTable()
{
    // 清空表格
    ui->recordTable->setRowCount(0);

    // 计算总收入和总支出
    double totalIncome = 0;
    double totalExpense = 0;

    // 填充表格数据
    for (int i = 0; i < filteredRecords.size(); ++i) {
        const AccountRecord &record = filteredRecords[i];

        // 更新统计数据
        if (record.isIncome) {
            totalIncome += record.amount;
        } else {
            totalExpense += record.amount;
        }

        // 添加新行
        int row = ui->recordTable->rowCount();
        ui->recordTable->insertRow(row);

        // 设置表格内容
        ui->recordTable->setItem(
            row,
            0,
            new QTableWidgetItem(QString::number(row + 1))
            );

        ui->recordTable->setItem(
            row,
            1,
            new QTableWidgetItem(record.isIncome ? "收入" : "支出")
            );

        ui->recordTable->setItem(
            row,
            2,
            new QTableWidgetItem(QString::number(record.amount, 'f', 2))
            );

        ui->recordTable->setItem(
            row,
            3,
            new QTableWidgetItem(record.date.toString("yyyy-MM-dd"))
            );

        ui->recordTable->setItem(
            row,
            4,
            new QTableWidgetItem(record.note)
            );

        // 设置文本颜色（收入为绿色，支出为红色）
        QColor textColor = record.isIncome
                               ? QColor(0, 128, 0)
                               : QColor(200, 0, 0);

        for (int col = 0; col < 5; ++col) {
            if (ui->recordTable->item(row, col)) {
                ui->recordTable->item(row, col)->setForeground(
                    QBrush(textColor)
                    );
            }
        }
    }

    // 更新统计信息
    updateSummary();
}

// ================ 更新统计信息 ================
void Widget::updateSummary()
{
    double totalIncome = 0;
    double totalExpense = 0;

    // 计算总收入和总支出
    for (const AccountRecord &record : filteredRecords) {
        if (record.isIncome) {
            totalIncome += record.amount;
        } else {
            totalExpense += record.amount;
        }
    }

    // 显示统计结果
    ui->totalIncomeLabel->setText(
        QString::number(totalIncome, 'f', 2)
        );

    ui->totalExpenseLabel->setText(
        QString::number(totalExpense, 'f', 2)
        );

    ui->balanceLabel->setText(
        QString::number(totalIncome - totalExpense, 'f', 2)
        );

    // 设置余额颜色（正数为绿色，负数为红色）
    QPalette palette = ui->balanceLabel->palette();
    palette.setColor(
        QPalette::WindowText,
        (totalIncome - totalExpense) >= 0 ? Qt::darkGreen : Qt::red
        );

    ui->balanceLabel->setPalette(palette);
}

// ================ 清空输入框 ================
void Widget::clearInputs()
{
    // 清空输入控件
    ui->amountEdit->clear();
    ui->noteEdit->clear();

    // 重置日期为当前日期
    ui->dateEdit->setDate(QDate::currentDate());

    // 设置焦点到金额输入框
    ui->amountEdit->setFocus();
}

// ================ 输入验证 ================
bool Widget::validateInput()
{
    // 金额为空检查
    if (ui->amountEdit->text().isEmpty()) {
        QMessageBox::warning(
            this,
            "输入错误",
            "金额不能为空"
            );

        ui->amountEdit->setFocus();
        return false;
    }

    // 金额有效性检查
    bool ok;
    double amount = ui->amountEdit->text().toDouble(&ok);

    if (!ok || amount <= 0) {
        QMessageBox::warning(
            this,
            "输入错误",
            "请输入有效的金额（大于0的数字）"
            );

        ui->amountEdit->selectAll();
        ui->amountEdit->setFocus();
        return false;
    }

    // 备注长度检查
    if (ui->noteEdit->text().length() > 100) {
        QMessageBox::warning(
            this,
            "输入错误",
            "备注长度不能超过100个字符"
            );

        ui->noteEdit->selectAll();
        ui->noteEdit->setFocus();
        return false;
    }

    // 日期有效性检查
    if (ui->dateEdit->date() > QDate::currentDate()) {
        QMessageBox::warning(
            this,
            "日期错误",
            "日期不能晚于今天"
            );

        ui->dateEdit->setFocus();
        return false;
    }

    return true;
}

// ================ 保存数据 ================
void Widget::on_saveButton_clicked()
{
    saveToFile("records.dat");
}

// ================ 加载数据 ================
void Widget::on_loadButton_clicked()
{
    loadFromFile("records.dat");
}

// ================ 扩展功能：导出为CSV ================
void Widget::on_exportButton_clicked()
{
    // 打开文件对话框选择保存位置
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "导出CSV文件",
        "",
        "CSV文件 (*.csv)"
        );

    if (fileName.isEmpty()) {
        return;
    }

    // 创建并写入CSV文件
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(
            this,
            "错误",
            "无法创建CSV文件"
            );
        return;
    }

    // 写入CSV表头
    QTextStream out(&file);
    out << "类型,金额,日期,备注\n";

    // 写入所有记录
    for (const AccountRecord &record : allRecords) {
        out << (record.isIncome ? "收入" : "支出") << ","
            << QString::number(record.amount, 'f', 2) << ","
            << record.date.toString("yyyy-MM-dd") << ","
            << record.note << "\n";
    }

    // 关闭文件
    file.close();

    // 显示成功消息
    QMessageBox::information(
        this,
        "导出成功",
        "数据已导出为CSV格式"
        );

    // 记录操作日志
    logOperation(
        QString("导出CSV文件: %1").arg(fileName)
        );
}

// ================ 辅助功能：通用文件保存 ================
void Widget::saveToFile(const QString &fileName)
{
    // 打开文件
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(
            this,
            "错误",
            QString("无法创建文件: %1").arg(fileName)
            );
        return;
    }

    // 使用数据流写入数据
    QDataStream out(&file);
    out << allRecords;

    // 关闭文件
    file.close();

    // 显示成功消息
    QMessageBox::information(
        this,
        "保存成功",
        QString("数据已保存到 %1").arg(fileName)
        );

    // 记录操作日志
    logOperation(
        QString("保存数据到: %1").arg(fileName)
        );
}

// ================ 辅助功能：通用文件加载 ================
void Widget::loadFromFile(const QString &fileName)
{
    // 打开文件
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(
            this,
            "错误",
            QString("找不到数据文件: %1").arg(fileName)
            );
        return;
    }

    // 使用数据流读取数据
    QDataStream in(&file);
    in >> allRecords;

    // 关闭文件
    file.close();

    // 更新筛选记录并刷新表格
    filteredRecords = allRecords;
    refreshTable();

    // 显示成功消息
    QMessageBox::information(
        this,
        "加载成功",
        QString("已加载 %1 条记录").arg(allRecords.size())
        );

    // 记录操作日志
    logOperation(
        QString("从文件加载数据: %1").arg(fileName)
        );
}

// ================ 扩展功能：记录删除 ================
void Widget::deleteRecord(int row)
{
    // 检查行索引有效性
    if (row < 0 || row >= filteredRecords.size()) {
        return;
    }

    // 获取要删除的记录
    AccountRecord record = filteredRecords[row];

    // 确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "确认删除",
        QString("确定要删除该记录吗?\n金额: %1\n日期: %2\n备注: %3")
            .arg(record.amount)
            .arg(record.date.toString("yyyy-MM-dd"))
            .arg(record.note),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        // 从所有记录中删除 - 通过内容匹配而非索引
        for (int i = 0; i < allRecords.size(); ++i) {
            if (allRecords[i].date == record.date &&
                allRecords[i].amount == record.amount &&
                allRecords[i].note == record.note) {
                allRecords.removeAt(i);
                break;
            }
        }

        // 从筛选记录中删除
        filteredRecords.removeAt(row);

        // 刷新表格
        refreshTable();

        // 记录操作日志
        logOperation(
            QString("删除记录: 金额 %1, 日期 %2, 备注: %3")
                .arg(record.amount)
                .arg(record.date.toString("yyyy-MM-dd"))
                .arg(record.note)
            );
    }
}

// ================ 扩展功能：记录编辑 ================
void Widget::editRecord(int row)
{
    // 检查行索引有效性
    if (row < 0 || row >= filteredRecords.size()) {
        return;
    }

    // 获取要编辑的记录
    AccountRecord record = filteredRecords[row];

    // 创建编辑对话框
    bool ok;
    QString newNote = QInputDialog::getText(
        this,
        "编辑备注",
        "请输入新的备注:",
        QLineEdit::Normal,
        record.note,
        &ok
        );

    if (ok && !newNote.isEmpty()) {
        // 更新所有记录中的备注 - 通过内容匹配而非索引
        for (int i = 0; i < allRecords.size(); ++i) {
            if (allRecords[i].date == record.date &&
                allRecords[i].amount == record.amount &&
                allRecords[i].note == record.note) {
                allRecords[i].note = newNote;
                break;
            }
        }

        // 更新筛选记录中的备注
        filteredRecords[row].note = newNote;

        // 刷新表格
        refreshTable();

        // 记录操作日志
        logOperation(
            QString("编辑记录: 原备注 [%1] 改为 [%2]")
                .arg(record.note, newNote)
            );
    }
}

// ================ 扩展功能：右键菜单 ================
void Widget::showRecordContextMenu(const QPoint &pos)
{
    // 获取鼠标点击位置的表格索引
    QTableWidget *table = ui->recordTable;
    QModelIndex index = table->indexAt(pos);

    if (!index.isValid()) {
        return;
    }

    // 创建右键菜单
    QMenu menu(this);
    QAction *editAction = menu.addAction("编辑备注");
    QAction *deleteAction = menu.addAction("删除记录");

    // 显示菜单并获取用户选择
    QAction *selectedAction = menu.exec(
        table->viewport()->mapToGlobal(pos)
        );

    // 处理菜单项选择
    if (selectedAction == deleteAction) {
        deleteRecord(index.row());
    }
    else if (selectedAction == editAction) {
        editRecord(index.row());
    }
}






// ================ 扩展功能：分类统计 ================
void Widget::showCategoryStats()
{
    // 按备注分类统计收支情况
    QMap<QString, double> incomeCategories;
    QMap<QString, double> expenseCategories;

    for (const AccountRecord &record : allRecords) {
        if (record.isIncome) {
            incomeCategories[record.note] += record.amount;
        } else {
            expenseCategories[record.note] += record.amount;
        }
    }

    // 构建统计结果字符串
    QString stats = "===== 收入分类统计 =====\n";

    for (auto it = incomeCategories.begin(); it != incomeCategories.end(); ++it) {
        stats += QString("%1: %2\n")
        .arg(it.key())
            .arg(it.value(), 0, 'f', 2);
    }

    stats += "\n===== 支出分类统计 =====\n";

    for (auto it = expenseCategories.begin(); it != expenseCategories.end(); ++it) {
        stats += QString("%1: %2\n")
        .arg(it.key())
            .arg(it.value(), 0, 'f', 2);
    }

    // 显示统计结果对话框
    QMessageBox::information(
        this,
        "分类统计",
        stats
        );

    // 记录操作日志
    logOperation("生成分类统计报表");
}

// ================ 扩展功能：窗口关闭事件 ================
void Widget::closeEvent(QCloseEvent *event)
{
    // 显示确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "确认退出",
        "确定要退出程序吗?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        // 保存自动备份
        createAutoBackup();

        // 接受关闭事件
        event->accept();
    } else {
        // 忽略关闭事件
        event->ignore();
    }
}

// ================ 辅助功能：数据完整性检查 ================
bool Widget::checkDataIntegrity()
{
    // 检查记录数据的完整性
    if (allRecords.isEmpty()) {
        qDebug() << "数据完整性检查: 无记录";
        return true;
    }

    int errors = 0;

    for (const AccountRecord &record : allRecords) {
        if (record.amount <= 0) {
            qWarning() << "无效金额记录:" << record.amount;
            errors++;
        }

        if (record.date.isNull() || !record.date.isValid()) {
            qWarning() << "无效日期记录:" << record.date;
            errors++;
        }
    }

    if (errors > 0) {
        qCritical() << "发现" << errors << "个数据完整性问题";
        return false;
    }

    qDebug() << "数据完整性检查通过";
    return true;
}

// ================ 辅助功能：数据压缩 ================
void Widget::compressData()
{
    // 保存原始记录数量
    int originalCount = allRecords.size();

    // 合并相同日期、备注和类型的记录
    QList<AccountRecord> compressed;

    for (const AccountRecord &record : allRecords) {
        bool found = false;

        for (AccountRecord &comp : compressed) {
            // 检查日期、备注和收支类型是否相同
            if (comp.date == record.date &&
                comp.note == record.note &&
                comp.isIncome == record.isIncome) {
                comp.amount += record.amount;
                found = true;
                break;
            }
        }

        if (!found) {
            compressed.append(record);
        }
    }

    // 计算实际压缩的记录数
    int compressedCount = originalCount - compressed.size();

    // 如果有记录被压缩
    if (compressedCount > 0) {
        // 更新记录
        allRecords = compressed;
        filteredRecords = allRecords;

        // 刷新表格
        refreshTable();

        // 显示压缩信息
        QMessageBox::information(
            this,
            "数据压缩",
            QString("压缩了 %1 条重复记录").arg(compressedCount)
            );
    } else {
        QMessageBox::information(
            this,
            "数据压缩",
            "未找到可压缩的重复记录"
            );
    }
}

// ================ 扩展功能：金额格式化 ================
QString Widget::formatCurrency(double amount)
{
    // 格式化金额为货币形式
    return QString("¥%1").arg(
        QString::number(amount, 'f', 2)
        );
}

// ================ 扩展功能：数据恢复 ================
void Widget::restoreBackup()
{
    // 确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "恢复备份",
        "确定要恢复最近一次备份吗?\n当前数据将会丢失",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        // 从自动备份加载数据
        loadFromFile("autobackup.dat");
    }
}

// ================ 分类统计按钮处理 ================
void Widget::on_categoryStatsButton_clicked()
{
    showCategoryStats();
}

// ================ 数据压缩按钮处理 ================
void Widget::on_compressButton_clicked()
{
    compressData();
}

// ================ 恢复备份按钮处理 ================
void Widget::on_restoreButton_clicked()
{
    restoreBackup();
}
