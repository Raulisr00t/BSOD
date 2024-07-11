# Windows BSOD Trigger Example

This project demonstrates how to trigger a Blue Screen of Death (BSOD) on a Windows system by using the `RtlAdjustPrivilege` and `NtRaiseHardError` functions from the `ntdll.dll` library. **Use with extreme caution as this will crash your system and can lead to data loss.**

## Description

The provided code enables the `SeShutdownPrivilege` and then calls the `NtRaiseHardError` function with a specific error status (`STATUS_SYSTEM_PROCESS_TERMINATED`). This will cause the system to crash and display a BSOD.

## Requirements

- Windows Operating System
- C++ Compiler (e.g., Visual Studio)

## Disclaimer

**This code is for educational purposes only. Running this code will cause your system to crash. Use it only in a controlled environment where data loss is not a concern. The author is not responsible for any damage caused by the use of this code.**

## Usage

1. Clone the repository or download the code file.
2. Open the project in your preferred C++ development environment (e.g., Visual Studio).
3. Build the project.
4. Run the compiled executable.

## Code Explanation

```cpp
#include <iostream>
#include <Windows.h>
#include <winternl.h>

#define SeShutdownPrivilege 19
#define STATUS_SYSTEM_PROCESS_TERMINATED ((NTSTATUS)0xC000021A)

using namespace std;

typedef NTSTATUS(NTAPI *pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

int main() {
    // Load RtlAdjustPrivilege
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

    // Load NtRaiseHardError
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
```
## Key Functions
RtlAdjustPrivilege: Enables or disables privileges for the current process.
NtRaiseHardError: Triggers a hard error, causing a system crash.

## Important Notes
Use this code responsibly. Running it on a production or personal system can cause serious disruptions and data loss.
Always test such code in a virtual machine or a controlled test environment.

## License
This project is licensed under the MIT License - see the LICENSE file for details.
