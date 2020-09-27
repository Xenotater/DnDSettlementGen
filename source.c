//DnD Settlement Character Algorithm
//Kyler Froman, Michael Bledsoe

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Settings Struct
typedef struct _Settings{
	char filename[256];
	int amount;
	char nb[10];
	int classNum;
	int classSides;
	int lvlNum;
	int lvlSides;
	int lvlMod;
	char basic[10];
	int maleNum;
	int femaleNum;
	int nbNum;
	int lastNum;
}Settings;

//Character Struct
typedef struct _Character{
    int gender;
		char lastName[256];
		char firstName[256];
		int age;
		char * ageGroup;
		char * race;
		float ageMod;
		char * hybridRaceA;
		char * hybridRaceB;
		char * class;
		int level;
		char * job;
		char * talent;
		char * best;
		char * worst;
		char * appearance;
		char * interaction;
		char * posTrait;
		char * negTrait;
		
}Character;

//Regenerates the settings file
//Takes in nothing, returns 0 if successful and 1 if failed
int regenSettings()
{
	FILE *fp;
	if((fp = fopen("settings.txt", "w")) == NULL)
	{
		printf("*** Error: settings.txt could not be opened or was corrupted and a new one could not be generated. ***\n");
		return 1;
	}
	else
	{
		fprintf(fp, "Settings File for Village Generator. Modify Carefully.\n\noutput-filename: village\nresident-number: 50\ninclude-nb: true\nclass-chance: 1 d 6\nclass-level: 1 d 10 + 0\nbasic-races: true\n");
		fprintf(fp, "male-names: 582\nfemale-names: 180\nnb-names: 32\nlast-names: 40\n\n\n");
		fprintf(fp, "- \"output-filename\" is the name of the file that will be output. This file will always be a .csv, which can be opened in excel, so don't include an extension.\n\n");
		fprintf(fp, "- \"resident-number\" is the number of residents you'd like to be generated. Must be a positive integer.\n\n");
		fprintf(fp, "- \"include-nb\" is whether or not to include non-binary as a gender or just use male/female. Must be true or false (anything else will be treated as false).\n\n");
		fprintf(fp, "- \"class-chance\" is the chance for a resident to have levels in a class. Must be in the format \"(num) d (sides)\".\n\n");
		fprintf(fp, "- \"class-level\" is the level a resident who has a class will be. Must be in the format \"(num) d (sides) + (mod)\".\n\n");
		fprintf(fp, "- \"basic-races\" is whether to use all races or just races that one would normally expect to find in a settlement. Must be true or false (anything else will be treated as false).\n\n");
		fprintf(fp, "- \"___-names\" are the number of names listed in the names files. Must be a positive integer. Only change if you've edited the names files.\n\n");
		fclose(fp);
		printf("settings.txt could not be opened or was corrupted, so a new one was generated with defaults.\n");
	}
	return 0;
}

//Loads the settings file
//Takes in nothing, returns a settings struct or NULL if loading failed.
Settings * loadSettings()
{
	FILE *fp;
	char trash[256], trash2;
	Settings * s = malloc(sizeof(Settings));
	if(s)
	{
		if((fp = fopen("settings.txt", "r")) == NULL)
		{
			if(regenSettings() == 0)
				s = loadSettings();
			else
				return NULL;
		}
		else
		{
			fscanf(fp, "%s %s %s %s %s %s %s\n\n", trash, trash, trash, trash, trash, trash, trash);
			fscanf(fp, "%s %s\n", trash, s->filename);
			fscanf(fp, "%s %d\n", trash, &(s->amount));
			fscanf(fp, "%s %s\n", trash, s->nb );
			fscanf(fp, "%s %d %c %d\n", trash, &(s->classNum), &trash2, &(s->classSides));
			fscanf(fp, "%s %d %c %d %c %d\n", trash, &(s->lvlNum), &trash2, &(s->lvlSides), &trash2, &(s->lvlMod));			
			fscanf(fp, "%s %s\n", trash, s->basic);			
			fscanf(fp, "%s %d\n", trash, &(s->maleNum));
			fscanf(fp, "%s %d\n", trash, &(s->femaleNum));			
			fscanf(fp, "%s %d\n", trash, &(s->nbNum));			
			fscanf(fp, "%s %d\n", trash, &(s->lastNum));			

			fclose(fp);
		}
	}
	return s;
}

