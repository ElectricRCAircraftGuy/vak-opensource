# include       "trek.h"

/*
**  RAM SOME OBJECT
**
**      You have run into some sort of object.  It may be a Klingon,
**      a star, or a starbase.  If you run into a star, you are really
**      stupid, because there is no hope for you.
**
**      If you run into something else, you destroy that object.  You
**      also rack up incredible damages.
*/

void ram(ix, iy)
int     ix, iy;
{
	register int            i;
	register char           c;

	printf("�������: ������ ����������\n");
	c = Sect[ix][iy];
	switch (c)
	{

	  case KLINGON:
		printf("%s ���������� ������ � %d,%d\n", Ship.shipname, ix, iy);
		killk(ix, iy);
		break;

	  case STAR:
	  case INHABIT:
		printf("���� ����: �������, �� ������ ��, ���������� ��������� ?\n");
		sleep(2);
		printf("����: ����������� ������� ����� 550 �� ��������.\n");
		lose(L_STAR);

	  case BASE:
		printf("�� ����������� �������� ���� � %d,%d\n", ix, iy);
		killb(Ship.quadx, Ship.quady);
		/* don't penalize the captain if it wasn't his fault */
		if (!damaged(SINS))
			Game.killb += 1;
		break;
	}
	sleep(2);
	printf("%s ������ ���������\n", Ship.shipname);

	/* select the number of deaths to occur */
	i = 10 + ranf(20 * Game.skill);
	Game.deaths += i;
	Ship.crew -= i;
	printf("������: �, ����; � ��� ������� %d �������.\n", i);

	/* damage devices with an 80% probability */
	for (i = 0; i < NDEV; i++)
	{
		if (ranf(100) < 20)
			continue;
		damage(i, (2.5 * (franf() + franf()) + 1.0) * Param.damfac[i]);
	}

	/* no chance that your shields remained up in all that */
	Ship.shldup = 0;
}
