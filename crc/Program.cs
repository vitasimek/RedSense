using System;
using System.Linq;
using System.Collections.Generic;
using NullFX.CRC;

namespace crc
{
    class Program
    {
        static void Main(string[] args)
        {
            var sums = new Dictionary<uint, long>();

            for (long i = 0; i < Math.Pow(2, 24); i++)
            {
                uint sum = Crc4.ComputeChecksum(
                    (byte)((i & 0x00000F) >> 0x00), (byte)((i & 0x0000F0) >> 0x04), (byte)((i & 0x000F00) >> 0x08),
                    (byte)((i & 0x00F000) >> 0x0C), (byte)((i & 0x0F0000) >> 0x10), (byte)((i & 0xF00000) >> 0x14));

                if (sums.ContainsKey(sum))
                {
                    sums[sum]++;
                }
                else
                {
                    sums.Add(sum, 1);
                }
            }

            var orderedSums = from s in sums
                              orderby s.Value descending
                              select s;

            System.Console.WriteLine($"Checksum Count: {sums.Count}");
            foreach (var sum in orderedSums)
            {
                System.Console.WriteLine($"[{sum.Key}]: {sum.Value}");
            }
        }
    }
}
