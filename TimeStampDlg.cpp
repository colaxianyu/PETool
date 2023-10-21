#include "TimeStampDlg.h"
#include "resource.h"
#include "AnalysePE.h"
#include <time.h>
#include <tchar.h>
import Utils;

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

    tm time;
    DecodeTimeStamp(ts, time);

    char timeYMD[32] = { 0 };
    char timeHMS[32] = { 0 };
    strftime(timeYMD, 32, "%Y-%m-%d", &time);
    strftime(timeHMS, 32, "%H:%M:%S", &time);

    TCHAR ymd[32] = { 0 };
    TCHAR hms[32] = { 0 };    
    wsprintfW(ymd, L"%08S", timeYMD);
    wsprintfW(hms, L"%08S", timeHMS);

    /*TCHAR* y = const_cast<TCHAR*>(TEXT("Äê"));
    TCHAR* m = const_cast<TCHAR*>(TEXT("ÔÂ"));
    TCHAR* d = const_cast<TCHAR*>(TEXT("ÈÕ"));
    ymd[4] = *y;
    ymd[7] = *m;
    ymd[10] = *d;*/
    
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