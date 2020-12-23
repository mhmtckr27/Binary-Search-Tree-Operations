#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node * tree;
typedef struct node{
	int id;
	char *nameSurname;
	int * friends;
	tree left, right;
}node;

tree topRoot = NULL;
static int userCount = 0;
static int deleteFlag = 0;

#define LINESIZE 100
#define FRIENDSIZE 50
#define IDLENGTH 3
#define NAMELENGTH 32
#define FILENAMELENGTH 100

#define RED   "\x1B[31;1m"	//kirmizi yazi
#define GRN   "\x1B[32;1m"	//yesil	
#define YEL   "\x1B[33;1m"	//sari
#define BLU   "\x1B[34;1m"	//mavi
#define MAG   "\x1B[35;1m"	//magenta
#define CYN   "\x1B[36;1m"	//cyan
#define WHT   "\x1B[37;1m"	//beyaz
#define GRY "\x1B[30;1m"	//gri

void createNewUser2(FILE* fp);
tree createNewUser(char* id, char* name, char* friends);
void tokenizeFriends(tree newUser, char* friends);
tree insertNewUser(tree node, tree userToInsert);
void printNode(tree node);
void printInOrder(tree node);
void printInOrderConditional(tree node, int id);
void printGreater(tree node, int id);
void printNext(tree node, int id);
tree contains(int id, tree node);
void friends(int id, tree node);
tree minValueNode(tree node);
tree deleteUser(tree node, int id);
void Start();


void createNewUser2(FILE* fp)
{
	/*asagidaki degiskenlerin boyutunu sabit yapma sebebim, fonksiyon sonlaninca yani her
	kullanici eklendikce zaten silinecekler, her degisken icin malloc cagirma maliyetinden kacindim.*/

	char tmpBuf[LINESIZE];	//Dosyadan 1 satir almak icin
	char tmpBuf2[LINESIZE];	//tmpBuf, strtok fonksiyonu kullaninca bozulacagi icin once buna kopyaliyorum.
	char id[IDLENGTH];		//kullanici id'sini tutmak icin
	char name[NAMELENGTH];	//kullanici ad-soyadini tutmak icin
	char friends[FRIENDSIZE];//arkadas dizisini tutmak icin
	tree user;				//yeni kullanici olusturuluyor.
	if (fgets(tmpBuf, LINESIZE, fp) == NULL) { printf(RED"End of file!\n"); return; }
	if (!strchr(tmpBuf, ','))
	{
		printf(RED"Empty line!");
		return;
	}
	strcpy(tmpBuf2, tmpBuf);
	if (strchr(tmpBuf, ',') == (strrchr(tmpBuf, ',')))//arkadasi yoksa
	{
		strcpy(id, strtok(tmpBuf, ","));	//id'yi ceker.
		strcpy(name, strtok(NULL, "\n"));	//ad-soyadi ceker.
		user = createNewUser(id, name, NULL);
		user->left = NULL;
		user->right = NULL;
		user->friends = (int*)malloc(sizeof(int));
		*user->friends = -111;
		insertNewUser(topRoot, user);
	}
	else									//arkadasi varsa
	{
		strcpy(id, strtok(tmpBuf, ","));	//id'yi ceker.
		strcpy(name, strtok(NULL, ","));	//ad soyad ceker
		strcpy(friends, strtok(NULL, "\n"));//arkadas dizisini ceker
		user = createNewUser(id, name, friends);
		user->left = NULL;
		user->right = NULL;
		insertNewUser(topRoot, user);
	}
	++userCount;
}

tree createNewUser(char* id, char* name, char* friends)
{
	tree newUser = (tree)malloc(sizeof(node));	//yeni kullanici icin yer ayriliyor.
	newUser->id = atoi(id);
	newUser->nameSurname = (char*)malloc(strlen(name) + 1);
	strcpy(newUser->nameSurname, name);
	if (friends != NULL) { tokenizeFriends(newUser, friends); }
	newUser->right = NULL;
	newUser->left = NULL;
	printf(GRN"User added with id (%s) name (%s)\n", id, name);
	return newUser;
}

