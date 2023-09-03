#include "MainForm.h"
#include <Windows.h>
#include <process.h>
#include <tchar.h>
using namespace System;
using namespace System::Windows::Forms;
using namespace SideSlideAudio;

HANDLE hMutex;
void OnFormClosed(Object^ sender, FormClosedEventArgs^ e);
[STAThreadAttribute]

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    hMutex = CreateMutex(NULL, true, _T("SSA"));
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(hMutex);
        System::Windows::Forms::MessageBox::Show("Програма вже запущена", "Не більше однієї",
            System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
        return 0;
    }
    else {
        Application::EnableVisualStyles();
        Application::SetCompatibleTextRenderingDefault(false);
        MainForm^ form = gcnew MainForm();
        form->FormClosed += gcnew FormClosedEventHandler(OnFormClosed);
        Application::Run(form);
    }
    return 0;
}


void OnFormClosed(Object^ sender, FormClosedEventArgs^ e)
{
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
}


