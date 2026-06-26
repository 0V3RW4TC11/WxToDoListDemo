#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	wxButton* addButton;
	wxCheckListBox* checkListBox;
	wxButton* clearButton;
	wxStaticText* headlineText;
	wxTextCtrl* inputField;
	wxPanel* panel;

	void CreateControls();
	void BindEventHandlers();
	void OnAddButtonClicked(wxCommandEvent& event);
	void AddTaskFromInput();
	void OnInputEnter(wxCommandEvent& event);
	void OnListKeyDown(wxKeyEvent& event);
	void DeleteSelectedTask();
	void MoveSelectedTask(int offset);
	void SwapTasks(int i, int j);
	void OnClearButtonClicked(wxCommandEvent& event);
	void OnWindowClosed(wxCloseEvent& event);
	void AddSavedTasks();
};

