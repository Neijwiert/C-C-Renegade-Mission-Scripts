using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace ScriptUsageFinder
{
    class Program
    {
        private enum Mode
        {
            INVALID_MODE = -1,
            ALL_SCRIPTS_MODE,
            ALL_USAGES_MODE
        }

        static void Main(string[] args)
        {
            try
            {
                IDictionary<string, LevelDumpFile> levels = new Dictionary<string, LevelDumpFile>();

                ObjectsDumpFile objectsDumpFile = new ObjectsDumpFile(Properties.Resources.objects);
                objectsDumpFile.ParseFile();

                LevelDumpFile levelDumpFile01 = new LevelDumpFile(Properties.Resources.m01);
                levelDumpFile01.ParseFile();
                levels.Add(nameof(Properties.Resources.m01), levelDumpFile01);

                LevelDumpFile levelDumpFile02 = new LevelDumpFile(Properties.Resources.m02);
                levelDumpFile02.ParseFile();
                levels.Add(nameof(Properties.Resources.m02), levelDumpFile02);

                LevelDumpFile levelDumpFile03 = new LevelDumpFile(Properties.Resources.m03);
                levelDumpFile03.ParseFile();
                levels.Add(nameof(Properties.Resources.m03), levelDumpFile03);

                LevelDumpFile levelDumpFile04 = new LevelDumpFile(Properties.Resources.m04);
                levelDumpFile04.ParseFile();
                levels.Add(nameof(Properties.Resources.m04), levelDumpFile04);

                LevelDumpFile levelDumpFile05 = new LevelDumpFile(Properties.Resources.m05);
                levelDumpFile05.ParseFile();
                levels.Add(nameof(Properties.Resources.m05), levelDumpFile05);

                LevelDumpFile levelDumpFile06 = new LevelDumpFile(Properties.Resources.m06);
                levelDumpFile06.ParseFile();
                levels.Add(nameof(Properties.Resources.m06), levelDumpFile06);

                LevelDumpFile levelDumpFile07 = new LevelDumpFile(Properties.Resources.m07);
                levelDumpFile07.ParseFile();
                levels.Add(nameof(Properties.Resources.m07), levelDumpFile07);

                LevelDumpFile levelDumpFile08 = new LevelDumpFile(Properties.Resources.m08);
                levelDumpFile08.ParseFile();
                levels.Add(nameof(Properties.Resources.m08), levelDumpFile08);

                LevelDumpFile levelDumpFile09 = new LevelDumpFile(Properties.Resources.m09);
                levelDumpFile09.ParseFile();
                levels.Add(nameof(Properties.Resources.m09), levelDumpFile09);

                LevelDumpFile levelDumpFile10 = new LevelDumpFile(Properties.Resources.m10);
                levelDumpFile10.ParseFile();
                levels.Add(nameof(Properties.Resources.m10), levelDumpFile10);

                LevelDumpFile levelDumpFile11 = new LevelDumpFile(Properties.Resources.m11);
                levelDumpFile11.ParseFile();
                levels.Add(nameof(Properties.Resources.m11), levelDumpFile11);

                LevelDumpFile levelDumpFile13 = new LevelDumpFile(Properties.Resources.m13);
                levelDumpFile13.ParseFile();
                levels.Add(nameof(Properties.Resources.m13), levelDumpFile13);

                LevelDumpFile levelDumpFileTu = new LevelDumpFile(Properties.Resources.mtu);
                levelDumpFileTu.ParseFile();
                levels.Add(nameof(Properties.Resources.mtu), levelDumpFileTu);

                Mode mode = GetMode();
                switch (mode)
                {
                    case Mode.ALL_SCRIPTS_MODE:
                        LevelDumpFile level = GetLevel(levels);
                        PrintAllScriptsForLevel(level);

                        break;
                    case Mode.ALL_USAGES_MODE:
                        string script = GetScriptName();
                        PrintAllReferencesForScript(levels, script);

                        break;
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex);

                Console.ReadLine();
            }
        }

        private static Mode GetMode()
        {
            Mode result = Mode.INVALID_MODE;
            while (result == Mode.INVALID_MODE)
            {
                Console.Write($"What mode ({(int)Mode.ALL_SCRIPTS_MODE} = all scripts for level, {(int)Mode.ALL_USAGES_MODE} = all usages for script): ");

                string strMode = Console.ReadLine();
                if (string.IsNullOrWhiteSpace(strMode))
                {
                    continue;
                }

                if (!int.TryParse(strMode, out int intMode))
                {
                    continue;
                }

                result = (Mode)intMode;
            }

            return result;
        }

        private static Process StartNotepad()
        {
            Process notepadProcess = Process.Start("notepad.exe");
            notepadProcess.WaitForInputIdle();

            return notepadProcess;
        }

        private static void WriteToNotepad(Process notepadProcess, string text)
        {
            IntPtr hwnd = Imports.FindWindowEx(notepadProcess.MainWindowHandle, new IntPtr(0), "Edit", null);
            Imports.SendMessage(hwnd, Imports.WM_SETTEXT, 0, text);
        }

        private static LevelDumpFile GetLevel(IDictionary<string, LevelDumpFile> levels)
        {
            LevelDumpFile result = null;
            while(result == null)
            {
                Console.Write("What level (examples: mtu or m01): ");

                string level = Console.ReadLine();
                if(string.IsNullOrWhiteSpace(level))
                {
                    continue;
                }

                level = level.ToLower();

                levels.TryGetValue(level, out result);
            }

            return result;
        }

        private static void PrintAllScriptsForLevel(LevelDumpFile file)
        {
            Process notepadProcess = StartNotepad();

            IDictionary<string, ISet<int>> referencedScripts = new Dictionary<string, ISet<int>>();
            foreach(LevelDumpFile.CreatedObject obj in file.Objects.Values)
            {
                foreach(string referencedScript in obj.GetReferencedScripts())
                {
                    if (referencedScripts.TryGetValue(referencedScript, out ISet<int> objIds))
                    {
                        objIds.Add(obj.Id);
                    }
                    else
                    {
                        objIds = new HashSet<int>
                        {
                            obj.Id
                        };

                        referencedScripts.Add(referencedScript, objIds);
                    }
                }
            }

            StringBuilder builder = new StringBuilder();
            foreach (KeyValuePair<string, ISet<int>> entry in referencedScripts)
            {
                builder.Append(entry.Key).Append(" ->");

                foreach (int objId in entry.Value)
                {
                    builder.Append(' ').Append(objId);
                }

                builder.Append(Environment.NewLine);
            }

            WriteToNotepad(notepadProcess, builder.ToString());
        }

        private static string GetScriptName()
        {
            string result = null;
            while(result == null)
            {
                Console.Write("What script (case-sensitive): ");

                string script = Console.ReadLine();
                if(PresetDefintions.Instance.HasScript(script))
                {
                    result = script;
                }
            }

            return result;
        }

        private static void PrintAllReferencesForScript(IDictionary<string, LevelDumpFile> levels, string scriptName)
        {
            Process notepadProcess = StartNotepad();

            StringBuilder builder = new StringBuilder();
            foreach(KeyValuePair<string, LevelDumpFile> entry in levels)
            {
                string levelName = entry.Key.ToUpper();
                LevelDumpFile level = entry.Value;

                bool prependedLevelName = false;

                foreach(LevelDumpFile.CreatedObject obj in level.Objects.Values)
                {
                    if(obj.HasScript(scriptName))
                    {
                        if(!prependedLevelName)
                        {
                            prependedLevelName = true;

                            builder.Append(levelName).Append(" ->");
                        }

                        builder.Append(' ').Append(obj.Id);
                    }
                }

                if(prependedLevelName)
                {
                    builder.Append(Environment.NewLine);
                }
            }

            WriteToNotepad(notepadProcess, builder.ToString());
        }
    }
}
