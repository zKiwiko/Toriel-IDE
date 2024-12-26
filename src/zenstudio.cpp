#include "zenstudio.h"

#include <iostream>
#include <winuser.h>

bool ZenStudio::EnumerateProcess(const std::wstring &processName, std::vector<HWND> &hwndList) {
    HWND hwnd = nullptr;

    auto EnumWIndowsCallback = [](HWND hwnd, LPARAM lParam) -> BOOL {
        auto& data = *reinterpret_cast<std::pair<std::wstring, std::vector<HWND>&>*>(lParam);
        DWORD processId = 0;

        GetWindowThreadProcessId(hwnd, &processId);

        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
        if(processId == data.first) {
            data.second.push_back(hwnd);
        }
        return TRUE;
    };
    std::pair<DWORD, std::vector<HWND>&> data = { targetProcessId, hwndList };
    EnumWindows(EnumWindowsCallback, reinterpret_cast<LPARAM>(&data));

    return !hwndList.empty();
}

void ZenStudio::SendGPCMessage(RemoteCMD cmd, const QString &message) {
    CopyData cds;
    cds.dwData = cmd;
    cds.cbData = static_cast<int>(message.size());
    cds.lpData = const_cast<char*>(message.toStdString.c_str());

    HWND zenStudioWindow = FindWindow(nullptr, L"ZenStudio");
    if(!zenStudioWindow) {
        return;
    }

    DWORD processId = 0;
    GetWindowThreadProcessId(zenStudioWindow, &processId);
    if(processId == 0) {
        return;
    }
    std::vector<HWND> hwndList;
    if (EnumerateProcess(processId, hwndList)) {
        for (auto hwnd : hwndList) {
            SendMessage(hwnd, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&cds));
        }
    } else {
        return;
    }
}

void ZenStudio::SendToStudio(const QString &message) {
    SendGPCMessage(GpcTab, message);
}
