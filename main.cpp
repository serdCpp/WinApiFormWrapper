//#include "stdafx.h"

#include <windows.h>
#include "gui.hh"
#include <iostream>

class FORM1 : serd::FORM {
private:
	serd::BUTTON* mButtonSend;
	serd::EDIT* mSentText;
	serd::EDIT* mChatHistory;
public:
	FORM1() : FORM(0, 0, 400, 200) {
		int width = 500;
		int height = 300;
		int indent = 10;
		int heightLineSend = 20;
		int widthButton = 60;

		setRect(0, 0, width, height);
		setStyle(WS_OVERLAPPEDWINDOW);
		init(L"test create form2");

		mSentText = new serd::EDIT(indent, indent, width - indent * 3 - widthButton, heightLineSend);
		mSentText->setStyle( WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL);
		//mSentText->setSizebleWidth(true);
		mSentText->setAction(serd::DELEGATE(this, &FORM1::send));
		mSentText->init(this, L"send text");

		mButtonSend = new serd::BUTTON(width - indent - widthButton, indent, widthButton, heightLineSend);
		mButtonSend->setStyle( BS_CENTER | BS_VCENTER | BS_PUSHBUTTON | BS_TEXT);
		mButtonSend->setAction(serd::DELEGATE(this, &FORM1::send));
		mButtonSend->init(this, L"send");

		mChatHistory = new serd::EDIT(indent, indent * 2 + heightLineSend, width - indent * 2, height - indent * 3 - heightLineSend);
		mChatHistory->setStyle( WS_BORDER | ES_AUTOVSCROLL | ES_MULTILINE);
		//mSentText->setSizebleHeight(true);
		//mSentText->setSizebleWidth(true);
		mChatHistory->init(this, L"chat history");
	};
	~FORM1() {
		delete mButtonSend;
		delete mSentText;
		delete mChatHistory;
	};
public:
	bool send(serd::CONTROL* sender) {
		serd::STRING text;

		text = mSentText->getText();
		
		if (text.size() > 0) {
			text  = "you:" + text + "\r\n";
			mSentText->setText("");
			mChatHistory->addText(text);
		};

		return true;
	};
};

int main(int argc, const char* argv[]) {
	
#ifdef _DEBUG
	::RECT rect;
	memset(&rect, 0, sizeof(rect));
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
	MoveWindow(GetConsoleWindow(), rect.right - 600, 0, 600, rect.bottom, TRUE);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif // DEBUG
	FORM1 test;
	serd::APP::run();

	return 0;
}
