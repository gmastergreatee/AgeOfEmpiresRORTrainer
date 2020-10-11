#include <iostream>
#include <conio.h>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "sendKey.h"

#define GREEN 2
#define RED 4
#define WHITE 7
#define BRIGHT_GREEN 10
#define BRIGHT_BLUE 11
#define BRIGHT_RED 12
#define BRIGHT_YELLOW 14
#define BRIGHT_WHITE 15

wchar_t *mainProcessName = L"empiresx.exe";
wchar_t *mainModuleName = L"empiresx.exe";
int consoleWidth = 0;

bool woodActive = false;
bool foodActive = false;
bool goldActive = false;
bool stoneActive = false;
bool populationActive = false;
bool viewingIngameCheats = false;

float woodVal = 0;
float foodVal = 0;
float goldVal = 0;
float stoneVal = 0;
float freePopulationVal = 0;
float currentPopulationVal = 0;

DWORD procId = 0;
uintptr_t modbaseAddr = 0;
uintptr_t ptrBase = 0;
uintptr_t displayPtrBase = 0;
uintptr_t mainStuffPtr = 0;
uintptr_t healthItemPtr = 0;

std::vector<unsigned int> allOffsets = {0x40, 0x04, 0x50};
std::vector<unsigned int> woodOffset = {0x4};
std::vector<unsigned int> foodOffset = {0x0};
std::vector<unsigned int> goldOffset = {0xC};
std::vector<unsigned int> stoneOffset = {0x8};
std::vector<unsigned int> freePopulationOffset = {0x10};
std::vector<unsigned int> currentPopulationOffset = {0x2C};

std::vector<unsigned int> healthItemOffsets = {0x4C4, 0x114};
std::vector<unsigned int> healthOffset = {0x30};
std::vector<unsigned int> itemOffset = {0x44};

uintptr_t foodAddr = 0;
uintptr_t woodAddr = 0;
uintptr_t goldAddr = 0;
uintptr_t stoneAddr = 0;
uintptr_t freePopulationAddr = 0;
uintptr_t currentPopulationAddr = 0;

uintptr_t healthAddr = 0;
uintptr_t itemAddr = 0;

float maxStuff = 100000;
float maxFreePopulation = 19;
float maxCurrentPopulation = 1;

HHOOK hHook{NULL};
HANDLE hProcess = 0;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void D_Vals()
{
    if (woodActive)
        SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
    else
        SetConsoleTextAttribute(hConsole, RED);
    std::cout << "(NUMPAD 1)";
    SetConsoleTextAttribute(hConsole, WHITE);
    // std::cout << "Wood Count --------------- :: " << woodVal << std::endl;
    std::cout << " -------- Unlimited Wood" << std::endl;

    if (foodActive)
        SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
    else
        SetConsoleTextAttribute(hConsole, RED);
    std::cout << "(NUMPAD 2)";
    SetConsoleTextAttribute(hConsole, WHITE);
    // std::cout << "Food Count --------------- :: " << foodVal << std::endl;
    std::cout << " -------- Unlimited Food" << std::endl;

    if (goldActive)
        SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
    else
        SetConsoleTextAttribute(hConsole, RED);
    std::cout << "(NUMPAD 3)";
    SetConsoleTextAttribute(hConsole, WHITE);
    // std::cout << "Gold Count --------------- :: " << goldVal << std::endl;
    std::cout << " -------- Unlimited Gold" << std::endl;

    if (stoneActive)
        SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
    else
        SetConsoleTextAttribute(hConsole, RED);
    std::cout << "(NUMPAD 4)";
    SetConsoleTextAttribute(hConsole, WHITE);
    // std::cout << "Stone Count -------------- :: " << stoneVal << std::endl;
    std::cout << " -------- Unlimited Stone" << std::endl;

    // if (populationActive)
    //     SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
    // else
    //     SetConsoleTextAttribute(hConsole, RED);
    // std::cout << "(NUMPAD 5) ";
    // SetConsoleTextAttribute(hConsole, WHITE);
    // std::cout << "Free Population Count ---- :: " << freePopulationVal << std::endl;

    if (populationActive)
        SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
    else
        SetConsoleTextAttribute(hConsole, RED);
    std::cout << "(NUMPAD 5)";
    SetConsoleTextAttribute(hConsole, WHITE);
    // std::cout << "Current Population Count - :: " << currentPopulationVal << std::endl;
    std::cout << " -------- Unlimited Population" << std::endl;

    std::cout << std::endl;

    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE);
    std::cout << "(F6)";
    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << " -------------- Selection gets 9999 health" << std::endl;

    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE);
    std::cout << "(F7)";
    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << " -------------- Selection gets 9999 items" << std::endl;
    
    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE);
    std::cout << "(F8)";
    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << " -------------- Selection gets 1 health" << std::endl;

    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE);
    std::cout << "(F12)";
    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << " ------------- Selection gets 1 item" << std::endl;
    
    std::cout << std::endl;
    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE);
    std::cout << "(HOME)";
    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << " ------------ Important in-game cheat bindings" << std::endl;

    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    std::cout << "(NUMPAD0)";
    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << " --------- Fix trainer not working after new-game" << std::endl;

    SetConsoleTextAttribute(hConsole, BRIGHT_RED);
    std::cout << "(INSERT)";
    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << " ---------- Exit Trainer" << std::endl;
}

