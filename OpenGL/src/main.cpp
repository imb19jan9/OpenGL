#include <QtWidgets/QApplication>
#include <QSurfaceFormat>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLContext>
#include <iostream>

#include "TestMenu.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	QSurfaceFormat format;
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	TestMenu testMenu;
	testMenu.show();

	return a.exec();
}
