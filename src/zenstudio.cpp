#include "zenstudio.h"

#include <iostream>
#include <psapi.h>
#include <QByteArray>

bool ZenStudio::EnumerateProcess(const std::wstring &processName, std::vector<HWND> &hwndList) {
    HWND hwnd = nullptr;

    auto EnumWindowsCallback = [](HWND hwnd, LPARAM lParam) -> BOOL {
        auto& data = *reinterpret_cast<std::pair<std::wstring, std::vector<HWND>&>*>(lParam);
        DWORD processId = 0;
        GetWindowThreadProcessId(hwnd, &processId);

        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
        if (processHandle) {
            wchar_t exeName[MAX_PATH] = { 0 };
            if (GetModuleBaseName(processHandle, nullptr, exeName, MAX_PATH)) {
                if (data.first == exeName) {
                    data.second.push_back(hwnd);
                }
            }
            CloseHandle(processHandle);
        }
        return TRUE;
    };

    std::pair<std::wstring, std::vector<HWND>&> data = { processName, hwndList };
    EnumWindows(EnumWindowsCallback, reinterpret_cast<LPARAM>(&data));

    return !hwndList.empty();
}

void ZenStudio::SendGPCMessage(RemoteCMD cmd, const QString &message) {
    QByteArray messageBytes = message.toUtf8(); // Proper conversion to UTF-8
    COPYDATASTRUCT cds;
    cds.dwData = static_cast<ULONG_PTR>(cmd);
    cds.cbData = static_cast<DWORD>(messageBytes.size());
    cds.lpData = messageBytes.data();

    HWND zenStudioWindow = FindWindow(nullptr, L"ZenStudio");
    if (!zenStudioWindow) {
        std::cerr << "ZenStudio window not found.\n";
        return;
    }

    DWORD processId = 0;
    GetWindowThreadProcessId(zenStudioWindow, &processId);
    if (processId == 0) {
        std::cerr << "Failed to retrieve ZenStudio process ID.\n";
        return;
    }

    std::vector<HWND> hwndList;
    if (EnumerateProcess(L"ZenStudio.exe", hwndList)) {
        for (auto hwnd : hwndList) {
            SendMessage(hwnd, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&cds));
        }
    } else {
        std::cerr << "Failed to enumerate ZenStudio process windows.\n";
    }
}

void ZenStudio::SendToStudio(const QString &message) {
    RemoteCMD cmd = BuildandRun;
    SendGPCMessage(cmd, message);
}
