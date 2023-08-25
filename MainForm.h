#pragma once
#include <windows.h>
#include <iostream>
#include <psapi.h>
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <vector>
#include <string>
#include <msclr/marshal_cppstd.h>
#include <vcclr.h>
#include <GdiPlus.h>
#include <endpointvolume.h>
#include <cmath>

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Gdiplus.lib")

#using <System.Runtime.dll>

#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }
extern "C" int __cdecl MyGetIconInfo(HICON hIcon, ICONINFO * pIconInfo)
{
	return GetIconInfo(hIcon, pIconInfo);
}
namespace SideSlideAudio {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
    using namespace msclr::interop;
    using namespace std;
    using namespace System::Runtime::ExceptionServices;
    using namespace System::Security;

	/// <summary>
	/// Сводка для MainForm
	/// </summary>
    int Index = 0;
    static int CountDiff = 0;
    static bool start = false;
    static bool isEntered = false;
	public ref class MainForm : public System::Windows::Forms::Form
	{
        static cli::array<String^>^ Volumes = gcnew cli::array<String^>(100);

        ref class TSC
        {
        private:
            String^ SesCou = "";

        public:
            property String^ SC
            {
                String^ get()
                {
                    return SesCou;
                }
                void set(String^ value)
                {
                    if (value != SesCou)
                    {
                        SesCou = value;
                        start = true;
                    }
                }
            }

            TSC()
            {
                SesCou = "";
                CountDiff++;
                start = true;
            }

            ~TSC() {
                CountDiff--;
                start = true;
            }
        };

        ref class LPB: public PictureBox
        {
        private:
            float loud;

        public:
            int ProgressLoud;
            property float Loud
            {
                float get()
                {
                    return loud;
                }
                void set(float value)
                {
                    if (value != loud)
                    {
                        loud = value;
                        UpdateBackgroundImage();
                    }
                }
            }

            LPB()
            {
                loud = 0.0f;
                UpdateBackgroundImage();
            }

        private:
            void UpdateBackgroundImage()
            {

                int num = Loud * 100;
                ProgressLoud = (num == 97) ? 100 : num;
                Invalidate();
            }
        };

        ref class VPB : public PictureBox
        {
        private:
            float voll;
            int ind;

        public:
            int ProgressVol;
            property float Voll
            {
                float get()
                {
                    return voll;
                }
                void set(float value)
                {
                    if (value != voll)
                    {
                        voll = value;
                        SASV();
                        USV();
                    }
                }
            }

            VPB()
            {      
                ind = Index;
                //Тут иногда какого-то хрена индекс лишний вылазит
                voll = System::Single::Parse(Volumes[ind]);
                Index++;
                SASV();
                USV();
            }

        private:
            void SASV() {
                HRESULT hr;
                IMMDeviceEnumerator* pDeviceEnumerator = NULL;
                IMMDevice* pDevice = NULL;
                IAudioSessionManager2* pSessionManager = NULL;
                IAudioSessionEnumerator* pSessionEnumerator = NULL;

                CoInitialize(NULL);

                hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
                if (FAILED(hr)) return;

                hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
                if (FAILED(hr)) return;

                hr = pDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pSessionManager);
                if (FAILED(hr)) return;

                hr = pSessionManager->GetSessionEnumerator(&pSessionEnumerator);
                if (FAILED(hr)) return;

                IAudioSessionControl* pSessionControl = NULL;
                ISimpleAudioVolume* pSimpleAudioVolume = NULL;

                pSessionEnumerator->GetSession(ind, &pSessionControl);
                pSessionControl->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&pSimpleAudioVolume);

                pSimpleAudioVolume->SetMasterVolume(Voll, NULL);

                pSimpleAudioVolume->Release();
                pSessionControl->Release();

                pSessionEnumerator->Release();
                pSessionManager->Release();
                pDevice->Release();
                pDeviceEnumerator->Release();

                CoUninitialize();
            }

