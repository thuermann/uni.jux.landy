/*
 *  fib.sl - example program for simple interpreter
 *
 *  94/11/17 by Urs Thuermann for JuX.
 */

f0 = 0;
f1 = 1;
for (n = 0; n <= 20; n++) {
	print(f0);
	s = f0 + f1;
	f0 = f1;
	f1 = s;
}
