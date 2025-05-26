#include "DarkMode.hpp"

#ifdef WIN32
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

void ActivateDarkMode(sf::Window& window) {
	BOOL value = TRUE;
	DwmSetWindowAttribute(window.getSystemHandle(), 20, &value, sizeof(value));

    HWND hwnd = window.getSystemHandle();
	
    if (IsWindowVisible(hwnd) && !IsIconic(hwnd)) {
        // Get the current window rect.
        RECT rect = {};
        ::GetWindowRect(hwnd, &rect);

        if (IsZoomed(hwnd)) {
            // Window is currently maximized.
            WINDOWPLACEMENT placement = {};
            GetWindowPlacement(hwnd, &placement);

            // Remember the old restore rect.
            const RECT oldrect = placement.rcNormalPosition;

            // Change the restore rect to almost the same as the current
            // maximized rect.
            placement.rcNormalPosition = rect;
            placement.rcNormalPosition.right -= 1;
            SetWindowPlacement(hwnd, &placement);

            // Restore and then re-maximize the window. Don't update in-between.
            LockWindowUpdate(hwnd);
            ShowWindow(hwnd, SW_SHOWNORMAL);
            ShowWindow(hwnd, SW_SHOWMAXIMIZED);
            LockWindowUpdate(nullptr);

            // Put back the old restore rect.
            placement.rcNormalPosition = oldrect;
            SetWindowPlacement(hwnd, &placement);
    }
        else {
            // Window is currently normal. Change and then restore the window width.
            // Use Defer functions to make the change less noticeable. Don't update
            // in-between.
            HDWP defer = BeginDeferWindowPos(2);
            DeferWindowPos(defer, hwnd, NULL, 0, 0,
                rect.right - rect.left - 1, rect.bottom - rect.top,
                SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
            DeferWindowPos(defer, hwnd, NULL, 0, 0, rect.right - rect.left,
                rect.bottom - rect.top,
                SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
            LockWindowUpdate(hwnd);
            EndDeferWindowPos(defer);
            LockWindowUpdate(nullptr);
        }
}
}

#else 

void ActivateDarkMode(sf::Window& window) {
	
}
#endif