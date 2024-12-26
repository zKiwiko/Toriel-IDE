#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include <QString>

class ZenStudio {
private:
    enum RemoteCMD {
        GpcTab = 1,
        BulildandRun = 2
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
