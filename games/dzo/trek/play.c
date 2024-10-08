# include       "trek.h"
# include       "getpar.h"

/*
**  INSTRUCTION READ AND MAIN PLAY LOOP
**
**      Well folks, this is it.  Here we have the guts of the game.
**      This routine executes moves.  It sets up per-move variables,
**      gets the command, and executes the command.  After the command,
**      it calls events() to use up time, attack() to have Klingons
**      attack if the move was not free, and checkcond() to check up
**      on how we are doing after the move.
*/
extern void     abandon(), destruct(), dock(), shell(), dcrept(), torped();
extern void     capture(), help(), dumpgame(), computer(), impulse(), shield();
extern void     rest(), reset(), srscan(), warp(), visual(), lrscan();
extern void     undock(), setwarp(), phaser();

void reset() {
# include       <setjmp.h>
	extern jmp_buf jmpbuf ;

	longjmp( jmpbuf, 0 ) ;
}

struct cvntab   Comtab[] =
{
      { "abandon",              "",             0,  0,  abandon   },
      { "��������",             "",             0,  0,  abandon   },
      { "ca",                   "pture",        0,  0,  capture   },
      { "��",                   "����",         0,  0,  capture   },
      { "cl",                   "oak",          -1, 0,  shield    },
      { "���",                  "������",       -1, 0,  shield    },
      { "c",                    "omputer",      0,  0,  computer  },
      { "�",                    "��������",     0,  0,  computer  },
      { "da",                   "mages",        0,  0,  dcrept    },
      { "��",                   "���������",    0,  0,  dcrept    },
      { "destruct",             "",             0,  0,  destruct  },
      { "���������",            "",             0,  0,  destruct  },
      { "do",                   "ck",           0,  0,  dock      },
      { "��",                   "������",       0,  0,  dock      },
      { "help",                 "",             0,  0,  help      },
      { "������",               "",             0,  0,  help      },
      { "i",                    "mpulse",       0,  0,  impulse   },
      { "�",                    "������",       0,  0,  impulse   },
      { "m",                    "ove",          0,  0,  warp      },
      { "��",                   "������",       0,  0,  warp      },
      { "l",                    "rscan",        0,  0,  lrscan    },
      { "�",                    "���",          0,  0,  lrscan    },
      { "p",                    "hasers",       0,  0,  phaser    },
      { "�",                    "����",         0,  0,  phaser    },
      { "ram",                  "",             1,  0,  warp      },
      { "�����",                "",             1,  0,  warp      },
      { "dump",                 "",             0,  0,  dumpgame  },
      { "���",                  "������",       0,  0,  dumpgame  },
      { "r",                    "est",          0,  0,  rest      },
      { "�",                    "�����",        0,  0,  rest      },
      { "shell",                "",             0,  0,  shell     },
      { "����",                 "",             0,  0,  shell     },
      { "sh",                   "ield",         0,  0,  shield    },
      { "�",                    "�����",        0,  0,  shield    },
      { "s",                    "rscan",        0,  0,  srscan    },
      { "�",                    "���",          0,  0,  srscan    },
      { "st",                   "atus",         -1, 0,  srscan    },
      { "��",                   "�������",      -1, 0,  srscan    },
      { "terminate",            "",             0,  0,  reset     },
      { "�����",                "",             0,  0,  reset     },
      { "t",                    "orpedo",       0,  0,  torped    },
      { "�",                    "������",       0,  0,  torped    },
      { "u",                    "ndock",        0,  0,  undock    },
      { "�",                    "������������", 0,  0,  undock    },
      { "v",                    "isual",        0,  0,  visual    },
      { "�",                    "��������",     0,  0,  visual    },
      { "w",                    "arp",          0,  0,  setwarp   },
      { "��",                   "������",       0,  0,  setwarp   },
      {  0, 0, 0, 0, 0 }
};

play()
{
	struct cvntab           *r;

	while (1)
	{
		Move.free = 1;
		Move.time = 0.0;
		Move.shldchg = 0;
		Move.newquad = 0;
		Move.resting = 0;
		skiptonl(0);
		r = getcodpar("\n�������", Comtab);
		(*r->fun)(r->value);
		events(0);
		attack(0);
		checkcond();
	}
}
