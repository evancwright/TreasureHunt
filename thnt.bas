100 REM  =================================================
120 REM  TREASURE HUNT BY LANCE MICKLUS, WINOOSKI, VERMONT
140 REM  COPYRIGHT SEPTEMBER 1978
160 REM  VERSION 2.1 : TRS-80/16K LEVEL II
180 REM  =================================================
200 CLS
220 PRINT TAB(18)"T R E A S U R E   H U N T"
240 PRINT TAB(23)"BY LANCE MICKLUS"
260 PRINT
280 PRINT"THE LUMUS CAVES, LOCATED IN LUMUSVILLE, VERMONT, ARE SAID TO"
300 PRINT"HAVE 20 HIDDEN TREASURES IN THEM. FEW EXPLORE THE CAVES"
320 PRINT"BECAUSE IT IS SAID THAT PIRATES AND DRAGONS LIVE THERE, AND"
340 PRINT"THERE ARE DEEP PITS WHICH MANY HAVE FALLEN INTO AND DIED."
360 PRINT
380 PRINT"YOU, A SMART AND BRAVE HUMAN, AND I, AN ALERT COMPUTER, WILL"
400 PRINT"EXPLORE THE CAVES AND TRY TO FIND THE TREASURE. I WILL BE YOUR"
420 PRINT"EYES AND EARS, AND WILL TELL YOU IF THERE'S DANGER AHEAD."
440 PRINT
460 PRINT"I HOPE YOU BROUGHT A MAP. IF NOT, YOU'LL HAVE TO MAKE ONE UP"
480 PRINT"HAS WE EXPLORE."
500 PRINT
520 INPUT"HIT THE ''ENTER'' KEY WHEN YOUR READY TO BEGIN";B$
540 CLEAR 50 : CLS
560 DEFINT A-Z: DIM A(421)
580 REM
600 REM  ORISMOLOGY OF THE A ARRAY
620 REM  -------------------------
640 REM  A(0) TO A(94)     CAVE CONTENTS
660 REM  A(96) TO A(130)   TREASURE TO BE KEPT
680 REM  A(131) TO A(133)  TREASURE BEING CARRIED
700 REM  A(134) TO A(136)  ADJACENT CAVE CONNECTIONS
720 REM  A(137) TO A(421)  CAVE CONNECTION DATA ARRAY
740 REM
760 REM  ORISMOLOGY OF NON TREASURES
780 REM  ---------------------------
800 REM  1 = NOT USED
820 REM  2 = PITS
840 REM  3 = PIRATE
860 REM  4 = DRAGON
880 REM  5 = VENDING MACHINE
900 REM  6 = CAVE IN >>> DANGER <<<
920 REM  7 = ELF
940 REM  8 = MAGICIAN'S CAVE
960 REM  9 = INVISABLE DOG
980 REM
1000 REM
1020 REM     INITIALIZE & SET UP THE CAVES
1040 REM   ---------------------------------
1060 FOR I = 1 TO 3
1080 N = 3 + RND(88)
1100 IF A(N) <> 0 THEN 1080
1120 A(N) = 2
1140 NEXT I
1160 FOR I = 3 TO 9
1180 N = 3 + RND(88)
1200 IF A(N) <> 0 THEN 1180
1220 A(N) = I
1240 NEXT I
1260 FOR I = 288 TO 304
1280 N = RND(94)
1300 IF A(N) <> 0 THEN 1280
1320 A(N) = I
1340 NEXT I
1360 RESTORE
1380 FOR N = 1 TO 23
1400 READ A$
1420 NEXT N
1440 FOR N = 137 TO 421
1460 READ A(N)
1480 NEXT N
1500 W = 0   :REM USER'S CAVE LOCATION
1520 M = 0   :REM BECOMES A 1 WHEN CARRYING MAGIC CARPET
1540 B = 200 :REM BATTERY POWER LEFT
1560 D = 0   :REM BECOMES A 1 AFTER INVISABLE DOG IS FOUND
1580 PRINT"WELCOME TO THE LUMUS CAVE WHERE YOU'LL FIND MYSTERY,"
1600 PRINT"TREASURE, AND ADVENTURE."
1620 PRINT
1640 GOTO 1780
1660 REM
1680 REM     MAIN ROUTINE
1700 REM   ----------------
1720 CLS
1740 B = B - 1
1760 IF B = 0 THEN 8320
1780 IF W = 0 THEN 7020
1800 REM
1820 REM   ::::: CHANCE SITUATIONS :::::
1840 REM
1860 IF RND(200) = 200 THEN GOSUB 4520
1880 IF RND(200) = 200 THEN GOSUB 4960
1900 IF D=0 AND A(W)=0 AND RND(50)=50 GOSUB 5880
1920 IF A(W)=6 AND RND(5)=5 THEN 7920
1940 IF M=1 AND RND(5)=5 GOSUB 6180
1960 REM
1980 REM   ::::: SEE IF SPECIAL SITUATION :::::
2000 REM
2020 IF A(W) = 2 THEN 7620
2040 IF A(W) = 4 GOTO 6520
2060 IF A(W) = 5 GOSUB 4280
2080 IF A(W)=7 PRINT"THERE'S A SHY LITTLE ELF IN HERE HIDING SOMETHING."
2100 IF A(W) = 8 THEN 5520
2120 REM
2140 REM   ::::: WHERE YOU CAN GO NEXT :::::
2160 REM
2180 GOSUB 7440
2200 Z = W
2220 GOSUB 9460
2240 FOR N = 134 TO 136
2260 IF A(N) < 0 THEN 2340
2280 PRINT TAB(5)"CAVE";A(N);
2300 IF A(N) = 0 PRINT "- THE ENTRANCE";
2320 PRINT
2340 NEXT N
2360 REM
2380 REM   ::::: QUIT & BATTERY TEST :::::
2400 REM
2420 IF B > 190 THEN 2520
2440 IF W <> 0 THEN 2520
2460 INPUT"DO YOU WANT TO EXPLORE SOME MORE";AN$
2480 IF LEFT$(AN$,1)="N" THEN 8480
2500 IF LEFT$(AN$,1)<>"Y" THEN 2460
2520 IF B=50 PRINT:PRINT"I HOPE YOU BROUGHT MORE FLASH LIGHT BATTERIES."
2540 IF B=25 PRINT:PRINT"YOUR FLASH LIGHT IS STARTING TO DIM A LITTLE."
2560 IF B=8 PRINT:PRINT"YOUR FLASH LIGHT IS ALMOST OUT. BETTER LEAVE NOW!!!"
2580 IF A(W) = 3 GOSUB 5300
2600 REM
2620 REM   ::::: ADJACENT CAVE'S CONTENTS :::::
2640 REM
2660 K = 0
2680 L = 0
2700 FOR N = 134 TO 136
2720 J = A(N)
2740 IF J < 0 THEN 2940
2760 IF A(J) = 5 THEN 2940
2780 IF A(J)>0 AND A(J)<10 AND K=0 PRINT: K=1
2800 IFA(J)=2 AND L=0 PRINT"THERE'S PITS NEARBY. WATCH YOUR STEP.":L=1
2820 IF A(J)=3 PRINT"CAREFUL. THERE'S A PIRATE NEAR HERE."
2840 IF A(J)=4 PRINT"I HEAR A HUNGRY DRAGON WAITING FOR HIS SUPPER."
2860 IF A(J)=6 PRINT"THERE'S A SIGN HERE THAT SAYS, >>> D A N G E R <<<"
2880 IF A(J)=7 PRINT"SOUNDS LIKE SOMEBODY IS SINGING. MUST BE AN ELF."
2900 IF A(J)=8 PRINT"THERE'S A STRANGE MIST IN THIS CAVE."
2920 IF A(J)=9 PRINT"''RUFF RUFF''  I HEAR A DOG BARKING."
2940 NEXT N
2960 IF A(131) = 0 THEN 3160
2980 PRINT
3000 PRINT"YOU'RE CARRYING:"
3020 FOR N = 131 TO 133
3040 IF A(N) < 100 THEN 3120
3060 Z = A(N)
3080 GOSUB 9300
3100 PRINT A$,
3120 NEXT N
3140 PRINT
3160 IF A(W) < 100 THEN PRINT : GOTO 3620
3180 REM
3200 REM   ::::: TREASURE IN THIS CAVE :::::
3220 REM
3240 PRINT
3260 Z = A(W)
3280 GOSUB 9300
3300 PRINT"THIS CAVE HAS ";A$;" IN IT."
3320 INPUT"DO YOU WANT TO TAKE IT WITH YOU";AN$
3340 IF LEFT$(AN$,1)="N" THEN 3620
3360 IF LEFT$(AN$,1)<>"Y" THEN 3320
3380 IF Z = 290 GOSUB 4000
3400 IF Z = 290 THEN 3620
3420 IF Z = 291 LET M = 1
3440 IF Z = 294 GOSUB 4700
3460 IF Z = 1 THEN 3620
3480 IF Z = 295 THEN 8100
3500 FOR N = 131 TO 133
3520 IF A(N) = 0 LET A(N) = A(W) : A(W) = 0 : GOTO 3600
3540 NEXT N
3560 PRINT"YOU CAN'T PICK IT UP NOW. YOU'RE CARRYING TOO MUCH."
3580 GOTO 3620
3600 PRINT"O.K. YOU'VE GOT ";A$;"."
3620 IF A(W) = 7 LET A(W) = 307
3640 REM
3660 REM   ::::: WHERE TO NEXT :::::
3680 REM
3700 INPUT"WHAT CAVE DO YOU WANT TO EXPLORE NEXT";J
3720 FOR N = 134 TO 136
3740 IF A(N) = -1 THEN 3780
3760 IF J = A(N) LET W = J : GOTO 1720
3780 NEXT N
3800 IF M=1 AND J>=0 AND J<=94 LET W=J: GOTO 1720
3820 PRINT"SORRY, BUT YOU CAN'T GO THERE FROM HERE."
3840 GOTO 3700
3860 REM
3880 REM
3900 REM     SPECIAL SITUATION SUBROUTINES
3920 REM   ---------------------------------
3940 REM
3960 REM   ::::: LOCK BOX :::::
3980 REM
4000 FOR N = 131 TO 133
4020 IF A(N) = 289 THEN 4100
4040 NEXT N
4060 PRINT"YOU CAN'T TAKE IT. IT'S TOO HEAVY."
4080 RETURN
4100 PRINT"THE BOX WAS TOO HEAVY SO I OPENNED IT WITH YOUR KEYS."
4120 PRINT"INSIDE THE BOX IS A RUBY WHICH YOU CAN TAKE."
4140 A(W) = 308
4160 Z = 308
4180 GOSUB 9300
4200 RETURN
4220 REM
4240 REM   ::::: VENDING MACHINE :::::
4260 REM
4280 PRINT"THERE'S A VENDING MACHINE HERE."
4300 FOR N = 131 TO 133
4320 IF A(N) = 288 THEN 4380
4340 NEXT N
4360 RETURN
4380 PRINT"I'LL GET SOME BATTERIES WITH ONE OF YOUR GOLD COINS."
4400 B = 200
4420 A(W) = 0
4440 RETURN
4460 REM
4480 REM   ::::: EARTHQUAKE :::::
4500 REM
4520 FOR N = 1 TO 10
4540 Z = 3 + RND(88)
4560 GOSUB 9120
4580 NEXT N
4600 PRINT"GUESS WHAT? WE JUST HAD AN EARTHQUAKE. BUT I'M O.K."
4620 RETURN
4640 REM
4660 REM   ::::: MAGIC WAND ROUTINE :::::
4680 REM
4700 FOR N = 131 TO 133
4720 IF A(N) = 293 THEN 4800
4740 NEXT N
4760 PRINT"IT'S STUCK IN THE ROCKS AND CAN'T BE PULLED OUT."
4780 Z = 1 : RETURN
4800 PRINT"LET ME USE YOUR MAGIC WAND A SECOND. ";
4820 FOR Z = 0 TO 1000 : NEXT Z
4840 PRINT"HOCUS, POCUS..."
4860 FOR Z = 0 TO 1000 : NEXT Z
4880 A(W) = 310 : Z = 0
4900 RETURN
4920 REM
4940 REM   ::::: BATS SHOW UP :::::
4960 PRINT@320," "
4980 PRINT"A SUPER BAT JUST FEW INTO THE CAVE AND PICKED YOU UP."
5000 IF A(131) <> 0 PRINT"YOU DROPPED ALL OF YOUR TREASURES."
5020 FOR N = 131 TO 133
5040 IF A(N) = 0 THEN 5080
5060 A(0) = A(N) : A(N) = 0 : Z = 0 : GOSUB 9120
5080 NEXT N
5100 N = RND(94)
5120 IF A(N) <> 0 THEN 5100
5140 W = N
5160 PRINT"THE BAT JUST DROPPED YOU INTO CAVE";W;"!!!"
5180 INPUT"IF YOU'RE ALRIGHT HIT THE ''ENTER'' KEY";B$
5200 CLS
5220 RETURN
5240 REM
5260 REM   ::::: PIRATE HERE :::::
5280 REM
5300 PRINT:PRINT"THERE WAS A PIRATE IN HERE."
5320 IF A(131) <> 0 PRINT"HE JUST STOLE ALL OF YOUR TREASURES."
5340 FOR N = 131 TO 133
5360 A(N) = 0
5380 NEXT N
5400 Z = W
5420 GOSUB 9120
5440 PRINT"HE'S GONE NOW."
5460 RETURN
5480 REM   ::::: MAGICIAN ROUTINE :::::
5500 REM
5520 CLS
5540 PRINT"THERE'S A MAGICIAN IN THIS CAVE. HE SAYS HE LOST HIS MAGIC"
5560 PRINT"BOOK. HE SAID HE'D GIVE YOU A GOLDEN HARP IF YOU'LL TELL HIM"
5580 PRINT"WHERE IT IS. TYPE IN THE CAVE NUMBER. IF YOU DON'T KNOW, THEN"
5600 PRINT"JUST TYPE 99."
5620 PRINT
5640 INPUT"WHERE'S HIS MAGIC BOOK";J
5660 IF J<0 OR J>95 THEN 5760
5680 IF A(J) <> 295 THEN 8540
5700 A(W) = 309
5720 A(J) = 0
5740 GOTO 1720
5760 Z = W
5780 GOSUB 9120
5800 GOTO 1720
5820 REM
5840 REM   ::::: INVISABLE MAN ROUTINE :::::
5860 REM
5880 PRINT@ 320,"";
5900 PRINT"THE INVISABLE MAN IS HERE LOOKING FOR HIS INVISABLE DOG."
5920 PRINT"HE SAYS HE'LL GIVE YOU A $1000 REWARD IF YOU CAN TELL HIM"
5940 PRINT"WHERE HIS DOG IS. IF YOU DON'T KNOW, GUESS."
5960 PRINT
5980 INPUT"WHAT CAVE IS THE INVISABLE DOG IN";J
6000 CLS
6020 IF J<0 OR J>95 OR A(J)<>9 RETURN
6040 A(W) = 305
6060 A(J) = 0
6080 D = 1
6100 RETURN
6120 REM
6140 REM   ::::: MAGIC CARPET DISSAPPEARS :::::
6160 REM
6180 FOR N = 131 TO 133
6200 IF A(N) = 291 THEN 6280
6220 NEXT N
6240 M = 0
6260 RETURN
6280 A(N) = 0
6300 IF N = 133 THEN 6400
6320 FOR J = N TO 132
6340 A(J) = A(J+1)
6360 A(J+1) = 0
6380 NEXT J
6400 PRINT"POOF !!!     YOUR MAGIC CARPET JUST DISSAPPEARED."
6420 M = 0
6440 RETURN
6460 REM
6480 REM   ::::: DRAGON ROUTINE :::::
6500 REM
6520 FOR N = 131 TO 133
6540 IF A(N) = 292 THEN 6600
6560 NEXT N
6580 GOTO 7760
6600 PRINT"YIKS !!!! THERE'S A DRAGON IN HERE."
6620 PRINT"GIVE ME YOUR GUN, QUICK !!!"
6640 FOR N = 0 TO 2000 : NEXT N
6660 FOR N = 1 TO 5
6680 PRINT TAB(RND(40))"B A N G ! ! !"
6700 FOR J = 0 TO 500 : NEXT J
6720 NEXT N
6740 PRINTTAB(RND(40))"POOF !!!"
6760 PRINT"INCREDIBLE???  THE DRAGON JUST VANISHED WHEN I SHOT HIM RIGHT"
6780 PRINT"BETWEEN THE EYES."
6800 PRINT:PRINT"BUT LOOK AT THIS. HE LEFT BEHIND HIS LITTLE BLACK BOOK"
6820 PRINT"WITH THE ADDRESSES AND PHONE NUMBERS OF EVERY BEAUTIFUL"
6840 PRINT"PRINCESS THAT LIVES IN VERMONT."
6860 PRINT
6880 INPUT"HIT THE ''ENTER'' KEY AFTER YOU CATCH YOUR BREATH";B$
6900 A(W) = 306
6920 B = B + 1
6940 GOTO 1720
6960 REM
6980 REM   ::::: AT ENTRANCE ROUTINE :::::
7000 REM
7020 IF A(131) = 0 THEN 7160
7040 FOR N = 131 TO 133
7060 IF A(N) = 0 THEN 7140
7080 J = 96
7100 IF A(J) <> 0 LET J = J + 1 : GOTO 7100
7120 A(J) = A(N)
7140 NEXT N
7160 IF A(96) = 0 THEN 7280
7180 PRINT"SO FAR, YOU'VE FOUND THESE TREASURES IN THE CAVES:"
7200 FOR N = 96 TO 130
7220 IF A(N) = 0 THEN 7280
7240 Z = A(N) : GOSUB 9300 : PRINT A$,
7260 NEXT N
7280 FOR N = 131 TO 133 : A(N) = 0 : NEXT N
7300 PRINT
7320 PRINT:PRINT"YOU'RE AT A CAVE ENTRANCE WHICH LEADS TO:"
7340 B = B + 1
7360 GOTO 2200
7380 REM
7400 REM   ::::: IN CAVE ROUTINE :::::
7420 REM
7440 PRINT"YOU'RE IN CAVE";W;"WHICH LEADS TO:"
7460 RETURN
7480 REM
7500 REM
7520 REM     END OF PROGRAM ROUTINES
7540 REM   ---------------------------
7560 REM
7580 REM   ::::: FELL INTO A PIT :::::
7600 REM
7620 PRINT@ 320,"";
7640 PRINT"SORRY, BUT I TRIED TO WARN YOU."
7660 PRINT"YOU FELL INTO A DEEP PIT AND KILLED YOURSELF !!!"
7680 END
7700 REM
7720 REM   ::::: EATEN BY THE DRAGON :::::
7740 REM
7760 PRINT@ 320,"";
7780 PRINT"SORRY, BUT I TRIED TO TELL YOU ABOUT THAT SOUND."
7800 PRINT"SUPPER IS NOW BEING SERVED HERE IN THE DRAGON'S CHAMBER..."
7820 PRINT"AND YOU ARE THE SUPPER."
7840 END
7860 REM
7880 REM   ::::: ROOF FELL IN :::::
7900 REM
7920 PRINT@ 320,"";
7940 PRINT"THE ROOF JUST FELL IN AND BURRIED YOU ALIVE. TOO BAD I DON'T"
7960 PRINT"HAVE A SHOVEL OR I'D DIG YOU OUT.
7980 PRINT
8000 PRINT"SEE YAH LATER."
8020 END
8040 REM
8060 REM   ::::: PICKED UP MAGIC BOOK :::::
8080 REM
8100 CLS
8120 PRINT@ 320,"";
8140 PRINT"I DON'T THINK YOU SHOULD HAVE DONE THAT. THE MAGICIAN WHO"
8160 PRINT"OWNS THAT MAGIC BOOK PUT A SPELL ON IT. ANYBODY WHO TRIES TO"
8180 PRINT"PICK IT UP TURNS INTO A FROG."
8200 PRINT
8220 PRINT"EXCUSE ME WHILE I LOOK FOR SOME FLIES."
8240 END
8260 REM
8280 REM   ::::: BATTERIES WENT DEAD :::::
8300 REM
8320 PRINT@ 320,"";
8340 PRINT"NOW LOOK WHAT YOU DID. YOUR FLASH LIGHT WENT OUT AND YOU FELL"
8360 PRINT"INTO A PIT AND KILLED YOURSELF. TOO BAD. ESPECIALLY AFTER YOU"
8380 PRINT"WERE DOING SO WELL."
8400 END
8420 REM
8440 REM   ::::: USER QUITS :::::
8460 REM
8480 PRINT"THE TREASURES ARE YOURS TO KEEP. GOOD LUCK !!!"
8500 END
8520 REM   ::::: MAGICIAN UPSET :::::
8540 PRINT
8560 PRINT"YOU'VE MADE THE MAGICIAN VERY ANGRY. CAVE";J;"DOESN'T HAVE"
8580 PRINT"A MAGIC BOOK IN IT. TO PUNISH YOU, THE MAGICIAN CAST A SPELL"
8600 PRINT"ON YOU AND NOW YOU'RE ONLY 2 INCHES TALL. WORST YET, THE"
8620 PRINT"THE MAGICIAN PUT YOU IN A SMALL JAR. IF YOU EVER GET OUT OF"
8640 PRINT"THIS MESS, LET ME KNOW."
8660 END
9000 REM
9020 REM     UTILITY SUBROUTINES
9040 REM   -----------------------
9060 REM
9080 REM   ::::: MOVE ITEM TO NEW CAVE LOCATION :::::
9100 REM
9120 Y = 3 + RND(88)
9140 IF A(Y) <> 0 THEN 9120
9160 IF (Y = W) + (Y = Z) THEN 9120
9180 A(Y) = A(Z)
9200 A(Z) = 0
9220 RETURN
9240 REM
9260 REM   ::::: MAKE A$ NAME OF TREASURE :::::
9280 REM
9300 RESTORE
9320 FOR X = 0 TO Z - 288
9340 READ A$
9360 NEXT X
9380 RETURN
9400 REM
9420 REM   ::::: SET UP CONNECTING CAVE NUMBERS :::::
9440 REM
9460 FOR X = 0 TO 2
9480 A(134+X) = A(137+X+(Z*3))
9500 NEXT X
9520 RETURN
10000 REM
10020 REM55
10040 REM     TREASURE LIST
10060 REM   -----------------
10080 DATAGOLD COINS
10100 DATASOME KEYS
10120 DATAA LOCK BOX
10140 DATAA MAGIC CARPET
10160 DATAAN OLD GUN
10180 DATAA MAGIC WAND
10200 DATAA SWORD
10220 DATAA MAGIC BOOK
10240 DATAAN OLD CLOCK
10260 DATAFURS
10280 DATAA SILVER BELL
10300 DATAA NECKLACE
10320 DATAA PEARL
10340 DATAA DIAMOND
10360 DATAA GOLD WATCH
10380 DATAAN EMERALD
10400 DATAJEWELRY
10420 DATAA $1000 BILL
10440 DATAA BLACK BOOK
10460 DATASOME ELF FOOD
10480 DATAA RUBY
10500 DATAA GOLDEN HARP
10520 DATAA SWORD
10540 REM
10560 REM     DATA FOR CONNECTING CAVES
10580 REM   -----------------------------
10600 DATA1,94,-1
10620 DATA0,2,3
10640 DATA1,4,5
10660 DATA1,6,7
10680 DATA2,8,9
10700 DATA2,10,11
10720 DATA3,12,13
10740 DATA3,14,15
10760 DATA4,16,17
10780 DATA4,18,19
10800 DATA5,20,21
10820 DATA5,22,23
10840 DATA6,24,25
10860 DATA6,26,27
10880 DATA7,28,29
10900 DATA7,30,31
10920 DATA8,32,33
10940 DATA8,34,35
10960 DATA9,36,37
10980 DATA9,38,39
11000 DATA10,40,41
11020 DATA10,42,43
11040 DATA11,44,45
11060 DATA11,46,47
11080 DATA12,48,49
11100 DATA12,50,51
11120 DATA13,52,53
11140 DATA13,54,55
11160 DATA14,56,57
11180 DATA14,58,59
11200 DATA15,60,61
11220 DATA15,62,63
11240 DATA16,63,64
11260 DATA16,34,64
11280 DATA17,33,65
11300 DATA17,36,65
11320 DATA18,35,66
11340 DATA18,38,66
11360 DATA19,37,67
11380 DATA19,40,67
11400 DATA20,39,68
11420 DATA20,42,68
11440 DATA21,41,69
11460 DATA21,44,69
11480 DATA22,43,70
11500 DATA22,46,70
11520 DATA23,45,71
11540 DATA23,48,71
11560 DATA24,47,72
11580 DATA24,50,72
11600 DATA25,49,73
11620 DATA25,52,73
11640 DATA26,51,74
11660 DATA26,54,74
11680 DATA27,53,75
11700 DATA27,56,75
11720 DATA28,55,76
11740 DATA28,58,76
11760 DATA29,57,77
11780 DATA29,60,77
11800 DATA30,59,78
11820 DATA30,62,78
11840 DATA31,61,79
11860 DATA31,32,79
11880 DATA32,33,80
11900 DATA34,35,80
11920 DATA36,37,81
11940 DATA38,39,81
11960 DATA40,41,82
11980 DATA42,43,82
12000 DATA44,45,83
12020 DATA46,47,83
12040 DATA48,49,84
12060 DATA50,51,84
12080 DATA52,53,85
12100 DATA54,55,85
12120 DATA56,57,86
12140 DATA58,59,86
12160 DATA60,61,87
12180 DATA62,63,87
12200 DATA64,65,88
12220 DATA66,67,88
12240 DATA68,69,89
12260 DATA70,71,89
12280 DATA72,73,90
12300 DATA74,75,90
12320 DATA76,77,91
12340 DATA78,79,91
12360 DATA80,81,92
12380 DATA82,83,92
12400 DATA84,85,93
12420 DATA86,87,93
12440 DATA88,89,94
12460 DATA90,91,94
12480 DATA0,92,93
