#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <QDate>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

// 定义收支记录结构体
struct AccountRecord {
    bool isIncome;    // 类型：true=收入，false=支出
    double amount;    // 金额
    QDate date;       // 日期
    QString note;     // 备注
};
Q_DECLARE_METATYPE(AccountRecord)
// 操作符重载声明（放在Widget类定义之外）
QDataStream &operator<<(QDataStream &out, const AccountRecord &record);
QDataStream &operator>>(QDataStream &in, AccountRecord &record);
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    // 按钮点击事件处理
    void on_incomeButton_clicked();
    void on_expenseButton_clicked();
    void on_filterButton_clicked();
    void on_saveButton_clicked();
    void on_loadButton_clicked();

private:
    Ui::Widget *ui;

    // 数据存储
    QList<AccountRecord> allRecords;      // 全部记录
    QList<AccountRecord> filteredRecords; // 筛选后的记录

    // 私有方法
    void refreshTable();   // 刷新表格显示
    void updateSummary();  // 更新统计信息
    void clearInputs();    // 清空输入框
    bool validateInput();  // 验证输入有效性
};

#endif // WIDGET_H
