#include "sendKey.h"

void GenerateKey(BYTE vk)
{
    INPUT input;
    // ZeroMemory(&input, sizeof(input));
    input.ki.time = 0;
    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
    input.ki.wVk = vk;
    SendInput(1, &input, sizeof(INPUT));

    return;
}

void SendText(char *szText)
{
    for (int i = 0; i < strlen(szText); i++)
    {
        GenerateKey((UCHAR)VkKeyScan(szText[i]));
    }
}

void PressEnter()
{
    INPUT input;
    // ZeroMemory(&input, sizeof(input));
    input.ki.time = 0;
    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
    input.ki.wVk = VK_RETURN;
    SendInput(1, &input, sizeof(INPUT));
}

void SendCheatCode(char *cheat)
{
    PressEnter();
    Sleep(20);
    SendText(cheat);
    Sleep(20);
    PressEnter();
    Sleep(20);
}