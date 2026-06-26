#include "app.h"
#include "mainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	auto mainFrame = new MainFrame("To-Do List");
	mainFrame->SetClientSize(800, 600);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}
