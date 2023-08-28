
#pragma once
#ifndef serd_gui_win_h
#define serd_gui_win_h

#include <Windows.h>
#include <commctrl.h>
#include <winnt.h>

#include "delegate.hpp"
#include "string.hpp"

#pragma comment (lib, "comctl32.lib")

#ifdef _M_IX86
#pragma comment (linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif _M_IX64
#pragma comment (linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif _M_X64
#pragma comment (linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment (linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

namespace serd {
	
	class APP {
	public:
		inline static APP& init() { static APP init; return init; };
		static void run();
		static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	public:
		inline const TCHAR* getClassName() const { return mWClass.lpszClassName; };
		inline HFONT getFont() const { return mFont; };
		inline HINSTANCE getHinstance() const { return mWClass.hInstance; };
	private:
		APP();
		~APP();
	private:
		HFONT mFont;
		WNDCLASSEX mWClass;
	};

	class CONTROL {
	public:
		typedef bool (*CallBack)(CONTROL* sender);
		enum TYPE {
			TYPE_FORM,
			TYPE_BUTTON,
			TYPE_EDIT
		};
		struct RECT {
			int x;
			int y;
			int width;
			int height;
		};
		operator HWND() const { return mHandle; };
	public: // set
				void	setRect(const int x, const int y, const int width, const int height);
				void	setRect(const RECT rect);
		inline	void	setStyle(const DWORD style);
		inline	void	setSizebleWidth(const bool value);
		inline	void	setSizebleHeight(const bool value);
			    void	setText(STRING str);
		
				void	setAction(DELEGATE action);
	public: // change
			    void	addText(STRING str);
				void	resize();
	public: // get
		inline	RECT	getRect() const;
		inline	TYPE	getType() const;
		inline	DWORD	getStyle() const;
				STRING	getText() const;
	public: // event
		inline virtual bool	action() { return mAction != nullptr && (*mAction)(this); };
	protected:
		CONTROL() = delete;
		CONTROL(TYPE type);
	protected:
		void init(CONTROL* parent, const TCHAR* className, const TCHAR* text);
	private:
		TYPE mType;
		HWND mHandle;
		DWORD mStyle;
		RECT mRect;
		struct {
			unsigned char width : 1;
			unsigned char height : 1;
		} mSizeble;
	private:
		DELEGATE* mAction;
	};

	class FORM : public CONTROL {
	public:
		void init(const TCHAR* title, FORM* parent = nullptr);
	public:
		FORM() : CONTROL(TYPE::TYPE_FORM) { };
		FORM(const int x, const int y, const int width, const int height) : FORM() { setRect(x, y, width, height);  };
	};

	class BUTTON : public CONTROL {
	public:
		void init(FORM* parent, const TCHAR* title);
	public:
		BUTTON() : CONTROL(TYPE::TYPE_BUTTON) { };
		BUTTON(const int x, const int y, const int width, const int height) : BUTTON() { setRect(x, y, width, height); };
	};

	class EDIT : public CONTROL {
	public:
		void init(FORM* form, const TCHAR* text);
	public:
		EDIT() : CONTROL(TYPE::TYPE_EDIT) { };
		EDIT(const int x, const int y, const int width, const int height) : EDIT() { setRect(x, y, width, height); };
	};

};

#endif