//Roll any number of any size of dice, + an optional modifier
//Takes in 3 ints for amount, size, and modifier respectively
//Returns a number
int rollDice(int amount, int size, int mod)
{
	int i, num = 0;
	
	for(i = 0; i < amount; i++)
		num += rand() % size + 1;	
	
	num += mod;
	
	return num;
}

//determine gender of a character
//takes a character, returns nothing
void rollGender(Character * c, Settings * s)
{
	int num = rollDice(1, 100, 0);
	if(num < 49)
		c->gender = 1;	//male
	else if(num < 99)
		c->gender = 2;	//female
	else if(strcmp("true", s->nb) == 0)
		c->gender = 0;	//NB
	else
		c->gender = rollDice(1, 2, 0);
	
	return;
}

//determine name of a character
//takes a character, returs nothing
//name will be blank if file can't open
void rollName(Character * c, Settings * s)
{
	int i;
	char  trash[20];
	FILE *fp;
	
	if(c->gender == 1)
	{
		if((fp = fopen("source/male.txt", "r")) == NULL)
		{
			printf("*** Error: male.txt could not be opened, make sure it exists in the source folder ***\n");
			return;
		}
		else
		{
			int offset = rollDice(1, s->maleNum , -1);
			for(i = 0; i < offset; i++)
				fscanf(fp, "%s\n", trash); 
			fscanf(fp, "%s\n", c->firstName);
			fclose(fp);
		}
	}
	else if(c->gender == 2)
	{
		if((fp = fopen("source/female.txt", "r")) == NULL)
		{
			printf("*** Error: female.txt could not be opened, make sure it exists in the source folder ***\n");
			return;
		}
		else
		{
			int offset = rollDice(1, s->femaleNum, -1);
			for(i = 0; i < offset; i++)
				fscanf(fp, "%s\n", trash);
			fscanf(fp, "%s\n", c->firstName);
			fclose(fp);
		}
	}
	else
	{	
		if((fp = fopen("source/nb.txt", "r")) == NULL)
		{
			printf("*** Error: nb.txt could not be opened, make sure it exists in the source folder ***\n");
			return;
		}
		else
		{
			int offset = rollDice(1, s->nbNum, -1);
			for(i = 0; i < offset; i++)
				fscanf(fp, "%s\n", trash);
			fscanf(fp, "%s\n", c->firstName);
			fclose(fp);
		}
	}
	
	if((fp = fopen("source/last.txt", "r")) == NULL)
	{
		printf("*** Error: last.txt could not be opened, make sure it exists in the source folder ***\n");
		return;
	}
	else
	{
		int offset = rollDice(1, s->lastNum, -1);
		for(i = 0; i < offset; i++)
			fscanf(fp, "%s\n", trash); 
		fscanf(fp, "%s\n", c->lastName);
		fclose(fp);
	}
	
	return;
}


