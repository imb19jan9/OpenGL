#include "TestMenu.h"
#include "ClearColor/ClearColor.h"
#include "TrackBall/TrackBall.h"
#include "Texture2D/Texture2D.h"
#include "Ldni/Ldni.h"
#include "Viewer3D/Viewer3D.h"

TestMenu::TestMenu(QWidget *parent)
	: QWidget(parent),
	current(nullptr)
{
	ui.setupUi(this);

	RegisterTest<ClearColor>("Clear Color");
	RegisterTest<Texture2D>("Texture 2D");
	RegisterTest<Ldni>("LDNI");
	RegisterTest<TrackBall>("TrackBall");
	RegisterTest<Viewer3D>("Viewer 3D");
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