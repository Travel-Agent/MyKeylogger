/*******************************************************************
*
* MyKeylogger 0.1 (2012-08-01)
*
********************************************************************/

#include <windows.h>

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, 
						 WPARAM wParam, LPARAM lParam ) 
{   
	static HINSTANCE hinstDll; // LoadLibrary() 리턴값 저장
	HOOKPROC hGetMsgProc; // GetProcAddress(,) 리턴값 저장
	static HHOOK hKeyHook; // SetWindowsHookEx(,,,) 리턴값 저장

    switch( iMessage ) 
    { 
		case WM_CREATE: // 윈도우 생성시 후킹 설정: 실제 후킹은 연결된 dll에서 수행
			// 1. dll loaging
			hinstDll = LoadLibrary("DLL_MyKeylogger.dll"); // DLL을 로드한다.
            if(!hinstDll){
				MessageBox(hWnd, "MyKeylogger.dll을 로드할 수 없습니다.", "오류", MB_OK);
				ExitProcess(1);
            }

			// 2. get point of function
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
    // 위에서 정의되지 않은 메시지는 기본으로 처리.
} 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd) 
{
	HWND hWnd; // 메모리에 윈도우 창 생성후, 그 위치를 저장할 변수
	MSG message; // 메세지 변수
	WNDCLASS wndclass; // 윈도우

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0; 
	// 이 두개는 추가 메모리를 사용하고자 할때 사용하는 것으로
	// 거의 사용하지 않는다.

	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	// 메인 윈도의 배경색 지정: 회색

    wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );        
    wndclass.hIcon =  LoadIcon( NULL, IDI_APPLICATION );
    wndclass.hInstance =  hInstance; 
    wndclass.lpfnWndProc = (WNDPROC)WndProc; // 콜백함수 등록
    wndclass.lpszClassName =  "WG WINDOW";        
    wndclass.lpszMenuName = NULL;
    wndclass.style = NULL;

	RegisterClass( &wndclass ); // 윈도우를 등록

	hWnd = CreateWindow("WG WINDOW", "MyKeylogger 0.1", WS_OVERLAPPEDWINDOW, 
		200, 200, 500, 300, NULL, (HMENU)NULL, hInstance, NULL );        

	ShowWindow(hWnd, nShowCmd); // 윈도우를 화면에 출력

	while( GetMessage( &message, NULL, 0, 0 ) ) {
		TranslateMessage( &message ); // 메시지 번역
		DispatchMessage( &message ); // 메시지 처리
	}

	return message.wParam;
}



