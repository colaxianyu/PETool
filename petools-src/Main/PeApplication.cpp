module;

module PEApplication;

PEApplication& PEApplication::instance() {
	static PEApplication instance;
	return instance;
}

void PEApplication::init_APP(HINSTANCE h_instance, int cmd_show) noexcept {
	DialogEX<void>::set_app_instance(h_instance);
	DialogEX<void>::set_cmd_show(cmd_show);
}