            void USV() {
                double nums = static_cast<double>(Voll * 100.0);
                int num = round(nums);
                ProgressVol = ((num < 0) ? 0 : num);
                Invalidate();
            }
        };

	public:
		MainForm(void)
		{
			InitializeComponent();
            DoubleBuffered = true;
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

    private: System::Windows::Forms::Panel^ MainPan;
    private: System::Windows::Forms::Timer^ UpdateLoud;
    private: System::Windows::Forms::Timer^ UpdateVolume;
    private: System::Windows::Forms::Timer^ ShowIt;
private: System::Windows::Forms::NotifyIcon^ InTray;
private: System::Windows::Forms::Timer^ ShowThis;
private: System::Windows::Forms::Timer^ HideThis;
private: System::Windows::Forms::ContextMenuStrip^ MainTray;
private: System::Windows::Forms::ToolStripMenuItem^ Item1;
private: System::Windows::Forms::ToolStripMenuItem^ Item2;



    private: System::ComponentModel::IContainer^ components;
    protected:

	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
            this->MainPan = (gcnew System::Windows::Forms::Panel());
            this->UpdateLoud = (gcnew System::Windows::Forms::Timer(this->components));
            this->UpdateVolume = (gcnew System::Windows::Forms::Timer(this->components));
            this->ShowIt = (gcnew System::Windows::Forms::Timer(this->components));
            this->InTray = (gcnew System::Windows::Forms::NotifyIcon(this->components));
            this->MainTray = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->Item1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->Item2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ShowThis = (gcnew System::Windows::Forms::Timer(this->components));
            this->HideThis = (gcnew System::Windows::Forms::Timer(this->components));
            this->MainTray->SuspendLayout();
            this->SuspendLayout();
            // 
            // MainPan
            // 
            this->MainPan->Location = System::Drawing::Point(6, 9);
            this->MainPan->Name = L"MainPan";
            this->MainPan->Size = System::Drawing::Size(86, 70);
            this->MainPan->TabIndex = 0;
            // 
            // UpdateLoud
            // 
            this->UpdateLoud->Interval = 33;
            this->UpdateLoud->Tick += gcnew System::EventHandler(this, &MainForm::UpdateLoud_Tick);
            // 
            // UpdateVolume
            // 
            this->UpdateVolume->Enabled = true;
            this->UpdateVolume->Interval = 1000;
            this->UpdateVolume->Tick += gcnew System::EventHandler(this, &MainForm::UpdateVolume_Tick);
            // 
            // ShowIt
            // 
            this->ShowIt->Enabled = true;
            this->ShowIt->Interval = 1000;
            this->ShowIt->Tick += gcnew System::EventHandler(this, &MainForm::ShowIt_Tick);
            // 
            // InTray
            // 
            this->InTray->ContextMenuStrip = this->MainTray;
            this->InTray->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"InTray.Icon")));
            this->InTray->Text = L"SSA";
            this->InTray->Visible = true;
            this->InTray->DoubleClick += gcnew System::EventHandler(this, &MainForm::InTray_DoubleClick);
            // 
            // MainTray
            // 
            this->MainTray->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->Item1, this->Item2 });
            this->MainTray->Name = L"MainTray";
            this->MainTray->Size = System::Drawing::Size(181, 48);
            this->MainTray->Closing += gcnew System::Windows::Forms::ToolStripDropDownClosingEventHandler(this, &MainForm::MainTray_Closing);
            // 
            // Item1
            // 
            this->Item1->Checked = true;
            this->Item1->CheckState = System::Windows::Forms::CheckState::Checked;
            this->Item1->Name = L"Item1";
            this->Item1->Size = System::Drawing::Size(180, 22);
            this->Item1->Text = L"toolStripMenuItem1";
            this->Item1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::Item1_MouseDown);
            // 
            // Item2
            // 
            this->Item2->Name = L"Item2";
            this->Item2->Size = System::Drawing::Size(180, 22);
            this->Item2->Text = L"toolStripMenuItem2";
            this->Item2->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::Item2_MouseDown);
            // 
            // ShowThis
            // 
            this->ShowThis->Interval = 5;
            this->ShowThis->Tick += gcnew System::EventHandler(this, &MainForm::ShowThis_Tick);
            // 
            // HideThis
            // 
            this->HideThis->Interval = 5;
            this->HideThis->Tick += gcnew System::EventHandler(this, &MainForm::HideThis_Tick);
            // 
            // MainForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(31)), static_cast<System::Int32>(static_cast<System::Byte>(35)),
                static_cast<System::Int32>(static_cast<System::Byte>(39)));
            this->ClientSize = System::Drawing::Size(85, 60);
            this->Controls->Add(this->MainPan);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
            this->KeyPreview = true;
            this->Name = L"MainForm";
            this->Text = L"SSA";
            this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
            this->Shown += gcnew System::EventHandler(this, &MainForm::MainForm_Shown);
            this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::MainForm_KeyDown);
            this->MainTray->ResumeLayout(false);
            this->ResumeLayout(false);

        }
