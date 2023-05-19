#include <windows.h>

#define internal static 
#define local_persist static
#define global_variable static

// TODO: This a global for now.
global_variable bool Running;
global_variable BITMAPINFO BitmapInfo;
global_variable void* BitmapMemory;
global_variable HBITMAP BitmapHandle;
global_variable HDC BitmapDeviceContext;

internal void Win32ResizeDIBSection(int Width, int Height)
{
    // TODO: Bulletproof this.
    // Maybe don't free first, free after, then free first if that fails.

    if (BitmapHandle)
    {
        DeleteObject(BitmapHandle);
    }

    if (!BitmapDeviceContext)
    {
        // TODO: Should we recreate these under certain special circumstances
        BitmapDeviceContext = CreateCompatibleDC(0);
    }

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    BitmapHandle = CreateDIBSection(BitmapDeviceContext, &BitmapInfo, DIB_RGB_COLORS, &BitmapMemory, 0, 0);
}

internal void Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
    StretchDIBits(DeviceContext, X, Y, Width, Height, X, Y, Width, Height, NULL, NULL, DIB_RGB_COLORS, SRCCOPY);
}

LRESULT Win32MainWindowCallback(
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
        RECT ClientRectangle;
        GetClientRect(Window, &ClientRectangle);
        LONG Width = ClientRectangle.right - ClientRectangle.left;
        LONG Height = ClientRectangle.bottom - ClientRectangle.left;
        Win32ResizeDIBSection(Width, Height);

        OutputDebugStringA("WM_SIZE\n");
    }
        break;
    case WM_DESTROY:
    {
        // TODO: Handle this with a message to the user ?
        Running = false;
    }
        break;
    case WM_CLOSE:
    {
        // TODO: Handle this as an error - recreate window ?
        Running = false;
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

        Win32UpdateWindow(Context, X, Y, Width, Height);

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

    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    // WindoClass.Icon = ;
    WindowClass.lpszClassName = "HandemandeHeorWindowClass";

    if (RegisterClassA(&WindowClass))
    {
        HWND WindowHandle = CreateWindowExA(
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
            Running = true;
            while (Running)
            {
                MSG Message;
                BOOL MessageResult = GetMessageA(&Message, 0, 0, 0);
                if (MessageResult > 0)
                {
                    TranslateMessage(&Message);
                    DispatchMessageA(&Message);
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