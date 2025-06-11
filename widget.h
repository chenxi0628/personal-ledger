#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <QDate>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QMetaType>
#include <QMenu>
#include <QInputDialog>
#include <QFileDialog>
#include <QDateTime>
#include <QPalette>
#include <QBrush>
#include <QColor>
#include <QHeaderView>
#include <QSizePolicy>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

// 定义收支记录结构体
struct AccountRecord {
    bool isIncome;  // 是否为收入
    double amount;  // 金额
    QDate date;     // 日期
    QString note;   // 备注信息
};

// 注册自定义类型用于信号槽传递
Q_DECLARE_METATYPE(AccountRecord)

// 操作符重载
QDataStream &operator<<(QDataStream &out, const AccountRecord &v);
QDataStream &operator>>(QDataStream &in, AccountRecord &v);

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_incomeButton_clicked();
    void on_expenseButton_clicked();
    void on_filterButton_clicked();
    void on_saveButton_clicked();
    void on_loadButton_clicked();
    void on_exportButton_clicked();

    void on_categoryStatsButton_clicked();
    void on_compressButton_clicked();
    void on_restoreButton_clicked();

    void showRecordContextMenu(const QPoint &pos);
    void deleteRecord(int row);
    void editRecord(int row);

private:
    Ui::Widget *ui;

    QList<AccountRecord> allRecords;      // 所有收支记录
    QList<AccountRecord> filteredRecords; // 筛选后的记录

    void refreshTable();        // 刷新表格显示
    void updateSummary();       // 更新统计信息
    void clearInputs();         // 清空输入框
    bool validateInput();       // 验证输入有效性

    void loadAutoBackup(); // 添加自动备份加载函数声明
    void createAutoBackup(); // 添加自动备份创建函数声明
    void logOperation(const QString &action);
    void saveToFile(const QString &fileName);
    void showCategoryStats(); // 添加分类统计函数声明
    void loadFromFile(const QString &fileName);

    bool checkDataIntegrity(); // 添加数据完整性检查函数声明

    // 数据管理功能
    void compressData(); // 数据压缩函数
    void restoreBackup(); // 恢复备份功能

};

#endif // WIDGET_H
