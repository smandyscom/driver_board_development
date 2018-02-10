using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace filterGenerator
{
    class Program
    {
        public enum arguments
        {
            WINDOW_LENGTH = 0,
            ARRAY_NAME,
            OUTPUT_PATH,
        }

        static void Main(string[] args)
        {
            //args = new String[]{"128","filter","filter.cpp"};     //test use
            Console.WriteLine("Filter generator started.");
            if (args.Length != Enum.GetValues(typeof(arguments)).Length) 
            {
                Console.WriteLine("Error : Insufficient Arguments");
                return;
            }


            int WINDOW_LENGTH = int.Parse(args[(int)arguments.WINDOW_LENGTH]);

            List<float> tempFilter = new List<float>();
            //------------------------------------
            //	Triangular filter profile , overall area should equal to 1
            //------------------------------------
            float currentHeight = 0;
            float slope = (float)(4 / Math.Pow((double)WINDOW_LENGTH, 2));		// slope = 4/L^2 , L the filter length
            for (int i = 0; i < WINDOW_LENGTH; i++)
            {
                if (i < (WINDOW_LENGTH / 2))
                {
                    // before peak
                    tempFilter.Add(currentHeight += slope);
                }
                else
                {
                    //after peak
                    tempFilter.Add(currentHeight -= slope);
                }
            }
            Console.WriteLine("tempFilter Generated , string on building....");

            //-----------------------
            //  Write tempFilter into file
            //-----------------------
            String header = string.Format("float {0}[]=\t{{", args[(int)arguments.ARRAY_NAME]);
            StringBuilder sb = new StringBuilder(header);
            for (int i = 0; i < tempFilter.Count; i++)
            {
                sb.AppendLine(string.Format("{0},",tempFilter[i]));
            }
            sb.AppendLine("};");
            Console.WriteLine("string build , file generating...");
            //-----------------------
            //  Write tempFilter into file
            //-----------------------
            StreamWriter sw = new StreamWriter(args[(int)arguments.OUTPUT_PATH]);
            sw.Write(sb.ToString());
            sw.Close();
        }
    }
}
