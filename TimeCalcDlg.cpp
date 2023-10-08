#include "TimeCalcDlg.h"
#include "resource.h"
#include "AnalysePE.h"
#include <time.h>
#include <unordered_set>

extern HINSTANCE appInst;

WNDPROC StampOldProc;
WNDPROC YMDOldProc;
WNDPROC HMSOldProc;

TimeCalcDlg* TimeCalcDlg::thisDlg_ = nullptr;

TimeCalcDlg::TimeCalcDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_TIMECALC, hParent),
    timeStamp_(0)
{
    
}

TimeCalcDlg::~TimeCalcDlg() {

}

void TimeCalcDlg::InitDlg() {
    SetThisDlg();
    
}

void TimeCalcDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)TimeCalcProc);
}

void TimeCalcDlg::InitEditInfo() {
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_STAMP, EM_LIMITTEXT, 8, 0);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_YMD, EM_LIMITTEXT, 10, 0);
    SendDlgItemMessage(hCurrentDlg_, IDC_EDIT_HMS, EM_LIMITTEXT, 8, 0);

    TCHAR stampBuffer[16] = { 0 };
    wsprintfW(stampBuffer, L"%08X", timeStamp_);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_STAMP, stampBuffer);

    TCHAR timeBuffer[16] = { 0 };
    tm time;
    AnalysePE::GetAnalyse().DecodeTimeStamp(timeStamp_, time);

    char timeYMD[16] = { 0 };
    char timeHMS[16] = { 0 };
    strftime(timeYMD, 15, "%Y-%m-%d", &time);
    strftime(timeHMS, 15, "%H:%M:%S", &time);

    TCHAR ymd[32] = { 0 };
    TCHAR hms[32] = { 0 };
    wsprintfW(ymd, L"%08S", timeYMD);
    wsprintfW(hms, L"%08S", timeHMS);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_YMD, ymd);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_HMS, hms);
}

LRESULT CALLBACK TimeCalcDlg::TimeCalcProc(HWND hTimeCalc, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG:
        StampOldProc = (WNDPROC)SetWindowLong(GetDlgItem(hTimeCalc, IDC_EDIT_STAMP), GWL_WNDPROC, (LONG)StampProc);
        YMDOldProc = (WNDPROC)SetWindowLong(GetDlgItem(hTimeCalc, IDC_EDIT_YMD), GWL_WNDPROC, (LONG)YMDProc);
        HMSOldProc = (WNDPROC)SetWindowLong(GetDlgItem(hTimeCalc, IDC_EDIT_HMS), GWL_WNDPROC, (LONG)HMSProc);

        thisDlg_->SetCurrentDlgHWND(hTimeCalc);
        thisDlg_->InitEditInfo();
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDOK:
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

LRESULT CALLBACK TimeCalcDlg::StampProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    TCHAR tCh;
    switch (message)
    {
    case WM_CHAR:
        tCh = (TCHAR)wParam;
        if (!IsTcharHex(tCh))
            return 0;
        break;
    }
    return CallWindowProc(StampOldProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK TimeCalcDlg::YMDProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    TCHAR tCh;
    switch (message)
    {
    case WM_CHAR:
        tCh = (TCHAR)wParam;
        if (!IsTcharHex(tCh))
            return 0;
        break;
    }
    return CallWindowProc(YMDOldProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK TimeCalcDlg::HMSProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    TCHAR tCh;
    switch (message)
    {
    case WM_CHAR:
        tCh = (TCHAR)wParam;
        if (!IsTcharHex(tCh))
            return 0;
        break;
    }
    return CallWindowProc(HMSOldProc, hwnd, message, wParam, lParam);
}

BOOL TimeCalcDlg::IsTcharHex(TCHAR tCh) {
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