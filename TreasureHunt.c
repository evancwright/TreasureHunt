/*  =================================================
  TREASURE HUNT BY LANCE MICKLUS, WINOOSKI, VERMONT
  COPYRIGHT SEPTEMBER 1978
  VERSION 2.1 : TRS-80/16K LEVEL II
  PORTED TO C BY EVAN C. WRIGHT, 2019
  ================================================= */

#include  <stdio.h>
#include  <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_CAVES 95

/* ORISMOLOGY OF NON TREASURES */
#define EMPTY 0
#define PITS 1
#define PIRATE 2
#define DRAGON 3
#define VENDING_MACHINE 4
#define CAVE_IN_DANGER 5
#define ELF 6
#define MAGICIANS_CAVE 7
#define INVISIBLE_DOG 8

#define GOLD_COINS 9
#define SOME_KEYS  10
#define LOCK_BOX 11
#define MAGIC_CARPET 12
#define OLD_GUN 13
#define MAGIC_WAND 14
#define STUCK_SWORD 15
#define A_MAGIC_BOOK 16
#define AN_OLD_CLOCK 17
#define FURS 18
#define A_SILVER_BELL 19
#define A_NECKLACE 20
#define A_PEARL 21
#define A_DIAMOND 22
#define A_GOLD WATCH 23
#define AN_EMERALD 24
#define JEWELRY 25
#define A_BLACK_BOOK 26
#define SOME_ELF_FOOD 27
#define A_RUBY 28
#define A_GOLDEN_HARP 29
#define A_SWORD 30
#define A_1000_BILL 31

#define MAX_TREASURES 20

void EntranceRoutine();
void TakeItem(int item);
void RetakeItem(int item);
void PrintInventory();
void StoreTreasures();
void RoofFellIn();
void FellIntoPit();
void MagiciansCave();
void MagicianUpset(int caveGuess);
void MagicCarpetDissapears();
void InvisibleMan();
void EarthQuake();
void Bats();
void ScatterTreasures();
void EatenByDragon();
void PickedUpMagicBook();
void DeadBattery();
void PirateHere();
void ShootDragon();
void DragonRoutine();
void LockBox();
void PrintWelcome();
void Reset();
void PlaceItems();
void MagicWandRoutine();
void VendingMachine();
void MainLoop();
void ChanceSituations();
void SpecialSituations();
void ListNeighbors();
void PrintAdjacentRoomInfo();
void ConnectCaves();
void Connect(int cave, int neighbor1, int neighbor2, int neighbor3);
void ReadKbd();
void CLS();
void HandleTake();
void RemoveItem(int item);
int NextTo(int item);
int PlayerHas(int item);
int IntSafe(int item);
int IsNeighbor(int room);
void MoveItemToNewCave(int item, int oldRoom);
void PrintLocation();
void xyzzy();
void plugh();
void Save();
void Restore();
void Quit();
void Help();
void Score();
void PromptTakeBack();
void ListTreasures();
void PlayAgain();

typedef struct Room
{
	int neighbors[3];
	int item;
} Room;
  
int CurrentRoom = 0;
int SafeTreasures[MAX_TREASURES];
int Inventory[3];
int NumCarriedItems=0;
int NumSafeTreasures=0;
int BatteryLife = 200;
int DogFound = 0; /* BECOMES A 1 AFTER INVISABLE DOG IS FOUND */
int Welcome=0;
int Debug=0;

Room Rooms[NUM_CAVES]; 



char Buffer[80]; /*kbd buffer*/

char *ItemNames[] = {
	"NOT_USED",
	"PITS",
	"PIRATE",
	"DRAGON",
	"VENDING MACHINE",
	">>>DANGER: CAVE INS<<< SIGN", 
	"ELF",
	"MAGICIANS CAVE",
	"INVISIBLE DOG",
	"GOLD COINS", 
	"SOME KEYS",   /* 10 */
	"A LOCK BOX", 
	"A MAGIC CARPET",
	"AN OLD GUN",
	"A MAGIC WAND",
	"A SWORD EMBEDDED IN A LARGE ROCK",
	"A MAGIC BOOK",
	"AN OLD CLOCK",
	"FURS",
	"A SILVER BELL",
	"A NECKLACE",   /* 20 */
	"A PEARL",
	"A DIAMOND",
	"A GOLD WATCH",
	"AN EMERALD",
	"JEWELRY",   /* 25 */
	"A BLACK BOOK",  
	"SOME ELF FOOD",
	"A RUBY",
	"A GOLDEN HARP",
	"A SWORD",  /* 30 */
	"A $1000 BILL"
	};
	

int main(int argc, char **argv)
{
	srand(clock());
	ConnectCaves();
	PlaceItems();
	PrintWelcome();
		
	if (argc == 2 && strcmp(argv[1],"-d")==0)
		Debug = 1;
	
	while (1)
	{
		MainLoop();
	}
		
	return 0;
} 

