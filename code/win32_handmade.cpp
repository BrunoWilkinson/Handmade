#include <windows.h>

LRESULT MainWindowCallback(
    HWND Window,
    UINT Message,
    WPARAM WParam,
    LPARAM LParam 
)
{
    LRESULT Result = 0;

    switch (Message)
    {
    case WM_SIZE:
    {
        OutputDebugStringA("WM_SIZE\n");
    }
        break;
    case WM_DESTROY:
    {
        OutputDebugStringA("WM_DESTROY\n");
    }
        break;
    case WM_CLOSE:
    {
        OutputDebugStringA("WM_CLOSE\n");
    }
        break;
    case WM_ACTIVATEAPP:
    {
        OutputDebugStringA("WM_ACTIVATEAPP\n");
    }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT Paint;
        HDC Context = BeginPaint(Window, &Paint);
   
        LONG X = Paint.rcPaint.left;
        LONG Y = Paint.rcPaint.top;
        LONG Width = Paint.rcPaint.right - Paint.rcPaint.left;
        LONG Height = Paint.rcPaint.bottom - Paint.rcPaint.left;

        static DWORD Operation = WHITENESS;

        PatBlt(Context, X, Y, Width, Height, Operation);
        if (Operation == WHITENESS)
        {
            Operation = BLACKNESS;
        }
        else
        {
            Operation = WHITENESS;
        }

        EndPaint(Window, &Paint);
    }
        break;
    default:
    {
        // OutputDebugStringA("default\n");
        Result = DefWindowProc(Window, Message, WParam, LParam);
    }
        break;
    }

    return Result;
}

int APIENTRY WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CommandLine, int ShowCode)
{
    WNDCLASS WindowClass = {};

    // TODO: Check if HREDRAW/VREDRAW/OWNNDC are still relevant
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    // WindoClass.Icon = ;
    WindowClass.lpszClassName = "HandemandeHeorWindowClass";

    if (RegisterClass(&WindowClass))
    {
        HWND WindowHandle = CreateWindowEx(
            0,
            WindowClass.lpszClassName,
            "Handmade Hero",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            Instance,
            0
        );

        if (WindowHandle)
        {
            MSG Message;
            for (;;)
            {
                BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
                if (MessageResult > 0)
                {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            // TODO: Logging
        }
    }
    else
    {
        // TODO: Logging
    }

    return 0;
}