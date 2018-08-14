namespace crc
{
    public static class Crc4
    {
        static byte[] table = {
            0x0, 0x7, 0xe, 0x9, 0xb, 0xc, 0x5, 0x2,
            0x1, 0x6, 0xf, 0x8, 0xa, 0xd, 0x4, 0x3,
         };
        public static byte InitialValue = byte.MinValue;


        public static uint ComputeChecksum(params byte[] bytes)
        {
            return ComputeChecksum(bytes, 0, bytes?.Length ?? 0);
        }

        // uint sum = Crc4.ComputeChecksum((byte)((i & 0x000F) >> 0), (byte)((i & 0x00F0) >> 4), (byte)((i & 0x0F00) >> 8));
        public static byte ComputeChecksum(byte[] bytes, int start, int length)
        {
            var crc = InitialValue;
            var end = start + length;
            if (bytes != null && bytes.Length > 0 && start < length && end <= bytes.Length)
            {
                for (int i = start; i < end; ++i)
                {
                    var low4Bits = bytes[i] & 0x0F;
                    var high4Bits = bytes[i] >> 4;
                    crc = table[crc ^ high4Bits];
                    crc = table[crc ^ low4Bits];

                    //crc = table[crc ^ bytes[i]];
                }
            }
            return crc;
        }
    }
}