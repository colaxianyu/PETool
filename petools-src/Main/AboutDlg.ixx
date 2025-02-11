//module;
//
//export module AboutDlg;
//
//import DialogEX;
//
//
//export class AboutDlg : public DialogEX<AboutDlg> {
//public:
//    explicit AboutDlg(INT template_id, HWND h_parent = nullptr) noexcept;
//
//    virtual ~AboutDlg() noexcept override;
//
//	virtual void show_dialog() override;
//	virtual void close_dialog() override;
//
//private:
//    LRESULT dialog_proc(HWND h_about, UINT message, WPARAM w_param, LPARAM l_param) override;
//};