void D_Addresses()
{
    std::cout << std::endl;
    // std::cout << "Main Ptr :: 0x" << std::hex << std::uppercase << mainPtr << std::endl
    //           << std::endl;

    // std::cout << "Wood Address --------------- :: 0x" << woodAddr << std::endl;
    // std::cout << "Food Address --------------- :: 0x" << foodAddr << std::endl;
    // std::cout << "Gold Address --------------- :: 0x" << goldAddr << std::endl;
    // std::cout << "Stone Address -------------- :: 0x" << stoneAddr << std::endl;
    // std::cout << "Free Population Address ---- :: 0x" << freePopulationAddr << std::endl;
    // std::cout << "Current Population Address - :: 0x" << currentPopulationAddr << std::endl
    //           << std::endl;
}

void D_Header()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    std::vector<char *> title = {
        "",
        "Age of Empires Expansion Trainer",
        "gmastergreatee",
        "",
    };

    std::vector<int> colors = {
        0,
        BRIGHT_YELLOW,
        BRIGHT_RED,
        0,
    };

    for (int i = 0; i < title.size(); i++)
    {
        int pads = consoleWidth / 2 - (strlen(title[i]) / 2);
        std::string spacer = "";
        while (spacer.length() < pads)
        {
            spacer += " ";
        }

        if (colors[i] > 0)
        {
            SetConsoleTextAttribute(hConsole, colors[i]);
        }

        std::cout << spacer << title[i] << std::endl;
    }
    SetConsoleTextAttribute(hConsole, WHITE);
}

void D_Seperator(char seperatorChar = '=')
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    std::string spacer = "";
    while (spacer.length() < columns)
    {
        spacer += seperatorChar;
    }
    std::cout << spacer;
}

bool Calc_BasePointers()
{
    procId = GetProcId(mainProcessName);
    if (procId == 0)
    {
        return false;
    }

    modbaseAddr = GetModuleBaseAddress(procId, mainModuleName);
    ptrBase = modbaseAddr + 0x2A1808;
    displayPtrBase = modbaseAddr + 0x1830F4;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    return true;
}

void Calc_Addresses()
{
    mainStuffPtr = FindPointerAddress(hProcess, ptrBase, allOffsets);
    healthItemPtr = FindPointerAddress(hProcess, displayPtrBase, healthItemOffsets);
    if (mainStuffPtr > 0)
    {
        foodAddr = FindPointerAddress(hProcess, mainStuffPtr, foodOffset);
        woodAddr = foodAddr + woodOffset[0];
        goldAddr = foodAddr + goldOffset[0];
        stoneAddr = foodAddr + stoneOffset[0];
        freePopulationAddr = foodAddr + freePopulationOffset[0];
        currentPopulationAddr = foodAddr + currentPopulationOffset[0];
    }
}

