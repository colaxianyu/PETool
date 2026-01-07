//module;
//
//#include <windows.h>
//
//export module CalculateDlg;
//
//import Dialog;
//
//// 用于计算一个地址在PE文件中的位置
//// 可通过FOA、VA与RVA三类地址计算，且验证其合法性
//
//export class CalculateDlg : public DialogEX
//{
//public:
//	CalculateDlg(HWND h_parent = nullptr);
//	virtual ~CalculateDlg() override;
//	virtual void init_dlg() override;
//	virtual void plant() override;
//
//	void init_radio_button();
//	void on_click_VA();
//	void on_click_RVA();
//	void OnClickFOA();
//	void OnClickCalc();
//
//	void from_VA_calc(DWORD VA);
//	void from_RVA_calc(DWORD RVA);
//	void from_FOA_calc(DWORD FOA);
//
//private:
//	static CalculateDlg* this_dlg_;
//	void set_this_dlg() { this_dlg_ = this; }
//
//	static LRESULT CALLBACK calc_proc(HWND h_calc, UINT message, WPARAM w_param, LPARAM l_param);
//
//	static LRESULT CALLBACK RVA_new_proc(HWND, UINT, WPARAM, LPARAM);
//	static LRESULT CALLBACK VA_new_proc(HWND, UINT, WPARAM, LPARAM);
//	static LRESULT CALLBACK FOA_new_proc(HWND, UINT, WPARAM, LPARAM);
//};