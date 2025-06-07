#include "widget.h"         // 包含主窗口头文件（用户自定义）
#include <QApplication>     // 包含Qt应用程序类头文件

// 程序入口函数（所有Qt程序必须有一个main函数）
int main(int argc, char *argv[])
{
    // 创建Qt应用程序对象（管理全局资源，如命令行参数）
    QApplication a(argc, argv);

    // 创建主窗口对象（Widget是自定义的窗口类）
    Widget w;

    // 显示主窗口（默认窗口是隐藏的，需手动调用show()）
    w.show();

    // 进入应用程序事件循环（程序在此等待用户操作，如点击按钮）
    // exec()返回值是应用程序的退出码（通常由QApplication::quit()触发）
    return a.exec();
}
