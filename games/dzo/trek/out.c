# include       "trek.h"

/*
**  Announce Device Out
*/

void out(dev)
int     dev;
{
	register struct device  *d;

	d = &Device[dev];
	printf("%s ��������, ��� %s ���������%s.\n", d->person, d->name, d->ending);
}
