#include "zenstudio.h"
#include <iostream>
#include <psapi.h>
#include <QByteArray>
#include <QMessageBox>
#include <QDebug>

bool ZenStudio::EnumerateProcess(const std::wstring &processName, std::vector<HWND> &hwndList) {
    HWND hwnd = nullptr;
    auto EnumWindowsCallback = [](HWND hwnd, LPARAM lParam) -> BOOL {
        auto& data = *reinterpret_cast<std::pair<std::wstring, std::vector<HWND>&>*>(lParam);

        if (!IsWindowVisible(hwnd)) {
            return TRUE;
        }

        DWORD processId = 0;
        GetWindowThreadProcessId(hwnd, &processId);

        wchar_t windowTitle[256];
        GetWindowTextW(hwnd, windowTitle, 256);
        qDebug() << "Found window:" << QString::fromWCharArray(windowTitle)
                 << "Process ID:" << processId;

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
    HWND zenStudioWindow = FindWindow(nullptr, L"ZenStudio");

    if (!zenStudioWindow) {

        std::vector<HWND> hwndList;
        if (EnumerateProcess(L"ZenStudio.exe", hwndList)) {
            qDebug() << "Found" << hwndList.size() << "ZenStudio windows";

            QByteArray messageBytes = message.toUtf8();
            COPYDATASTRUCT cds;
            cds.dwData = static_cast<ULONG_PTR>(cmd);
            cds.cbData = static_cast<DWORD>(messageBytes.size());
            cds.lpData = messageBytes.data();

            for (auto hwnd : hwndList) {
                if (hwnd != nullptr) {
                    qDebug() << "Sending message to HWND:" << hwnd;
                    SendMessage(hwnd, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&cds));
                }
            }
        } else {
            QMessageBox::critical(nullptr, "Error", "ZenStudio window not found. Please ensure ZenStudio is running.");
            return;
        }
    } else {
        QByteArray messageBytes = message.toUtf8();
        COPYDATASTRUCT cds;
        cds.dwData = static_cast<ULONG_PTR>(cmd);
        cds.cbData = static_cast<DWORD>(messageBytes.size());
        cds.lpData = messageBytes.data();
        SendMessage(zenStudioWindow, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&cds));
    }
}

void ZenStudio::SendToStudio(const QString &message) {
    RemoteCMD cmd = BuildandRun;
    SendGPCMessage(cmd, message);
}
