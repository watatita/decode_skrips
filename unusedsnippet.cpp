//
//void huffTableCreate(htree* node, listHuff* hf)
//{
//    bool path[100];
//    huffTableRecurPath(node->left, hf,0,path,0);
//    huffTableRecurPath(node->right, hf,1,path,0);
//}
//void huffTableRecurPath(htree* node, listHuff* hf, bool dir, bool path[], int pathLen)
//{
//    if (node==NULL)
//        return;
//    int i;
//  /* append this node to the path array */
//    path[pathLen]=dir;
//    pathLen++;
//
////    printf("show path %d\n",node->frekuensi);
//  /* it's a leaf, so print the path that led to here  */
//    if (node->left==NULL && node->right==NULL)
//    {
//        huffTableAdd(hf,node->symbol,path,pathLen);
//    }
//    else
//    {
//    /* otherwise try both subtrees */
//
//            huffTableRecurPath(node->left ,hf,0,path,pathLen);
//            huffTableRecurPath(node->right,hf,1,path,pathLen);
//    }
//}
//void huffTableAdd(listHuff* hf,char symbol[], bool path[], int pathLen)
//{
//    int i;
//    huffTable* hh=(huffTable*) malloc(sizeof(huffTable));
//    hh->next=NULL;
//    hh->huff.len=pathLen;
//    strcpy(hh->huff.symbol,symbol);
//
//    for (i=0; i<pathLen; i++)
//    {
//        hh->huff.encode[i]=path[i];
//    }
//
//    if(hf->first==NULL)
//    {
//        hf->first=hh;
//    }else
//    {
//        huffTable* last_symbol=hf->first;
//        while(last_symbol->next!=NULL)
//        {
//            last_symbol=last_symbol->next;
//        }
//        last_symbol->next=hh;
//    }
//    hh=NULL;
//}
//void huffTablePrint(listHuff* hf)
//{
//    int i;int j=0;
//    int countTotalBit=0;
//    huffTable* a=hf->first;
//    while(a!=NULL)
//    {
//        j++;
//        printf("%d ",j);
//        for(i=0;i<a->huff.len;i++)
//        {
//            printf("%d ",a->huff.encode[i]);
//        }
//        printf("%s\n",a->huff.symbol);
//        a=a->next;
//    }
//}
//
//void huffTreeCreateFromBit(htree* node,bool path[32],char pathlen,char sim[10])
//{
//    int i=0,j=0;
//    htree* tempNode=node;
////    printf("len %d\n",pathlen);
//
//    for(i=0;i<pathlen;i++)
//    {
////        printf("%d",path[i]);
//        if(path[i]==false)
//        {
//            if(tempNode->left==NULL)
//            {
//                htree* a=(htree*) malloc(sizeof(htree));
//                if(a==NULL)
//                {
//                    printf("\nfailed to create huffman Tree\n");
//                    return;
//                }
//                a->left=NULL;
//                a->right=NULL;
//                if(i==(pathlen-1))
//                {
////                    strcpy(a->symbol,sim);
//                    for(j=0;j<10;j++)
//                    {
//                        a->symbol[j]=sim[j];
//                    }
//                }
//
//                tempNode->left=a;
//                printf("%d ",tempNode->left);
//                a=NULL;
//            }else{
//                tempNode=tempNode->left;
//            }
//
//        }
//        if(path[i]==true)
//        {
//            if(tempNode->right==NULL)
//            {
//                htree* a=(htree*) malloc(sizeof(htree));
//                if(a==NULL)
//                {
//                    printf("failed to create huffman Tree\n");
//                    return;
//                }
//                a->left=NULL;
//                a->right=NULL;
//                if(i==(pathlen-1))
//                {
////                    strcpy(a->symbol,sim);
//                    for(j=0;j<10;j++)
//                    {
//                        a->symbol[j]=sim[j];
//                    }
//                }
//                tempNode->right=a;
//                printf("%d ",tempNode->right);
//                a=NULL;
//            }else{
//                tempNode=tempNode->right;
//            }
//        }
////        printf("%d",path[i]);
//    }
///*
//    tempNode=node;
//    for(i=0;i<pathlen;i++)
//    {
//        if(path[i]==false)
//        {
//            //pindah ke kiri
//            if(tempNode->left==NULL) printf("\nits empty left\n");
//            tempNode=tempNode->left;
//            //apakah sudah di ujung?
////            printf("%d",bitEncode[i]);
//            if(tempNode->left ==NULL)
//            {
////                fprintf(endOfMisery,"%s",tempTree->symbol);
//                printf("%s",tempNode->symbol);
//                printf("\n");
//                tempNode=node;
//            }
//        }
//        if(path[i]==true)
//        {
//            //pindah ke kanan
//            if(tempNode->right==NULL) printf("\nits empty left\n");
//            tempNode=tempNode->right;
//            //apakah sudah di ujung?
////            printf("%d",bitEncode[i]);
//            if(tempNode->right ==NULL)
//            {
////                fprintf(endOfMisery,"%s",tempTree->symbol);
//                printf("%s",tempNode->symbol);
//                printf("\n");
//                tempNode=node;
//            }
//        }
//    }
//*/
////    printf("\n");
//}
