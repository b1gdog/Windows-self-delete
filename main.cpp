#include <windows.h>
#include <string>
#pragma warning(disable: 4996)

std::wstring convertToWString(const char* str) {
    int strLength = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
    std::wstring wstr(strLength, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], strLength);
    return wstr;
}

bool deleteFile(const std::wstring& filePath) {

    HANDLE hFile;
    hFile = CreateFileW(filePath.c_str(), DELETE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    const std::wstring newFileName = L":deadExe";
    DWORD renameInfoSize = sizeof(FILE_RENAME_INFO) + newFileName.size() * sizeof(wchar_t);
    FILE_RENAME_INFO* renameInfo = (FILE_RENAME_INFO*)new BYTE[renameInfoSize];
    renameInfo->ReplaceIfExists = TRUE;
    renameInfo->RootDirectory = NULL;
    renameInfo->FileNameLength = newFileName.size() * sizeof(wchar_t);
    wcscpy(renameInfo->FileName, newFileName.c_str());

    if (!SetFileInformationByHandle(hFile, FileRenameInfo, renameInfo, renameInfoSize)) {
        delete[] renameInfo;
        CloseHandle(hFile);
        return false;
    }

    delete[] renameInfo;
    CloseHandle(hFile);

    hFile = CreateFileW(filePath.c_str(), DELETE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    FILE_DISPOSITION_INFO deleteInfo;
    deleteInfo.DeleteFileW = TRUE;

    if (!SetFileInformationByHandle(hFile, FileDispositionInfo, &deleteInfo, sizeof(deleteInfo))) {
        CloseHandle(hFile);
        return false;
    }

    CloseHandle(hFile);
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        MessageBoxA(NULL, "NULL", "2222222222", MB_OK);
        return 1;
    }
    std::wstring filePath = convertToWString(argv[0]);
    if (!deleteFile(filePath)) {
        return 1;
    }
    MessageBoxA(NULL, "NULL", "OK", MB_OK);
    return 0;
}