void CLS()
{
	printf("\x1b[2J"); //cls
	printf("\x1b[0;0H"); //position cursor583
}

	

void  PlaceItems()
{
	/* remove existing items */
	
	for (int i=0; i < NUM_CAVES; i++)
	{
		Rooms[i].item = EMPTY;
	}
	
	/* Set up 3 pits in rooms (3 - 91) */
	for (int i=1; i <= 3; i++)
	{
		int room;
		do
		{
			room = rand()%88+3;
		}
		while (Rooms[room].item != EMPTY);
		
		Rooms[room].item = PITS;
	}

	/* Place items others randomly. Don't place items that are substituted in later */
	for (int i=PIRATE; i <= JEWELRY; i++)
	{
		int room;
		do
		{
			room = rand()%88+3;
		}
		while (Rooms[room].item  != EMPTY);
		
		Rooms[room].item = i;
	}
	
}

 
void MainLoop()
{	
	CLS();
	
	if (!Welcome)
	{
		printf("WELCOME TO THE LUMUS CAVE WHERE YOU'LL FIND MYSTERY, TREASURE, AND ADVENTURE.\n\n");
		printf("YOU CAN SAVE OR RESTORE YOUR GAME AT ANY TIME BY USING THE ");
		printf("'SAVE' AND 'RESTORE'\nCOMMANDS.\n");
		Welcome = 1;
	}


	BatteryLife--;
	
	if (BatteryLife==0) DeadBattery();

	EntranceRoutine(); /* save treasures */
	
	if (Rooms[CurrentRoom].item != PITS)
		PrintLocation();
	
	if (CurrentRoom != 0)
	{
		if (Rooms[CurrentRoom].item != PITS)
			PrintAdjacentRoomInfo();
		
		ChanceSituations();
		SpecialSituations();
		PrintInventory();
		
		if (Rooms[CurrentRoom].item != EMPTY)
		{
			if (Rooms[CurrentRoom].item != MAGICIANS_CAVE && 
				Rooms[CurrentRoom].item != INVISIBLE_DOG && 
				Rooms[CurrentRoom].item != VENDING_MACHINE )
			{
				printf("THIS CAVE HAS %s IN IT.\n", ItemNames[  Rooms[CurrentRoom].item  ]);
			}
			
			if (Rooms[CurrentRoom].item != CAVE_IN_DANGER &&
				Rooms[CurrentRoom].item != MAGICIANS_CAVE && 
				Rooms[CurrentRoom].item != INVISIBLE_DOG && 
				Rooms[CurrentRoom].item != VENDING_MACHINE)
			{
				HandleTake();
			}
		}
	}
	
	
	/*   ::::: WHERE TO NEXT ::::: */
	do
	{
		printf("\nWHAT CAVE DO YOU WANT TO EXPLORE NEXT?\n");
		ReadKbd();
		
		if (strcmp(Buffer,"xyzzy")==0 && Debug == 1)
		{
			xyzzy();
			break;
		}
		else if (strcmp(Buffer,"save")==0)
		{
			Save();
			break;
		}
		else if (strcmp(Buffer,"restore")==0)
		{
			Restore();
			break;
		}
		else if (strcmp(Buffer,"quit")==0 || strcmp(Buffer,"bye") == 0 || Buffer[0] == 'q')
		{
			Quit();
			break;
		}
		else if (strcmp(Buffer,"testdog")==0 && Debug == 1)
		{
			InvisibleMan();
		}
		else if (strcmp(Buffer,"testquake")==0 && Debug == 1)
		{
			EarthQuake();
		}
		else if (strcmp(Buffer,"testbat")==0 && Debug == 1)
		{
			Bats();
		}
		else if (strcmp(Buffer,"plugh")==0 && Debug == 1)
		{
			plugh();		
		}
		else 
		{
			int room = atoi(Buffer);
		
			if (!IsNeighbor(room) && !Debug)
			{
				printf("SORRY, BUT YOU CAN'T GO THERE FROM HERE.\n");
			}
			else
			{
				if (Rooms[CurrentRoom].item == ELF)
				{
					Rooms[CurrentRoom].item = SOME_ELF_FOOD; 
				}
				CurrentRoom = room;
				break;
			}
		}
	} while (1);
}

/*   ::::: CHANCE SITUATIONS :::::  */		
void ChanceSituations()
{
	if ( rand()% 200 == 0) EarthQuake();
	if ( rand() % 100 == 0) Bats();
	if ( DogFound == 0 &&  Rooms[CurrentRoom].item == EMPTY && rand()%50==0 ) InvisibleMan();
	if (  Rooms[CurrentRoom].item == CAVE_IN_DANGER && rand()%5==0 ) RoofFellIn();
	if (  PlayerHas(MAGIC_CARPET)  && rand()%10==0 ) MagicCarpetDissapears();
}


