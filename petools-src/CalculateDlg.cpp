#include "CalculateDlg.h"
#include "resource.h"
#include "AnalysePE.h"
#include <unordered_set>
import Utils;
extern HINSTANCE appInst;

WNDPROC RvaOldProc;
WNDPROC VaOldProc;
WNDPROC FoaOldProc;

CalculateDlg* CalculateDlg::thisDlg_ = nullptr;

CalculateDlg::CalculateDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_CALCULATE, hParent)
{

}

CalculateDlg::~CalculateDlg() {

}

void CalculateDlg::InitDlg() {
    SetThisDlg();
}

void CalculateDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)CalcProc);
}

void CalculateDlg::InitRadioButton() {
    CheckRadioButton(hCurrentDlg_, IDC_RADIO_VA, IDC_RADIO_FOA, IDC_RADIO_RVA);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_VA, EM_SETREADONLY, 1, 0);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_FOA, EM_SETREADONLY, 1, 0);

    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_VA, EM_LIMITTEXT, 8, 0);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_RVA, EM_LIMITTEXT, 8, 0);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_FOA, EM_LIMITTEXT, 8, 0);
}


/*
*   VA Radio Button event
*/
void CalculateDlg::OnClickVA() {
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), nullptr);
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), nullptr);
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), nullptr);
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_LOCATE), nullptr);

    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_VA, EM_SETREADONLY, 0, 0);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_RVA, EM_SETREADONLY, 1, 0);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_FOA, EM_SETREADONLY, 1, 0);
}

/*
*   RVA Radio Button event
*/
void CalculateDlg::OnClickRVA() {
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), nullptr);
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), nullptr);
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), nullptr);
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_LOCATE), nullptr);

    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_VA, EM_SETREADONLY, 1, 0);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_RVA, EM_SETREADONLY, 0, 0);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_FOA, EM_SETREADONLY, 1, 0);
}

/*
*   FOA Radio Button event
*/
void CalculateDlg::OnClickFOA() {
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), nullptr);
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), nullptr);
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), nullptr);
    SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_LOCATE), nullptr);

    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_VA, EM_SETREADONLY, 1, 0);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_RVA, EM_SETREADONLY, 1, 0);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_FOA, EM_SETREADONLY, 0, 0);
}

/*
*   计算按钮 event
*/
void CalculateDlg::OnClickCalc() {
    TCHAR editBuffer[9] = { 0 };
    if (IsDlgButtonChecked(hCurrentDlg_, IDC_RADIO_VA)) {
        SendMessage(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), WM_GETTEXT, 9, (LPARAM)editBuffer);
        DWORD VA = -1;
        TcharToDword(editBuffer, &VA, 16);
        FromVACalc(VA);

    }
    else if (IsDlgButtonChecked(hCurrentDlg_, IDC_RADIO_RVA)) {
        SendMessage(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), WM_GETTEXT, 9, (LPARAM)editBuffer);
        DWORD RVA = -1;
        TcharToDword(editBuffer, &RVA, 16);
        FromRVACalc(RVA);
    }
    else {
        SendMessage(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), WM_GETTEXT, 9, (LPARAM)editBuffer);
        DWORD FOA = -1;
        TcharToDword(editBuffer, &FOA, 16);
        FromFOACalc(FOA);
    }
}

