# include       "trek.h"
# include       "getpar.h"

/*
**  SET WARP FACTOR
**
**      The warp factor is set for future move commands.  It is
**      checked for consistancy.
*/

void setwarp()
{
	FLOAT   warpfac;

	warpfac = getfltpar("��������");
	if (warpfac < 0.0)
		return;
	if (warpfac < 1.0) {
		printf("����������� �������� 1.0\n");
		return;
		}
	if (warpfac > 10.0) {
		printf("������������ �������� 10.0\n");
		return;
		}
	if (warpfac > 6.0)
		printf("��� �������� ������ 6.0 ����� ��������� ���������\n");
	Ship.warp = warpfac;
	Ship.warp2 = Ship.warp * warpfac;
	Ship.warp3 = Ship.warp2 * warpfac;
}
