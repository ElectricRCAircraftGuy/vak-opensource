/*d* === score === ������ ����� ����        07.01.85   version    9 */

#include "../common/adv_common"

score() {
    int scor, maxsco, obj, i;

    scor=maxsco=0;
    for(obj=treasr; obj<=objt; ++obj) {
	if( place(obj)>0 ) {
	    maxsco=maxsco+20;
	    if( prop(obj)==0                         /* ��������� � ���� */
	    && (place(obj)==3 || place(obj)==caried) ) { /* ��� ���� ��� */
		scor=scor+20;
	    } else if( (prop(obj)&0377)!=inipro ) {  /* ��������� ����� */
		scor=scor+5;
	    }
	}
    }
    rspeak(32);
    mscore(scor,maxsco);

    obj = 1;
    for(i=1; i<=plcl; ++i)  if( cval(i) && scor>=cval(i) ) obj = i;
    mes(ctext(obj));
}