#pragma endregion

    int CountOfSeanse = 0;
    int OldCount = 0;
    int coef = 0;

    bool Over = false;

    bool Side = false;

    cli::array<TSC^>^ TrigNam = gcnew cli::array<TSC^>(100);

    cli::array<String^>^ Names = gcnew cli::array<String^>(100);
    cli::array<String^>^ Icons = gcnew cli::array<String^>(100);
    cli::array<bool>^ Mutes = gcnew cli::array<bool>(100);

    //Обновляет звук и громкость и колво сеансов

    [HandleProcessCorruptedStateExceptions]
    void RNT() {
        HRESULT hr;
        IMMDeviceEnumerator* pDeviceEnumerator = NULL;
        IMMDevice* pDevice = NULL;
        IAudioSessionManager2* pSessionManager = NULL;
        IAudioSessionEnumerator* pSessionEnumerator = NULL;
        IAudioSessionControl* pSessionControl = NULL;
        IAudioMeterInformation* pMeterInfo = NULL;
        int sessionCount = CountOfSeanse;
        float peak;

        CoInitialize(NULL);

        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
        if (FAILED(hr)) {

            goto Exit;
        }

        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
        if (FAILED(hr)) {

            goto Exit;
        }

        hr = pDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pSessionManager);
        if (FAILED(hr)) {

            goto Exit;
        }

        hr = pSessionManager->GetSessionEnumerator(&pSessionEnumerator);
        if (FAILED(hr)) {

            goto Exit;
        }

        for (int i = 0; i < sessionCount; i++) {
            try {
                hr = pSessionEnumerator->GetSession(i, &pSessionControl);
            }
            catch (...) {
                throw gcnew System::NullReferenceException;
            }
            if (FAILED(hr)) {

                goto Exit;
            }

            hr = pSessionControl->QueryInterface(__uuidof(IAudioMeterInformation), (void**)&pMeterInfo);
            if (FAILED(hr)) {

                goto Exit;
            }

            hr = pMeterInfo->GetPeakValue(&peak);
            if (FAILED(hr)) {

                goto Exit;
            }
            try {
                LC[i]->Loud = peak;
            }
            catch (System::NullReferenceException^) {
                throw gcnew System::NullReferenceException;
            }

            SAFE_RELEASE(pMeterInfo)
                SAFE_RELEASE(pSessionControl)

            IAudioSessionControl* pSessionControl = NULL;
            ISimpleAudioVolume* pSimpleAudioVolume = NULL;

            try {
                pSessionEnumerator->GetSession(i, &pSessionControl);
                pSessionControl->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&pSimpleAudioVolume);
            }
            catch (...) {
                throw gcnew System::NullReferenceException;
            }

            float volume;
            pSimpleAudioVolume->GetMasterVolume(&volume);
            try {
                VC[i]->Voll = volume;
            }
            catch (System::NullReferenceException^) {
                throw gcnew System::NullReferenceException;
            }

            BOOL muteState;
            pSimpleAudioVolume->GetMute(&muteState);
            try {
                if (muteState != Mutes[i]) {
                    Mutes[i] = muteState;
                    AutoMuteState(IconsOf[i]);
                }
            }
            catch (System::NullReferenceException^) {
                throw gcnew System::NullReferenceException;
            }

            pSimpleAudioVolume->Release();
            pSessionControl->Release();
        }

    Exit:
        SAFE_RELEASE(pDeviceEnumerator)
            SAFE_RELEASE(pDevice)
            SAFE_RELEASE(pSessionManager)
            SAFE_RELEASE(pSessionEnumerator)
            CoUninitialize();
    }

    void GetSC() {
        HRESULT hr;
        IMMDeviceEnumerator* pDeviceEnumerator = NULL;
        IMMDevice* pDevice = NULL;
        IAudioSessionManager2* pSessionManager = NULL;
        IAudioSessionEnumerator* pSessionEnumerator = NULL;
        int sessionCount;

        CoInitialize(NULL);

        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
        if (FAILED(hr)) {
            goto Exit;
        }

        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
        if (FAILED(hr)) {
            goto Exit;
        }

        hr = pDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pSessionManager);
        if (FAILED(hr)) {
            goto Exit;
        }

        hr = pSessionManager->GetSessionEnumerator(&pSessionEnumerator);
        if (FAILED(hr)) {
            goto Exit;
        }

        hr = pSessionEnumerator->GetCount(&sessionCount);
        if (FAILED(hr)) {
            goto Exit;
        }

        CountOfSeanse = sessionCount;

    Exit:
        SAFE_RELEASE(pDeviceEnumerator)
            SAFE_RELEASE(pDevice)
            SAFE_RELEASE(pSessionManager)
            SAFE_RELEASE(pSessionEnumerator)
            CoUninitialize();
    }

    void GetAudioSessionInfo(cli::array<String^>^ Volumes, cli::array<String^>^ Names, cli::array<String^>^ Icons) {
        HRESULT hr;
        IMMDeviceEnumerator* pDeviceEnumerator = NULL;
        IMMDevice* pDevice = NULL;
        IAudioSessionManager2* pSessionManager = NULL;
        IAudioSessionEnumerator* pSessionEnumerator = NULL;

        CoInitialize(NULL);

        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);


        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);


        hr = pDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pSessionManager);


        hr = pSessionManager->GetSessionEnumerator(&pSessionEnumerator);


        int sessionCount;
        pSessionEnumerator->GetCount(&sessionCount);

        std::vector<float> volumes(sessionCount);
        std::vector<std::wstring> icons(sessionCount);
        std::vector<std::wstring> names(sessionCount);
        std::vector<DWORD> processIds(sessionCount);
        std::vector<std::wstring> exePaths(sessionCount);

        for (int i = 0; i < sessionCount; i++) {
            IAudioSessionControl* pSessionControl = NULL;
            IAudioSessionControl2* pSessionControl2 = NULL;
            ISimpleAudioVolume* pSimpleAudioVolume = NULL;
            LPWSTR displayName = NULL;
            LPWSTR iconPath = NULL;

            pSessionEnumerator->GetSession(i, &pSessionControl);
            pSessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&pSessionControl2);
            pSessionControl->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&pSimpleAudioVolume);

            float volume;
            pSimpleAudioVolume->GetMasterVolume(&volume);
            Volumes[i] = (volume).ToString();

            pSessionControl2->GetDisplayName(&displayName);
            Names[i] = gcnew System::String(displayName);

            pSessionControl2->GetIconPath(&iconPath);
            Icons[i] = gcnew System::String(iconPath);

            DWORD processId;
            pSessionControl2->GetProcessId(&processId);
            processIds[i] = processId;

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
            if (hProcess != NULL) {
                WCHAR exePath[MAX_PATH];
                DWORD exePathSize = MAX_PATH;
                if (QueryFullProcessImageName(hProcess, 0, exePath, &exePathSize)) {
                    exePaths[i] = exePath;

                    // Get the file name from the executable path
                    WCHAR fileName[MAX_PATH];
                    _wsplitpath_s(exePath, NULL, 0, NULL, 0, fileName, MAX_PATH, NULL, 0);
                    Names[i] = gcnew System::String(fileName);

                    // Get the icon path from the executable path
                    Icons[i] = gcnew System::String(exePath);
                }
                CloseHandle(hProcess);
            }

            CoTaskMemFree(displayName);
            CoTaskMemFree(iconPath);
            pSimpleAudioVolume->Release();
            pSessionControl2->Release();
            pSessionControl->Release();
        }

        // Use the volumes, icons, names, processIds, and exePaths arrays here

        pSessionEnumerator->Release();
        pSessionManager->Release();
        pDevice->Release();
        pDeviceEnumerator->Release();

        CoUninitialize();
    }

    [HandleProcessCorruptedStateExceptions]
    [SecurityCritical]
    void GetNames() {
        HRESULT hr;
        IMMDeviceEnumerator* pDeviceEnumerator = NULL;
        IMMDevice* pDevice = NULL;
        IAudioSessionManager2* pSessionManager = NULL;
        IAudioSessionEnumerator* pSessionEnumerator = NULL;
        

        CoInitialize(NULL);

        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);

        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);

        hr = pDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pSessionManager);

        hr = pSessionManager->GetSessionEnumerator(&pSessionEnumerator);

        int sessionCount = CountOfSeanse;

        vector<DWORD> processIds(sessionCount);
        vector<std::wstring> exePaths(sessionCount);

        for (int i = 0; i < sessionCount; i++) {
            IAudioSessionControl* pSessionControl = NULL;
            IAudioSessionControl2* pSessionControl2 = NULL;

            try {
                pSessionEnumerator->GetSession(i, &pSessionControl);
                pSessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&pSessionControl2);
            }
            catch (...) {
                throw gcnew System::NullReferenceException;
            }

            LPWSTR displayName = NULL;
            pSessionControl2->GetDisplayName(&displayName);

            if (sessionCount > OldCount) {
                for (int k = OldCount; k < sessionCount; k++) {
                    TrigNam[k] = gcnew TSC();
                }
                OldCount = sessionCount;
            }

            else if (sessionCount < OldCount) {
                for (int k = OldCount-1; k >= sessionCount; k--) {
                    delete TrigNam[k];
                    TrigNam[k] = nullptr;
                }
                OldCount = sessionCount;
            }

            DWORD processId;
            pSessionControl2->GetProcessId(&processId);
            processIds[i] = processId;

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
            if (hProcess != NULL) {
                WCHAR exePath[MAX_PATH];
                DWORD exePathSize = MAX_PATH;
                if (QueryFullProcessImageName(hProcess, 0, exePath, &exePathSize)) {
                    exePaths[i] = exePath;

                    // Get the file name from the executable path
                    WCHAR fileName[MAX_PATH];
                    _wsplitpath_s(exePath, NULL, 0, NULL, 0, fileName, MAX_PATH, NULL, 0);
                    TrigNam[i]->SC = gcnew System::String(fileName);
                }
                CloseHandle(hProcess);
            }

            CoTaskMemFree(displayName);
            pSessionControl2->Release();
            pSessionControl->Release();
        }

        pSessionEnumerator->Release();
        pSessionManager->Release();
        pDevice->Release();
        pDeviceEnumerator->Release();

        CoUninitialize();
    }

    void ExtractIconAndSetImage(String^ exeFilePath, PictureBox^ pictureBox)
    {
        pin_ptr<const wchar_t> exeFilePathPtr = PtrToStringChars(exeFilePath);
        HICON hIcon = ExtractIconW(NULL, exeFilePathPtr, 0);

        if (hIcon != NULL)
        {
            Bitmap^ iconBitmap = nullptr;

            // Получаем размер иконки
            ICONINFO iconInfo;
            GetIconInfo(hIcon, &iconInfo);
            int iconWidth = iconInfo.xHotspot * 2;
            int iconHeight = iconInfo.yHotspot * 2;

            // Создаем Bitmap для размещения изображения иконки
            iconBitmap = gcnew Bitmap(iconWidth, iconHeight, Imaging::PixelFormat::Format32bppArgb);

            // Рисуем иконку на Bitmap
            Graphics^ g = Graphics::FromImage(iconBitmap);
            g->DrawIcon(System::Drawing::Icon::FromHandle(IntPtr(hIcon)), System::Drawing::Rectangle(0, 0, iconWidth, iconHeight));
            delete g;

            // Устанавливаем изображение в PictureBox
            pictureBox->Image = iconBitmap;

            // Освобождаем дескриптор иконки
            DestroyIcon(hIcon);
        }
        else {
            pictureBox->Image = gcnew Bitmap("sys.png");
        }
    }

    void MuteAudioSession(int sessionIndex) {
        HRESULT hr;
        IMMDeviceEnumerator* pDeviceEnumerator = NULL;
        IMMDevice* pDevice = NULL;
        IAudioSessionManager2* pSessionManager = NULL;
        IAudioSessionEnumerator* pSessionEnumerator = NULL;

        CoInitialize(NULL);

        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
        hr = pDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pSessionManager);
        hr = pSessionManager->GetSessionEnumerator(&pSessionEnumerator);

        int sessionCount;
        pSessionEnumerator->GetCount(&sessionCount);

        if (sessionIndex >= 0 && sessionIndex < sessionCount) {
            IAudioSessionControl* pSessionControl = NULL;
            ISimpleAudioVolume* pSimpleAudioVolume = NULL;

            pSessionEnumerator->GetSession(sessionIndex, &pSessionControl);
            pSessionControl->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&pSimpleAudioVolume);

            BOOL isMuted;
            pSimpleAudioVolume->GetMute(&isMuted);
            pSimpleAudioVolume->SetMute(!isMuted, NULL);

            pSimpleAudioVolume->Release();
            pSessionControl->Release();
        }

        pSessionEnumerator->Release();
        pSessionManager->Release();
        pDevice->Release();
        pDeviceEnumerator->Release();

        CoUninitialize();
    }

    void GetAudioSessionMuteStates(cli::array<bool>^ Mutes) {
        HRESULT hr;
        IMMDeviceEnumerator* pDeviceEnumerator = NULL;
        IMMDevice* pDevice = NULL;
        IAudioSessionManager2* pSessionManager = NULL;
        IAudioSessionEnumerator* pSessionEnumerator = NULL;

        CoInitialize(NULL);

        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
        hr = pDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pSessionManager);
        hr = pSessionManager->GetSessionEnumerator(&pSessionEnumerator);

        int sessionCount;
        pSessionEnumerator->GetCount(&sessionCount);

        for (int i = 0; i < sessionCount; i++) {
            IAudioSessionControl* pSessionControl = NULL;
            ISimpleAudioVolume* pSimpleAudioVolume = NULL;

            pSessionEnumerator->GetSession(i, &pSessionControl);
            pSessionControl->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&pSimpleAudioVolume);

            BOOL muteState;
            pSimpleAudioVolume->GetMute(&muteState);
            Mutes[i] = muteState;

            pSimpleAudioVolume->Release();
            pSessionControl->Release();
        }

        pSessionEnumerator->Release();
        pSessionManager->Release();
        pDevice->Release();
        pDeviceEnumerator->Release();

        CoUninitialize();
    }

    void Updates(int CDiff) {

        Array::Clear(Volumes, 0, Volumes->Length);
        Array::Clear(Names, 0, Names->Length);
        Array::Clear(Icons, 0, Icons->Length);

        GetAudioSessionInfo(Volumes, Names, Icons);

        if (CDiff < 0) {
            for (int l = CountOfSeanse - CDiff - 1; l >= CountOfSeanse; l--) {
                LC[l]->Controls->Remove(IconsOf[l]);
                VC[l]->Controls->Remove(LC[l]);
                MainPan->Controls->Remove(VC[l]);

                DES(IconsOf, l);
                DES(LC, l);
                DES(VC, l);

                coef -= 65;

                Index--;
            }
            if (10 + coef < System::Windows::Forms::Screen::PrimaryScreen->Bounds.Height - 80) {
                Over = false;
                Height = 10 + coef;
                MainPan->Size = System::Drawing::Size(92, -5 + coef);
                SetRegion();
                SetMiddle();
            }
            else {
                Over = true;
            }
        }

        else if (CDiff > 0) {
            for (int l = CountOfSeanse - CountDiff; l < CountOfSeanse; l++) {
                VC[l] = gcnew VPB();
                VC[l]->Location = System::Drawing::Point(0, 0 + coef);
                VC[l]->Name = "VC" + l;
                VC[l]->Size = System::Drawing::Size(60, 60);
                VC[l]->BackColor = System::Drawing::Color::Transparent;
                VC[l]->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::PicTes_Paint);
                MainPan->Controls->Add(VC[l]);

                LC[l] = gcnew LPB();
                LC[l]->Location = System::Drawing::Point(0, 0);
                LC[l]->Name = "LC" + l;
                LC[l]->Size = System::Drawing::Size(60, 60);
                LC[l]->BackColor = System::Drawing::Color::Transparent;
                LC[l]->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::PicTes_Paint);
                VC[l]->Controls->Add(LC[l]);

                IconsOf[l] = gcnew PictureBox();
                IconsOf[l]->Location = System::Drawing::Point(14, 14);
                IconsOf[l]->Name = "Icon" + l;
                IconsOf[l]->Size = System::Drawing::Size(32, 32);
                IconsOf[l]->BackColor = System::Drawing::Color::Transparent;
                LC[l]->Controls->Add(IconsOf[l]);
                AutoMuteState(IconsOf[i]);
                IconsOf[l]->DoubleClick += gcnew System::EventHandler(this, &MainForm::IconClick);
                IconsOf[l]->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::IconMouseDown);
                IconsOf[l]->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::IconMouseMove);
                IconsOf[l]->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pictureBox1_MouseWheel);
                IconsOf[l]->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::Icon_MouseUp);

                coef += 65;
            }

            if (Height + 65 < System::Windows::Forms::Screen::PrimaryScreen->Bounds.Height - 80) {
                Over = false;
                Height = 10 + coef;
                MainPan->Size = System::Drawing::Size(92, -5 + coef);
                SetRegion();
                SetMiddle();
            }
            else {
                Over = true;
            }
        }

        for (int i = 0; i < CountOfSeanse; i++) {

            IconsOf[i]->Name = "Icon" + i;
            ExtractIconAndSetImage(Icons[i], IconsOf[i]);

            VC[i]->Name = "VC" + i;


            LC[i]->Name = "LC" + i;

        }

        CountDiff = 0;
        start = false;
    }

    int StartX = 0;
    int MainPanStartX = 0;

    int OldWidth = 0;
    int OldRight = 0;

	private: System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e) {
        System::Type^ controlType = System::Windows::Forms::Control::typeid;
        controlType->InvokeMember("DoubleBuffered",
            System::Reflection::BindingFlags::SetProperty | System::Reflection::BindingFlags::Instance | System::Reflection::BindingFlags::NonPublic,
            nullptr, MainPan, gcnew cli::array<Object^>{ true });
        Height = 10;
        SetRegion();
        Width = 85;	
        MainPanStartX = MainPan->Location.X;
		SetMiddle();
        GetAudioSessionInfo(Volumes, Names, Icons);
        GetSC();
        OldCount = CountOfSeanse;
        Open();
        UpdateLoud->Enabled = true;
        StartX = Location.X;
        TopMost = true;
        SideSlideAudio::Rectangle screenBounds = System::Windows::Forms::Screen::PrimaryScreen->Bounds;
        OldWidth = screenBounds.Width;
        OldRight = screenBounds.Right;
	}

	void SetRegion()
	{
		Drawing::Drawing2D::GraphicsPath^ path = gcnew Drawing::Drawing2D::GraphicsPath();
		int cornerRadius = 30;
		int width = this->Width;
		int height = this->Height;

		path->StartFigure();
		path->AddArc(0, 0, cornerRadius, cornerRadius, 180, 90);
		path->AddLine(cornerRadius, 0, width - cornerRadius, 0);
		path->AddArc(width - cornerRadius, 0, cornerRadius, cornerRadius, 270, 90);
		path->AddLine(width, cornerRadius, width, height - cornerRadius);
		path->AddArc(width - cornerRadius, height - cornerRadius, cornerRadius, cornerRadius, 0, 90);
		path->AddLine(width - cornerRadius, height, cornerRadius, height);
		path->AddArc(0, height - cornerRadius, cornerRadius, cornerRadius, 90, 90);
		path->AddLine(0, height - cornerRadius, 0, cornerRadius);
		path->CloseFigure();

		this->Region = gcnew Drawing::Region(path);
	}

	private: System::Void MainForm_Shown(System::Object^ sender, System::EventArgs^ e) {
	}

	void SetMiddle() {
        SideSlideAudio::Rectangle screenBounds = System::Windows::Forms::Screen::PrimaryScreen->Bounds;
        MainPan->Location = System::Drawing::Point(MainPanStartX + (Side ? 0 : 14), MainPan->Location.Y);
        if (Side) {
            int formX = screenBounds.Right - this->Width;
            int formY = screenBounds.Y + (screenBounds.Height - this->Height) / 2;

            this->Location = Point(formX + 15, formY);
        }
        else {
            int formX = screenBounds.Left;
            int formY = screenBounds.Y + (screenBounds.Height - this->Height) / 2;

            this->Location = Point(formX - 15, formY);
        }
    }

    cli::array<PictureBox^>^ IconsOf = gcnew cli::array<PictureBox^>(100);
    cli::array<VPB^>^ VC = gcnew cli::array<VPB^>(100);
    cli::array<LPB^>^ LC = gcnew cli::array<LPB^>(100);

    void Open() {

        /*Vol1->Controls->Add(this->Loud1);
        Loud1->Controls->Add(this->Tst1);
        Tst1->Location = System::Drawing::Point(14, 14);
        Tst1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Main::pictureBox1_MouseWheel);

        ExtractIconAndSetImage(Icons[1], Tst1);*/

        for (int i = 0; i < CountOfSeanse; i++) {

            TrigNam[i] = gcnew TSC();
            TrigNam[i]->SC = Names[i];

            VC[i] = gcnew VPB();
            VC[i]->Location = System::Drawing::Point(0, 0 + coef);
            VC[i]->Name = "VC" + i;
            VC[i]->Size = System::Drawing::Size(60, 60);
            VC[i]->BackColor = System::Drawing::Color::Transparent;
            VC[i]->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::PicTes_Paint);
            MainPan->Controls->Add(VC[i]);

            LC[i] = gcnew LPB();
            LC[i]->Location = System::Drawing::Point(0, 0);
            LC[i]->Name = "LC" + i;
            LC[i]->Size = System::Drawing::Size(60, 60);
            LC[i]->BackColor = System::Drawing::Color::Transparent;
            LC[i]->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::PicTes_Paint);
            VC[i]->Controls->Add(LC[i]);

            IconsOf[i] = gcnew PictureBox();
            IconsOf[i]->Location = System::Drawing::Point(14, 14);
            IconsOf[i]->Name = "Icon" + i;
            IconsOf[i]->Size = System::Drawing::Size(32, 32);
            IconsOf[i]->BackColor = System::Drawing::Color::Transparent;
            LC[i]->Controls->Add(IconsOf[i]);
            AutoMuteState(IconsOf[i]);
            IconsOf[i]->DoubleClick += gcnew System::EventHandler(this, &MainForm::IconClick);
            IconsOf[i]->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::IconMouseDown);
            IconsOf[i]->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::IconMouseMove);
            IconsOf[i]->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pictureBox1_MouseWheel);
            IconsOf[i]->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::Icon_MouseUp);

            coef += 65;
               
            if (coef < System::Windows::Forms::Screen::PrimaryScreen->Bounds.Height - 80) {
                Over = false;
                Height = 10 + coef;
                MainPan->Size = System::Drawing::Size(92, -5 + coef);
                SetRegion();
                SetMiddle();
            }
            else {
                Over = true;
            }
        }

        CountDiff = 0;
        start = false;

    }

    template<typename T>
    void DES(cli::array<T>^ arr, int indexToDelete)
    {
        for (int i = indexToDelete; i < arr->Length - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
    }

void UV() {
    for (int i = 0; i < CountOfSeanse; i++) {
        try {
            int num = (System::Single::Parse(Volumes[i]) * 100);
            VC[i]->BackgroundImage = gcnew Bitmap("Resources\\Vol\\Vol_" + num + ".png");
        }
        catch (System::ArgumentNullException^) {
            continue;
        } 
    } 
}

private: System::Void pictureBox1_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
    if ((Control::ModifierKeys & Keys::Control) != Keys::Control) {
        return;
    }

    PictureBox^ TB = safe_cast<PictureBox^>(sender);

    int ind = System::Convert::ToInt32(TB->Name->Substring(4));

    float value = VC[ind]->Voll * 100.0;

    if (e->Delta > 0 && value != 100) {
        value++;
    }
    else if (e->Delta < 0 && value != 0) {
        value--;
    }

    VC[ind]->Voll = value / 100.0;
}


