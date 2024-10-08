# include       "trek.h"

/*
**  PRINT OUT LOSER MESSAGES
**
**      The messages are printed out, the score is computed and
**      printed, and the game is restarted.  Oh yeh, any special
**      actions which need be taken are taken.
*/

char    *Losemsg[] =
{
	"����� ���������",
	"������� ���������",
	"�� ����������",
	"�� ������� � ���������� �������������� ������",
	"�� ������� ��� ������ ����� ������",
	"�� ������� ��� ������ ����������",
	"�� �������� ������� � ������ ������������",
	"��� �� ������ �������������������",
	"\n *** ������ ������� ��� ������ ***",
	"�� ������� � ������",
	"��, �� ���������� ����, �� ���� �� � ���� ���� ���-���� �������",
	"�� ���� ��������� � ���� �������� � ���������� ����������",
	"��������� ���� ����� ������� �����",
};

void lose(why)
int     why;
{
	Game.killed = 1;
	sleep(1);
	printf("\n%s\n", Losemsg[why - 1]);
	switch (why)
	{

	  case L_NOTIME:
		Game.killed = 0;
		break;
	}
	Move.endgame = -1;
	score();
	skiptonl(0);
	reset();
}