//determine race of a character
//takes a character, returns nothing
void rollRace(Character * c, Settings * s)
{
	int num = rollDice(1, 100, 0);
	
	if(strcmp("true", s->basic) == 0)
	{
		if(num < 50)
		{
			c->race = "Human";
			c->ageMod = 1;
		}
		else if(num < 68)
		{
			c->race = "Halfling";
			c->ageMod = 3.6;
		}
		else if(num < 74)
		{
			c->race = "Elf";
			c->ageMod = 10;
		}
		else if(num < 80)
		{
			c->race = "Half-Elf";
			c->ageMod = 2.5;
		}
		else if(num < 86)
		{
			c->race = "Half-Orc";
			c->ageMod = 1;
		}
		else if(num < 92)
		{
			c->race = "Dwarf";
			c->ageMod = 5;
		}
		else if(num < 98)
		{
			c->race = "Gnome";
			c->ageMod = 1;
		}
		else if(num < 99)
		{
			c->race = "Dragonborn";
			c->ageMod = 1;
		}
		else if(num < 100)
		{
			c->race = "Tiefling";
			c->ageMod = 1.2;
		}
		else
		{
			c->race = "Aasimar";
			c->ageMod = 1.2;
		}
	}
	else
	{
		if(num < 35)
		{
			c->race = "Human";
			c->ageMod = 1;
		}
		else if(num < 49)
		{
			c->race = "Halfling";
			c->ageMod = 3.5;
		}
		else if(num < 53)
		{
			c->race = "Elf";
			c->ageMod = 10;
		}
		else if(num < 59)
		{
			c->race = "Half-Elf";
			c->ageMod = 2.5;
		}
		else if(num < 64)
		{
			c->race = "Half-Orc";
			c->ageMod = 1;
		}
		else if(num < 69)
		{
			c->race = "Dwarf";
			c->ageMod = 5;
		}
		else if(num < 74)
		{
			c->race = "Gnome";
			c->ageMod = 7;
		}
		else if(num < 77)
		{
			c->race = "Dragonborn";
			c->ageMod = 1;
		}
		else if(num < 80)
		{
			c->race = "Tiefling";
			c->ageMod = 1.2;
		}
		else if(num < 83)
		{
			c->race = "Aasimar";
			c->ageMod = 1.2;
		}
		else if(num < 85)
		{
			c->race = "Goblin";
			c->ageMod = 0.7;
		}
		else if(num < 87)
		{
			c->race = "Hobgoblin";
			c->ageMod = 1;
		}
		else if(num < 89)
		{
			c->race = "Genasi";
			c->ageMod = 1.7;
		}
		else if(num < 91)
		{
			c->race = "Orc";
			c->ageMod = 0.7;
		}
		else if(num < 92)
		{
			c->race = "Bugbear";
			c->ageMod = 1;
		}
		else if(num < 93)
		{
			c->race = "Kalashtar";
			c->ageMod = 1;
		}
		else if(num < 94)
		{
			c->race = "Tortle";
			c->ageMod = 0.7;
		}
		else if(num < 95)
		{
			c->race = "Tabaxi";
			c->ageMod = 1;
		}
		else if(num < 96)
		{
			c->race = "Kenku";
			c->ageMod = 1;
		}
		else if(num < 97)
		{
			c->race = "Kobold";
			c->ageMod = 2;
		}
		else if(num < 98)
		{
			c->race = "Lizardfolk";
			c->ageMod = 1;
		}
		else if(num < 99)
		{
			c->race = "Triton";
			c->ageMod = 3;
		}
		else if(num < 100)
		{
			c->race = "Yuan-Ti";
			c->ageMod = 1;
		}
		else
		{
			c->race = "Warforged";
			c->ageMod = 0;
		}
	}
	return;
}

//determine age of a character
//takes a character, returns nothing
void rollAge(Character * c)
{
	switch(rollDice(2, 6, 0))
	{
		case 2:
			c->ageGroup = "Infant";
			c->age = rollDice(1, 4, 0) * c->ageMod;
			break;
		case 3:
			c->ageGroup = "Child";
			c->age = rollDice(1, 6, 4) * c->ageMod;
			break;
		case 4:
			c->ageGroup = "Teen";
			c->age = rollDice(1, 10, 10) * c->ageMod;
			break;
		case 5:
		case 6:
		case 7:
			c->ageGroup = "Adult";
			c->age = rollDice(2, 10, 20) * c->ageMod;
			break;
		case 8:
		case 9:
			c->ageGroup = "Middle Aged";
			c->age = rollDice(1, 20, 30) * c->ageMod;
			break;
		case 10:
		case 11:
			c->ageGroup = "Elderly";
			c->age = rollDice(2, 20, 50) * c->ageMod;
			break;
		case 12:
			c->ageGroup = "Ancient";
			c->age = rollDice(1, 12, 90) * c->ageMod;
			break;
	}
	
	return;
}

