#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define ID_TIMER 1
#define CELL_SIZE 10
#define DEFAULT_WIDTH 50
#define DEFAULT_HEIGHT 30

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void DrawGrid(HDC hdc);
void UpdateGrid();
void ToggleCell(int x, int y);
void StartGame();
void PauseGame();
void ClearGrid();
void ResizeGrid(int width, int height);
void SetRules(int birth, int survive);

bool running = false;
bool **grid;
bool **newGrid;
int gridWidth = DEFAULT_WIDTH;
int gridHeight = DEFAULT_HEIGHT;
int birthRule = 3;
int surviveRule = 2;
HWND hwndStart, hwndPause, hwndClear, hwndExit;
HWND hwndBirthRule, hwndSurviveRule, hwndWidth, hwndHeight;

void InitializeGrid(int width, int height)
{
    grid = (bool **)malloc(height * sizeof(bool *));
    newGrid = (bool **)malloc(height * sizeof(bool *));
    for (int y = 0; y < height; y++)
    {
        grid[y] = (bool *)malloc(width * sizeof(bool));
        newGrid[y] = (bool *)malloc(width * sizeof(bool));
        memset(grid[y], 0, width * sizeof(bool));
        memset(newGrid[y], 0, width * sizeof(bool));
    }
}

void FreeGrid()
{
    for (int y = 0; y < gridHeight; y++)
    {
        free(grid[y]);
        free(newGrid[y]);
    }
    free(grid);
    free(newGrid);
}

int APIENTRY
WinMain(HINSTANCE hInstance,
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
        900,
        600,
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

    InitializeGrid(gridWidth, gridHeight);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    FreeGrid();

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static RECT gridRect = {0, 0, CELL_SIZE * DEFAULT_WIDTH, CELL_SIZE * DEFAULT_HEIGHT};

    switch (uMsg)
    {
    case WM_CREATE:
        hwndStart = CreateWindow(
            "BUTTON",
            "Start",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            600,
            20,
            100,
            30,
            hwnd,
            (HMENU)1,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hwndPause = CreateWindow(
            "BUTTON",
            "Pause",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            600,
            60,
            100,
            30,
            hwnd,
            (HMENU)2,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hwndClear = CreateWindow(
            "BUTTON",
            "Clear",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            600,
            100,
            100,
            30,
            hwnd,
            (HMENU)3,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hwndExit = CreateWindow(
            "BUTTON",
            "Exit",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            600,
            140,
            100,
            30,
            hwnd,
            (HMENU)4,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);

        CreateWindow(
            "STATIC",
            "Birth Rule:",
            WS_VISIBLE | WS_CHILD,
            600,
            200,
            100,
            20,
            hwnd,
            NULL,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hwndBirthRule = CreateWindow(
            "EDIT",
            "3",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
            700,
            200,
            50,
            20,
            hwnd,
            NULL,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);

        CreateWindow(
            "STATIC",
            "Survive Rule:",
            WS_VISIBLE | WS_CHILD,
            600,
            230,
            100,
            20,
            hwnd,
            NULL,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hwndSurviveRule = CreateWindow(
            "EDIT",
            "2",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
            700,
            230,
            50,
            20,
            hwnd,
            NULL,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);

        CreateWindow(
            "STATIC",
            "Width:",
            WS_VISIBLE | WS_CHILD,
            600,
            260,
            100,
            20,
            hwnd,
            NULL,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hwndWidth = CreateWindow(
            "EDIT",
            "50",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
            700,
            260,
            50,
            20,
            hwnd,
            NULL,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);

        CreateWindow(
            "STATIC",
            "Height:",
            WS_VISIBLE | WS_CHILD,
            600,
            290,
            100,
            20,
            hwnd,
            NULL,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hwndHeight = CreateWindow(
            "EDIT",
            "30",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
            700,
            290,
            50,
            20,
            hwnd,
            NULL,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);

        SetTimer(hwnd, ID_TIMER, 100, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1)
        { // Start button
            StartGame();
        }
        else if (LOWORD(wParam) == 2)
        { // Pause button
            PauseGame();
        }
        else if (LOWORD(wParam) == 3)
        { // Clear button
            ClearGrid();
            InvalidateRect(hwnd, &gridRect, TRUE);
        }
        else if (LOWORD(wParam) == 4)
        { // Exit button
            PostQuitMessage(0);
        }
        break;

    case WM_TIMER:
        if (wParam == ID_TIMER && running)
        {
            UpdateGrid();
            InvalidateRect(hwnd, &gridRect, TRUE);
        }
        break;

    case WM_LBUTTONDOWN:
        if (!running)
        {
            int x = LOWORD(lParam) / CELL_SIZE;
            int y = HIWORD(lParam) / CELL_SIZE;
            ToggleCell(x, y);
            InvalidateRect(hwnd, &gridRect, TRUE);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DrawGrid(hdc);
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_SIZE:
        gridRect.right = CELL_SIZE * gridWidth;
        gridRect.bottom = CELL_SIZE * gridHeight;
        break;

    case WM_DESTROY:
        KillTimer(hwnd, ID_TIMER);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void DrawGrid(HDC hdc)
{
    HBRUSH aliveBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH deadBrush = CreateSolidBrush(RGB(255, 255, 255));

    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            RECT cellRect = {x * CELL_SIZE,
                             y * CELL_SIZE,
                             (x + 1) * CELL_SIZE,
                             (y + 1) * CELL_SIZE};
            FillRect(hdc, &cellRect, grid[y][x] ? aliveBrush : deadBrush);
        }
    }

    DeleteObject(aliveBrush);
    DeleteObject(deadBrush);
}

void UpdateGrid()
{
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            int aliveNeighbors = 0;

            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    if (dx == 0 && dy == 0)
                        continue;
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight && grid[ny][nx])
                    {
                        aliveNeighbors++;
                    }
                }
            }

            newGrid[y][x] = (grid[y][x] && (aliveNeighbors >= 2 && aliveNeighbors <= 3)) || (!grid[y][x] && (aliveNeighbors == birthRule));
        }
    }

    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            grid[y][x] = newGrid[y][x];
        }
    }
}

void ToggleCell(int x, int y)
{
    if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight)
    {
        grid[y][x] = !grid[y][x];
    }
}

void StartGame()
{
    running = true;
}

void PauseGame()
{
    running = false;
}

void ClearGrid()
{
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            grid[y][x] = false;
        }
    }
}

void ResizeGrid(int newWidth, int newHeight)
{
    if (newWidth == gridWidth && newHeight == gridHeight)
    {
        return;
    }

    bool **tempGrid = (bool **)malloc(newHeight * sizeof(bool *));
    bool **tempNewGrid = (bool **)malloc(newHeight * sizeof(bool *));
    for (int y = 0; y < newHeight; y++)
    {
        tempGrid[y] = (bool *)malloc(newWidth * sizeof(bool));
        tempNewGrid[y] = (bool *)malloc(newWidth * sizeof(bool));
        for (int x = 0; x < newWidth; x++)
        {
            if (y < gridHeight && x < gridWidth)
            {
                tempGrid[y][x] = grid[y][x];
                tempNewGrid[y][x] = newGrid[y][x];
            }
            else
            {
                tempGrid[y][x] = false;
                tempNewGrid[y][x] = false;
            }
        }
    }

    FreeGrid();

    grid = tempGrid;
    newGrid = tempNewGrid;
    gridWidth = newWidth;
    gridHeight = newHeight;
}

void SetRules(int birth, int survive)
{
    birthRule = birth;
    surviveRule = survive;
}