void CalculateDlg::FromVACalc(DWORD VA) {
    DWORD RVA = VA - (DWORD)AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase;
    PositionInPE pos = AnalysePE::GetAnalyse().PositonInfoRVA(RVA);

    switch (pos) {
    case inHead:
    {
        TCHAR* locateName = const_cast<TCHAR*>(TEXT("Header"));
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_LOCATE), locateName);

        TCHAR RVABuffer[9] = { 0 };
        wsprintfW(RVABuffer, L"%08X", RVA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), RVABuffer);

        TCHAR FOABuffer[9] = { 0 };
        wsprintfW(FOABuffer, L"%08X", RVA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), FOABuffer);

        TCHAR VABuffer[9] = { 0 };
        wsprintfW(VABuffer, L"%08X", VA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), VABuffer);
        return;
    }
    case inSection:
    {
        TCHAR* locateName = nullptr;
        DWORD secIndex = AnalysePE::GetAnalyse().InWhichSectionRVA(RVA);
        AnalysePE::GetAnalyse().GetTcharSectionName(secIndex, &locateName);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_LOCATE), locateName);

        TCHAR RVABuffer[9] = { 0 };
        wsprintfW(RVABuffer, L"%08X", RVA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), RVABuffer);

        DWORD FOA = -1;
        FOA = AnalysePE::GetAnalyse().RVAToFOA(RVA);
        TCHAR FOABuffer[9] = { 0 };
        wsprintfW(FOABuffer, L"%08X", FOA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), FOABuffer);

        TCHAR VABuffer[9] = { 0 };
        wsprintfW(VABuffer, L"%08X", VA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), VABuffer);
        return;
    }
    case outFile:
    {
        MessageBox(0, TEXT("Out Of File!"), TEXT("Error"), MB_OK);
        return;
    }
    default:
        return;
    }


   /* DWORD secIndex = AnalysePE::GetAnalyse().InWhichSectionRVA(RVA);
    if (secIndex == outFile) {
        MessageBox(0, TEXT("Out Of File!"), TEXT("Error"), MB_OK);
    }
    else if (secIndex == inHead) {
        TCHAR* locateName = const_cast<TCHAR*>(TEXT("Header"));
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_LOCATE), locateName);

        TCHAR RVABuffer[9] = { 0 };
        wsprintfW(RVABuffer, L"%08X", RVA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), RVABuffer);

        TCHAR FOABuffer[9] = { 0 };
        wsprintfW(FOABuffer, L"%08X", RVA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), FOABuffer);

        TCHAR VABuffer[9] = { 0 };
        wsprintfW(VABuffer, L"%08X", VA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), VABuffer);
    }
    else {
        TCHAR* locateName = nullptr;
        AnalysePE::GetAnalyse().GetTcharSectionName(secIndex, &locateName);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_LOCATE), locateName);

        TCHAR RVABuffer[9] = { 0 };
        wsprintfW(RVABuffer, L"%08X", RVA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), RVABuffer);

        DWORD FOA = -1;
        FOA = AnalysePE::GetAnalyse().RVAToFOA(RVA);
        TCHAR FOABuffer[9] = { 0 };
        wsprintfW(FOABuffer, L"%08X", FOA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), FOABuffer);

        TCHAR VABuffer[9] = { 0 };
        wsprintfW(VABuffer, L"%08X", VA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), VABuffer);
    }*/
}

void CalculateDlg::FromRVACalc(DWORD RVA) {
    PositionInPE pos = AnalysePE::GetAnalyse().PositonInfoRVA(RVA);
    switch (pos) {
    case inHead:
    {
        TCHAR* locateName = const_cast<TCHAR*>(TEXT("Header"));
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_LOCATE), locateName);

        TCHAR RVABuffer[9] = { 0 };
        wsprintfW(RVABuffer, L"%08X", RVA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), RVABuffer);

        TCHAR FOABuffer[9] = { 0 };
        wsprintfW(FOABuffer, L"%08X", RVA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), FOABuffer);

        TCHAR VABuffer[9] = { 0 };
        wsprintfW(VABuffer, L"%08X", RVA + (DWORD)AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), VABuffer);
        return;
    }
    case inSection:
    {
        DWORD secIndex = AnalysePE::GetAnalyse().InWhichSectionRVA(RVA);
        TCHAR* locateName = nullptr;
        AnalysePE::GetAnalyse().GetTcharSectionName(secIndex, &locateName);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_LOCATE), locateName);

        TCHAR RVABuffer[9] = { 0 };
        wsprintfW(RVABuffer, L"%08X", RVA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), RVABuffer);

        DWORD FOA = -1;
        FOA = AnalysePE::GetAnalyse().RVAToFOA(RVA);
        TCHAR FOABuffer[9] = { 0 };
        wsprintfW(FOABuffer, L"%08X", FOA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), FOABuffer);

        TCHAR VABuffer[9] = { 0 };
        wsprintfW(VABuffer, L"%08X", RVA + (DWORD)AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), VABuffer);
        return;
    }
    case outFile:
    {
        MessageBox(0, TEXT("Out Of File!"), TEXT("Error"), MB_OK);
        return;
    }
    default:
        return;
    }
}

