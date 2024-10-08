# include       "trek.h"
# include       "getpar.h"

/*
**  Signal game won
**
**      This routine prints out the win message, arranges to print out
**      your score, tells you if you have a promotion coming to you,
**      cleans up the current input line, and arranges to have you
**      asked whether or not you want another game (via the reset()
**      call).
**
**      Pretty straightforward, although the promotion algorithm is
**      pretty off the wall.
*/

void win()
{
	long                    s;
	extern long             score();
	extern struct cvntab    Skitab[];
	register struct cvntab  *p;

	sleep(1);
	printf("\n�����������, �� ������ ���������\n");
	Move.endgame = 1;

	/* print and return the score */
	s = score();

	/* decide if she gets a promotion */
	if (Game.helps == 0 && Game.killb == 0 && Game.killinhab == 0 && 5 * Game.kills + Game.deaths < 100 &&
			s >= 1000 && Ship.ship == ENTERPRISE)
	{
		printf("�� ���� ������� ���� �����,\n");
		if (Game.skill >= 6)
			printf("  ��� ��������� ������ \"Commodore Emeritus\".\n");
		else
		{
			p = &Skitab[2*Game.skill - 1];
			printf("� %s%s ", p->abrev, p->full);
			p += 2;
			printf("�� %s%s.\n", p->abrev, p->full);
		}
	}

	/* clean out input, and request new game */
	skiptonl(0);
	reset();
}
