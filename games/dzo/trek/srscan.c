# include       "trek.h"
# include       "getpar.h"

/*
**  SHORT RANGE SENSOR SCAN
**
**      A short range scan is taken of the current quadrant.  If the
**      flag 'f' is one, it is an "auto srscan", which is not done
**      unless in 'fast' mode.  It does a status report and a srscan.
**      If 'f' is -1, you get a status report only.  If it is zero,
**      you get a srscan and an optional status report.  The status
**      report is taken if you enter "srscan yes"; for all srscans
**      thereafter you get a status report with your srscan until
**      you type "srscan no".  It defaults to on.
**
**      The current quadrant is filled in on the computer chart.
*/

char    *Color[4] =
{
	"�������",
	"��������",
	"������",
	"�������"
};

void srscan(f)
int     f;
{
	register int            i, j;
	register int            statinfo;
	char                    *s;
	int                     percent;
	struct quad             *q;
	extern struct cvntab    Skitab[];
	extern struct cvntab    Lentab[];
	struct cvntab           *p;

	if (f >= 0 && check_out(SRSCAN))
	{
		return;
	}
	if (f)
		statinfo = 1;
	else
	{
		if (!testnl())
			Etc.statreport = getynpar("���������� � ���������");
		statinfo = Etc.statreport;
	}
	if (f > 0)
	{
		Etc.statreport = 1;
		if (!Etc.fast)
			return;
	}
	if (f >= 0)
	{
		printf("\n���������� �������� �������� ������\n");
		q = &Quad[Ship.quadx][Ship.quady];
		q->scanned = q->klings * 100 + q->bases * 10 + q->stars;
		printf("  ");
		for (i = 0; i < NSECTS; i++)
		{
			printf("%d ", i);
		}
		printf("\n");
	}

	for (i = 0; i < NSECTS; i++)
	{
		if (f >= 0)
		{
			printf("%d ", i);
			for (j = 0; j < NSECTS; j++)
				printf("%c ", Sect[i][j]);
			printf("%d", i);
			if (statinfo)
				printf("   ");
		}
		if (statinfo)
			switch (i)
			{
			  case 0:
				printf("������. ����� %.2f", Now.date);
				break;
			  case 1:
				printf("���������     %s", Color[Ship.cond]);
				if (Ship.cloaked)
					printf(", ������");
				break;
			  case 2:
				printf("����������    %d,%d/%d,%d",Ship.quadx, Ship.quady, Ship.sectx, Ship.secty);
				break;
			  case 3:
				printf("��������      %.1f", Ship.warp);
				break;
			  case 4:
				printf("�������       %d", Ship.energy);
				break;
			  case 5:
				printf("������        %d", Ship.torped);
				break;
			  case 6:
				s = "����";
				if (Ship.shldup)
					s = "���";
				if (damaged(SHIELD))
					s = "����������";
				percent = 100.0 * Ship.shield / Param.shield;
				printf("������        %s, %d%%", s, percent);
				break;
			  case 7:
				printf("���. �������  %d", Now.klings);
				break;
			  case 8:
				printf("���. �������  %.2f", Now.time);
				break;
			  case 9:
				printf("����������.   ");
				if (damaged(LIFESUP))
				{
					printf("���������, ������ = %.2f", Ship.reserves);
					break;
				}
				printf("�������");
				break;
			}
		printf("\n");
	}
	if (f < 0)
	{
		printf("������        %d\n", Ship.crew);
		printf("���� ��� ������� %d\n", Ship.brigfree);
		printf("�������� ������� %d\n", Param.klingpwr);
		p = &Lentab[2*Game.length - 1];
		if (Game.length > 2)
			p -= 2;
		printf("�����,���������  %s%s,", p->abrev, p->full);
		p = &Skitab[2*Game.skill - 1];
		printf("%s%s\n", p->abrev, p->full);
		return;
	}
	printf("  ");
	for (i = 0; i < NSECTS; i++)
		printf("%d ", i);
	printf("\n");

	if (q->qsystemname & Q_DISTRESSED)
		printf("��� ������� ");
	if (q->qsystemname)
		printf("�������� ������� %s\n", systemname(q));
}
