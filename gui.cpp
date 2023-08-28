
#include "gui.hh"
#include <iostream>

//#include <string>
//#include <bitset>

namespace serd {

	void APP::run() {
		MSG msg;
		bool keyCtrl = false;

		memset(&msg, 0, sizeof(msg));

		while (GetMessage(&msg, 0, 0, 0)) {
			switch (msg.message) {
			case WM_KEYDOWN:
				switch (msg.wParam) {
				case VK_CONTROL:
					keyCtrl = true;
					break;
				case VK_RETURN:
					if (!keyCtrl) {
						HWND parent = GetParent(msg.hwnd);

						if (parent != 0 && SendMessage(GetParent(msg.hwnd), WM_COMMAND, BN_CLICKED, reinterpret_cast<LPARAM>(msg.hwnd)))
							continue;
					};
					break;
				};
				break;
			case WM_KEYUP:
				switch (msg.wParam) {
				case VK_CONTROL:
					keyCtrl = false;
					break;
				};
				break;
			};

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		};
	};
	LRESULT WINAPI APP::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		CONTROL* control = reinterpret_cast<CONTROL*> (GetWindowLongPtr(hWnd, GWLP_USERDATA));
		
		if (control != 0 && *control == hWnd) {
			switch (uMsg) {
			case WM_COMMAND:
				switch (HIWORD(wParam)) {
				case BN_CLICKED:
					CONTROL* sender = reinterpret_cast<CONTROL*>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
					
					if (sender != 0 &&
						*sender == reinterpret_cast<HWND>(lParam) &&
						sender->action()) {
						return true;
					};

					break;
				};
				break;
			case WM_SIZE: {
				CONTROL::RECT newValue({0, 0, LOWORD(lParam) , HIWORD(lParam) });
				CONTROL::RECT oldValue = control->getRect();
#ifdef _DEBUG
				std::wcout << oldValue.width << L"," << oldValue.height <<
					L"->" << newValue.width << L"," << newValue.height << std::endl;
#endif
				//MoveWindow(hWnd, oldValue.x, oldValue.y, oldValue.width, oldValue.height, TRUE);

				HWND curHWnd = GetWindow(hWnd, GW_CHILD);
				while (curHWnd != 0) {
					std::cout << std::hex << curHWnd << std::endl;
					curHWnd = GetWindow(curHWnd, GW_HWNDNEXT);
				};
			};
				
				control->resize();
				break;
			case WM_CLOSE:
				DestroyWindow(hWnd);
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			default:
				break;
			};
		};
		
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		
		switch (uMsg) {
		case WM_PAINT:
		{
			/*
			HDC hdc;
			PAINTSTRUCT ps;
			int height, width;

			hdc = BeginPaint(hWnd, &ps);

			RECT rectClient = ps.rcPaint;
			HPEN hpen = CreatePen(PS_SOLID, 0, RGB(0,50,150));
			SelectObject(hdc, hpen);

			height  = rectClient.bottom - rectClient.top;
			width   = rectClient.right - rectClient.left;

			std::wstring str1  = L"w_" + std::to_wstring(width) + L"\0";
			std::wstring str2 = L"h_" + std::to_wstring(height) + L"\0";

			int side = 10;

			for (int i = 0; i <= height / side; ++i) {
				MoveToEx(hdc, 0, i * side, 0);
				LineTo(hdc, width, i * side);
			};

			for (int i = 0; i <= width / side; ++i) {
				MoveToEx(hdc, i * side, 0, 0);
				LineTo(hdc, i * side, height);
			};

			TextOut(hdc, 0, 0, str1.c_str(), str1.size());
			TextOut(hdc, 0, 20, str2.c_str(), str2.size());

			EndPaint(hWnd, &ps);
			*/
		};
		break;
		case WM_SIZE:
		{
			HWND curHWnd = GetWindow(hWnd, GW_CHILD);
			while (curHWnd != 0)
			{
				std::cout << std::hex << curHWnd << std::endl;
				curHWnd = GetWindow(curHWnd, GW_HWNDNEXT);
			};
		}
		break;
		};

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	};

	APP::APP() {
		//INITCOMMONCONTROLSEX icc;
		//icc.dwSize = sizeof(icc);
		//icc.dwICC = ICC_STANDARD_CLASSES;
		//InitCommonControlsEx(&icc);

		NONCLIENTMETRICS pNonClientMetrics;
		pNonClientMetrics.cbSize = sizeof(pNonClientMetrics);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS, pNonClientMetrics.cbSize, &pNonClientMetrics, 0);
		mFont = CreateFontIndirect(&pNonClientMetrics.lfMenuFont);

		size_t sizeOfClassEx = sizeof(mWClass);
		memset(&mWClass, 0, sizeOfClassEx);
		mWClass.lpszClassName = L"serd_Windows";
		mWClass.hInstance = GetModuleHandle(NULL);
		mWClass.lpfnWndProc = APP::WndProc;
		mWClass.cbSize = sizeOfClassEx;
		mWClass.style = CS_HREDRAW | CS_VREDRAW;
		mWClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		mWClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		mWClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		RegisterClassEx(&mWClass);
	};
	APP::~APP() {
		DeleteObject(mFont);
	};

	CONTROL::CONTROL(CONTROL::TYPE type) {
		//memset(this, 0, sizeof(this));
		mType = type;
		mHandle = nullptr;
		mAction = nullptr;
		mRect = { 0, 0, 50, 50 };
		mSizeble.height = false;
		mSizeble.width = false;
		mStyle = 0;
	};

	void CONTROL::setRect(const int x, const int y, const int width, const int height) {
		setRect({ x,y,width,height });
	};
	void CONTROL::setRect(const RECT rect) {
		mRect = rect;

		if (*this != nullptr) {
			// resize;
		};
	};
	inline void CONTROL::setStyle(const DWORD style) {
		mStyle = style;
	};
	inline void CONTROL::setSizebleWidth(const bool value) {
		mSizeble.width = value;
	};
	inline void CONTROL::setSizebleHeight(const bool value) {
		mSizeble.height = value;
	};
		   void CONTROL::setText(STRING str) {
		SendMessage(*this, WM_SETTEXT, NULL, reinterpret_cast<WPARAM>(str.c_str()));
	};

		   void CONTROL::addText(STRING str) {
		int length = GetWindowTextLength(*this);
		SendMessage(*this, EM_SETSEL, static_cast<WPARAM>(length), static_cast<WPARAM>(length));
		SendMessage(*this, EM_REPLACESEL, FALSE, reinterpret_cast<WPARAM>(str.c_str()));
	};
		   void CONTROL::resize() {
		int i;
	};

	void CONTROL::setAction(DELEGATE action) {
		if (mAction != nullptr)
			delete mAction;

		mAction = new DELEGATE(action);
	};

	inline CONTROL::RECT CONTROL::getRect() const {
		return mRect;
	};
	inline CONTROL::TYPE CONTROL::getType() const {
		return mType;
	};
	inline DWORD CONTROL::getStyle() const {
		return mStyle;
	};
	STRING CONTROL::getText() const {
		int length = GetWindowTextLength(*this);
		STRING result;

		if (length == 0)
			return result;

		++length;

		TCHAR* text = new TCHAR[length];

		SendMessage(*this, WM_GETTEXT, static_cast<WPARAM>(length), reinterpret_cast<WPARAM>(text));

		result = text;
		delete[] text;
		
		return result;
	};

	void CONTROL::init(CONTROL* parent, const TCHAR* className, const TCHAR* text) {
		if (mHandle != nullptr)
			return;

		int x = mRect.x;
		int y = mRect.y;
		int width = mRect.width;
		int height = mRect.height;

		if (mType == TYPE::TYPE_FORM) {
			::RECT rect;

			memset(&rect, 0, sizeof(rect));

			rect.right = width;
			rect.bottom = height;

			AdjustWindowRect(&rect, mStyle, NULL);

			width = rect.right - rect.left;
			height = rect.bottom - rect.top;

			SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

			y = rect.bottom - height - y;
		}
		else {
			RECT parSize = parent->getRect();
			y = parSize.height - mRect.height - y;
		};

		mHandle = ::CreateWindow(
			className,
			text,
			mStyle,
			x, y,
			width, height,
			parent != nullptr ? *parent : NULL,
			NULL,
			APP::init().getHinstance(),
			NULL
		);

		SetWindowLongPtr(mHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		SendMessage(mHandle, WM_SETFONT, reinterpret_cast<WPARAM>(APP::init().getFont()), TRUE);
	};

	void FORM::init(const TCHAR* title, FORM* parent) {
		CONTROL::init(parent, APP::init().getClassName(), title);
		ShowWindow(*this, SW_SHOW);
	};

	void BUTTON::init(FORM* form, const TCHAR* text) {
		DWORD style = getStyle();
		style |= WS_VISIBLE | WS_CHILD;
		setStyle(style);
		CONTROL::init(form, L"BUTTON", text);
	};

	void EDIT::init(FORM* form, const TCHAR* text) {
		DWORD style = getStyle();
		style |= WS_VISIBLE | WS_CHILD;
		setStyle(style);

		CONTROL::init(form, L"EDIT", text);
		ShowScrollBar(*this, SB_VERT, TRUE);
	};

};