void tokenizeFriends(tree newUser,char* friends)
{
	int friendCounter = 0;	//arkadas sayisini tutmak icin
	int j = 0;				//1.while dongusunun degiskeni
	int i = 0;				//2.while dongusunun degiskeni
	char* token = (char*)malloc(IDLENGTH+1);	//arkadas idlerini tokenize edip bu degiskene atayacagim.
	if (friends != NULL) { friendCounter = 1; }
	else return;
	while (j < (strlen(friends)-1))			//kac arkadasi oldugunu - karakteri sayisina gore bulur
	{
		char ch = *(friends + j);
		if (ch == '-')
		{
			++friendCounter;
		}
		++j;
	}
	newUser->friends = (int*)malloc(sizeof(int)*(friendCounter + 1));
	strcpy(token, strtok(friends, "-"));
	*(newUser->friends) = atoi(token);
	++i;
	while (i < friendCounter - 1)
	{
		strcpy(token, strtok(NULL, "-"));

		*(newUser->friends + i) = atoi(token);
		++i;
	}
	strcpy(token, strtok(NULL, "\0"));
	*(newUser->friends + i) = atoi(token);
	*(newUser->friends + i + 1) = -111;
}

tree insertNewUser(tree node,tree userToInsert)
{
	if (userToInsert == NULL) { userToInsert = createNewUser(NULL,NULL,NULL); }
	if (topRoot == NULL) { topRoot = userToInsert; return userToInsert; }
	if (node == NULL) { return userToInsert; }
	if ((userToInsert->id) < (node->id))
	{
		if (node->left != NULL) { node->left = insertNewUser(node->left, userToInsert); }
		else node->left = userToInsert;
	}
	else if ((userToInsert->id) > (node->id))
	{
		if (node->right != NULL) { node->right = insertNewUser(node->right, userToInsert); }
		else node->right = userToInsert;
	}
	return node;
}

void printNode(tree node)
{
	printf(CYN"ID number: %d\n", node->id);
	printf("Name-Surname: %s\n\n", node->nameSurname);
}

void printInOrder(tree node)
{
	if (node == NULL) { return; }
	printInOrder(node->left);
	printNode(node);
	printInOrder(node->right);
}

void printInOrderConditional(tree node,int id)
{
	if (node == NULL) { return; }
	printInOrderConditional(node->left, id);
	if (node->id > id) { printNode(node); }
	printInOrderConditional(node->right, id);
}

void printGreater(tree node,int id)
{
	tree tmp = contains(id,node);
	if (tmp == NULL) { printf(RED"Could not find user with given id! (%d)\n", id); return; }
	printInOrderConditional(topRoot,tmp->id);
}

void printNext(tree node, int id)
{
	tree tmp = contains(id, node);
	if (tmp == NULL) { printf(RED"Could not find user with given id! (%d)\n", id); return; }
	printInOrder(tmp);
}

tree contains(int id, tree node)
{
	if (node == NULL || node->id == id) { return node; }
	//printf(YEL"Searching for: "RED"%d != %d"CYN"\t:Currently found\n", id, node->id); 		//Contains testi icin yazildi, kullanilmasi durumunda bastaki // ifadesini siliniz.
	if (node->id < id) { return contains(id, node->right); }
	return contains(id, node->left);
}

void friends(int id, tree node)
{
	int i = 0;	//dongu degiskeni
	int j = 0;	//dongu degiskeni
	tree tmpnode = NULL;
	node = contains(id, node);
	if (node == NULL)
	{
		printf(RED"Could not find user with given id! (%d)\n", id);
		return;
	}
	if (*(node->friends) == -111) { printf(GRN"Does not have any friends :(\n"); return; }
	printf(YEL"Printing friends in tree..\n");
	while (*(node->friends + i) != -111)
	{
		char* nameSurname;
		if ((tmpnode = contains(*(node->friends + i), topRoot)) != NULL)
		{
			++j;
			printf(CYN"%d.Friend: ", j);
			printf("%s\n", tmpnode->nameSurname);
		}
		++i;
	}
}

tree minValueNode(tree node)
{
	tree tmp = node;
	while ((tmp->left) != NULL) { tmp = tmp->left; }
	return tmp;
}

