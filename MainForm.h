#pragma once
#include <windows.h>
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
	/// <summary>
	/// ������ ��� MainForm
	/// </summary>
    int Index = 0;
    static int CountDiff = 0;
    static bool start = false;

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
                String^ way = "Resources\\Loud\\Loud_" + ((num == 97 ) ? 100 : num) + ".png";
                BackgroundImage = gcnew Bitmap(way);
            }
        };

        ref class VPB : public PictureBox
        {
        private:
            float voll;
            int ind;

        public:
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
                //��� ������ ������-�� ����� ������ ������ �������
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
                BackgroundImage = gcnew Bitmap("Resources\\Vol\\Vol_" + num + ".png");
            }
        };

	public:
		MainForm(void)
		{
			InitializeComponent();
            DoubleBuffered = true;
			//
			//TODO: �������� ��� ������������
			//
		}

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
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
    private: System::ComponentModel::IContainer^ components;
    protected:

	protected:

	private:
		/// <summary>
		/// ������������ ���������� ������������.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������ � �� ��������� 
		/// ���������� ����� ������ � ������� ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
            this->MainPan = (gcnew System::Windows::Forms::Panel());
            this->UpdateLoud = (gcnew System::Windows::Forms::Timer(this->components));
            this->UpdateVolume = (gcnew System::Windows::Forms::Timer(this->components));
            this->SuspendLayout();
            // 
            // MainPan
            // 
            this->MainPan->AutoScroll = true;
            this->MainPan->Location = System::Drawing::Point(6, 0);
            this->MainPan->Name = L"MainPan";
            this->MainPan->Size = System::Drawing::Size(86, 60);
            this->MainPan->TabIndex = 0;
            // 
            // UpdateLoud
            // 
            this->UpdateLoud->Interval = 33;
            this->UpdateLoud->Tick += gcnew System::EventHandler(this, &MainForm::UpdateLoud_Tick);
            // 
            // UpdateVolume
            // 
            this->UpdateVolume->Tick += gcnew System::EventHandler(this, &MainForm::UpdateVolume_Tick);
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
            this->Name = L"MainForm";
            this->Text = L"SSA";
            this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
            this->Shown += gcnew System::EventHandler(this, &MainForm::MainForm_Shown);
            this->ResumeLayout(false);

        }