void CalculateDlg::FromFOACalc(DWORD FOA) {
    PositionInPE pos = AnalysePE::GetAnalyse().PositonInfoFOA(FOA);
    switch (pos) {
    case inHead:
    {
        TCHAR* locateName = const_cast<TCHAR*>(TEXT("Header"));
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_LOCATE), locateName);

        TCHAR RVABuffer[9] = { 0 };
        wsprintfW(RVABuffer, L"%08X", FOA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), RVABuffer);

        TCHAR FOABuffer[9] = { 0 };
        wsprintfW(FOABuffer, L"%08X", FOA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), FOABuffer);

        TCHAR VABuffer[9] = { 0 };
        wsprintfW(VABuffer, L"%08X", FOA + (DWORD)AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), VABuffer);
        return;
    }
    case inSection:
    {
        DWORD secIndex = AnalysePE::GetAnalyse().InWhichSectionFOA(FOA);
        TCHAR* locateName = nullptr;
        AnalysePE::GetAnalyse().GetTcharSectionName(secIndex, &locateName);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_LOCATE), locateName);

        DWORD RVA = -1;
        RVA = AnalysePE::GetAnalyse().FOAToRVA(FOA);
        TCHAR RVABuffer[9] = { 0 };
        wsprintfW(RVABuffer, L"%08X", RVA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_RVA), RVABuffer);

        TCHAR FOABuffer[9] = { 0 };
        wsprintfW(FOABuffer, L"%08X", FOA);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_FOA), FOABuffer);

        TCHAR VABuffer[9] = { 0 };
        wsprintfW(VABuffer, L"%08X", RVA + (DWORD)AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase);
        SetWindowText(GetDlgItem(hCurrentDlg_, IDC_EDIT_VA), VABuffer);
        return;
    }
    case outFile:
    {
        MessageBox(0, TEXT("Out Of File!"), TEXT("Error"), MB_OK);
        return;
    }
    default:
        return;
    }
}

LRESULT CALLBACK CalculateDlg::CalcProc(HWND hCalc, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG:
        // calculate窗口子类化
        RvaOldProc = (WNDPROC)SetWindowLong(GetDlgItem(hCalc, IDC_EDIT_RVA), GWL_WNDPROC, (LONG)RvaNewProc);
        VaOldProc = (WNDPROC)SetWindowLong(GetDlgItem(hCalc, IDC_EDIT_VA), GWL_WNDPROC, (LONG)VaNewProc);
        FoaOldProc = (WNDPROC)SetWindowLong(GetDlgItem(hCalc, IDC_EDIT_FOA), GWL_WNDPROC, (LONG)FoaNewProc);

        thisDlg_->SetCurrentDlgHWND(hCalc);
        thisDlg_->InitRadioButton();
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_RADIO_VA:
            thisDlg_->OnClickVA();
            break;
        case IDC_RADIO_RVA:
            thisDlg_->OnClickRVA();
            break;
        case IDC_RADIO_FOA:
            thisDlg_->OnClickFOA();
            break;
        case IDC_BUTTON_CALC:
            thisDlg_->OnClickCalc();
            break;
        case IDCANCEL:
            thisDlg_->CloseDlg();
            break;
        }
        break;
    }
    case WM_CLOSE:
        thisDlg_->CloseDlg();
        break;
    default:
        return FALSE;
    }
    return FALSE;
}

/*
*   实现edit控件的输入限制，hex only
*/
LRESULT CALLBACK CalculateDlg::RvaNewProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    TCHAR tCh;
    switch (message)
    {
    case WM_CHAR:
        tCh = (TCHAR)wParam;
        if (!IsTcharHex(tCh))
            return 0;
        break;
    }
    return CallWindowProc(RvaOldProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK CalculateDlg::VaNewProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    TCHAR tCh;
    switch (message)
    {
    case WM_CHAR:
        tCh = (TCHAR)wParam;
        if (!IsTcharHex(tCh))
            return 0;
        break;
    }
    return CallWindowProc(VaOldProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK CalculateDlg::FoaNewProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    TCHAR tCh;
    switch (message)
    {
    case WM_CHAR:
        tCh = (TCHAR)wParam;
        if (!IsTcharHex(tCh))
            return 0;
        break;
    }
    return CallWindowProc(FoaOldProc, hwnd, message, wParam, lParam);
}

/*
*   用于判断输入的字符是否是合法16进制数
*/

BOOL CalculateDlg::IsTcharHex(TCHAR tCh) {
    std::unordered_set<TCHAR> hexList;
    TCHAR* tempHex = new TCHAR[1];
    for (int i = 0; i < 16; i++) {
        if (i >= 10) {
            wsprintfW(tempHex, L"%x", i);
            hexList.emplace(*tempHex);
            wsprintfW(tempHex, L"%X", i);
            hexList.emplace(*tempHex);
        }
        else {
            wsprintfW(tempHex, L"%X", i);
            hexList.emplace(*tempHex);
        }
    }
    if (hexList.count(tCh)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

