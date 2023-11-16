#include "PeApplication.h"
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    PeApplication::GetApp().InitApp(hInstance, nCmdShow);
    PeApplication::GetApp().CreateMainDlg();

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
    }
}