int startY = 0;
int value = 100;
int prevY = 0;

private: System::Void IconMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
    PictureBox^ TB = safe_cast<PictureBox^>(sender);
    int ind = System::Convert::ToInt32(TB->Name->Substring(4));

    if (e->Button == System::Windows::Forms::MouseButtons::Left) {
        // Запомните начальную позицию курсора
        startY = e->Y;
        prevY = 0;
        value = System::Single::Parse(Volumes[ind]) * 100;
        isEntered = true;
    }
}

private: System::Void IconMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
    PictureBox^ TB = safe_cast<PictureBox^>(sender);
    int ind = System::Convert::ToInt32(TB->Name->Substring(4));
    float tempv = VC[ind]->Voll * 100.0;
    value = round(tempv);
    if (e->Button == System::Windows::Forms::MouseButtons::Left) {

        if (e->Y > prevY) {
            value -= 1;
        }
        else if (e->Y < prevY) {
            value += 1;
        }

        if (value < 0) value = 0;
        if (value > 100) value = 100;

        VC[ind]->Voll = value / 100.0;

        prevY = e->Y;
    }

}

void AutoMuteState(PictureBox^ Tst) {
        int ind = System::Convert::ToInt32(Tst->Name->Substring(4));
        GetAudioSessionMuteStates(Mutes);
        ExtractIconAndSetImage(Icons[ind], Tst);
        if (Mutes[ind]) {
            GrowIcon(Tst);
        }
}

