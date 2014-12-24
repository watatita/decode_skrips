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

void resetEncodeTable();
void addEncodeTable(bool val);

void huffTableCreate(htree* node, listHuff* hf);
void huffTableRecurPath(htree* node, listHuff* hf, bool dir, bool path[], int pathLen);
void huffTableAdd(listHuff* hf,char symbol[], bool path[], int pathLen);
void huffTablePrint(listHuff* hf);

void huffTreeCreateFromBit(htree* node,bool path[32],char pathlen,char sim[10]);
void decodeTable(const char* filename,const char* filenameKey);
void printBit(char a);

int main(int argc,char **argv)
{
    int i=0,j=0;
    char b,symbol;
    char magicword[10];
    char* buffer;
    if (argc!=3){
        printf("nama file encoded  :");scanf("%s",&filename1);
        printf("nama file key :");scanf("%s",&filename2);

    }else
    {
        strcpy(filename1,argv[1]);
        strcpy(filename2,argv[2]);

    }


//    FILE* jj=fopen(filename1,"r");
//    FILE* kk=fopen(filename2,"r");
    decodeTable(filename1,filename2);
    printf("\nauaua\n");
    return 0;
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

void huffTreeCreateFromBit(htree* node,bool path[32],char pathlen,char sim[10])
{
    int i=0,j=0;
    htree* tempNode=node;
//    printf("len %d\n",pathlen);

    for(i=0;i<pathlen;i++)
    {
//        printf("%d",path[i]);
        if(path[i]==false)
        {
            if(tempNode->left==NULL)
            {
                htree* a=(htree*) malloc(sizeof(htree));
                if(a==NULL)
                {
                    printf("\nfailed to create huffman Tree\n");
                    return;
                }
                a->left=NULL;
                a->right=NULL;
                if(i==(pathlen-1))
                {
//                    strcpy(a->symbol,sim);
                    for(j=0;j<10;j++)
                    {
                        a->symbol[j]=sim[j];
                    }
                }

                tempNode->left=a;
                printf("%d ",tempNode->left);
                a=NULL;
            }else{
                tempNode=tempNode->left;
            }

        }
        if(path[i]==true)
        {
            if(tempNode->right==NULL)
            {
                htree* a=(htree*) malloc(sizeof(htree));
                if(a==NULL)
                {
                    printf("failed to create huffman Tree\n");
                    return;
                }
                a->left=NULL;
                a->right=NULL;
                if(i==(pathlen-1))
                {
//                    strcpy(a->symbol,sim);
                    for(j=0;j<10;j++)
                    {
                        a->symbol[j]=sim[j];
                    }
                }
                tempNode->right=a;
                printf("%d ",tempNode->right);
                a=NULL;
            }else{
                tempNode=tempNode->right;
            }
        }
//        printf("%d",path[i]);
    }
/*
    tempNode=node;
    for(i=0;i<pathlen;i++)
    {
        if(path[i]==false)
        {
            //pindah ke kiri
            if(tempNode->left==NULL) printf("\nits empty left\n");
            tempNode=tempNode->left;
            //apakah sudah di ujung?
//            printf("%d",bitEncode[i]);
            if(tempNode->left ==NULL)
            {
//                fprintf(endOfMisery,"%s",tempTree->symbol);
                printf("%s",tempNode->symbol);
                printf("\n");
                tempNode=node;
            }
        }
        if(path[i]==true)
        {
            //pindah ke kanan
            if(tempNode->right==NULL) printf("\nits empty left\n");
            tempNode=tempNode->right;
            //apakah sudah di ujung?
//            printf("%d",bitEncode[i]);
            if(tempNode->right ==NULL)
            {
//                fprintf(endOfMisery,"%s",tempTree->symbol);
                printf("%s",tempNode->symbol);
                printf("\n");
                tempNode=node;
            }
        }
    }
*/
//    printf("\n");
}

void decodeTable(const char* filename,const char* filenameKey)
{
    int keycount=0,encodecount=0;
    int i=0,j=0,x=0;
    int maxi;
    bool whatever;
    char b=0;
    char c[4];

    bool path[32];

    char fileOutputName[50];
    int symbolLen,binaryLen,fileLen,fileLenEncode;
    int dataKeyPos;
    char* bufferFileKey;
    char* bufferFileEnc;

    jj=fopen(filename,"rb");
    kk=fopen(filenameKey,"rb");
    if(jj==NULL) return;
    if(kk==NULL) return;

    printf("file output name :");scanf("%s",&fileOutputName);
    FILE* endOfMisery=fopen(fileOutputName,"w");

    fseek(kk,4,SEEK_SET);
    fseek(jj,4,SEEK_SET);

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

    htree* itsrandom=(htree*)malloc(sizeof(htree));
    listHuff* anorandom=(listHuff*)malloc(sizeof(listHuff));
    itsrandom->left=NULL;
    itsrandom->right=NULL;
    anorandom->first=NULL;


    i=dataKeyPos;
    while(i<fileLen)
    {
        symbolLen=bufferFileKey[i];
        i++;
        printf("%d len %d ",x,symbolLen);
        strcpy(huffSimbol,"");
        for(j=0;j<symbolLen;j++)
        {
            huffSimbol[j]=bufferFileKey[i];
            if(bufferFileKey[i]=='\r')
            {
                if(bufferFileKey[i+1]=='\n')
                {
//                    printf("\nnewline\n");
                    huffSimbol[j]=0x60;
                    i++;
                }
            }

//            printf("%c",huffSimbol[j]);
            i++;
        }
        huffSimbol[symbolLen]='\0';
//        printf("%s",huffSimbol);

//        huffSimbol[symbolLen]='\0';
        binaryLen=bufferFileKey[i];
//        printf(" %d ",binaryLen);
        i++;
        for(j=0;j<4;j++)
        {
            c[j]=bufferFileKey[i];
            printBit(c[j]);
            i++;
        }
        printf(" ");

        path [0]=((c[0]&0x80)&&0x80);path [1]=((c[0]&0x40)&&0x40);path [2]=((c[0]&0x20)&&0x20);path [3]=((c[0]&0x10)&&0x10);
        path [4]=((c[0]&0x08)&&0x08);path [5]=((c[0]&0x04)&&0x04);path [6]=((c[0]&0x02)&&0x02);path [7]=((c[0]&0x01)&&0x01);
        path [8]=((c[1]&0x80)&&0x80);path [9]=((c[1]&0x40)&&0x40);path[10]=((c[1]&0x20)&&0x20);path[11]=((c[1]&0x10)&&0x10);
        path[12]=((c[1]&0x08)&&0x08);path[13]=((c[1]&0x04)&&0x04);path[14]=((c[1]&0x02)&&0x02);path[15]=((c[1]&0x01)&&0x01);
        path[16]=((c[2]&0x80)&&0x80);path[17]=((c[2]&0x40)&&0x40);path[18]=((c[2]&0x20)&&0x20);path[19]=((c[2]&0x10)&&0x10);
        path[20]=((c[2]&0x08)&&0x08);path[21]=((c[2]&0x04)&&0x04);path[22]=((c[2]&0x02)&&0x02);path[23]=((c[2]&0x01)&&0x01);
        path[24]=((c[3]&0x80)&&0x80);path[25]=((c[3]&0x40)&&0x40);path[26]=((c[3]&0x20)&&0x20);path[27]=((c[3]&0x10)&&0x10);
        path[28]=((c[3]&0x08)&&0x08);path[29]=((c[3]&0x04)&&0x04);path[30]=((c[3]&0x02)&&0x02);path[31]=((c[3]&0x01)&&0x01);


        huffTreeCreateFromBit(itsrandom,path,binaryLen,huffSimbol);
        printf("\n");
        fprintf(endOfMisery,"%d ",x);
        for(j=0;j<symbolLen;j++)
        {
            fprintf(endOfMisery,"%c",huffSimbol[j]);
        }
        fprintf(endOfMisery," ");
        for(j=0;j<binaryLen;j++)
        {
            fprintf(endOfMisery,"%d",path[j]);
        }
        fprintf(endOfMisery,"\n ");
        x++;
//        strcpy(huffSimbol," ");
    }
    printf("finished create tree from bit\n");
//    fclose(kk);
//    fclose(jj);
    fclose(endOfMisery);


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
//    printf("\nprint every bit encode %d\n",bitLen);
//    for(i=0;i<encodecount;i++)
//    {
//        printf("%d",bitEncode[i]);
//    }

    free(bufferFileKey);
    free(bufferFileEnc);
    printf("finished create tree from bit\n");
//    huffTableCreate(itsrandom,anorandom);
//    huffTablePrint(anorandom);

    printf("its 010110110 %s\n",itsrandom->left->right->left->right->right->left->right->right->right->symbol);


    htree* tempTree=itsrandom;
    for(i=0;i<encodecount;i++)
    {
        if(bitEncode[i]==false)
        {
            //pindah ke kiri
            if(tempTree->left==NULL) printf("\nits empty left\n");
            tempTree=tempTree->left;
            //apakah sudah di ujung?
            printf("%d",bitEncode[i]);
            if(tempTree->left ==NULL)
            {
//                fprintf(endOfMisery,"%s",tempTree->symbol);
//                printf("%s",tempTree->symbol);
                printf("\n");
                tempTree=itsrandom;
            }
        }
        if(bitEncode[i]==true)
        {
            //pindah ke kanan
            if(tempTree->right==NULL) printf("\nits empty right\n");
            tempTree=tempTree->right;
            //apakah sudah di ujung?
            printf("%d",bitEncode[i]);
            if(tempTree->right==NULL)
            {
//                fprintf(endOfMisery,"%s",tempTree->symbol);
//                printf("%s",tempTree->symbol);
                printf("\n");
                tempTree=itsrandom;
            }
        }
    }

/*
    for(i=0;i<keycount;i++)
    {

//        fscanf(kk,"%c",&b);//printf("%d ",b);
        len=fgetc(kk);
        for(j=0;j<b;j++)
        {
//            fscanf(kk,"%c",&symbol);//printf("%c",symbol);
            symbol=fgetc(kk);
            huffsimbol[j]=symbol;
        }
        huffsimbol[b]='\0';

//        fscanf(jj,"%c",&b);
        b=fgetc(kk);
        c[0]=fgetc(kk);
        c[1]=fgetc(kk);
        c[2]=fgetc(kk);
        c[3]=fgetc(kk);

        printf("len %d %s %d\n",len,huffsimbol,b);
        //AMBIL 2 BYTE HUFFMAN CODE
//        fscanf(kk,"%c",&c[0]); //printBit(b);printf("\n");
//        fscanf(kk,"%c",&c[1]); //printBit(c);printf("\n");
//        fscanf(kk,"%c",&c[2]);
//        fscanf(kk,"%c",&c[3]);

        //masa bodo sama koding panjang!! pala udah pusing

        path [0]=((c[0]&0x80)&&0x80);
        path [1]=((c[0]&0x40)&&0x40);
        path [2]=((c[0]&0x20)&&0x20);
        path [3]=((c[0]&0x10)&&0x10);
        path [4]=((c[0]&0x08)&&0x08);
        path [5]=((c[0]&0x04)&&0x04);
        path [6]=((c[0]&0x02)&&0x02);
        path [7]=((c[0]&0x01)&&0x01);

        path [8]=((c[1]&0x80)&&0x80);
        path [9]=((c[1]&0x40)&&0x40);
        path[10]=((c[1]&0x20)&&0x20);
        path[11]=((c[1]&0x10)&&0x10);
        path[12]=((c[1]&0x08)&&0x08);
        path[13]=((c[1]&0x04)&&0x04);
        path[14]=((c[1]&0x02)&&0x02);
        path[15]=((c[1]&0x01)&&0x01);

        path[16]=((c[2]&0x80)&&0x80);
        path[17]=((c[2]&0x40)&&0x40);
        path[18]=((c[2]&0x20)&&0x20);
        path[19]=((c[2]&0x10)&&0x10);
        path[20]=((c[2]&0x08)&&0x08);
        path[21]=((c[2]&0x04)&&0x04);
        path[22]=((c[2]&0x02)&&0x02);
        path[23]=((c[2]&0x01)&&0x01);

        path[24]=((c[3]&0x80)&&0x80);
        path[25]=((c[3]&0x40)&&0x40);
        path[26]=((c[3]&0x20)&&0x20);
        path[27]=((c[3]&0x10)&&0x10);
        path[28]=((c[3]&0x08)&&0x08);
        path[29]=((c[3]&0x04)&&0x04);
        path[30]=((c[3]&0x02)&&0x02);
        path[31]=((c[3]&0x01)&&0x01);

        //BUAT HUFFMAN TABLE DARI INFORMASI DIATAS
        huffTreeCreateFromBit(itsrandom,path,len,huffsimbol);

        //reset dulu mas bro
        for(j=0;j<32;j++)
        {
            path[j]=false;
        }

    }*/
//    printf("\nprint recently created huffman tree\n\t\t---\n");
//    huffTableCreate(itsrandom,anorandom);
//    huffTablePrint(&anorandom);
//    printf("\ndone\n");

    /*
    resetEncodeTable();

    i=0;
    symbol=fgetc(jj);
    while(symbol!=EOF)
    {
        tempScan[i]=symbol;
        i++;
        symbol=fgetc(jj);
    }
    tempScan[i]='\0';
    maxi=i;
    for(i=0;i<maxi;i++)
    {
        for(j=0;j<8;j++)
        {
            switch(j)
            {
                case 0: whatever=tempScan[i]&0x80;break;
                case 1: whatever=tempScan[i]&0x40;break;
                case 2: whatever=tempScan[i]&0x20;break;
                case 3: whatever=tempScan[i]&0x10;break;
                case 4: whatever=tempScan[i]&0x8;break;
                case 5: whatever=tempScan[i]&0x4;break;
                case 6: whatever=tempScan[i]&0x2;break;
                case 7: whatever=tempScan[i]&0x1;break;
            }
            addEncodeTable(whatever);
        }
    }
    //untuk mengubah stream bit sesuai dengan yang ada di File
    bitLen=encodecount;
    printf("\nthis is stream of encode (bitLen:%d encodecount:%d)\n",bitLen,encodecount);

    //mulai menDECODE dan menulis
    fprintf(endOfMisery,"{this is decoded file}\n");
    htree* tempTree=itsrandom;
    for(i=0;i<encodecount;i++)
    {
        if(bitEncode[i]==false)
        {
            //pindah ke kiri
            tempTree=tempTree->left;
            //apakah sudah di ujung?
            if(tempTree->left==NULL)
            {
                fprintf(endOfMisery,"%s",tempTree->symbol);
                tempTree=itsrandom;
            }
        }
        if(bitEncode[i]==true)
        {
            //pindah ke kanan
            tempTree=tempTree->right;
            //apakah sudah di ujung?
            if(tempTree->right==NULL)
            {
                fprintf(endOfMisery,"%s",tempTree->symbol);
                tempTree=itsrandom;
            }
        }
        printf("%d",bitEncode[i]);
    }
    */

    //fclose(endOfMisery);
	printf("ee\n");
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
