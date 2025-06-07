#ifndef WIDGET_H          // 防止头文件重复包含（预编译保护）
#define WIDGET_H

#include <QWidget>        // 包含Qt基础窗口类头文件
#include <QList>          // 包含Qt列表容器头文件（用于存储数据）
#include <QDate>          // 包含Qt日期类头文件
#include <QTableWidgetItem> // 包含表格项类头文件

// Qt命名空间声明（避免与其他库命名冲突）
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; } // 声明UI设计器生成的类（自动生成，无需手动编写）
QT_END_NAMESPACE

// 定义收支记录结构体（存储单笔收支数据）
struct AccountRecord {
    bool isIncome;    // 收支类型：true=收入，false=支出
    double amount;    // 金额（双精度浮点数）
    QDate date;       // 日期（使用Qt的QDate类，支持日期运算）
    QString note;     // 备注（使用Qt的QString类，支持Unicode）
};
// 告诉Qt元对象系统这个结构体存在（用于信号槽传递自定义类型）
Q_DECLARE_METATYPE(AccountRecord)

// 前置声明：操作符重载（用于将结构体写入/读取文件）
QDataStream &operator<<(QDataStream &out, const AccountRecord &record);
QDataStream &operator>>(QDataStream &in, AccountRecord &record);

// 主窗口类（继承自QWidget，所有UI组件的基类）
class Widget : public QWidget
{
    Q_OBJECT // 必须添加！启用Qt元对象系统（信号槽、属性系统等）

public:
    // 构造函数（parent参数用于指定父组件，实现组件层次管理）
    explicit Widget(QWidget *parent = nullptr);
    // 析构函数（清理资源）
    ~Widget();

private slots: // 槽函数（用于接收信号，必须声明在private slots或public slots下）
    // 以下是自动关联的槽函数（Qt根据UI设计器中的对象名自动连接信号）
    void on_incomeButton_clicked();   // 收入按钮点击信号的处理函数
    void on_expenseButton_clicked();  // 支出按钮点击信号的处理函数
    void on_filterButton_clicked();   // 筛选按钮点击信号的处理函数
    void on_saveButton_clicked();     // 保存按钮点击信号的处理函数
    void on_loadButton_clicked();     // 加载按钮点击信号的处理函数

private:
    Ui::Widget *ui; // 指向UI设计器生成的界面对象（通过ui->访问界面组件）

    // 数据存储容器
    QList<AccountRecord> allRecords;      // 存储所有收支记录（动态数组）
    QList<AccountRecord> filteredRecords; // 存储筛选后的记录（用于表格显示）

    // 私有成员函数（仅在类内部使用）
    void refreshTable();   // 刷新表格显示
    void updateSummary();  // 更新统计信息（总收入/支出/余额）
    void clearInputs();    // 清空输入框
    bool validateInput();  // 验证输入数据有效性
};

#endif // WIDGET_H
