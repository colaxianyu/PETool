module;

#include "resource.h"
#include <windows.h>

module TimeStampDlg;

import STL;
import AnalysePE;

using std::format;
using std::string;
using std::time_t;
using petools::DecodeTimeStamp;

extern HINSTANCE appInst;

TimeStampDlg* TimeStampDlg::thisDlg_ = nullptr;

TimeStampDlg::TimeStampDlg(HWND hParent)
	: DialogEX(IDD_DIALOG_TIMESTAMP, hParent)
{
    
}

TimeStampDlg::~TimeStampDlg() {

}

void TimeStampDlg::InitDlg() {
	SetThisDlg();
}

void TimeStampDlg::Plant() {
	DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)TsProc);
}

void TimeStampDlg::SetTSDlgInfo() {
    time_t ts = AnalysePE::GetAnalyse().GetHeaders().fileHeader->TimeDateStamp;

    TCHAR timeStamp[10] = { 0 };
    wsprintfW(timeStamp, L"%08X", ts);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_TimeEncode, timeStamp);

    string timeYMD;
    string timeHMS;
    DecodeTimeStamp(ts, timeYMD, timeHMS);


    TCHAR ymd[32] = { 0 };
    TCHAR hms[32] = { 0 };
    wsprintfW(ymd, L"%08S", timeYMD.c_str());
    wsprintfW(hms, L"%08S", timeHMS.c_str());

    SetDlgItemText(hCurrentDlg_, IDC_EDIT_TimeYMD, ymd);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_TimeHMS, hms);
}

LRESULT CALLBACK TimeStampDlg::TsProc(HWND hTime, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hTime);
        thisDlg_->SetTSDlgInfo();
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