/*   ::::: SEE IF SPECIAL SITUATION :::::  */
void SpecialSituations()
{
	if ( Rooms[CurrentRoom].item == PITS ) FellIntoPit();
	else if (Rooms[CurrentRoom].item == PIRATE) PirateHere();
	else if ( Rooms[CurrentRoom].item == DRAGON ) DragonRoutine();
	else if (Rooms[CurrentRoom].item == VENDING_MACHINE) VendingMachine();
	else if  (Rooms[CurrentRoom].item == ELF) printf("THERE'S A SHY LITTLE ELF IN HERE HIDING SOMETHING.\n");
	else if  (Rooms[CurrentRoom].item == MAGICIANS_CAVE) MagiciansCave();
}

 

/*   ::::: TREASURE IN THIS CAVE :::::   */
void HandleTake()
{
	do
	{
		printf("DO YOU WANT TO TAKE IT WITH YOU? (Y/N)");
		
		ReadKbd();
		
		if (Buffer[0] == 'N' || Buffer[0] == 'n') return;
		if (Buffer[0] == 'Y' || Buffer[0] == 'y' ) break; 
	} while (1);
	
	if (NumCarriedItems == 3)
	{
		printf("YOU CAN'T PICK IT UP NOW. YOU'RE CARRYING TOO MUCH.\n");
	}
	else
	{
		int item = Rooms[CurrentRoom].item;

		if (item == LOCK_BOX)  
			LockBox();
		else if ( item == STUCK_SWORD) 
			MagicWandRoutine();
		else if ( item == A_MAGIC_BOOK ) 
			PickedUpMagicBook();
		else
			TakeItem(item);
	
	}
	
}

/* Puts an item in the player's inventory and removes it from the room*/
void TakeItem(int item)
{
	printf("O.K. YOU'VE GOT %s.\n", ItemNames[item]);
	Inventory[NumCarriedItems++] = item;
	Rooms[CurrentRoom].item = EMPTY;
}

/* Puts an item in the player's inventory and removes it from the room*/
void RetakeItem(int item)
{
	printf("O.K. YOU'VE GOT %s.\n", ItemNames[item]);
	Inventory[NumCarriedItems++] = item;
}



/*   ::::: LOCK BOX :::::  */
void LockBox()
{
	if (PlayerHas(SOME_KEYS))
	{
		printf("THE BOX WAS TOO HEAVY SO I OPENNED IT WITH YOUR KEYS.\n");
		printf("INSIDE THE BOX IS A RUBY.\n");
		Rooms[CurrentRoom].item = A_RUBY;
		HandleTake();
	}
	else
	{
		printf("YOU CAN'T TAKE IT. IT'S TOO HEAVY.\n");
	}
}


/*   ::::: VENDING MACHINE ::::: */
void VendingMachine()
{
	printf("THERE'S A VENDING MACHINE HERE.\n");
	if (  PlayerHas(GOLD_COINS) )
	{
		printf("I'LL GET SOME BATTERIES WITH ONE OF YOUR GOLD COINS.\n");
		BatteryLife = 200;
	}
}

/*   ::::: EARTHQUAKE ::::: */
void EarthQuake()
{
	/* moves 10 items to new locations */
	for (int i=0; i < NUM_CAVES; i++)
	{
		if (Rooms[i].item != EMPTY)
		{
			MoveItemToNewCave(Rooms[i].item,i);
		}
	}

	printf("GUESS WHAT? WE JUST HAD AN EARTHQUAKE. BUT I'M O.K.\n");
}

/*   ::::: MAGIC WAND ROUTINE ::::: */
void MagicWandRoutine()
{
	if (!PlayerHas(MAGIC_WAND))
	{
		printf("IT'S STUCK IN THE ROCKS AND CAN'T BE PULLED OUT.\n");
	}
	else
	{
		fflush(stdout);
		usleep(100000);
		printf("LET ME USE YOUR MAGIC WAND A SECOND.\n");
		fflush(stdout);
		usleep(1000000);
		printf("HOCUS...");
		fflush(stdout);
		usleep(1000000);
		printf("POCUS...\n");
		fflush(stdout);
		usleep(1000000);
		Rooms[CurrentRoom].item = A_SWORD;
		printf("THE SWORD JUST LEVITATED FROM THE ROCK!\n");
		HandleTake();
	}

}


/*   ::::: BATS SHOW UP ::::: */
void Bats()
{
	int newRoom = 0;
	CLS();
	printf("A SUPER BAT JUST FEW INTO THE CAVE AND PICKED YOU UP.\n");
	if (NumCarriedItems > 0)
	{
		printf("YOU DROPPED ALL OF YOUR TREASURES ALONG THE WAY.\n");
		ScatterTreasures();
	}
	
	/* now move the player */
	do
	{
		newRoom = rand()%93 + 1;
		if (Rooms[newRoom].item != EMPTY) break;
	} while (1);
	CurrentRoom = newRoom;

	
	printf("THE BAT JUST DROPPED YOU INTO CAVE %d!!!\n", CurrentRoom);
	printf("IF YOU'RE ALRIGHT HIT THE ''ENTER'' KEY");
	ReadKbd();
	CLS();
}