void Calc_Values()
{
    ReadProcessMemory(hProcess, (BYTE *)woodAddr, &woodVal, sizeof(woodVal), 0);
    ReadProcessMemory(hProcess, (BYTE *)foodAddr, &foodVal, sizeof(foodVal), 0);
    ReadProcessMemory(hProcess, (BYTE *)goldAddr, &goldVal, sizeof(goldVal), 0);
    ReadProcessMemory(hProcess, (BYTE *)stoneAddr, &stoneVal, sizeof(stoneVal), 0);
    ReadProcessMemory(hProcess, (BYTE *)freePopulationAddr, &freePopulationVal, sizeof(freePopulationVal), 0);
    ReadProcessMemory(hProcess, (BYTE *)currentPopulationAddr, &currentPopulationVal, sizeof(currentPopulationVal), 0);
}

void D_RedrawConsole()
{
    system("CLS");
    D_Header();
    D_Seperator();
    D_Addresses();
    D_Vals();
}

bool IsForegroundProcess(DWORD pid)
{
    HWND hwnd = GetForegroundWindow();
    if (hwnd == NULL)
        return false;

    DWORD foregroundPid;
    if (GetWindowThreadProcessId(hwnd, &foregroundPid) == 0)
        return false;

    return (foregroundPid == pid);
}

void RedrawIngameCheats()
{
    system("CLS");
    D_Header();
    D_Seperator();

    std::cout << std::endl;
    SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
    std::cout << "TRAINER CHEATS ";
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    std::cout << "- triggered with the hotkey";
    SetConsoleTextAttribute(hConsole, BRIGHT_RED);
    std::cout << " - wont work with on-screen keyboard" << std::endl;
    SetConsoleTextAttribute(hConsole, WHITE);
    D_Seperator('-');
    std::cout << std::endl;
    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE);
    std::cout << "NUMPAD6";
    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << " -------- bigdaddy (missile car)" << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;
    SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
    std::cout << "OTHER CHEATS ";
    SetConsoleTextAttribute(hConsole, BRIGHT_YELLOW);
    std::cout << " - to be used manually" << std::endl;
    SetConsoleTextAttribute(hConsole, WHITE);
    D_Seperator('-');
    std::cout << std::endl;

    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE);
    std::cout << "steroids";
    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << " ------- super fast game (for enemies too - IRREVERSIBLE)" << std::endl;

    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE);
    std::cout << "hoyohoyo";
    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << " ------- priest hit-points & speed increased (use b4 research)" << std::endl;

    SetConsoleTextAttribute(hConsole, BRIGHT_BLUE);
    std::cout << "diediedie";
    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << " ------ instant victory" << std::endl;

    std::cout << std::endl;

    std::cout << "Press any key to go back..." << std::endl;
    viewingIngameCheats = true;
}

DWORD WINAPI TrainerCheatProc()
{
    // cheat loop
    DWORD dwExit = 0;
    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
    {
        // update pointer if pointer invalid
        if (FindPointerAddress(hProcess, mainStuffPtr, foodOffset) <= 0)
        {
            Calc_Addresses();
        }
        else
        {
            // freeze cheats if flag enabled
            if (woodActive)
            {
                WriteProcessMemory(hProcess, (BYTE *)woodAddr, &maxStuff, sizeof(maxStuff), NULL);
            }

            if (foodActive)
            {
                WriteProcessMemory(hProcess, (BYTE *)foodAddr, &maxStuff, sizeof(maxStuff), NULL);
            }

            if (goldActive)
            {
                WriteProcessMemory(hProcess, (BYTE *)goldAddr, &maxStuff, sizeof(maxStuff), NULL);
            }

            if (stoneActive)
            {
                WriteProcessMemory(hProcess, (BYTE *)stoneAddr, &maxStuff, sizeof(maxStuff), NULL);
            }

            if (populationActive)
            {
                WriteProcessMemory(hProcess, (BYTE *)freePopulationAddr, &maxFreePopulation, sizeof(maxFreePopulation), NULL);
                WriteProcessMemory(hProcess, (BYTE *)currentPopulationAddr, &maxCurrentPopulation, sizeof(maxCurrentPopulation), NULL);
            }
        }
        Sleep(10);
    }

    system("CLS");
    D_Header();
    D_Seperator();
    SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
    std::cout << "Game closed" << std::endl;

    SetConsoleTextAttribute(hConsole, WHITE);
    std::cout << "Press any key to exit.....";
    _getwche();

    exit(0);
}