//determines class of a character
//takes a character, returns nothing
void rollClass(Character * c, Settings * s)
{
	
	if(rollDice(s->classNum, s->classSides, 0) == s->classSides && (c->age > 10*c->ageMod || c->age == 0))
	{
		switch(rollDice(1, 13, 0))
		{
			case 1:
				c->class = "Barbarian";
				break;
			case 2:
				c->class = "Bard";
				break;
			case 3:
				c->class = "Cleric";
				break;
			case 4:
				c->class = "Druid";
				break;
			case 5:
				c->class = "Fighter";
				break;
			case 6:
				c->class = "Monk";
				break;
			case 7:
				c->class = "Paladin";
				break;
			case 8:
				c->class = "Ranger";
				break;
			case 9:
				c->class = "Rogue";
				break;
			case 10:
				c->class = "Sorcerer";
				break;
			case 11:
				c->class = "Warlock";
				break;
			case 12:
				c->class = "Wizard";
				break;
			case 13:
				c->class = "Artificer";
				break;
		}
		
		c->level = rollDice(s->lvlNum, s->lvlSides, s->lvlMod);
	}
	else
	{
		c->class = "None";
		c->level = 0;
	}
	
	return;
}

//determine job for a character
//takes a character, returns nothing
void rollJob(Character * c)
{
	if((c->age > 10*c->ageMod && c->age < 91*c->ageMod) || c->age == 0)
	{
		switch(rollDice(1, 20, 0))
		{
			case 1 ... 4:
				c->job = "Farmer";
				break;
			case 5 ... 8:
				c->job = "Glassblower/Leatherworker/Woodcarver/Weaver";
				break;
			case 9 ... 11:
				c->job = "Tutor/Teacher/Academic/Nanny";
				break;
			case 12 ... 13:
				c->job = "Animal Handler/Shepherd";
				break;
			case 14 ... 15:
				c->job = "Resturant/Tavern Worker/Owner";
				break;
			case 16:
				c->job = "Minstrel/Storyteller/Jester";
				break;
			case 17:
				c->job = "Builder";
				break;
			case 18:
				c->job = "Blacksmith";
				break;
			case 19:
				c->job = "Cook";
				break;
			case 20:
				c->job = "Diviner/Healer";
				break;
		}
	}
	else
		c->job = "None";
}

//determine talent of a character
//takes a character, returns nothing
void rollTalent(Character * c)
{
	if(c->age > 4 * c->ageMod || c->age == 0)
	{
		switch(rollDice(1, 20, 0))
		{
			case 1:
				c->talent = "Plays an Instrument";
				break;
			case 2:
				c->talent = "Multilingual";
				break;
			case 3:
				c->talent = "Super Lucky";
				break;
			case 4:
				c->talent = "Perfect Memory";
				break;
			case 5:
				c->talent = "Great with Animals";
				break;
			case 6:
				c->talent = "Great with Kids";
				break;
			case 7:
				c->talent = "Great at Puzzles";
				break;
			case 8:
				c->talent = "Great at a specific game";
				break;
			case 9:
				c->talent = "Great at impersonations";
				break;
			case 10:
				c->talent = "Draws";
				break;
			case 11:
				c->talent = "Paints";
				break;
			case 12:
				c->talent = "Sings";
				break;
			case 13:
				c->talent = "Drinks others under the table";
				break;
			case 14:
				c->talent = "Expert Carpenter";
				break;
			case 15:
				c->talent = "Expert Cook";
				break;
			case 16:
				c->talent = "Expert Darts/Rock Skipper";
				break;
			case 17:
				c->talent = "Expert Juggler";
				break;
			case 18:
				c->talent = "Actor & Master of Disguise";
				break;
			case 19:
				c->talent = "Dancer";
				break;
			case 20:
				c->talent = "Knows Thieves' Cant";
				break;
		}
	}
	else
		c->talent = "None";
	
	return;
}

//determine best/worst stats of a character
//takes a character, returns nothing
void rollStats(Character * c)
{
	int num1 = rollDice(1, 6, 0), num2;
	do {
		num2 = rollDice(1, 6, 0);
	} while (num1 == num2);
	
	switch(num1)
	{
		case 1:
			c->best = "Str";
			break;
		case 2:
			c->best = "Dex";
			break;
		case 3:
			c->best = "Con";
			break;
		case 4:
			c->best = "Int";
			break;
		case 5:
			c->best = "Wis";
			break;
		case 6:
			c->best = "Cha";
			break;
	}
	
	switch(num2)
	{
		case 1:
			c->worst = "Str";
			break;
		case 2:
			c->worst = "Dex";
			break;
		case 3:
			c->worst = "Con";
			break;
		case 4:
			c->worst = "Int";
			break;
		case 5:
			c->worst = "Wis";
			break;
		case 6:
			c->worst = "Cha";
			break;
	}
}

