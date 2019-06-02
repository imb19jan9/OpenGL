#pragma once

#include <QWidget>
#include <QPushButton>
#include "ui_TestMenu.h"

struct Test {
	QPushButton* button;
	std::function<QWidget*(QWidget*)> instance;
};

class TestMenu : public QWidget
{
	Q_OBJECT
private:
	std::vector<Test> tests;
	QWidget* current;

public:
	TestMenu(QWidget *parent = Q_NULLPTR);
	~TestMenu();

	template<typename T>
	void RegisterTest(const std::string& name_);

private slots:
	void ButtonClicked();

private:
	Ui::TestMenu ui;
};
