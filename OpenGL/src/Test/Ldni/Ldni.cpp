#include "Ldni.h"
#include "ui_Ldni.h"

#include <QFileDialog>

Ldni::Ldni(QWidget *menu_)
	: menu(menu_)
{
	ui = new Ui::Ldni();
	ui->setupUi(this);

	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(LoadMesh()));
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(Run()));
}

Ldni::~Ldni()
{
	delete ui;
}

void Ldni::LoadMesh()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"./",
		tr("Meshes (*.ply *.stl)"));

	ui->openGLWidget->LoadMesh(fileName.toStdString());
}

void Ldni::Run()
{
	ui->openGLWidget->RunLDNI();
}