/*   ::::: PIRATE HERE :::::  */
void PirateHere()
{
	printf("THERE WAS A PIRATE IN HERE.\n");
	
	if (NumCarriedItems > 0)
	{
		/*
		for (int i=0; i < 3; i++)
		{
			Inventory[i] = EMPTY;			
		}*/
		ScatterTreasures();
		printf("HE JUST STOLE ALL OF YOUR TREASURES.\n");
		NumCarriedItems=0;
	}
	
	MoveItemToNewCave(PIRATE,CurrentRoom);
	
	printf("HE'S GONE NOW.\n");
}

/*   ::::: MAGICIAN ROUTINE :::::  */
void MagiciansCave()
{
	CLS();
	printf("THERE'S A MAGICIAN IN THIS CAVE. HE SAYS HE LOST HIS MAGIC\n");
	printf("BOOK. HE SAID HE'D GIVE YOU A GOLDEN HARP IF YOU'LL TELL HIM\n");
	printf("WHERE IT IS. TYPE IN THE CAVE NUMBER. IF YOU DON'T KNOW, THEN\n");
	printf("JUST TYPE 99.\n\n");
	printf("WHERE'S HIS MAGIC BOOK?");
	
	ReadKbd();

	int guess = atoi(Buffer);
	
	if (guess != 99)
	{
		if (Rooms[guess].item != A_MAGIC_BOOK)
		{
			MagicianUpset(guess);
		}
		else
		{
			printf("THE WIZARD THANKS YOU AND VANISHES IN A PUFF OF SMOKE.\n");

			/* Remove the magic book */
			for (int i=0; i < NUM_CAVES; i++)
			{
				if (Rooms[i].item == A_MAGIC_BOOK)
				{
					Rooms[i].item = EMPTY;
					break;
				}
			}				
			
			
			Rooms[CurrentRoom].item = A_GOLDEN_HARP; /* replace magician with harp */
			
			printf("THERE IS %s HERE.\n", ItemNames[A_GOLDEN_HARP]);
			HandleTake();
						
		}
				
	}
	else
	{
		MoveItemToNewCave(MAGICIANS_CAVE,CurrentRoom);
	}

	CLS();
	PrintLocation();
	PrintAdjacentRoomInfo();
}

/*   ::::: INVISABLE MAN ROUTINE :::::  */
void InvisibleMan()
{
	printf("\nTHERE IS AN INVISIBLE MAN IS HERE LOOKING FOR HIS INVISIBLE DOG.\n");
	printf("HE SAYS HE'LL GIVE YOU A $1000 REWARD IF YOU CAN TELL HIM\n");
	printf("WHERE HIS DOG IS. IF YOU DON'T KNOW, GUESS.\n\n");

	printf("WHAT CAVE IS THE INVISIBLE DOG IN?");
	ReadKbd();
	int room = atoi(Buffer);	
	
	if (Rooms[room].item == INVISIBLE_DOG)
	{
			printf("***POOF***\n");
			printf("A $1000 BILL FLUTTERS TO THE GROUND!\n");
			Rooms[CurrentRoom].item = A_1000_BILL;
			DogFound = 1;
			HandleTake();
	}
	CLS();
	PrintLocation();
	PrintAdjacentRoomInfo();
}

/*  ::::: MAGIC CARPET DISSAPPEARS ::::: */
void MagicCarpetDissapears()
{
	printf("POOF !!!     YOUR MAGIC CARPET JUST DISSAPPEARED!\n");
	RemoveItem(MAGIC_CARPET);		
}



/*   ::::: DRAGON ROUTINE ::::: */
void DragonRoutine()
{
	if (PlayerHas(OLD_GUN))
	{
		ShootDragon();
	}
	else
	{
		 EatenByDragon();
	}
}

void ShootDragon()
{
	usleep(500000);
	fflush(stdout);
	printf("YIKES !!!! THERE'S A DRAGON IN HERE.\n");
	usleep(500000);
	fflush(stdout);
	printf("GIVE ME YOUR GUN, QUICK !!!");
	usleep(500000);
	fflush(stdout);
	for (int i=0; i < 5 ; i++)
	{
		usleep(250000);
		printf("\tBANG!");
		fflush(stdout);
	}
	
	usleep(1000000);
	printf("\nPOOF!!!\n");
	fflush(stdout);
	usleep(100000);

	printf("INCREDIBLE???  THE DRAGON JUST VANISHED WHEN I SHOT HIM RIGHT\n");
	printf("BETWEEN THE EYES.\n");
	printf("BUT LOOK AT THIS. HE LEFT BEHIND HIS LITTLE BLACK BOOK\n");
	printf("WITH THE ADDRESSES AND PHONE NUMBERS OF EVERY BEAUTIFUL\n");
	printf("PRINCESS THAT LIVES IN VERMONT.\n\n");
	printf("HIT THE ''ENTER'' KEY AFTER YOU CATCH YOUR BREATH\n");
	
	ReadKbd();
	
	Rooms[CurrentRoom].item = A_BLACK_BOOK;
	BatteryLife++;
	
	CLS();
	PrintLocation();
}

