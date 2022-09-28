#include <windows.h>

#define MAX_TITLE_LEN 256

void* MainThread(void *arg);
void SetTitle(HWND hwnd);
void RestoreTitle(HWND hwnd);

int main(int argc, char** argv)
{
    MainThread(NULL);
    return 0;
}

void* MainThread(void *arg)
{
    BOOL bWasPressed = FALSE;
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
                    
                    RestoreTitle(hwnd);
                    SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 1000, 800, SWP_NOMOVE | SWP_NOSIZE);
                }
            }
            if(addNew == TRUE)
            {
                SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 1000, 800, SWP_NOMOVE | SWP_NOSIZE);
                if(totalWindows == 1)
                {
                    winList[0] = GetWindowThreadProcessId(hwnd, NULL);
                    totalWindows++;
                    SetTitle(hwnd);
                }
                else
                {
                    winList = realloc(winList, totalWindows + 1);
                    winList[totalWindows] = GetWindowThreadProcessId(hwnd, NULL);
                    totalWindows++;
                    SetTitle(hwnd);
                }
            }
            bWasPressed = FALSE;
        }
        else
            bWasPressed = TRUE;
    }
    free(winList);
}

void SetTitle(HWND hwnd)
{
    char title[MAX_TITLE_LEN];
    GetWindowTextA(hwnd, title, MAX_TITLE_LEN);
    strncat(title, " (ONTOP)", MAX_TITLE_LEN - 1);
    SetWindowTextA(hwnd, title);
}

void RestoreTitle(HWND hwnd)
{
    char title[MAX_TITLE_LEN];
    GetWindowTextA(hwnd, title, MAX_TITLE_LEN);
    title[strlen(title) - strlen(" (ONTOP") - 1] = '\0';
    SetWindowTextA(hwnd, title);
}