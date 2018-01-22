#include "uitest.h"
#include <QFile>
#include <QFontDatabase>
#include <QIODevice>
#include <QDebug>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
	MainWindow mainWindow;

	QFile qss(".\\css\\black.css");
	qss.open(QFile::ReadOnly);
	qApp->setStyleSheet(qss.readAll());
	qss.close();

	QFont font;
	font.setFamily(QString::fromWCharArray(L"华文行楷"));
	font.setPointSize(10);
	a.setFont(font);

	mainWindow.show();
	return a.exec();

}


/*
int main()
{
    Library l;
    l.AdminLogin("awdrtg", "111");

	Book b(1);
	Book bb(115);
	Book bbb(116);

	string type = QString::fromWCharArray(L"书籍名称").toStdString();
	string name = QString::fromWCharArray(L"世界").toStdString();
	Search_Result r;
	r = l.Search(type, name);
	
}
*/