/*   ::::: AT ENTRANCE ROUTINE ::::: */
void EntranceRoutine()
{
	if (CurrentRoom == 0)
	{ 
		StoreTreasures();

		Score();
				
		if (NumSafeTreasures > 0)
		{	
			ListTreasures();
			PromptTakeBack();
		}
		printf("\n");
	}

	BatteryLife++;
}


	


/*
			END OF PROGRAM ROUTINES
			---------------------------
*/
/*   ::::: FELL INTO A PIT ::::: */

void FellIntoPit()
{
	printf("SORRY, BUT I TRIED TO WARN YOU.\n");
	printf("YOU FELL INTO A DEEP PIT AND KILLED YOURSELF !!!\n\n");
	PlayAgain();
}


/*   ::::: EATEN BY THE DRAGON ::::: */
void EatenByDragon()
{
	CLS();
	printf("SORRY, BUT I TRIED TO TELL YOU ABOUT THAT SOUND.\n");
	printf("SUPPER IS NOW BEING SERVED HERE IN THE DRAGON'S CHAMBER...\n");
	printf("AND YOU ARE THE SUPPER.\n\n");
	PlayAgain();
}

/*   ::::: ROOF FELL IN ::::: */
void RoofFellIn()
{
	printf("\n");
	printf("THE ROOF JUST FELL IN AND BURRIED YOU ALIVE. TOO BAD I DON'T\n");
	printf("HAVE A SHOVEL OR I'D DIG YOU OUT.\n");
	printf("\n");
	PlayAgain();
}
 
/*   ::::: PICKED UP MAGIC BOOK ::::: */
void PickedUpMagicBook()
{
	printf("\n");
	printf("I DON'T THINK YOU SHOULD HAVE DONE THAT. THE MAGICIAN WHO\n");
	printf("OWNS THAT MAGIC BOOK PUT A SPELL ON IT. ANYBODY WHO TRIES TO\n");
	printf("PICK IT UP TURNS INTO A FROG.\n");
	printf("\nEXCUSE ME WHILE I LOOK FOR SOME FLIES.\n");
	printf("BETTER LUCK NEXT TIME.\n");
	PlayAgain();
}

/*   ::::: BATTERIES WENT DEAD ::::: */
void DeadBattery()
{
	printf("\n");
	printf("NOW LOOK WHAT YOU DID. YOUR FLASH LIGHT WENT OUT AND YOU FELL\n");
	printf("INTO A PIT AND KILLED YOURSELF. TOO BAD. ESPECIALLY AFTER YOU\n");
	printf("WERE DOING SO WELL.\n\n");
	PlayAgain();
}

/*   ::::: USER QUITS ::::: */
void Quit()
{
	if (NumSafeTreasures > 0)
	{
		printf("THE TREASURES ARE YOURS TO KEEP.\n");
	}
	else
	{
		printf("BYE.\n");
	}
	exit(0);
}


/*   ::::: MAGICIAN UPSET */
void MagicianUpset(int caveGuess)
{
	printf("\n");
	printf("YOU'VE MADE THE MAGICIAN VERY ANGRY. CAVE %d DOESN'T HAVE\n", caveGuess);
	printf("A MAGIC BOOK IN IT. TO PUNISH YOU, THE MAGICIAN CAST A SPELL\n");
	printf("ON YOU AND NOW YOU'RE ONLY 2 INCHES TALL. WORST YET, THE\n");
	printf("THE MAGICIAN PUT YOU IN A SMALL JAR. IF YOU EVER GET OUT OF\n");
	printf("THIS MESS, LET ME KNOW.\n\n");
	PlayAgain();
}

/*   ::::: MOVE ITEM TO NEW CAVE LOCATION :::::   */
void MoveItemToNewCave(int item, int oldRoom)
{

	int newRoom = 0;  /* y is the new room */;
	do
	{
		newRoom = rand()%88 + 3;
	}
	while ( Rooms[newRoom].item != EMPTY );
	
	Rooms[newRoom].item = item;	
	Rooms[oldRoom].item = EMPTY;	
}





void PrintAdjacentRoomInfo()
{
	if (NextTo(PITS)) printf("THERE'S PITS NEARBY. WATCH YOUR STEP.\n");
	if (NextTo(PIRATE)) printf("CAREFUL. THERE'S A PIRATE NEAR HERE.\n");
	if (NextTo(DRAGON)) printf("I HEAR A HUNGRY DRAGON WAITING FOR HIS SUPPER.\n");
	if (NextTo(CAVE_IN_DANGER)) printf("THERE'S A SIGN HERE THAT SAYS, >>> D A N G E R <<<\n");
	if (NextTo(ELF)) printf("SOUNDS LIKE SOMEBODY IS SINGING. MUST BE AN ELF.\n");
	if (NextTo(MAGICIANS_CAVE)) printf("THERE'S A STRANGE MIST IN THIS CAVE.\n");
	if (NextTo(INVISIBLE_DOG)) printf("''RUFF RUFF''  I HEAR A DOG BARKING.\n");
}