LRESULT CALLBACK keyboard_hook(const int code, const WPARAM wParam, const LPARAM lParam)
{
    if (wParam == WM_KEYDOWN)
    {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
        DWORD wVirtKey = kbdStruct->vkCode;
        DWORD wScanCode = kbdStruct->scanCode;

        if (IsForegroundProcess(procId))
        {
            // in-game cheats
            if (wVirtKey == VK_NUMPAD6)
            {
                SendCheatCode("BIGDADDY");
                return -1;
            }
        }

        if (!viewingIngameCheats && wVirtKey == VK_HOME)
        {
            RedrawIngameCheats();
        }
        else if (viewingIngameCheats)
        {
            viewingIngameCheats = false;
            D_RedrawConsole();
        }

        // trainer cheats
        bool somethingChanged = false;

        if (wVirtKey == VK_NUMPAD1)
        {
            woodActive = !woodActive;
            somethingChanged = true;
        }

        if (wVirtKey == VK_NUMPAD2)
        {
            foodActive = !foodActive;
            somethingChanged = true;
        }

        if (wVirtKey == VK_NUMPAD3)
        {
            goldActive = !goldActive;
            somethingChanged = true;
        }

        if (wVirtKey == VK_NUMPAD4)
        {
            stoneActive = !stoneActive;
            somethingChanged = true;
        }

        if (wVirtKey == VK_NUMPAD5)
        {
            populationActive = !populationActive;
            somethingChanged = true;
        }

        if (wVirtKey == VK_F6 || wVirtKey == VK_F8)
        {
            healthAddr = FindPointerAddress(hProcess, healthItemPtr, healthOffset);
            if (healthAddr > 0)
            {
                float currentHealth = 0;
                ReadProcessMemory(hProcess, (BYTE *)healthAddr, &currentHealth, sizeof(currentHealth), NULL);
                if (currentHealth > 0)
                {
                    float newHealth = 1;
                    if (wVirtKey == VK_F6)
                    {
                        newHealth = 9999;
                    }
                    WriteProcessMemory(hProcess, (BYTE *)healthAddr, &newHealth, sizeof(newHealth), NULL);
                }
            }
        }

        if (wVirtKey == VK_F7 || wVirtKey == VK_F12)
        {
            itemAddr = FindPointerAddress(hProcess, healthItemPtr, itemOffset);
            if (itemAddr > 0)
            {
                float currItemCount = 0;
                ReadProcessMemory(hProcess, (BYTE *)itemAddr, &currItemCount, sizeof(currItemCount), NULL);
                if (currItemCount > 0)
                {
                    float newItemCount = 1;
                    if (wVirtKey == VK_F7)
                    {
                        newItemCount = 9999;
                    }
                    WriteProcessMemory(hProcess, (BYTE *)itemAddr, &newItemCount, sizeof(newItemCount), NULL);
                }
            }
        }

        if (wVirtKey == VK_NUMPAD0)
        {
            Calc_Addresses();
        }

        if (wVirtKey == VK_INSERT)
        {
            exit(0);
        }

        if (somethingChanged)
        {
            D_RedrawConsole();
        }
    }
    return (CallNextHookEx(hHook, code, wParam, lParam));
}

int main()
{
    D_Header();
    D_Seperator();

    if (!Calc_BasePointers())
    {
        SetConsoleTextAttribute(hConsole, BRIGHT_RED);
        std::wcout << std::endl
                   << "GAME NOT FOUND :: " << mainProcessName << std::endl;

        SetConsoleTextAttribute(hConsole, BRIGHT_GREEN);
        std::cout << "Please start the game and then run the trainer" << std::endl;

        SetConsoleTextAttribute(hConsole, WHITE);
        std::cout << "Press any key to exit.....";
        _getwche();
    }
    else
    {
        Calc_Addresses();
        hHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_hook, NULL, 0);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TrainerCheatProc, NULL, 0, NULL);
        D_Addresses();
        Calc_Values();
        D_Vals();

        // color test
        // for (int k = 1; k <= 15; k++)
        // {
        //     // pick the colorattribute k you want
        //     SetConsoleTextAttribute(hConsole, k);
        //     std::cout << std::dec << k << " I want to be nice today!" << std::endl;
        // }

        GetMessage(NULL, NULL, 0, 0);
    }
    UnhookWindowsHookEx(hHook);
    return 0;
}
