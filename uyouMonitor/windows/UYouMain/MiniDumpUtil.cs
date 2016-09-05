using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;

namespace UYouMain
{
    class MiniDumpUtil
    {
        [DllImport("kernel32.dll")]
        private static extern int GetCurrentThreadId();

        [DllImport("DbgHelp.dll")]
        private static extern bool MiniDumpWriteDump(IntPtr hProcess, int processId, IntPtr fileHandle, MiniDumpType dumpType, ref MiniDumpExceptionInfo excepInfo, IntPtr userInfo, IntPtr extInfo);

        [DllImport("DbgHelp.dll")]
        private static extern bool MiniDumpWriteDump(IntPtr hProcess, int processId, IntPtr fileHandle, MiniDumpType dumpType, IntPtr excepParam, IntPtr userInfo, IntPtr extInfo);

        public static bool TryWriteMiniDump(string dmpFileName, MiniDumpType dmpType)
        {
            try
            {
                using (FileStream stream = new FileStream(dmpFileName, FileMode.OpenOrCreate))
                {
                    Process process = Process.GetCurrentProcess();
                    MiniDumpExceptionInfo exceptionInfo = new MiniDumpExceptionInfo();
                    exceptionInfo.ThreadId = GetCurrentThreadId();
                    exceptionInfo.ExceptionPointers = Marshal.GetExceptionPointers();
                    exceptionInfo.ClientPointers = true;
                    if (exceptionInfo.ExceptionPointers == IntPtr.Zero)
                    {
                        return MiniDumpWriteDump(process.Handle, process.Id, stream.SafeFileHandle.DangerousGetHandle(), dmpType, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero);
                    }
                    else
                    {
                        return MiniDumpWriteDump(process.Handle, process.Id, stream.SafeFileHandle.DangerousGetHandle(), dmpType, ref exceptionInfo, IntPtr.Zero, IntPtr.Zero);
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            return true;
        }
    }
    [Flags]
    public enum MiniDumpType
    {
        MiniDumpNormal = 0x00000000,
        MiniDumpWithDataSegs = 0x00000001,
        MiniDumpWithFullMemory = 0x00000002,
        MiniDumpWithHandleData = 0x00000004,
        MiniDumpFilterMemory = 0x00000008,
        MiniDumpScanMemory = 0x00000010,
        MiniDumpWithUnloadedModules = 0x00000020,
        MiniDumpWithIndirectlyReferencedMemory = 0x00000040,
        MiniDumpFilterModulePaths = 0x00000080,
        MiniDumpWithProcessThreadData = 0x00000100,
        MiniDumpWithPrivateReadWriteMemory = 0x00000200,
        MiniDumpWithoutOptionalData = 0x00000400,
        MiniDumpWithFullMemoryInfo = 0x00000800,
        MiniDumpWithThreadInfo = 0x00001000,
        MiniDumpWithCodeSegs = 0x00002000,
        MiniDumpWithoutAuxiliaryState = 0x00004000,
        MiniDumpWithFullAuxiliaryState = 0x00008000,
        MiniDumpWithPrivateWriteCopyMemory = 0x00010000,
        MiniDumpIgnoreInaccessibleMemory = 0x00020000,
        MiniDumpWithTokenInformation = 0x00040000,
        MiniDumpWithModuleHeaders = 0x00080000,
        MiniDumpFilterTriage = 0x00100000,
        MiniDumpValidTypeFlags = 0x001fffff
    }

    public struct MiniDumpExceptionInfo
    {
        public int ThreadId;
        public IntPtr ExceptionPointers;
        public bool ClientPointers;
    }
}