void PrintWelcome()
{
	CLS();
	printf("T R E A S U R E   H U N T\n");
	printf("BY LANCE MICKLUS, 1978\n");
	printf("PORTED BY EVAN WRIGHT, 2019 (WITH PERMISSION)\n");
	printf("\n");
	printf("THE LUMUS CAVES, LOCATED IN LUMUSVILLE, VERMONT, ARE SAID TO\n");
	printf("HAVE 20 HIDDEN TREASURES IN THEM. FEW EXPLORE THE CAVES\n");
	printf("BECAUSE IT IS SAID THAT PIRATES AND DRAGONS LIVE THERE, AND\n");
	printf("THERE ARE DEEP PITS WHICH MANY HAVE FALLEN INTO AND DIED.\n");
	printf("\n");
	printf("YOU, A SMART AND BRAVE HUMAN, AND I, AN ALERT COMPUTER, WILL\n");
	printf("EXPLORE THE CAVES AND TRY TO FIND THE TREASURE. I WILL BE YOUR\n");
	printf("EYES AND EARS, AND WILL TELL YOU IF THERE'S DANGER AHEAD.\n");
	printf("\n");
	printf("I HOPE YOU BROUGHT A MAP. IF NOT, YOU'LL HAVE TO MAKE ONE UP\n");
	printf("AS WE EXPLORE.\n");
	printf("\n");
	printf("HIT THE ''ENTER'' KEY WHEN YOU'RE READY TO BEGIN\n");
	ReadKbd();
	CLS();
}

void PrintInventory()
{
	
	if (NumCarriedItems > 0)
	{
		printf("YOU ARE CARRYING:");
		for (int i=0; i < NumCarriedItems; i++)
		{
			if (Inventory[i] != EMPTY)
			{
				printf("\t%s",  ItemNames[Inventory[i]]);
			}
		}
		printf("\n");

	}
}


int PlayerHas(int item)
{
	for (int i=0; i < 3; i++)
	{
		if (Inventory[i] == item)
		{
			return 1;
		}
	}
	return 0;
}

int IsSafe(int item)
{
	for (int i=0; i <  MAX_TREASURES; i++)
	{
		if (SafeTreasures[i] == item)
			return 1;
	}
	return 0;
}


void BatteryMessages()
{
	if (BatteryLife == 50) printf("\n\nI HOPE YOU BROUGHT MORE FLASH LIGHT BATTERIES.\n");
	if (BatteryLife == 25) printf("\n\nYOUR FLASH LIGHT IS STARTING TO DIM A LITTLE.\n");
	if (BatteryLife == 8) printf("YOUR FLASH LIGHT IS ALMOST OUT. BETTER LEAVE NOW!!!\n");
}


void PrintLocation()
{
	if (CurrentRoom == 0)
	{
		printf("YOU'RE AT A CAVE ENTRANCE WHICH LEADS TO:\n");
	}
	else
	{
		printf("YOU'RE AT CAVE %d WHICH LEADS TO:\n", CurrentRoom);
	}
	
	ListNeighbors();
}

/* ::::: WHERE YOU CAN GO NEXT ::::: */
void ListNeighbors()
{
	
	for (int i=0; i < 3; i++)
	{
		if (Rooms[CurrentRoom].neighbors[i] == -1)
			break;
		
		printf("\tCAVE %d", Rooms[CurrentRoom].neighbors[i]);
		
		if (Rooms[CurrentRoom].neighbors[i] == 0)
			printf(" - CAVE ENTRANCE");
		printf("\n");	
	}
	
}


/*removes an item from the player's inventory*/
void RemoveItem(int item)
{
	/* shift items down */
	for (int i=0; i < 3; i++)
	{
		if (Inventory[i] == item)
		{
			for (int j=i ; j <= 2; j++)
			{
				Inventory[j] = Inventory[j+1];
			}
			NumCarriedItems--;
			break;
		}
	}	
}

/* Moves All Treasures To SafeTreasures arrays*/
void StoreTreasures()
{
	for (int i = 0; i < NumCarriedItems; i++)
	{
		SafeTreasures[NumSafeTreasures++] = Inventory[i];
		Inventory[i] = EMPTY;
	}
	NumCarriedItems = 0;
}

int NextTo(int item)
{
	for (int i=0; i < 3; i++)
	{
		int n = Rooms[CurrentRoom].neighbors[i];
		
		if (Rooms[n].item == item)
			return 1;
	}
	return 0;
}

