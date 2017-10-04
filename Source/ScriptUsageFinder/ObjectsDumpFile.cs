using System.IO;
using System.Text.RegularExpressions;

namespace ScriptUsageFinder
{
    sealed class ObjectsDumpFile : DumpFile
    {
        public static readonly Regex presetNameRegex = new Regex(@"(?<=name )(.*)(?= id)", RegexOptions.Compiled);
        public static readonly Regex presetIdRegex = new Regex(@"(?<=id )(\d+)$", RegexOptions.Compiled);
        public static readonly Regex scriptNameRegex = new Regex(@"(?<=\t)(.*)(?= )", RegexOptions.Compiled);
        public static readonly Regex scriptNameRegex2 = new Regex(@"(?<=\t)(.*)$", RegexOptions.Compiled);
        public static readonly Regex scriptParamsOnNewLineRegex = new Regex(@"\d+$", RegexOptions.Compiled);

        public ObjectsDumpFile(byte[] rawFileData) : base(rawFileData)
        {

        }

        protected override void InternalParseFile(StreamReader reader)
        {
            uint currentPresetId = uint.MinValue;
            string currentPresetName = null;

            string line;
            while((line = reader.ReadLine()) != null)
            {
                if(string.IsNullOrWhiteSpace(line))
                {
                    continue;
                }

                Match presetNameMatch = presetNameRegex.Match(line);
                if(presetNameMatch.Success)
                {
                    string presetName = presetNameMatch.Value;
                    if(string.IsNullOrWhiteSpace(presetName))
                    {
                        throw new IOException("Invalid preset name");
                    }

                    currentPresetName = presetName;

                    Match presetIdMatch = presetIdRegex.Match(line);
                    if(!presetIdMatch.Success)
                    {
                        throw new IOException("Expected a preset id");
                    }

                    string strPresetId = presetIdMatch.Value;
                    if(string.IsNullOrWhiteSpace(strPresetId))
                    {
                        throw new IOException("Invalid preset id");
                    }

                    if (!uint.TryParse(strPresetId, out uint presetId))
                    {
                        throw new IOException("Invalid preset id");
                    }

                    currentPresetId = presetId;

                    Definitions.Instance.RegisterPreset(presetId, presetName);
                }
                else
                {
                    if(currentPresetId == uint.MinValue || currentPresetName == null)
                    {
                        throw new IOException("Expected a preset block, got script instead");
                    }

                    Match scriptNameMatch = scriptNameRegex.Match(line);
                    if(!scriptNameMatch.Success)
                    {
                        scriptNameMatch = scriptNameRegex2.Match(line);
                        if (!scriptNameMatch.Success)
                        {
                            Match scriptParamsOnNewLineMatch = scriptParamsOnNewLineRegex.Match(line);
                            if (scriptParamsOnNewLineMatch.Success)
                            {
                                continue;
                            }

                            continue;
                        }
                    }

                    string scriptName = scriptNameMatch.Value;
                    if(string.IsNullOrWhiteSpace(scriptName))
                    {
                        throw new IOException("Invalid script name");
                    }

                    Definitions.Instance.RegisterPresetScript(currentPresetId, currentPresetName, scriptName);
                }
            }
        }
    }
}
