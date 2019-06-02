#include <QtWidgets/QApplication>
#include <QSurfaceFormat>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>
#include <iostream>

#include "TestMenu.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	TestMenu testMenu;
	testMenu.show();

	return a.exec();
}