private: System::Void IconClick(System::Object^ sender, System::EventArgs^ e) {
    PictureBox^ TB = safe_cast<PictureBox^>(sender);
    int ind = System::Convert::ToInt32(TB->Name->Substring(4));
    MuteAudioSession(ind);
    GetAudioSessionMuteStates(Mutes);
    if (!Mutes[ind]) {
        ExtractIconAndSetImage(Icons[ind], TB);
    }
    else {
        GrowIcon(TB);
    }
}

void GrowIcon(PictureBox^ Tst) {
    Bitmap^ image = gcnew Bitmap(Tst->Image);
    for (int y = 0; y < image->Height; y++)
    {
        for (int x = 0; x < image->Width; x++)
        {
            Color pixelColor = image->GetPixel(x, y);
            int grayScale = (int)((pixelColor.R * 0.3) + (pixelColor.G * 0.59) + (pixelColor.B * 0.11));
            Color newColor = Color::FromArgb(pixelColor.A, grayScale, grayScale, grayScale);
            image->SetPixel(x, y, newColor);
        }
    }
    Tst->Image = image;
}

private: System::Void UpdateLoud_Tick(System::Object^ sender, System::EventArgs^ e) {
    GetSC();
    try {
        GetNames();
        if (start) {
            Updates(CountDiff);
        }
        RNT();
    }
    catch (...) {

    }
    
}
    
