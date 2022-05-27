#include <stdlib.h>

/* xtoa can be used for all conversion functions, so 'x' is a placeholder for 'i', 'f', and sor forth... */
static inline void xtoa(int value, char *buffer, unsigned int base, int negative);

char *
itoa(int value, char *buffer, unsigned int base)
{
	if (base == 10 && value < 0)
		xtoa((unsigned int)value, buffer, base, 1);
	else
		xtoa((unsigned int)value, buffer, base, 0);
	return buffer;
}

static inline void 
xtoa(int value, char *buffer, unsigned int base, int negative)
{
	char *p;
	char *firstdigit;
	char tmp;
	unsigned digit;

	p = buffer;

	if (negative)
	{
		*p++ = '-';
		value = (unsigned int)(-(long)value);
	}

	firstdigit = p;
	do {
		digit = (unsigned)(value % base);
		value /= base;

		if (digit > 9)
		{
			*p++ = (char)(digit - 10 + 'a');
		}
		else
		{
			*p++ = (char)(digit + '0');
		}
	} while (value > 0);

	*p-- = '\0';
	do {
		tmp = *p;
		*p = *firstdigit;
		*firstdigit = tmp;
		p--;
		firstdigit++;
	} while (firstdigit < p);
}
