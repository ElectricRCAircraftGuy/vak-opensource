/*d* === chnloc ===        07.01.85   version    9 */
/* ��������� ������� ha ������� #newloc */

#include "../common/adv_common"

int drkfla;

chnloc(newloc) {
    static int drkold;
    drkold=dark();
    loc=newloc;
    if( dark() && drkfla && drkold && pct(30) ) {
	rspeak(23);                   /* �������� b ������� b temhote */
	score();
	exit(0);
    }
    drkfla=1;
    descr();
}