private: System::Void UpdateVolume_Tick(System::Object^ sender, System::EventArgs^ e) {  
    SideSlideAudio::Rectangle screenBounds = System::Windows::Forms::Screen::PrimaryScreen->Bounds;
    if (screenBounds.Right != OldRight || screenBounds.Width != OldWidth) {
        OldWidth = screenBounds.Width;
        OldRight = screenBounds.Right;
        HideThis->Enabled = false;
        ShowThis->Enabled = false;
        SetMiddle();
        StartX = Location.X;
        Location = System::Drawing::Point(StartX + 70, Location.Y);
    }
}

    bool FullSC()
    {
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        HWND hwnd = WindowFromPoint(cursorPos);
        if (hwnd == GetDesktopWindow())
        {
            return false;
        }

        LONG style = GetWindowLong(hwnd, GWL_STYLE);
        RECT rect;
        GetWindowRect(hwnd, &rect);

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        return (style & WS_POPUP) && (rect.right - rect.left == screenWidth) && (rect.bottom - rect.top == screenHeight);
    }

private: System::Void ShowIt_Tick(System::Object^ sender, System::EventArgs^ e) {
    POINT cursorPos;
    GetCursorPos(&cursorPos);  

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);  

    int threshold = 1;

    if (Side) {
        if (screenWidth - cursorPos.x <= threshold && !FullSC()) {
            HideThis->Enabled = true; //Показать
        }

        else if (!this->ClientRectangle.Contains(this->PointToClient(Control::MousePosition)) && !isEntered) {
            UpdateLoud->Enabled = false;
            ShowThis->Enabled = true; //Скрыть
        }
    }
    else {
        if (cursorPos.x <= threshold && !FullSC()) {
            ShowThis->Enabled = true; //Показать
        }

        else if (!this->ClientRectangle.Contains(this->PointToClient(Control::MousePosition)) && !isEntered) {
            UpdateLoud->Enabled = false;
            HideThis->Enabled = true; //Скрыть
        }
    }

}

