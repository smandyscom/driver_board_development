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
        // the second version of filter-generator , which generate integer pulse train rather the float type
        // 2015.04.26
        public enum arguments
        {
            WINDOW_LENGTH = 0,          // as length k , should be odds
            //AREA_SIZE,                 // as area m , pulses accumulation should equal to area size 
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


            int __windowLength = int.Parse(args[(int)arguments.WINDOW_LENGTH]);
            double __areaSize_f = Math.Pow(((__windowLength + 1) / 2), 2);
            int __areasize = 0;
            if (!int.TryParse(__areaSize_f.ToString(), out __areasize))
            {
                Console.Write("Area Size is not integer");
                return;
            }
                //int.Parse(args[(int)arguments.AREA_SIZE]);
            List<float> tempFilter = new List<float>();     //used to store pulses
            //------------------------------------
            //	Triangular filter profile , overall area should equal to 1
            //------------------------------------
            float currentHeight = 0;
            //float slope = (float)(4 / Math.Pow((double)__windowLength, 2));		// slope = 4/L^2 , L the filter length
            float v = (float)(2 * Math.Sqrt(__areasize) / (__windowLength + 1));
            float slope = (float)Math.Pow(v,2);		// slope = 4/L^2 , L the filter length
            for (int i = 0; i < __windowLength; i++)
            {
                if (i < ((__windowLength+1) / 2))
                {
                    // before/including peak
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
            String header = string.Format("int {0}[]=\t{{", args[(int)arguments.ARRAY_NAME]);
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


            // argument tested : 29 225 test test123.txt , well done , Hsien  , 2015.04.26
        }
    }
}
