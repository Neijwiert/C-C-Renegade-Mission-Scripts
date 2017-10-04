using System.Collections.Generic;

namespace ScriptUsageFinder
{
    sealed class Definitions
    {
        public sealed class Preset
        {
            private readonly ISet<string> scripts;

            public Preset(uint id, string name)
            {
                this.scripts = new HashSet<string>();
                this.Id = id;
                this.Name = name;
            }

            public override string ToString()
            {
                return this.Name;
            }

            public bool AddScript(string script)
            {
                Definitions.instance.RegisterScript(script);

                return this.scripts.Add(script);
            }

            public bool HasScript(string script)
            {
                return this.scripts.Contains(script);
            }

            public uint Id
            {
                get;
            }

            public string Name
            {
                get;
            }

            public IReadOnlyCollection<string> Scripts
            {
                get
                {
                    return (IReadOnlyCollection<string>)this.scripts;
                }
            }
        }

        public sealed class CinematicScript
        {
            private readonly IDictionary<string, ISet<int>> cinematicOccurences;

            public CinematicScript(string name)
            {
                Definitions.instance.RegisterScript(name);

                this.cinematicOccurences = new Dictionary<string, ISet<int>>();
                this.Name = name;
            }

            public override string ToString()
            {
                return this.Name;
            }

            public bool AddOccurence(string cinematicFile, int frame)
            {
                if(this.cinematicOccurences.TryGetValue(cinematicFile, out ISet<int> frames))
                {
                    return frames.Add(frame);
                }
                else
                {
                    frames = new HashSet<int>();

                    bool result = frames.Add(frame);

                    this.cinematicOccurences.Add(cinematicFile, frames);

                    return result;
                }

            }

            public string Name
            {
                get;
            }

            public IReadOnlyDictionary<string, IReadOnlyCollection<int>> CinematicOccurences
            {
                get
                {
                    IDictionary<string, IReadOnlyCollection<int>> result = new Dictionary<string, IReadOnlyCollection<int>>();
                    foreach(KeyValuePair<string, ISet<int>> pair in this.cinematicOccurences)
                    {
                        result.Add(pair.Key, (IReadOnlyCollection<int>)pair.Value);
                    }

                    return (IReadOnlyDictionary<string, IReadOnlyCollection<int>>)result;
                }
            }
        }

        private static readonly Definitions instance = new Definitions();

        private readonly ISet<string> scripts;
        private readonly IDictionary<uint, Preset> presets;
        private readonly IDictionary<string, CinematicScript> cinematicScripts;

        private Definitions()
        {
            this.scripts = new HashSet<string>();
            this.presets = new Dictionary<uint, Preset>();
            this.cinematicScripts = new Dictionary<string, CinematicScript>();
        }

        public void RegisterScript(string script)
        {
            this.scripts.Add(script);
        }

        public void RegisterPreset(uint id, string name)
        {
            if (!this.presets.ContainsKey(id))
            {
                this.presets.Add(id, new Preset(id, name));
            }
        }

        public void RegisterPresetScript(uint presetId, string presetName, string script)
        {
            if (this.presets.TryGetValue(presetId, out Preset preset))
            {
                preset.AddScript(script);
            }
            else
            {
                preset = new Preset(presetId, presetName);
                preset.AddScript(script);

                this.presets.Add(presetId, preset);
            }
        }

        public void RegisterCinematicScript(string name)
        {
            if(!this.cinematicScripts.ContainsKey(name))
            {
                this.cinematicScripts.Add(name, new CinematicScript(name));
            }
        }

        public void RegisterCinematicScriptOccurence(string name, string cinematicFile, int frame)
        {
            if (this.cinematicScripts.TryGetValue(name, out CinematicScript cinematicScript))
            {
                cinematicScript.AddOccurence(cinematicFile, frame);
            }
            else
            {
                cinematicScript = new CinematicScript(name);
                cinematicScript.AddOccurence(cinematicFile, frame);

                this.cinematicScripts.Add(name, cinematicScript);
            }
        }

        public bool HasScript(string script)
        {
            return this.scripts.Contains(script);
        }

        public IReadOnlyCollection<string> Scripts
        {
            get
            {
                return (IReadOnlyCollection<string>)this.scripts;
            }
        }

        public IReadOnlyDictionary<uint, Preset> Presets
        {
            get
            {
                return (IReadOnlyDictionary<uint, Preset>)this.presets;
            }
        }

        public IReadOnlyDictionary<string, CinematicScript> CinematicScripts
        {
            get
            {
                return (IReadOnlyDictionary<string, CinematicScript>)this.cinematicScripts;
            }
        }

        public static Definitions Instance
        {
            get
            {
                return instance;
            }
        }
    }
}
