# include       "trek.h"
# include       "getpar.h"

char *mrok( n )
{
	char    *s;
	s = "��";
	if( 10>(n%100) || (n%100)>20 )
		switch( n%10 ) {
		    case 1:
			s = "";
			break;
		    case 2:
		    case 3:
		    case 4:
			s = "�";
		}
	return( s );
}

/*
**  PRINT OUT THE CURRENT SCORE
*/

long score()
{
	register int            u;
	register int            t;
	long                    s;
	FLOAT                   r;
	extern struct cvntab    Skitab[];
	static char             *vrk1[] = {
		"��", "��", "��", "��", "��", "��", "��", "��", "��", "��" };
	static char             *vrk2[] = {
		"",   "�",  "�",  "�",  "�",  "",   "",   "",   "",   "" };
	static char             *vrk3[] = {
		"��", "��", "��", "��", "��", "��", "��", "��", "��", "��" };
	static char             *vrk4[] = {
		"",   "�",  "�",  "�",  "�",  "",   "",   "",   "",   "" };

	printf("\n*** ���� ����������:\n");
	s = t = Param.klingpwr / 4 * (u = Game.killk);
	if (t != 0)
		printf("%d �����%s ����%s   \t\t\t%6d\n", u, mrok(u), u==1?"":"�", t);
	r = Now.date - Param.date;
	if (r < 1.0)
		r = 1.0;
	r = Game.killk / r;
	s += (t = (int)(400 * r));
	if (t != 0)
		printf("�� ��������� %.2f ������/����  \t\t%6d\n", r, t);
	r = Now.klings;
	r /= (FLOAT)(Game.killk + 1);
	s += (t = (int)(-400 * r));
	if (t != 0)
		printf("%d �����%s �����%s \t\t\t%6d\n", Now.klings, mrok(Now.klings), Now.klings==1?"��":"���", t);
	if (Move.endgame > 0)
	{
		s += (t = 100 * (u = Game.skill));
		printf("���� �� ���������� ���� �� '%s%s'\t\t%6d\n", Skitab[u+u - 1].abrev, Skitab[u+u - 1].full, t);
	}
	if (Game.killed)
	{
		s -= 500;
		printf("�� ������������\t\t\t\t  -500\n");
	}
	s += (t = -100 * (u = Game.killb));
	if (t != 0)
		printf("���������� %d ������%s ���%s\t\t%6d\n", u, vrk1[u], vrk2[u], t);
	s += (t = -100 * (u = Game.helps));
	if (t != 0)
		printf("%d ������%s � ������\t\t\t%6d\n", u, vrk2[u], t);
	s += (t = -5 * (u = Game.kills));
	if (t != 0)
		printf("%d �����%s ���������%c\t\t\t%6d\n", u,
		      (u<20&&u>10)?"":vrk2[u%10], u==1?'�':'�', t);
	s += (t = -150 * (u = Game.killinhab));
	if (t != 0)
		printf("%d �������%s ������%s ������%s ���������%c\t%6d\n", u,
		      (u<20&&u>10)?"":vrk3[u%10],
		      (u<20&&u>10)?"":vrk3[u%10],
		      (u<20&&u>10)?"":vrk4[u%10], u==1?'�':'�', t);
	if (Ship.ship != ENTERPRISE)
	{
		s -= 200;
		printf("�� ���������� �������\t\t\t  -200\n");
	}
	s += (t = 3 * (u = Game.captives));
	if (t != 0)
		printf("%d �����%s �������������%s\t\t\t%6d\n", u, mrok(u), u==1?"":"�", t);
	s += (t = -(u = Game.deaths));
	if (t != 0)
		printf("%d ����%s    \t\t\t\t%6d\n", u, u==1?"  ":"��", t);
	printf("\n***  �����\t\t\t%14ld\n", s);
	return (s);
}