//determines appearance of a character
//takes a character, returns nothing
void rollAppearance(Character * c)
{
	if(c->age > 10 * c->ageMod || c->age == 0)
	{
		switch(rollDice(1, 20, 0))
		{
			case 1:
				c->appearance = "Unique Jewelery";
				break;
			case 2:
				c->appearance = "Piercings";
				break;
			case 3:
				c->appearance = "Flamboyant";
				break;
			case 4:
				c->appearance = "Well-Dressed";
				break;
			case 5:
				c->appearance = "Poorly-Dressed";
				break;
			case 6:
				c->appearance = "Scars";
				break;
			case 7:
				c->appearance = "Missing/False Teeth";
				break;
			case 8:
				c->appearance = "Missing/Prosthetic Left/Right Arm/Leg";
				break;
			case 9:
				c->appearance = "Unusual Eye Color";
				break;
			case 10:
				c->appearance = "Tattoos";
				break;
			case 11:
				c->appearance = "Moles/Freckles/Birthmark";
				break;
			case 12:
				c->appearance = "Unusual Skin Color";
				break;
			case 13:
				c->appearance = "Bald";
				break;
			case 14:
				c->appearance = "Braids";
				break;
			case 15:
				c->appearance = "Unusual Hair Color";
				break;
			case 16:
				c->appearance = "Eye Twitch";
				break;
			case 17:
				c->appearance = "Distinctive Nose";
				break;
			case 18:
				c->appearance = "Distinctive Posture";
				break;
			case 19:
				c->appearance = "Beautiful";
				break;
			case 20:
				c->appearance = "Ugly";
				break;
		}
	}
	else
		c->appearance = "None";
	
	return;
}

//determine traits for a character
//takes a character, returns nothing
void rollTraits(Character * c)
{
	if(c->age > 4 || c->age == 0)
	{
		switch(rollDice(1, 12, 0))
		{
			case 1:
				c->interaction = "Argumentative";
				break;
			case 2:
				c->interaction = "Arrogant";
				break;
			case 3:
				c->interaction = "Blustering";
				break;
			case 4:
				c->interaction = "Rude";
				break;
			case 5:
				c->interaction = "Curious";
				break;
			case 6:
				c->interaction = "Friendly";
				break;
			case 7:
				c->interaction = "Honest";
				break;
			case 8:
				c->interaction = "Hot-Tempered";
				break;
			case 9:
				c->interaction = "Irritable";
				break;
			case 10:
				c->interaction = "Ponderous";
				break;
			case 11:
				c->interaction = "Quiet";
				break;
			case 12:
				c->interaction = "Suspicious";
				break;
		}
		
		if(c->age > 10 || c->age ==0)
		{
			int num1 = rollDice(1, 12, 0), num2;
			do {
				num2 = rollDice(1, 12, 0);
			} while (num1 == num2);
			
			switch(num1)
			{
				case 1:
				c->posTrait = "Immune to Sexual/Romantic Charms";
				break;
			case 2:
				c->posTrait = "Non-Indulgent";
				break;
			case 3:
				c->posTrait = "Humble";
				break;
			case 4:
				c->posTrait = "Supports other in higher positions";
				break;
			case 5:
				c->posTrait = "Charitable";
				break;
			case 6:
				c->posTrait = "Unwavering Calm";
				break;
			case 7:
				c->posTrait = "Close Best Friend";
				break;
			case 8:
				c->posTrait = "Brave";
				break;
			case 9:
				c->posTrait = "Reputable Past";
				break;
			case 10:
				c->posTrait = "Great Kindness";
				break;
			case 11:
				c->posTrait = "Great Teacher/Mentor";
				break;
			case 12:
				c->posTrait = "Calculating";
				break;
			}
			
			switch(num2)
			{
				case 1:
				c->negTrait = "Forbidden Love/Susceptible to Romance";
				break;
			case 2:
				c->negTrait = "Enjoys Decadent Pleasures";
				break;
			case 3:
				c->negTrait = "Arrogant";
				break;
			case 4:
				c->negTrait = "Envious";
				break;
			case 5:
				c->negTrait = "Greedy";
				break;
			case 6:
				c->negTrait = "Prone to Rage";
				break;
			case 7:
				c->negTrait = "Powerful Enemy/Rival";
				break;
			case 8:
				c->negTrait = "Phobia of Dark/Spiders/Small Spaces";
				break;
			case 9:
				c->negTrait = "Shameful/Scandalous Past";
				break;
			case 10:
				c->negTrait = "Secret Crime/Misdeed";
				break;
			case 11:
				c->negTrait = "Possesses Forbidden Lore";
				break;
			case 12:
				c->negTrait = "Foolish";
				break;
			}
		}
		else
		{
			c->posTrait = "None";
			c->negTrait = "None";
		}
	}
	else
	{
		c->posTrait = "None";
		c->negTrait = "None";
		c->interaction = "None";
	}
}

