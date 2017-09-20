using System.Collections.Generic;

namespace ScriptUsageFinder
{
    sealed class PresetDefintions
    {
        public class Preset
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
                PresetDefintions.instance.RegisterScript(script);

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

        private static readonly PresetDefintions instance = new PresetDefintions();

        private readonly ISet<string> scripts;
        private readonly IDictionary<uint, Preset> presets;

        private PresetDefintions()
        {
            this.scripts = new HashSet<string>();
            this.presets = new Dictionary<uint, Preset>();
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

        public static PresetDefintions Instance
        {
            get
            {
                return instance;
            }
        }
    }
}
