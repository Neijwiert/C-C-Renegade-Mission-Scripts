using System.IO;
using System.Reflection;

namespace ScriptUsageFinder
{
    abstract class DumpFile
    {
        private readonly byte[] rawFileData;

        public DumpFile(byte[] rawFileData)
        {
            this.rawFileData = rawFileData;
        }

        public void ParseFile()
        {
            using (Stream stream = new MemoryStream(this.rawFileData))
            {
                using (StreamReader reader = new StreamReader(stream))
                {
                    InternalParseFile(reader);
                }
            }
        }

        protected abstract void InternalParseFile(StreamReader reader);
    }
}
