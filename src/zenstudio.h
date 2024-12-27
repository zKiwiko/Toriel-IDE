#pragma once

#include <string>
#include <vector>
#include <QString>
#include <windows.h>

class ZenStudio {
private:
    enum RemoteCMD {
        GpcTab = 1,
        BuildandRun = 2
    };

    struct CopyData {
        RemoteCMD dwData;
        int cbData;
        void* lpData;
    };

    bool EnumerateProcess(const std::wstring& processName, std::vector<HWND>& hwndList);
    void SendGPCMessage(RemoteCMD cmd, const QString& message);

public:
    void SendToStudio(const QString& message);

};
