#include <windows.h>
#include <stdio.h>
#include "lifeSimulation.h"

#define CELL_SIZE 10
#ifndef HEIGHT
#define HEIGHT 10
#endif
#ifndef LENGTH
#define LENGTH 10
#endif
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

field Tfield;
HWND hwndStep, hwndClear, hwndExit;

int WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR lpCmdLine,
            int nCmdShow)
{
    const char CLASS_NAME[] = "GameOfLife";

    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Game of Life",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        640,
        480,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (!hwnd)
    {
        MessageBox(
            NULL,
            "Window Creation Failed!",
            "Error",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

field DrawGrid(HDC hdc, field curr)
{
    HBRUSH aliveBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH deadBrush = CreateSolidBrush(RGB(255, 255, 255));

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < LENGTH; x++)
        {
            RECT cellRect = {x * CELL_SIZE,
                             y * CELL_SIZE,
                             (x + 1) * CELL_SIZE,
                             (y + 1) * CELL_SIZE};
            FillRect(hdc, &cellRect, curr.arr[y][x] ? aliveBrush : deadBrush);
        }
    }

    DeleteObject(aliveBrush);
    DeleteObject(deadBrush);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    static RECT gridRect = {0, 0, CELL_SIZE * HEIGHT, CELL_SIZE * LENGTH};
    printf("%d, %d\n", LOWORD(lParam), HIWORD(lParam));
    switch (Msg)
    {
    case WM_CREATE:
        hwndStep = CreateWindow(
            "BUTTON",
            "Step",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            500,
            20,
            100,
            30,
            hwnd,
            (HMENU)1,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hwndClear = CreateWindow(
            "BUTTON",
            "Clear",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            500,
            60,
            100,
            30,
            hwnd,
            (HMENU)2,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hwndExit = CreateWindow(
            "BUTTON",
            "Exit",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            500,
            100,
            100,
            30,
            hwnd,
            (HMENU)3,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
    case WM_COMMAND:
        if (LOWORD(wParam) == 1)
        { // Step button
            Tfield = lifeSimulation(Tfield);
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            DrawGrid(hdc, Tfield);
            EndPaint(hwnd, &ps);
            InvalidateRect(hwnd, &gridRect, TRUE);
        }
        else if (LOWORD(wParam) == 2)
        { // Clear button
            Tfield = makeNewField();
            InvalidateRect(hwnd, &gridRect, TRUE);
            // PAINTSTRUCT ps;
            // HDC hdc = BeginPaint(hwnd, &ps);
            // DrawGrid(hdc, Tfield);
            // EndPaint(hwnd, &ps);
        }
        else if (LOWORD(wParam) == 3)
        { // Exit button
            PostQuitMessage(0);
        }
        break;

    case WM_LBUTTONDOWN:
        int x = LOWORD(lParam) / CELL_SIZE;
        int y = HIWORD(lParam) / CELL_SIZE;
        Tfield = changeCellWeight(Tfield, x, y);
        InvalidateRect(hwnd, &gridRect, TRUE);
        // PAINTSTRUCT ps;
        // HDC hdc = BeginPaint(hwnd, &ps);
        // DrawGrid(hdc, Tfield);
        // EndPaint(hwnd, &ps);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DrawGrid(hdc, Tfield);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, Msg, wParam, lParam);
    }

    return 0;
}
