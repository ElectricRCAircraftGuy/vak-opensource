# include       "trek.h"

/**
 **     move under impulse power
 **/

void impulse()
{
	int                     course;
	register int            power;
	FLOAT                   dist, time;
	register int            percent;
	extern FLOAT            move();

	if (Ship.cond == DOCKED) {
		printf("������: �������, �������, �� �� ��������� � �����.\n");
		return;
		}
	if (damaged(IMPULSE)) {
		out(IMPULSE);
		return;
		}
	if (getcodi(&course, &dist))
		return;
	power = 20 + 100 * dist;
	percent = 100 * power / Ship.energy + 0.5;
	if (percent >= 85)
	{
		printf("������: �� ��� ����� %d%% ���������� �������.\n",
			percent);
		if (!getynpar("������ ���"))
			return;
		printf("��, ���\n");
	}
	time = dist / 0.095;
	percent = 100 * time / Now.time + 0.5;
	if (percent >= 85)
	{
		printf("����: �� ��� ����� %d%% ����������� �������.\n",
			percent);
		if (!getynpar("������ ���"))
			return;
		printf("(�� ������������ ����� � ���)\n");
	}
	Move.time = move(0, course, time, 0.095);
	Ship.energy -= 20 + 100 * Move.time * 0.095;
}
