/*
 *  pi.sl - example program for simple interpreter
 *
 *  94/11/17 by Urs Thuermann for JuX.
 */

pi    = 0;
sign  = 1;
i     = 0;
ival  = 1000;
count = 100000;

for (n = 0; n < count; n++) {
	s += sign * 4 / (2 * n + 1);
	sign *= -1;
	if (i < 2)
		print(s);
	if (++i == ival)
		i = 0;
}
