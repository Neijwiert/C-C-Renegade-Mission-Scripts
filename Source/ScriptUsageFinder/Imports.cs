using System;
using System.Runtime.InteropServices;

namespace ScriptUsageFinder
{
    internal static class Imports
    {
        [DllImport("user32.dll", EntryPoint = "SendMessage", CharSet = CharSet.Unicode)]
        public static extern IntPtr SendMessage(IntPtr windowHandle, int message, int wParam, string text);

        [DllImport("user32.dll", EntryPoint = "FindWindowEx")]
        public static extern IntPtr FindWindowEx(IntPtr hwndParent, IntPtr hwndChildAfter, string lpszClass, string lpszWindow);


        public const int WM_SETTEXT = 0x000C;
    }
}