int IsNeighbor(int n)
{
	for (int i=0; i < 3; i++)
	{
		if (Rooms[CurrentRoom].neighbors[i] == n)
			return 1;
	}
	return 0;
}


/*   ::::: SET UP CONNECTING CAVE NUMBERS :::::  */
void ConnectCaves()
{
	Connect(0,1,94,-1);
	Connect(1,0,2,3);
	Connect(2,1,4,5);
	Connect(3,1,6,7);
	Connect(4,2,8,9);
	Connect(5,2,10,11);
	Connect(6,3,12,13);
	Connect(7,3,14,15);
	Connect(8,4,16,17);
	Connect(9,4,18,19);
	Connect(10,5,20,21);
	Connect(11,5,22,23);
	Connect(12,6,24,25);
	Connect(13,6,26,27);
	Connect(14,7,28,29);
	Connect(15,7,30,31);
	Connect(16,8,32,33);
	Connect(17,8,34,35);
	Connect(18,9,36,37);
	Connect(19,9,38,39);
	Connect(20,10,40,41);
	Connect(21,10,42,43);
	Connect(22,11,44,45);
	Connect(23,11,46,47);
	Connect(24,12,48,49);
	Connect(25,12,50,51);
	Connect(26,13,52,53);
	Connect(27,13,54,55);
	Connect(28,14,56,57);
	Connect(29,14,58,59);
	Connect(30,15,60,61);
	Connect(31,15,62,63);
	Connect(32,16,63,64);
	Connect(33,16,34,64);
	Connect(34,17,33,65);
	Connect(35,17,36,65);
	Connect(36,18,35,66);
	Connect(37,18,38,66);
	Connect(38,19,37,67);
	Connect(39,19,40,67);
	Connect(40,20,39,68);
	Connect(41,20,42,68);
	Connect(42,21,41,69);
	Connect(43,21,44,69);
	Connect(44,22,43,70);
	Connect(45,22,46,70);
	Connect(46,23,45,71);
	Connect(47,23,48,71);
	Connect(48,24,47,72);
	Connect(49,24,50,72);
	Connect(50,25,49,73);
	Connect(51,25,52,73);
	Connect(52,26,51,74);
	Connect(53,26,54,74);
	Connect(54,27,53,75);
	Connect(55,27,56,75);
	Connect(56,28,55,76);
	Connect(57,28,58,76);
	Connect(58,29,57,77);
	Connect(59,29,60,77);
	Connect(60,30,59,78);
	Connect(61,30,62,78);
	Connect(62,31,61,79);
	Connect(63,31,32,79);
	Connect(64,32,33,80);
	Connect(65,34,35,80);
	Connect(66,36,37,81);
	Connect(67,38,39,81);
	Connect(68,40,41,82);
	Connect(69,42,43,82);
	Connect(70,44,45,83);
	Connect(71,46,47,83);
	Connect(72,48,49,84);
	Connect(73,50,51,84);
	Connect(74,52,53,85);
	Connect(75,54,55,85);
	Connect(76,56,57,86);
	Connect(77,58,59,86);
	Connect(78,60,61,87);
	Connect(79,62,63,87);
	Connect(80,64,65,88);
	Connect(81,66,67,88);
	Connect(82,68,69,89);
	Connect(83,70,71,89);
	Connect(84,72,73,90);
	Connect(85,74,75,90);
	Connect(86,76,77,91);
	Connect(87,78,79,91);
	Connect(88,80,81,92);
	Connect(89,82,83,92);
	Connect(90,84,85,93);
	Connect(91,86,87,93);
	Connect(92,88,89,94);
	Connect(93,90,91,94);
	Connect(94,0,92,93);
}

void Connect(int cave, int n1, int n2, int n3)
{
	Rooms[cave].neighbors[0] = n1;
	Rooms[cave].neighbors[1] = n2;
	Rooms[cave].neighbors[2] = n3;
}


void ReadKbd()
{
	char *line = Buffer;
	size_t len = 80;
	getline(&line, &len, stdin);
	Buffer[strlen(Buffer)-1] = 0;
	printf("\n");
}

void xyzzy()
{
	printf("WHICH CAVE WOULD YOU LIKE TO TELEPORT TO?\n");
	ReadKbd();
	int room = atoi(Buffer);
	printf("GOING TO ROOM %d\n", room);
	CurrentRoom = room;
}

void plugh()
{
	printf("A HOLLOW VOICE SAYS...\n");
	
	for (int i=0; i < NUM_CAVES; i++)
	{
		if (Rooms[i].item != EMPTY)
		{
			printf("%s IS IN CAVE %d.\n", ItemNames[Rooms[i].item], i);
		}		
	}
}



