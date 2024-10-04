#include <windows.h>
#include <stdio.h>
#include <math.h>


int clickCount = 0;  // To track the number of clicks
int autoClickers = 0;  // Number of auto-clickers
int autoClickerCost = 10;  // Initial cost of an auto-clicker
char clickText[50];  // Buffer to store the click display text
char autoClickerText[50];  // Buffer to store the auto-clicker display text
char buyButtonText[50];  // Buffer to store the buy button text
UINT_PTR timerID = 0;  // Timer ID for auto-clicker

// Function prototypes
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void UpdateClickText(HWND hwnd);
void UpdateAutoClickerText(HWND hwnd);
void StartAutoClickerTimer(HWND hwnd);
void AutoClickerTick(HWND hwnd);
void ResizeControls(HWND hwnd, int width, int height);

// Main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "ClickerGameWindowClass";
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = WindowProcedure;  // Set the window procedure
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create the main window with a resizable style (WS_OVERLAPPEDWINDOW | WS_SIZEBOX)
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Clicker Game",         // Window title
        WS_OVERLAPPEDWINDOW | WS_SIZEBOX,  // Make window resizable
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,  // Position and size
        NULL,                   // Parent window
        NULL,                   // Menu
        hInstance,
        NULL                    // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Window Procedure for the main game
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hButton, hClickText, hAutoClickerText, hBuyButton;
    static int windowWidth, windowHeight;

    switch (message) {
        case WM_CREATE:
            // Create a button that the user can click
            hButton = CreateWindow(
                "BUTTON",  // Predefined class for button
                "Click Me!",  // Button text
                WS_VISIBLE | WS_CHILD,  // Button styles
                150, 100, 100, 30,  // Button position and size
                hwnd,  // Parent window
                (HMENU)1,  // ID of the button
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );

            // Create static text to show click count
            hClickText = CreateWindow(
                "STATIC",  // Predefined class for static text
                "",  // Initial text (empty)
                WS_VISIBLE | WS_CHILD,  // Styles
                150, 50, 200, 20,  // Position and size
                hwnd,  // Parent window
                NULL,  // No ID
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );

            // Create static text to show auto-clicker count
            hAutoClickerText = CreateWindow(
                "STATIC",  // Predefined class for static text
                "",  // Initial text (empty)
                WS_VISIBLE | WS_CHILD,  // Styles
                150, 130, 200, 20,  // Position and size
                hwnd,  // Parent window
                NULL,  // No ID
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );

            // Create the buy auto-clicker button
            sprintf(buyButtonText, "Buy Auto-Clicker (Cost: %d)", autoClickerCost);
            hBuyButton = CreateWindow(
                "BUTTON",  // Predefined class for button
                buyButtonText,   // Button text
                WS_VISIBLE | WS_CHILD,  // Button styles
                130, 170, 150, 30,  // Button position and size
                hwnd,  // Parent window
                (HMENU)2,  // ID of the buy button
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );

            // Set the initial click and auto-clicker text
            UpdateClickText(hClickText);
            UpdateAutoClickerText(hAutoClickerText);

            // Start the auto-clicker timer right after creating the window
            StartAutoClickerTimer(hwnd);
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Create a white brush
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); // White color
            FillRect(hdc, &ps.rcPaint, hBrush);  // Fill the window with the white brush

            DeleteObject(hBrush); // Clean up the brush
            EndPaint(hwnd, &ps);
        }
        break;

        case WM_SIZE:
            // Get the new window width and height
            windowWidth = LOWORD(lParam);
            windowHeight = HIWORD(lParam);

            // Resize controls based on the new window size
            ResizeControls(hwnd, windowWidth, windowHeight);

            // Invalidate the entire window to trigger a repaint
            InvalidateRect(hwnd, NULL, TRUE);  
            UpdateWindow(hwnd);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {  // Button with ID 1 (Click Me) is clicked
                clickCount++;  // Increment the click count
                UpdateClickText(hClickText);  // Update the click counter text
            } else if (LOWORD(wParam) == 2) {  // Buy auto-clicker button clicked
                if (clickCount >= autoClickerCost) {
                    clickCount -= autoClickerCost;  // Deduct clicks
                    autoClickers++;  // Increase the number of auto-clickers
                    autoClickerCost = (int)(autoClickerCost * 1.2);  // Apply 1.2x cost multiplier

                    // Update the click and auto-clicker text
                    UpdateClickText(hClickText);
                    UpdateAutoClickerText(hAutoClickerText);

                    // Update the buy button text with the new cost
                    sprintf(buyButtonText, "Buy Auto-Clicker (Cost: %d)", autoClickerCost);
                    SetWindowText(hBuyButton, buyButtonText);

                    StartAutoClickerTimer(hwnd);  // Ensure auto-clicker timer is running
                } else {
                    MessageBox(hwnd, "Not enough clicks!", "Error", MB_OK | MB_ICONERROR);
                }
            }
            break;

        case WM_TIMER:
            if (wParam == 1) {  // Timer ID 1 for auto-clicking
                AutoClickerTick(hClickText);
            }
            break;

        case WM_DESTROY:
            KillTimer(hwnd, timerID);  // Stop the auto-clicker timer when the window is closed
            PostQuitMessage(0);  // Quit the application
            break;

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

// Function to update the click count text
void UpdateClickText(HWND hClickText) {
    sprintf(clickText, "Clicks: %d", clickCount);
    SetWindowText(hClickText, clickText);  // Update the click count display
}

// Function to update the auto-clicker count text
void UpdateAutoClickerText(HWND hAutoClickerText) {
    sprintf(autoClickerText, "Auto-Clickers: %d", autoClickers);
    SetWindowText(hAutoClickerText, autoClickerText);  // Update the auto-clicker count display
}

// Function to start the auto-clicker timer (1 click per second per auto-clicker)
void StartAutoClickerTimer(HWND hwnd) {
    timerID = SetTimer(hwnd, 1, 1000, NULL);  // Set a timer to tick every second
}

// Function to add clicks for each auto-clicker every second
void AutoClickerTick(HWND hClickText) {
    clickCount += autoClickers;  // Add 1 click per second for each auto-clicker
    UpdateClickText(hClickText);  // Update the click count display
}

// Function to resize and reposition controls when the window is resized
void ResizeControls(HWND hwnd, int width, int height) {
    int controlWidth = width / 3;  // Controls take up 1/3 of the window width
    int controlHeight = 30;  // Set a fixed control height

    // Center the controls vertically
    int yPos = height / 4;

    // Resize and reposition the auto-clicker count text (Auto-Clickers)
    SetWindowPos(GetDlgItem(hwnd, 0), NULL, width / 2 - controlWidth / 2, yPos, controlWidth, controlHeight, SWP_NOZORDER);
    yPos += controlHeight + 20;  // Add space between controls

    // Resize and reposition the click count text (Clicks)
    SetWindowPos(GetDlgItem(hwnd, 1), NULL, width / 2 - controlWidth / 2, yPos, controlWidth, controlHeight, SWP_NOZORDER);
    yPos += controlHeight + 20;

    // Resize and reposition the click button (Click Me)
    SetWindowPos(GetDlgItem(hwnd, 2), NULL, width / 2 - controlWidth / 2, yPos, controlWidth, controlHeight, SWP_NOZORDER);
    yPos += controlHeight + 20;

    // Resize and reposition the buy auto-clicker button (Buy Auto-Clicker)
    SetWindowPos(GetDlgItem(hwnd, 3), NULL, width / 2 - controlWidth / 2, yPos, controlWidth, controlHeight, SWP_NOZORDER);
}
