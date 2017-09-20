using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace ScriptUsageFinder
{
    sealed class LevelDumpFile : DumpFile
    {
        public class CreatedObject
        {
            private readonly ISet<string> scripts;

            public CreatedObject(int id, uint presetId)
            {
                this.scripts = new HashSet<string>();
                this.Id = id;
                this.PresetId = presetId;
            }

            public bool AddScript(string script)
            {
                PresetDefintions.Instance.RegisterScript(script);

                return this.scripts.Add(script);
            }

            public IReadOnlyCollection<string> GetReferencedScripts()
            {
                ISet<string> referencedScripts = new HashSet<string>(this.scripts);

                foreach(string script in this.Preset.Scripts)
                {
                    referencedScripts.Add(script);
                }

                return (IReadOnlyCollection<string>)referencedScripts;
            }

            public bool HasScript(string script)
            {
                if(this.scripts.Contains(script))
                {
                    return true;
                }

                return this.Preset.HasScript(script);
            }

            public int Id
            {
                get;
            }

            public uint PresetId
            {
                get;
            }

            public PresetDefintions.Preset Preset
            {
                get
                {
                    PresetDefintions.Instance.Presets.TryGetValue(this.PresetId, out PresetDefintions.Preset preset);

                    return preset;
                }
            }

            public IReadOnlyCollection<string> Scripts
            {
                get
                {
                    return (IReadOnlyCollection<string>)this.scripts;
                }
            }
        }

        public static readonly Regex presetIdRegex = new Regex(@"(?<= defintion id )(\d+)$", RegexOptions.Compiled);
        public static readonly Regex objectIdRegex = new Regex(@"(?<= id )(\d+)(?= defintion id )", RegexOptions.Compiled);
        public static readonly Regex scriptNameRegex = new Regex(@"(?<=\t)(.*)(?= )", RegexOptions.Compiled);
        public static readonly Regex scriptNameRegex2 = new Regex(@"(?<=\t)(.*)$", RegexOptions.Compiled);

        private readonly IDictionary<int, CreatedObject> objects;

        public LevelDumpFile(byte[] rawFileData) : base(rawFileData)
        {
            this.objects = new Dictionary<int, CreatedObject>();
        }

        protected override void InternalParseFile(StreamReader reader)
        {
            int currentObjectId = int.MinValue;
            uint currentPresetId = uint.MinValue;

            string line;
            while ((line = reader.ReadLine()) != null)
            {
                if (string.IsNullOrWhiteSpace(line))
                {
                    continue;
                }

                Match presetIdMatch = presetIdRegex.Match(line);
                if(presetIdMatch.Success)
                {
                    string strPresetId = presetIdMatch.Value;
                    if(string.IsNullOrWhiteSpace(strPresetId))
                    {
                        throw new IOException("Invalid preset id");
                    }

                    if (!uint.TryParse(strPresetId, out uint presetId))
                    {
                        throw new IOException("Invalid preset id");
                    }

                    Match objectIdMatch = objectIdRegex.Match(line);
                    if(!objectIdMatch.Success)
                    {
                        throw new IOException("Expected an object id");
                    }

                    string strObjectId = objectIdMatch.Value;
                    if(string.IsNullOrWhiteSpace(strObjectId))
                    {
                        throw new IOException("Invalid object id");
                    }

                    if (!int.TryParse(strObjectId, out int objectId))
                    {
                        throw new IOException("Invalid object id");
                    }

                    currentObjectId = objectId;
                    currentPresetId = presetId;

                    this.objects.Add(objectId, new CreatedObject(objectId, presetId));
                }
                else
                {
                    if (currentObjectId == int.MinValue || currentPresetId == uint.MinValue)
                    {
                        throw new IOException("Expected an object block, got script instead");
                    }

                    Match scriptNameMatch = scriptNameRegex.Match(line);
                    if(!scriptNameMatch.Success)
                    {
                        scriptNameMatch = scriptNameRegex2.Match(line);
                        if (!scriptNameMatch.Success)
                        {
                            throw new IOException("Expected a script name");
                        }
                    }

                    string scriptName = scriptNameMatch.Value;
                    if(string.IsNullOrWhiteSpace(scriptName))
                    {
                        continue;
                    }

                    if (this.objects.TryGetValue(currentObjectId, out CreatedObject createdObject))
                    {
                        createdObject.AddScript(scriptName);
                    }
                    else
                    {
                        createdObject = new CreatedObject(currentObjectId, currentPresetId);
                        createdObject.AddScript(scriptName);

                        this.objects.Add(currentObjectId, createdObject);
                    }
                }
            }
        }

        public IReadOnlyDictionary<int, CreatedObject> Objects
        {
            get
            {
                return (IReadOnlyDictionary<int, CreatedObject>)this.objects;
            }
        }
    }
}
