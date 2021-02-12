#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node
{
	char kelime[30];
	char anlami[500];
	struct Node *next;
}Node;


_Bool ilk_okumami = 1;


void insert_beginning(Node **root, char girdi_kelime[], char girdi_anlami[] )
{
    Node *new_node = malloc(sizeof(Node));
    if(new_node == NULL)
    {
        exit(2);
    }

    //new_node->x = value;
    strcpy(new_node->kelime, girdi_kelime);
    strcpy(new_node->anlami, girdi_anlami);
    new_node->next = *root;
    *root = new_node;
}

void insert_after(Node *node, char girdi_kelime[], char girdi_anlami[])
{

	Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL)
    {
    	exit(4);
    }

    //new_node->x = value;
    strcpy(new_node->kelime, girdi_kelime);
    strcpy(new_node->anlami, girdi_anlami);
    new_node->next = node->next;
    node->next = new_node;

}

void insert_sorted(Node **root, char girdi_kelime[], char girdi_anlami[])
{

    if(*root == NULL || (strcmp((**root).kelime, girdi_kelime) > 0) )
    {
        insert_beginning(root, girdi_kelime, girdi_anlami);
        return;
    }

    Node *curr = *root;
    while (curr->next != NULL) // iteration koşulu
    {	//if(curr->next->x >= value)
        if(strcmp(curr->next->kelime, girdi_kelime) > 0)
        {
            insert_after(curr, girdi_kelime, girdi_anlami);//şimdiki pozisyona değeri ekle
            return;
        }
        curr = curr->next;
    }

    insert_after(curr, girdi_kelime, girdi_anlami);
    //mesela 3 ve 6 yı ekledim. 7 ekleyecek olsam
    //eğer üstteki satırdaki kod olmazsa 7 yi eklemez. Çünkü yukarıdaki
    //döngü, benden sonra büyük var mı diye baktı ama bulamadı. Dolayısıyla
    // " | " şeklinde olan imleç en sonra geldi ve orada kaldı. Bundan
    //dolayı bize düşen sadece imlecin olduğu pozisyona değeri eklemek
}

char *dosyadan_satir_oku(char *dosya_adi)
{
	FILE *my_stream;
	char *my_string = NULL;
	size_t nbytes = 100;
	size_t read;
	static int cursor_position = 0;

	my_stream = fopen(dosya_adi, "r");

	if(ilk_okumami == 1)
	{
		fseek(my_stream, cursor_position, SEEK_SET);
		my_string = (char *)malloc(nbytes);
	    read = getline(&my_string, &nbytes, my_stream);
	    cursor_position = (int)read;
	    ilk_okumami = 0;

	}

	else
	{	//imlec pozizyonuna kaldıgı yerden okumaya devam et
		fseek(my_stream, cursor_position, SEEK_SET);
		my_string = (char *)malloc(nbytes);
		read = getline(&my_string, &nbytes, my_stream);
		cursor_position += (int)read; //yeni imlec pozisyonu
	}


	if(read != -1) //dosyanin sonuna ulasmadıgın surece
	{
		return my_string;

	}
	else
	{
		cursor_position = 0;
		return "x";
	}

	fclose(my_stream);
	free(my_string);

}

void dosyaya_yazdir(char *kelime, char *anlami, char *dosya_adi)
{
	FILE *my_stream2;
	my_stream2 = fopen(dosya_adi, "a");
	fprintf(my_stream2, "%s:%s", kelime, anlami);
	fclose(my_stream2);

}



int main()
{

	Node *root = NULL;

	int i = 0;
	int j;
	while(strcmp(dosyadan_satir_oku("input.txt"), "x")) //dosyanın içinde kaç satır var
	{
		i++;
	}

	for(j = 0; j < i; j++) //satırları gez
	{
		char str[700];
		strcpy(str, dosyadan_satir_oku("input.txt"));
		char *token;
		char *rest = str;
		char dizi[2][700];
		int i = 0;
		while ((token = strtok_r(rest, ":", &rest)))
			{
				strcpy(dizi[i], token);
				i++;
			}

		insert_sorted(&root, dizi[0], dizi[1]);
	}


	for(Node *curr = root; curr != NULL; curr = curr->next) // Node'ları gez
		{
			printf("%s: %s", curr->kelime, curr->anlami);
			dosyaya_yazdir(curr->kelime, curr->anlami, "output.txt");

		}

}
