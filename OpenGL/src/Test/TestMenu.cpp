#include "TestMenu.h"

#include "ClearColor.h"
#include "Texture2D.h"
#include "LDNI.h"

TestMenu::TestMenu(QWidget *parent)
	: QWidget(parent),
	current(nullptr)
{
	ui.setupUi(this);

	RegisterTest<ClearColor>("Clear Color");
	RegisterTest<Texture2D>("Texture 2D");
	RegisterTest<LDNI>("LDNI");
}

TestMenu::~TestMenu()
{
	for (int i = 0; i < tests.size(); i++)
		delete tests[i].button;
}

template<typename T>
void TestMenu::RegisterTest(const std::string & name_)
{
	QPushButton* button = new QPushButton(name_.c_str(), this);
	ui.verticalLayout->addWidget(button);
	connect(button, SIGNAL(clicked()), this, SLOT(ButtonClicked()));

	tests.push_back({ button, [](QWidget* w_) { return new T(w_); }});
}

void TestMenu::ButtonClicked()
{
	for (int i = 0; i < tests.size(); i++) {
		if (tests[i].button == sender()) {
			current = tests[i].instance(this);
			current->setVisible(true);

			this->setVisible(false);
		}
	}
}