private: System::Void Icon_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
    isEntered = false;
}
private: System::Void InTray_DoubleClick(System::Object^ sender, System::EventArgs^ e) {
    Application::Exit();
}

       int i = 0;

private: System::Void ShowThis_Tick(System::Object^ sender, System::EventArgs^ e) {
    if (i <= (Side ? 70 : 0)) {
        Location = System::Drawing::Point(StartX + i, Location.Y);
        i+=5;
    }
    else {
        ShowThis->Enabled = false;
        UpdateLoud->Enabled = (Side ? false : true);
    }
}

private: System::Void HideThis_Tick(System::Object^ sender, System::EventArgs^ e) {
    if (i >= (Side ? 0 : -70)) {
        Location = System::Drawing::Point(StartX + i, Location.Y);
        i-=5;
    }

    else {
        i = (Side ? 0 : -70);
        HideThis->Enabled = false;
        UpdateLoud->Enabled = (Side ? true : false);
    }
}
private: System::Void PicTes_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
    
    PictureBox^ pictureBox = dynamic_cast<PictureBox^>(sender);
    Graphics^ g = e->Graphics;

    string nam = marshal_as<string>(pictureBox->Name->Substring(0, 2));
    int ind = System::Convert::ToInt64(pictureBox->Name->Substring(2));

    int angle = static_cast<int>(360.0 * (nam == "LC" ? LC[ind]->ProgressLoud : VC[ind]->ProgressVol) / 100);

    int diameter = (nam == "LC" ? 51 : 57);
    int x = (pictureBox->Width - diameter) / 2;
    int y = (pictureBox->Height - diameter) / 2;

    g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

    int penWidth = 3;
    Pen^ penBlue = gcnew Pen((nam == "LC" ? Color::Gray : Color::Yellow), penWidth);
    g->DrawArc(penBlue, x, y, diameter, diameter, -90, angle);
}
private: System::Void MainForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
    int dy = 60;

    Point scrollPos = MainPan->AutoScrollPosition;

    if (e->KeyCode == Keys::W) { //Ес чё Over - индикатор переполнения панели
       MainPan->AutoScrollPosition = Point(-scrollPos.X, -scrollPos.Y - dy);
    }
    else if (e->KeyCode == Keys::S) {
        MainPan->AutoScrollPosition = Point(-scrollPos.X, -scrollPos.Y + dy);
    }
}

    bool keepOpen = false;

private: System::Void MainTray_Closing(System::Object^ sender, System::Windows::Forms::ToolStripDropDownClosingEventArgs^ e) {
    if (e->CloseReason == ToolStripDropDownCloseReason::ItemClicked && keepOpen)
    {
        e->Cancel = true;
    }
    else {
        e->Cancel = false;
    }
}
private: System::Void Item1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
    keepOpen = true;
    Item1->Checked = true;
    Item2->Checked = false;
}
private: System::Void Item2_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
    keepOpen = true;
    Item2->Checked = true;
    Item1->Checked = false;
}
};
}
