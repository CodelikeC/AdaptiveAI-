#include "file_access_sensor.h"

#include <iostream>

File_access_sensor :: File_access_sensor(const wstring &directoryToMonitor)
: directory(directoryToMonitor), monitoring(false), hdir(nullptr){}

File_access_sensor :: ~File_access_sensor()
{
    stopMonitoring();
}

void File_access_sensor :: setOnFileAccess(function<void(const wstring&, const wstring&)> callback)
{
    onAccess = callback; 
}

void File_access_sensor :: startMonitoring()
{
    if (monitoring) return; 

    hdir = CreateFileW(
        directory.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );
    if (hdir == INVALID_HANDLE_VALUE)
    {
        cout <<"Failed to open directory" << endl; 
        return ; 
    }

    monitoring = true; 
    monitorThread = thread(&File_access_sensor :: monitor, this );
}

void File_access_sensor :: stopMonitoring()
{
    monitoring = false;

    if (hdir != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hdir); 
        hdir = INVALID_HANDLE_VALUE; 
    }

    if (monitorThread.joinable()){
        monitorThread.join();
    }
}

void File_access_sensor :: monitor()
{
    char buffer[1024]; 
    DWORD bytesReturned; 

    while (monitoring)
    {
        if (ReadDirectoryChangesW(
                hdir,
                &buffer,
                sizeof(buffer),
                TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
                FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SECURITY,
                &bytesReturned,
                NULL,
                NULL))
        {
            FILE_NOTIFY_INFORMATION *fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*> (buffer); 

            do 
            {
                wstring fileName(fni -> FileName, fni ->FileNameLength / sizeof(WCHAR)); 
                wstring actionStr; 

                switch(fni -> Action)
                {
                    case FILE_ACTION_ADDED: actionStr = L"ADDED"; break;
                    case FILE_ACTION_REMOVED: actionStr = L"REMOVED"; break;
                    case FILE_ACTION_MODIFIED: actionStr = L"MODIFIED"; break;
                    case FILE_ACTION_RENAMED_OLD_NAME: actionStr = L"RENAMED_FROM"; break;
                    case FILE_ACTION_RENAMED_NEW_NAME: actionStr = L"RENAMED_TO"; break;
                    default: actionStr = L"UNKNOWN"; 
                    break;
                }
                if (onAccess)
                {
                    onAccess(fileName, actionStr); 
                }
                if (fni -> NextEntryOffset == 0)
                {
                    break;
                }
                fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
                    reinterpret_cast<BYTE*>(fni) + fni->NextEntryOffset
                );
            }while (true); 
        }
        else 
        {
            cout <<"ReadirectoryChangesW failed" << endl; 
            break; 
        }
        Sleep(100); // Gioi han tan suat polling ..//
    }
}
