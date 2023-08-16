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
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Gdiplus.lib")
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

	/// <summary>
	/// Сводка для MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
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
            this->UpdateLoud->Interval = 1;
            this->UpdateLoud->Tick += gcnew System::EventHandler(this, &MainForm::UpdateLoud_Tick);
            // 
            // UpdateVolume
            // 
            this->UpdateVolume->Interval = 10;
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

    cli::array<String^>^ Volumes = gcnew cli::array<String^>(100);
    cli::array<String^>^ Names = gcnew cli::array<String^>(100);
    cli::array<String^>^ Icons = gcnew cli::array<String^>(100);
    cli::array<String^>^ Louds = gcnew cli::array<String^>(100);
    cli::array<bool>^ Mutes = gcnew cli::array<bool>(100);

    void RecordNoiseLevels(cli::array<String^>^ Louds) {
        HRESULT hr;
        IMMDeviceEnumerator* pDeviceEnumerator = NULL;
        IMMDevice* pDevice = NULL;
        IAudioSessionManager2* pSessionManager = NULL;
        IAudioSessionEnumerator* pSessionEnumerator = NULL;
        IAudioSessionControl* pSessionControl = NULL;
        IAudioMeterInformation* pMeterInfo = NULL;
        int sessionCount;
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

        hr = pSessionEnumerator->GetCount(&sessionCount);
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

            Louds[i] = peak.ToString();

            SAFE_RELEASE(pMeterInfo)
                SAFE_RELEASE(pSessionControl)
        }

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

        CountOfSeanse = sessionCount;

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

    void SetAudioSessionVolume(int sessionIndex, float volume) {
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

        pSessionEnumerator->GetSession(sessionIndex, &pSessionControl);
        pSessionControl->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&pSimpleAudioVolume);

        pSimpleAudioVolume->SetMasterVolume(volume, NULL);

        pSimpleAudioVolume->Release();
        pSessionControl->Release();

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

    void Update(bool Way) {
        if (Way) {

            int i = CountOfSeanse - 1;

            VC[i] = gcnew PictureBox();
            VC[i]->Location = System::Drawing::Point(0, 10 + coef);
            VC[i]->Name = "VC" + i;
            VC[i]->Size = System::Drawing::Size(60, 60);
            VC[i]->BackColor = System::Drawing::Color::Transparent;
            MainPan->Controls->Add(VC[i]);

            LC[i] = gcnew PictureBox();
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
            MainPan->Size = System::Drawing::Size(92, 10 + coef);
            SetRegion();
            SetMiddle();

            for (int i = 0; i < CountOfSeanse; i++) {

                IconsOf[i]->Name = "Icon" + i;
                ExtractIconAndSetImage(Icons[i], IconsOf[i]);

                VC[i]->Name = "VC" + i;


                LC[i]->Name = "LC" + i;

            }

            VC[i]->BackgroundImage = gcnew Bitmap("Resources\\Vol\\Vol_" + 50 + ".png");

            Height = 10 + coef;
            MainPan->Size = System::Drawing::Size(92, 10 + coef);
            SetRegion();
            SetMiddle();

        }
        else {
            LC[CountOfSeanse]->Controls->Remove(IconsOf[CountOfSeanse]);
            VC[CountOfSeanse]->Controls->Remove(LC[CountOfSeanse]);
            MainPan->Controls->Remove(VC[CountOfSeanse]);
            DES(IconsOf, CountOfSeanse);
            DES(LC, CountOfSeanse);
            DES(VC, CountOfSeanse);
            coef = 0;
            for (int i = 0; i < CountOfSeanse; i++) {
                IconsOf[i]->Location = System::Drawing::Point(14, 14);
                IconsOf[i]->Name = "Icon" + i;
                ExtractIconAndSetImage(Icons[i], IconsOf[i]);

                VC[i]->Location = System::Drawing::Point(0, 10 + coef);
                VC[i]->Name = "VC" + i;

                LC[i]->Location = System::Drawing::Point(0, 0);
                LC[i]->Name = "LC" + i;

                coef += 65;
            }
            Height = 10 + coef;
            MainPan->Size = System::Drawing::Size(92, 10 + coef);
            SetRegion();
            SetMiddle();
        }
    }

	private: System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e) {
        Width = 70;
        Height = 10;
		SetRegion();
		SetMiddle();
        GetAudioSessionInfo(Volumes, Names, Icons);
        OldCount = CountOfSeanse;
        Open();
        RecordNoiseLevels(Louds);
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
    cli::array<PictureBox^>^ VC = gcnew cli::array<PictureBox^>(100);
    cli::array<PictureBox^>^ LC = gcnew cli::array<PictureBox^>(100);

    void Open() {

        /*Vol1->Controls->Add(this->Loud1);
        Loud1->Controls->Add(this->Tst1);
        Tst1->Location = System::Drawing::Point(14, 14);
        Tst1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Main::pictureBox1_MouseWheel);

        ExtractIconAndSetImage(Icons[1], Tst1);*/

        for (int i = 0; i < CountOfSeanse; i++) {

            VC[i] = gcnew PictureBox();
            VC[i]->Location = System::Drawing::Point(0, 10 + coef);
            VC[i]->Name = "VC" + i;
            VC[i]->Size = System::Drawing::Size(60, 60);
            VC[i]->BackColor = System::Drawing::Color::Transparent;
            MainPan->Controls->Add(VC[i]);

            LC[i] = gcnew PictureBox();
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
            int num = (System::Single::Parse(Volumes[i]) * 100);
            VC[i]->BackgroundImage = gcnew Bitmap("Resources\\Vol\\Vol_" + num + ".png");
        } 
    }

    private: System::Void pictureBox1_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
        PictureBox^ TB = safe_cast<PictureBox^>(sender);
        int ind = System::Convert::ToInt32(TB->Name->Substring(4));

        Array::Clear(Volumes, 0, Volumes->Length);
        GetAudioSessionInfo(Volumes, Names, Icons);
        float num = System::Single::Parse(Volumes[ind]);
        float value = num * 100.0;
        if (e->Delta > 0 && value != 100) {
            value++;
        }
        else if (e->Delta < 0 && value != 0) {
            value--;
        }
        SetAudioSessionVolume(ind, value / 100.0);

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
        }
    }

    private: System::Void IconMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
        PictureBox^ TB = safe_cast<PictureBox^>(sender);
        int ind = System::Convert::ToInt32(TB->Name->Substring(4));

        if (e->Button == System::Windows::Forms::MouseButtons::Left) {

            if (e->Y > prevY) {
                value -= 2;
            }
            else if (e->Y < prevY) {
                value += 2;
            }

            if (value < 0) value = 0;
            if (value > 100) value = 100;

            SetAudioSessionVolume(ind, value / 100.0);
            UV();

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
        Array::Clear(Louds, 0, Louds->Length);
        for (int i = 0; i < CountOfSeanse; i++) {
            try {
                RecordNoiseLevels(Louds);
                int num = (System::Single::Parse(Louds[i]) * 100);
                String^ way = "Resources\\Loud\\Loud_" + num + ".png";
                LC[i]->BackgroundImage = gcnew Bitmap(way);
            }
            catch (System::ArgumentNullException^) {

            }
        }
    }
private: System::Void UpdateVolume_Tick(System::Object^ sender, System::EventArgs^ e) {
    Array::Clear(Volumes, 0, Volumes->Length);
    Array::Clear(Names, 0, Names->Length);
    Array::Clear(Icons, 0, Icons->Length);
    GetAudioSessionInfo(Volumes, Names, Icons);
    if (OldCount != CountOfSeanse) {
        if (CountOfSeanse > OldCount) {
            Update(true);
        }
        else {
            Update(false);
        }
        OldCount = CountOfSeanse;
    }
    UV();
}
};
}
