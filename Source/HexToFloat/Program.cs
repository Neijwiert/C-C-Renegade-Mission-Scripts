using System;
using System.Globalization;
using System.Linq;
using System.Windows.Forms;

namespace HexToFloat
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Console.WriteLine("Press ESC to exit program");
            do
            {
                while (!Console.KeyAvailable)
                {
                    string line = Console.ReadLine();
                    if(line == null)
                    {
                        continue;
                    }

                    if(line.StartsWith("0x", true, CultureInfo.InvariantCulture))
                    {
                        line = line.Substring(2);
                    }

                    if (int.TryParse(line, NumberStyles.AllowHexSpecifier, null, out int value))
                    {
                        byte[] bytes = BitConverter.GetBytes(value);

                        float result = BitConverter.ToSingle(bytes, 0);

                        Console.WriteLine(result);

                        Clipboard.SetText(result.ToString() + 'f');
                    }
                }
            } while (Console.ReadKey(true).Key != ConsoleKey.Escape);
        }
    }
}
