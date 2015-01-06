#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define _DEBUG_MODE_
#define START_ENCODE_POS    4

bool* bitEncode;
int bitLen;
char filename1[50];
char filename2[50];
const char tableBinaryConv[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
char huffSimbol[10];
FILE* jj;
FILE* kk;


struct htree
{
    char symbol[10];
    int frekuensi;
    struct htree* right;
    struct htree* left;
};

//-----------
struct huffData
{
	char symbol[10];
	bool encode[32];
	int len;
	int frek;
};

struct huffTable
{
	huffData huff;
	struct huffTable* next;
};

struct listHuff
{
	struct huffTable* first;
};
//-----------

struct htree_list
{
	struct htree* branch;
	struct htree_list* next;
};

struct htlist_head
{
	struct htree_list* first;
};
//-------------

void resetEncodeTable();
void addEncodeTable(bool val);

void huffTableCreate(htree* node, listHuff* hf);
void huffTableRecurPath(htree* node, listHuff* hf, bool dir, bool path[], int pathLen);
void huffTableAdd(listHuff* hf,char symbol[], bool path[], int pathLen);
void huffTablePrint(listHuff* hf);
//
//void huffTreeCreateFromBit(htree* node,bool path[32],char pathlen,char sim[10]);
void decodeTable(const char* filename,const char* filenameKey);
void printBit(char a);

void add_tree_list(htlist_head* L,char symbol[6],int frek);
void add_tree_list(htlist_head* L,htree* t_tree);
void sort_tree_list(htlist_head* L);
void del_htlist_front(htlist_head* L);
void create_tree(htlist_head* L);


int main(int argc,char **argv)
{

    if (argc!=3){
        printf("nama file encoded  :");scanf("%s",&filename1);
        printf("nama file key :");scanf("%s",&filename2);

    }else
    {
        strcpy(filename1,argv[1]);
        strcpy(filename2,argv[2]);

    }

    decodeTable(filename1,filename2);
    printf("\nauaua\n");
    return 0;
}

void huffTableCreate(htree* node, listHuff* hf)
{
    bool path[100];
    huffTableRecurPath(node->left, hf,0,path,0);
    huffTableRecurPath(node->right, hf,1,path,0);
}
void huffTableRecurPath(htree* node, listHuff* hf, bool dir, bool path[], int pathLen)
{
    if (node==NULL)
        return;
    int i;
  /* append this node to the path array */
    path[pathLen]=dir;
    pathLen++;

//    printf("show path %d\n",node->frekuensi);
  /* it's a leaf, so print the path that led to here  */
    if (node->left==NULL && node->right==NULL)
    {
        huffTableAdd(hf,node->symbol,path,pathLen);
    }
    else
    {
    /* otherwise try both subtrees */

            huffTableRecurPath(node->left ,hf,0,path,pathLen);
            huffTableRecurPath(node->right,hf,1,path,pathLen);
    }
}
void huffTableAdd(listHuff* hf,char symbol[], bool path[], int pathLen)
{
    int i;
    huffTable* hh=(huffTable*) malloc(sizeof(huffTable));
    hh->next=NULL;
    hh->huff.len=pathLen;
    strcpy(hh->huff.symbol,symbol);

    for (i=0; i<pathLen; i++)
    {
        hh->huff.encode[i]=path[i];
    }

    if(hf->first==NULL)
    {
        hf->first=hh;
    }else
    {
        huffTable* last_symbol=hf->first;
        while(last_symbol->next!=NULL)
        {
            last_symbol=last_symbol->next;
        }
        last_symbol->next=hh;
    }
    hh=NULL;
}
void huffTablePrint(listHuff* hf)
{
    int i;int j=0;
    int countTotalBit=0;
    huffTable* a=hf->first;
    while(a!=NULL)
    {
        j++;
        printf("%d ",j);
        for(i=0;i<a->huff.len;i++)
        {
            printf("%d ",a->huff.encode[i]);
        }
        printf("%s\n",a->huff.symbol);
        a=a->next;
    }
}

void resetEncodeTable()
{
    bitLen=0;
}

void addEncodeTable(bool val)
{
    bitEncode[bitLen]=val;
    bitLen++;
}

void add_tree_list(htlist_head* L,char symbol[6],int frek)
{

    htree_list* hlist=(htree_list*) malloc(sizeof(htree_list));
    htree*  t_branch=(htree*) malloc(sizeof(htree));

	strcpy(t_branch->symbol,symbol);
//	printf("add symbol %s\n",t_branch->symbol);
	t_branch->frekuensi=frek;
	t_branch->left=NULL;
	t_branch->right=NULL;
	hlist->branch=t_branch;
	hlist->next=NULL;

	if(L->first==NULL)
    {
        L->first=hlist;
    }else
    {
        htree_list* last_symbol=L->first;
        while(last_symbol->next!=NULL)
        {
            last_symbol=last_symbol->next;
        }
        last_symbol->next=hlist;
    }
    t_branch=NULL;
	hlist=NULL;
}

void add_tree_list(htlist_head* L,htree* t_tree)
{
    htree_list* hlist=(htree_list*) malloc(sizeof(htree_list));

	hlist->branch=t_tree;
	hlist->next=NULL;

	if(L->first==NULL)
    {
        L->first=hlist;
    }else
    {
        htree_list* last_symbol=L->first;
        while(last_symbol->next!=NULL)
        {
            last_symbol=last_symbol->next;
        }
        last_symbol->next=hlist;
    }

	hlist=NULL;
}

void sort_tree_list(htlist_head* L)
{

    if(L->first!=NULL)
    {
        htree_list* l1=L->first;
        htree_list* l2=NULL;
        while(l1->next!=NULL)
        {
            l2=l1->next;
            while(l2!=NULL)
            {
                if(l1->branch->frekuensi > l2->branch->frekuensi)
                {
                    htree* t_branch=l1->branch;
                    l1->branch=l2->branch;
                    l2->branch=t_branch;
                }
                l2=l2->next;
            }
            l1=l1->next;
        }

    }else
    {
        printf("list NULL\n");
    }
}

void del_htlist_front(htlist_head* L)
{
    if(L->first!=NULL)
    {
        htree_list* t_l=L->first;
        t_l->branch=NULL;
        L->first=L->first->next;
        t_l->next=NULL;
        free(t_l);
    }
}
void create_tree(struct htlist_head* L)
{
    if(L->first!=NULL)
    {
        while(L->first->next!=NULL)
        {
            sort_tree_list(L);
            htree* b=(htree*) malloc(sizeof(htree));
            b->left=L->first->branch;
            b->right=L->first->next->branch;
            b->frekuensi=b->left->frekuensi + b->right->frekuensi;
            add_tree_list(L,b);
            del_htlist_front(L);
            del_htlist_front(L);
//            printf("branch created %d\n",b->frekuensi);
        }

    }else
    {
        printf("list NULL\n");
    }
}

void decodeTable(const char* filename,const char* filenameKey)
{
    int keycount=0,encodecount=0;
    int i=0,j=0,x=0;
//    int maxi;
    bool whatever;
//    char b=0;
//    char c[4];

//    bool path[32];

    char fileOutputName[50];
    int symbolLen,keyFrek,fileLen,fileLenEncode;
    int dataKeyPos;
    char* bufferFileKey;
    char* bufferFileEnc;

    jj=fopen(filename,"rb");
    kk=fopen(filenameKey,"rb");
    if(jj==NULL) return;
    if(kk==NULL) return;

    printf("file output name :");scanf("%s",&fileOutputName);
    FILE* endOfMisery=fopen(fileOutputName,"w");

    fscanf(kk,"%d",&keycount);
    fscanf(kk,"%d",&encodecount);
    #ifdef _DEBUG_MODE_
    printf("%d %d\n",keycount,encodecount);
    #endif // _DEBUG_MODE_
    dataKeyPos=ftell(kk)+1;

    fseek(kk,0,SEEK_END);
    fileLen=ftell(kk);
    bufferFileKey=(char*)malloc(sizeof(char)*(fileLen+1));
    fseek(kk,0,SEEK_SET);
    fread(bufferFileKey,1,fileLen,kk);

    fseek(jj,0,SEEK_END);
    fileLenEncode=ftell(jj);
    bufferFileEnc=(char*) malloc(sizeof(char)*(fileLenEncode+1));
    fseek(jj,0,SEEK_SET);
    fread(bufferFileEnc,1,fileLenEncode,jj);

    bitEncode=(bool*) malloc(encodecount*8+1);
    resetEncodeTable();

    fclose(jj);
    fclose(kk);

    htree* itsrandom;
    listHuff anorandom;
    anorandom.first=NULL;
    htlist_head* akarpohon=(htlist_head*)malloc(sizeof(htlist_head));
    akarpohon->first=NULL;

//process key file
    FILE* notimportant=fopen("TREE_reconstruct_debug.txt","w");
    i=dataKeyPos;
    while(i<fileLen)
    {
        symbolLen=bufferFileKey[i];
        i++;
//        printf("%d len %d ",x,symbolLen);
        strcpy(huffSimbol,"");//printf("%d ",symbolLen);
        for(j=0;j<symbolLen;j++)
        {
            huffSimbol[j]=bufferFileKey[i];
//            printf("%c",bufferFileKey[i]);
            if(bufferFileKey[i]=='\r')
            {
                if(bufferFileKey[i+1]=='\n')
                {
//                    printf("\nnewline\n");
                    huffSimbol[j]='\n';
                    i++;
                }
            }
            i++;
        }
        huffSimbol[symbolLen]='\0';//        printf("%s ",huffSimbol);
        keyFrek=bufferFileKey[i];  //        printf("%d\n",keyFrek);
        i++;
        //add tree list
        add_tree_list(akarpohon,huffSimbol,keyFrek);

        x++;
    }
    fprintf(notimportant,"{list key}\n");
    //create tree
    create_tree(akarpohon);
    itsrandom=akarpohon->first->branch;
    huffTableCreate(itsrandom,&anorandom);
    huffTablePrint(&anorandom);

    huffTable* a=anorandom.first;
    while(a!=NULL)
    {
        for(i=0;i<a->huff.len;i++)
        {
            fprintf(notimportant,"%d",a->huff.encode[i]);
        }
        fprintf(notimportant," %s\n",a->huff.symbol);
        a=a->next;
    }


    printf("finished create tree from key\n");
//finished process key

//conversion from char into bit by bit
    i=START_ENCODE_POS;
    while(i<fileLenEncode)
    {
        for(j=0;j<8;j++)
        {
            whatever=((bufferFileEnc[i]&tableBinaryConv[j])&&tableBinaryConv[j]);
            addEncodeTable(whatever);
        }
        i++;
    }
    //-------------------------
    for(i=0;i<encodecount;i++)
    {
        fprintf(notimportant,"%d",bitEncode[i]);
    }
    fclose(notimportant);
    //-------------------------
    free(bufferFileKey);
    free(bufferFileEnc);

//decode file
    htree* tempTree=itsrandom;
    for(i=0;i<encodecount;i++)
    {
        if(bitEncode[i]==false)
        {
            //pindah ke kiri
            if(tempTree->left==NULL) printf("\nits empty left\n");
            tempTree=tempTree->left;
            //apakah sudah di ujung?
            if(tempTree->left ==NULL)
            {
                fprintf(endOfMisery,"%s",tempTree->symbol);
//                printf("%s",tempTree->symbol);
                tempTree=itsrandom;
            }
        }
        if(bitEncode[i]==true)
        {
            //pindah ke kanan
            if(tempTree->right==NULL) printf("\nits empty right\n");
            tempTree=tempTree->right;
            //apakah sudah di ujung?
            if(tempTree->right==NULL)
            {
                fprintf(endOfMisery,"%s",tempTree->symbol);
//                printf("%s",tempTree->symbol);
                tempTree=itsrandom;
            }
        }
    }
    fclose(endOfMisery);
	printf("\nfinished\n");
}

void printBit(char a)
{
//    printf("%d",a/256);
    bool g;
    g=((a&0x80)&&0x80);printf("%d",g);
    g=((a&0x40)&&0x40);printf("%d",g);
    g=((a&0x20)&&0x20);printf("%d",g);
    g=((a&0x10)&&0x10);printf("%d",g);
    g=((a&0x08)&&0x08);printf("%d",g);
    g=((a&0x04)&&0x04);printf("%d",g);
    g=((a&0x02)&&0x02);printf("%d",g);
    g=a&1;printf("%d",g);
}


/*
        path [0]=((c[0]&0x80)&&0x80);path [1]=((c[0]&0x40)&&0x40);path [2]=((c[0]&0x20)&&0x20);path [3]=((c[0]&0x10)&&0x10);
        path [4]=((c[0]&0x08)&&0x08);path [5]=((c[0]&0x04)&&0x04);path [6]=((c[0]&0x02)&&0x02);path [7]=((c[0]&0x01)&&0x01);
        path [8]=((c[1]&0x80)&&0x80);path [9]=((c[1]&0x40)&&0x40);path[10]=((c[1]&0x20)&&0x20);path[11]=((c[1]&0x10)&&0x10);
        path[12]=((c[1]&0x08)&&0x08);path[13]=((c[1]&0x04)&&0x04);path[14]=((c[1]&0x02)&&0x02);path[15]=((c[1]&0x01)&&0x01);
        path[16]=((c[2]&0x80)&&0x80);path[17]=((c[2]&0x40)&&0x40);path[18]=((c[2]&0x20)&&0x20);path[19]=((c[2]&0x10)&&0x10);
        path[20]=((c[2]&0x08)&&0x08);path[21]=((c[2]&0x04)&&0x04);path[22]=((c[2]&0x02)&&0x02);path[23]=((c[2]&0x01)&&0x01);
        path[24]=((c[3]&0x80)&&0x80);path[25]=((c[3]&0x40)&&0x40);path[26]=((c[3]&0x20)&&0x20);path[27]=((c[3]&0x10)&&0x10);
        path[28]=((c[3]&0x08)&&0x08);path[29]=((c[3]&0x04)&&0x04);path[30]=((c[3]&0x02)&&0x02);path[31]=((c[3]&0x01)&&0x01);
*/