tree deleteUser(tree node,int id)
{
	if (topRoot == NULL)
	{
		printf(RED"Empty tree!\n");
		return NULL;
	}
	if (node == NULL)
	{
		printf(RED"Could not find the user with given id! (%d)\n",id);
		return NULL;
	}
	if (id < node->id) { node->left = deleteUser(node->left, id); }
	else if (id > node->id) { node->right = deleteUser(node->right, id); }
	else
	{
		//cocuk yoksa ya da 1 cocuk varsa
		tree tmp;
		if (node->left == NULL)
		{
			tmp = node->right;
			free(node->nameSurname);
			free(node->friends);
			free(node);
			--userCount;
			deleteFlag = 1;
			return tmp;
		}
		if (node->right == NULL)
		{
			tmp = node->left;
			free(node->nameSurname);
			free(node->friends);
			free(node);
			--userCount;
			deleteFlag = 1;
			return tmp;
		}
		//iki cocuk varsa
		tmp = minValueNode(node->right);
		node->id = tmp->id;
		strcpy(node->nameSurname ,tmp->nameSurname);
		node->friends = tmp->friends;
		node->right = deleteUser(node->right, tmp->id);
		deleteFlag = 1;
	}
	return node;
}

void Start()
{
	int opCode;			//islem kodu
	char fileName[FILENAMELENGTH];	//dosya adi	
	FILE* fp;			//dosya pointerı
	tree newUser;		//containsden donen degeri atamak icin.
	system("COLOR 85");
	printf(WHT"Enter file name to create tree: "YEL);
	scanf("%s", fileName);
	fp = fopen(fileName, "r");
	printf(GRN"0 -> Exit\n1 -> insertNewUser\n2 -> deleteUser\n3 -> contains\n4 -> friends\n5 -> size\n6 -> printNext\n7 -> printGreater\n8 -> printInOrder\n\n");
	do
	{
		int id;					//kullanici idsi, case 2-3-4-6-7 ' de kullanicidan almak icin.
		printf(WHT"\n->Select an operation: "YEL);
		scanf("%d", &opCode);
		printf("\n");
		switch(opCode)
		{
		case 0:
			return;
		case 1:
			createNewUser2(fp);
			break;
		case 2:
			printf(WHT"Enter an id to delete user: "YEL);
			scanf("%d", &id);
			deleteFlag = 0;
			topRoot = deleteUser(topRoot, id);
			if (deleteFlag) { printf(GRN"User deleted with given id (%d)\n", id); }
			break;
		case 3:
			printf(YEL"--------------------------------------------------------------------------------\n");
			printf(WHT"Enter an id to find user: ");
			scanf("%d", &id);
			if ((newUser = contains(id, topRoot)) != NULL)
			{
				//printf(YEL"Searching for: "GRN"%d != %d"CYN" :Currently found\n", id, id);					//Contains testi icin yazildi, kullanilmasi durumunda bastaki // ifadesini siliniz.
				printf(GRN"User found with given id (%d): %s\n", newUser->id, newUser->nameSurname);
			}
			else { printf(RED"Could not find user with given id (%d)!\n", id); }
			printf(YEL"--------------------------------------------------------------------------------\n");
			break;
		case 4:
			printf(YEL"--------------------------------------------------------------------------------\n");
			printf(WHT"Enter an id to print friends: "YEL);
			scanf("%d", &id);
			friends(id, topRoot);
			printf(YEL"--------------------------------------------------------------------------------\n");
			break;
		case 5:
			printf(GRN"User #: %d\n", userCount);
			break;
		case 6:
			printf(WHT"Enter an id: "YEL);
			scanf("%d", &id);
			printf(YEL"Printing next...\n" YEL "--------------------------------------------------------------------------------\n");
			printNext(topRoot, id);
			printf(YEL"--------------------------------------------------------------------------------\n");
			break;
		case 7:
			printf(WHT"Enter an id: "YEL);
			scanf("%d", &id);
			printf(YEL"Printing greater...\n" YEL "--------------------------------------------------------------------------------\n");
			printGreater(topRoot, id);
			printf(YEL"--------------------------------------------------------------------------------\n");
			break;
		case 8:
			printf(YEL"Printing in order...\n" YEL "--------------------------------------------------------------------------------\n");
			printInOrder(topRoot);
			printf(YEL"--------------------------------------------------------------------------------\n");
			break;
		default:
			printf(RED"Undefined operation code!\n");
			break;
		}
	} while (opCode);
	fclose(fp);
}

int main()
{
	Start();
	system("PAUSE");
	return 0;
}