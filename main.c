#include <stdio.h>
#include <windows.h>

void *MainThread(void *vargp);

int main(int argc, char** argv)
{
    MainThread(NULL);
    return 0;
}

void *MainThread(void *vargp)
{
    BOOL bWasPressed = FALSE;
    char title[256];
    int totalWindows = 1;
    int *winList = malloc(totalWindows);
    winList[0] = 0;
    while(1)
    {
        Sleep(10);
        if(GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(0x51) & 0x8000)
        {
            free(winList);
            return NULL;
        }
        if(GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(VK_LSHIFT) & 0x8000)
        {
            if(bWasPressed == FALSE)
                continue;
            HWND hwnd = GetForegroundWindow();
            BOOL addNew = TRUE;
            for(int i = 0; i < totalWindows; i++)
            {
                if(winList[i] == GetWindowThreadProcessId(hwnd, NULL))
                {
                    addNew = FALSE;
                    int *newList = malloc(totalWindows-1);
                    for(int x = 0, j = 0; j < totalWindows; j++)
                    {
                        if(j == i)
                            continue;
                        else
                            newList[x++] = winList[j];
                    }
                    free(winList);
                    winList = newList;
                    totalWindows--;
                    
                    SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 1000, 800, SWP_NOMOVE | SWP_NOSIZE);
                    GetWindowTextA(hwnd, title, 256);
                    printf("Window \"%s\", unset from TOP\n", title);
                    fflush(stdout);
                }
            }
            if(addNew == TRUE)
            {
                GetWindowTextA(hwnd, title, 256);
                printf("Window \"%s\" set ONTOP\n", title);
                fflush(stdout);
                SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 1000, 800, SWP_NOMOVE | SWP_NOSIZE);
                if(totalWindows == 1)
                {
                    winList[0] = GetWindowThreadProcessId(hwnd, NULL);
                    totalWindows++;
                }
                else
                {
                    winList = realloc(winList, totalWindows + 1);
                    winList[totalWindows] = GetWindowThreadProcessId(hwnd, NULL);
                    totalWindows++;
                }
            }
            bWasPressed = FALSE;
        }
        else
            bWasPressed = TRUE;
    }
    free(winList);
}