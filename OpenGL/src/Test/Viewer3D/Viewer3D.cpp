#include "Viewer3D.h"
#include "ui_Viewer3D.h"
#include "Model3D.h"
#include "Screen.h"

#include <QFileDialog>

Viewer3D::Viewer3D(QWidget *menu_)
	: menu(menu_)
{
	ui = new Ui::Viewer3D();
	ui->setupUi(this);

	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(LoadModel()));
	connect(ui->radioButton, SIGNAL(toggled(bool)), this, SLOT(GizmoChanged()));
	connect(ui->radioButton_2, SIGNAL(toggled(bool)), this, SLOT(GizmoChanged()));
	connect(ui->radioButton_3, SIGNAL(toggled(bool)), this, SLOT(GizmoChanged()));
}

Viewer3D::~Viewer3D()
{
	delete ui;
}

void Viewer3D::LoadModel()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
		"./",
		tr("Meshes (*.ply *.stl)"));
	if (filePath.isEmpty())
		return;

	Model3D* model = new Model3D;
	model->Load(filePath.toStdString());
	ui->openGLWidget->AddModel(model);
}

void Viewer3D::GizmoChanged()
{
	if (ui->radioButton->isChecked()) {
		ui->openGLWidget->SetGizmoType(VIEWER3D::Screen::TRANSLATE);
		ui->radioButton_2->setChecked(false);
		ui->radioButton_3->setChecked(false);
	}
	else if (ui->radioButton_2->isChecked()) {
		ui->openGLWidget->SetGizmoType(VIEWER3D::Screen::ROTATE);
		ui->radioButton->setChecked(false);
		ui->radioButton_3->setChecked(false);
	}
	else if (ui->radioButton_3->isChecked()) {
		ui->openGLWidget->SetGizmoType(VIEWER3D::Screen::SCALE);
		ui->radioButton->setChecked(false);
		ui->radioButton_2->setChecked(false);
	}

	ui->openGLWidget->update();
}