void Save()
{
	printf("ENTER SAVE FILE NAME:");
	ReadKbd();
	strcat(Buffer,".sav");
	
	FILE *fp = fopen(Buffer,"wb");
	if (fp)
	{
		fwrite(&CurrentRoom, sizeof(int), 1, fp );
		fwrite(&DogFound, sizeof(int), 1, fp );
		fwrite(&BatteryLife, sizeof(int), 1, fp );
		fwrite(&NumCarriedItems, sizeof(int), 1, fp );
		fwrite(&NumSafeTreasures, sizeof(int), 1, fp );
		fwrite(Inventory, sizeof(int), 3, fp );
		fwrite(SafeTreasures, sizeof(int), MAX_TREASURES, fp );
		fwrite(Rooms, sizeof(Room), NUM_CAVES, fp );
		fclose(fp);
		printf("GAME SAVED.\n");
	}
	else
	{
		printf("UNABLE TO OPEN FILE FOR WRITING.\n");
	}
	
	printf("PRESS ENTER TO CONTINUE.\n");
	ReadKbd();
	CLS();
}

void Restore()
{
	printf("ENTER FILE TO RESTORE:");
	ReadKbd();
	strcat(Buffer,".sav");
	
	FILE *fp = fopen(Buffer,"rb");
	if (fp)
	{
		fread(&CurrentRoom, sizeof(int), 1, fp );
		fread(&DogFound, sizeof(int), 1, fp );
		fread(&BatteryLife, sizeof(int), 1, fp );
		fread(&NumCarriedItems, sizeof(int), 1, fp );
		fread(&NumSafeTreasures, sizeof(int), 1, fp );
		fread(Inventory, sizeof(int), 3, fp );
		fread(SafeTreasures, sizeof(int), MAX_TREASURES, fp );
		fread(Rooms, sizeof(Room), NUM_CAVES, fp );
		fclose(fp);
		printf("GAME RESTORED.\n");
	}
	else
	{
		printf("UNABLE TO OPEN FILE FOR READING.\n");
	}
	printf("PRESS ENTER TO CONTINUE.\n");
	ReadKbd();
	CLS();
}

void Score()
{
	if (NumSafeTreasures == MAX_TREASURES)
	{ 
		printf("CONGRATULATIONS!\nYOU'VE RECOVERED ALL THE TREASURES.\n");
		
		printf("WOULD YOU LIKE TO KEEP EXPLORING? (Y/N)\n");
		ReadKbd();
		if (Buffer[0] == 'N' || Buffer[0] == 'n')
		{
			Quit();
		}
	}
}

/*puts player's treasures in random empty rooms*/
void ScatterTreasures()
{
	int newRoom = 0;
	for (int i=0; i < 3; i++)
	{
		do
		{
			newRoom = rand()%(NUM_CAVES-1) + 1;
			if (Rooms[newRoom].item != EMPTY) 
				break;
		} while (1);
		
		Rooms[newRoom].item = Inventory[i];
		Inventory[i] = 0;
	}
	
	NumCarriedItems = 0;
}

void ListTreasures()
{
	printf("SO FAR, YOU'VE FOUND THESE TREASURES IN THE CAVES:\n");

	for (int i = 0; i < NumSafeTreasures; i++)
	{
		printf("\t%s", ItemNames[SafeTreasures[i]]);
		if ((i+1) % 4 == 0)
			printf("\n");
	}
	printf("\n");

}

void PromptTakeBack()
{
	if (NumSafeTreasures > 0)
	{		
		printf("\nWOULD YOU LIKE TO PUT ANY TREASURES BACK IN YOUR INVENTORY? (Y/N)\n");
		ReadKbd();
		if (Buffer[0] == 'Y' || Buffer[0] == 'y')
		{
			for (int i=0; i < NumSafeTreasures ; i++)
			{
				printf("\t%d: %s",(i+1),ItemNames[ SafeTreasures[i] ]);
				if ((i+1)%4==0)
					printf("\n");
			}
			printf("\n");
			printf("ENTER THE NUMBER OF THE TREASURE TO TAKE?\n");
			
			ReadKbd();
			int n = atoi(Buffer);
			if ( n <= NumSafeTreasures && n > 0)
			{
				n--;  /* turn index into item # */
				int t = SafeTreasures[n];
				RetakeItem(t);
				
				/* now remove n from the safe treasures */
				for (int i=n; i < NumSafeTreasures-1; i++)
				{
					SafeTreasures[i] = SafeTreasures[i+1];
				}
				NumSafeTreasures--;
				
			}
			else
			{
				printf("INVALID CHOICE.");
			}
		}
	}
}


void Reset()
{
	CurrentRoom=0;
	NumCarriedItems=0;
	NumSafeTreasures=0;
	PlaceItems();
	EntranceRoutine(); /* save treasures */
	PrintLocation();
}

void PlayAgain()
{
	printf("WOULD YOU LIKE TO PLAY AGAIN? (Y/N)\n");
	ReadKbd();
	
	if (Buffer[0] == 'Y' || Buffer[0] == 'y')
	{
		CLS();
		Reset();
	}
	else
	{
		Quit();
	}
}