#pragma endregion

    int CountOfSeanse = 0;
    int OldCount = 0;
    int coef = 0;

    cli::array<TSC^>^ TrigNam = gcnew cli::array<TSC^>(100);

    cli::array<String^>^ Names = gcnew cli::array<String^>(100);
    cli::array<String^>^ Icons = gcnew cli::array<String^>(100);
    cli::array<bool>^ Mutes = gcnew cli::array<bool>(100);

    //��������� ���� � ��������� � ����� �������
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
            hr = pSessionEnumerator->GetSession(i, &pSessionControl);
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
            catch (System::AccessViolationException^) {
                throw gcnew System::AccessViolationException;
            }

            float volume;
            pSimpleAudioVolume->GetMasterVolume(&volume);
            try {
                VC[i]->Voll = volume;
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

            pSessionEnumerator->GetSession(i, &pSessionControl);
            pSessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&pSessionControl2);


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

            // �������� ������ ������
            ICONINFO iconInfo;
            GetIconInfo(hIcon, &iconInfo);
            int iconWidth = iconInfo.xHotspot * 2;
            int iconHeight = iconInfo.yHotspot * 2;

            // ������� Bitmap ��� ���������� ����������� ������
            iconBitmap = gcnew Bitmap(iconWidth, iconHeight, Imaging::PixelFormat::Format32bppArgb);

            // ������ ������ �� Bitmap
            Graphics^ g = Graphics::FromImage(iconBitmap);
            g->DrawIcon(System::Drawing::Icon::FromHandle(IntPtr(hIcon)), System::Drawing::Rectangle(0, 0, iconWidth, iconHeight));
            delete g;

            // ������������� ����������� � PictureBox
            pictureBox->Image = iconBitmap;

            // ����������� ���������� ������
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

            Height = 10 + coef;
            MainPan->Size = System::Drawing::Size(92, 10 + coef);
            SetRegion();
            SetMiddle();
        }

        else if (CDiff > 0) {
            for (int l = CountOfSeanse - CountDiff; l < CountOfSeanse; l++) {
                VC[l] = gcnew VPB();
                VC[l]->Location = System::Drawing::Point(0, 10 + coef);
                VC[l]->Name = "VC" + l;
                VC[l]->Size = System::Drawing::Size(60, 60);
                VC[l]->BackColor = System::Drawing::Color::Transparent;
                MainPan->Controls->Add(VC[l]);

                LC[l] = gcnew LPB();
                LC[l]->Location = System::Drawing::Point(0, 0);
                LC[l]->Name = "LC" + l;
                LC[l]->Size = System::Drawing::Size(60, 60);
                LC[l]->BackColor = System::Drawing::Color::Transparent;
                VC[l]->Controls->Add(LC[l]);

                IconsOf[l] = gcnew PictureBox();
                IconsOf[l]->Location = System::Drawing::Point(14, 14);
                IconsOf[l]->Name = "Icon" + l;
                IconsOf[l]->Size = System::Drawing::Size(32, 32);
                IconsOf[l]->BackColor = System::Drawing::Color::Transparent;
                LC[l]->Controls->Add(IconsOf[l]);
                ExtractIconAndSetImage(Icons[l], IconsOf[l]);
                IconsOf[l]->DoubleClick += gcnew System::EventHandler(this, &MainForm::IconClick);
                IconsOf[l]->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::IconMouseDown);
                IconsOf[l]->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::IconMouseMove);
                IconsOf[l]->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pictureBox1_MouseWheel);

                coef += 65;
            }

            Height = 10 + coef;
            MainPan->Size = System::Drawing::Size(92, 10 + coef);
            SetRegion();
            SetMiddle();
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

	private: System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e) {

        System::Type^ controlType = System::Windows::Forms::Control::typeid;
        controlType->InvokeMember("DoubleBuffered",
            System::Reflection::BindingFlags::SetProperty | System::Reflection::BindingFlags::Instance | System::Reflection::BindingFlags::NonPublic,
            nullptr, MainPan, gcnew cli::array<Object^>{ true });

        Width = 70;
        Height = 10;
		SetRegion();
		SetMiddle();
        GetAudioSessionInfo(Volumes, Names, Icons);
        GetSC();
        OldCount = CountOfSeanse;
        Open();
        UpdateLoud->Enabled = true;
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

		int formX = screenBounds.Right - this->Width;
		int formY = screenBounds.Y + (screenBounds.Height - this->Height) / 2;

		this->Location = Point(formX, formY);
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
            VC[i]->Location = System::Drawing::Point(0, 10 + coef);
            VC[i]->Name = "VC" + i;
            VC[i]->Size = System::Drawing::Size(60, 60);
            VC[i]->BackColor = System::Drawing::Color::Transparent;
            MainPan->Controls->Add(VC[i]);

            LC[i] = gcnew LPB();
            LC[i]->Location = System::Drawing::Point(0, 0);
            LC[i]->Name = "LC" + i;
            LC[i]->Size = System::Drawing::Size(60, 60);
            LC[i]->BackColor = System::Drawing::Color::Transparent;
            VC[i]->Controls->Add(LC[i]);

            IconsOf[i] = gcnew PictureBox();
            IconsOf[i]->Location = System::Drawing::Point(14, 14);
            IconsOf[i]->Name = "Icon" + i;
            IconsOf[i]->Size = System::Drawing::Size(32, 32);
            IconsOf[i]->BackColor = System::Drawing::Color::Transparent;
            LC[i]->Controls->Add(IconsOf[i]);
            ExtractIconAndSetImage(Icons[i], IconsOf[i]);
            IconsOf[i]->DoubleClick += gcnew System::EventHandler(this, &MainForm::IconClick);
            IconsOf[i]->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::IconMouseDown);
            IconsOf[i]->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::IconMouseMove);
            IconsOf[i]->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pictureBox1_MouseWheel);

            coef += 65;
            Height = 10 + coef;
            MainPan->Size = System::Drawing::Size(92, 10+coef);
            SetRegion();
            SetMiddle();
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

void UV() { //��� � ����
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
    PictureBox^ TB = safe_cast<PictureBox^>(sender);

    int ind = System::Convert::ToInt32(TB->Name->Substring(4));

    float value = VC[ind]->Voll * 100.0;

    if (e->Delta > 0 && value != 100) {
        value++;
    }
    else if (e->Delta < 0 && value != 0) {
        value--;
    }

    VC[ind]->Voll = value/100.0;
}

int startY = 0;
int value = 100;
int prevY = 0;

private: System::Void IconMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
    PictureBox^ TB = safe_cast<PictureBox^>(sender);
    int ind = System::Convert::ToInt32(TB->Name->Substring(4));

    if (e->Button == System::Windows::Forms::MouseButtons::Left) {
        // ��������� ��������� ������� �������
        startY = e->Y;
        prevY = 0;
        value = System::Single::Parse(Volumes[ind]) * 100;
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
    catch (System::NullReferenceException^) {

    }
    catch (System::AccessViolationException^) {

    }
    
}

private: System::Void UpdateVolume_Tick(System::Object^ sender, System::EventArgs^ e) {  
    
}

};
}