//compares two character's first names, for sorting
//takes two characters, returns positve or negative depending on which is higher
int comparator(const void * a, const void * b)
{
	return strcmp(((Character *)a)->firstName, ((Character *)b)->firstName);
}

//export character information to a file
//takes a character array and an int for the size, returns nothing
void export(Character * arr, int total, Settings * s)
{
	int i;
	FILE *fp;
	char * gender = "", * fname = s->filename;
	sprintf(fname, "%s.csv", fname);
	if((fp = fopen(fname, "w")) == NULL)
		printf("*** Failed to create output file... ***\n");
	else
	{
		fprintf(fp, "No.,First,Last,Gender,Race,Age Group,Age,Class,Lvl,Job,Appearance,Interaction,Positive Trait,Negative Trait,Talent,Best,Worst,Notes\n");
		for(i = 0; i < total; i++)
		{
			if(arr[i].gender == 1)
				gender = "Male";
			else if(arr[i].gender == 2)
				gender = "Female";
			else
				gender = "NB";
			if(strcmp(arr[i].race, "Hybrid") == 0)
				fprintf(fp, "%d,%s,%s,%s,%s - %s/%s,%s,%d,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s\n", i + 1, arr[i].firstName, arr[i].lastName,gender,arr[i].race, arr[i].hybridRaceA,
					arr[i].hybridRaceB, arr[i].ageGroup,arr[i].age,arr[i].class,arr[i].level,arr[i].job,
					arr[i].appearance,arr[i].interaction,arr[i].posTrait,arr[i].negTrait,arr[i].talent,arr[i].best,arr[i].worst);
			else
				fprintf(fp, "%d,%s,%s,%s,%s,%s,%d,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s\n", i + 1, arr[i].firstName, arr[i].lastName,gender,arr[i].race,
					arr[i].ageGroup,arr[i].age,arr[i].class,arr[i].level,arr[i].job,arr[i].appearance,
					arr[i].interaction,arr[i].posTrait,arr[i].negTrait,arr[i].talent,arr[i].best,arr[i].worst);
				//num,first,last,gender,race,group,age,class,lvl,job,appearance,interaction,pos,neg,talent,best,worst
		}
		fclose(fp);
		printf("Successfully created %s\n", fname);
	}
	return;
}

//main
int main(void)
{
	//random seed
	srand(time(0));
	
	printf("DnD Settlement Generator - Kyler Froman\n\n");
	printf("Loading Settings, if the program crashes after this try deleting settings.txt and running it again.\n");
	system("pause");
	Settings * settings = loadSettings();
	
	int i, total = settings->amount;	
	Character * townfolk = malloc(sizeof(Character) * total);
	
	for(i = 0; i < total; i++)
	{
		rollGender(&townfolk[i], settings);
		rollName(&townfolk[i], settings);
		rollRace(&townfolk[i], settings);
		rollAge(&townfolk[i]);
		rollClass(&townfolk[i], settings);
		rollJob(&townfolk[i]);
		rollTalent(&townfolk[i]);
		rollStats(&townfolk[i]);
		rollAppearance(&townfolk[i]);
		rollTraits(&townfolk[i]);
	}
	
	qsort(townfolk, total, sizeof(Character), comparator);

	export(townfolk, total, settings);
	system("pause");
	
	free(townfolk);
	free(settings);
	
	return 0;
}