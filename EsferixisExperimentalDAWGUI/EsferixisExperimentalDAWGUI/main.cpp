#include <QtGui>
#include <QWindow>

int main(int argc, char *argv[])
{
	QGuiApplication a(argc, argv);

	QWindow window;

	window.resize(800, 600);
	window.show();
	window.setTitle("Test");

	return a.exec();
}