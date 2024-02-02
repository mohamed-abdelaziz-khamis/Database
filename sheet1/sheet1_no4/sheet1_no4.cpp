#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>  //eof(file handler)
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys\stat.h>

void menu()
     {
      clrscr( );
      printf("1.Sorted index file.\n");
      printf("2.Index file using appropriate hash function.\n");
      printf("3.Exit.\n");
      printf("Enter your choice:");
     }

void submenu( )
     {
      clrscr( );
      printf("1.Add.\n");
      printf("2.Update.\n");
      printf("3.Delete.\n");
      printf("4.Organize.\n");
      printf("5.Back.\n");
      printf("Enter your choice:");
     }

struct master_record
     {
      unsigned long int stud_no;
      char name[20];
      char address[20];
      float grade;
     };

struct trans_record
     {
      unsigned int deleted:1;
      unsigned long int stud_no;
      char name[20];
      char address[20];
      float grade;
     };

struct index_record
     {
      unsigned int deleted:1;
      unsigned long int stud_no;
      unsigned long int relative_address;
     };

struct transArray_record
     {
       unsigned long int stud_no;
       unsigned long int relative_address;
     };

void Load_records_inMasterFile( ) //write data in a binary file
     {
      FILE *fp = fopen ("data.txt","r");
      FILE *master_fp = fopen("data_binary","wb");
      master_record stud; int stud_size=sizeof(master_record);

      char stud_no[11];
      //unsigned long int: 32 bits -->from 0 to 4,294,967,295 (10 digits)
      char name[20];
      char address[20];
      char grade[11];

      int i; char ch=fgetc(fp);
      while (ch!= EOF)
            {
             while (ch!=',')
                  {
                   ch=fgetc(fp);
                  }

             i=0;
             ch=fgetc(fp);
             while (ch!=',')
                   {
                    stud_no[i]=ch;
                    i++;
                    ch=fgetc(fp);
                   }
             stud_no[i]='\0';
             stud.stud_no=atol(stud_no);

             i=0;
             ch=fgetc(fp);
             while (ch!=',')
                   {
                    name[i]=ch;
                    i++;
                    ch=fgetc(fp);
                   }
             name[i]='\0';
             strcpy(stud.name,name);

             i=0;
             ch=fgetc(fp);
             while (ch!=',')
                   {
                    address[i]=ch;
                    i++;
                    ch=fgetc(fp);
                   }
             address[i]='\0';
             strcpy(stud.address,address);

             i=0;
             ch=fgetc(fp);
             while (ch!=',')
                   {
                    grade[i]=ch;
                    i++;
                    ch=fgetc(fp);
                   }
             grade[i]='\0';
             stud.grade=atof(grade);

             fwrite(&stud,stud_size,1,master_fp);
             ch=fgetc(fp);
            }
      fclose(fp);
      fclose(master_fp);
     }

void Display_BinaryFile( )
     {
        int file_handler=open("data_binary",O_RDONLY|O_BINARY|S_IREAD);
        int stud_size=sizeof(master_record);
        master_record stud;
        while (!eof(file_handler))
              {
               read(file_handler,&stud,stud_size);
               printf("%ld ",stud.stud_no);
               printf("%s ",stud.name);
               printf("%s ",stud.address);
               printf("%4.2f\n",stud.grade);
              }
        getch();
        close(file_handler);
     }

void Load_records_inIndexFile( )
     {
      FILE *fp = fopen ("data.txt","r");
      FILE *index_fp = fopen("index","wb");
      index_record index;
      int index_size=sizeof(index_record);
      int master_size=sizeof(master_record);

      char stud_no[11];
      //unsigned long int: 32 bits -->from 0 to 4,294,967,295 (10 digits)


      int i; char ch=fgetc(fp);   int j=0;
      while (ch!= EOF)
            {
             index.deleted=ch;
             while (ch!=',')
                  {
                   ch=fgetc(fp);
                  }

             i=0;
             ch=fgetc(fp);
             while (ch!=',')
                   {
                    stud_no[i]=ch;
                    i++;
                    ch=fgetc(fp);
                   }
             stud_no[i]='\0';
             index.stud_no=atol(stud_no);


             ch=fgetc(fp);
             while (ch!=',')
                   {
                    ch=fgetc(fp);
                   }

             ch=fgetc(fp);
             while (ch!=',')
                   {
                    ch=fgetc(fp);
                   }

             ch=fgetc(fp);
             while (ch!=',')
                   {
                    ch=fgetc(fp);
                   }

             index.relative_address=j*master_size;
             fwrite(&index,index_size,1,index_fp);
             ch=fgetc(fp);
             j++;
            }
      fclose(fp);
      fclose(index_fp);
     }


void initialize_IndexHash(int index_handler)
     {
       index_record index;
       int index_size=sizeof(index_record);

       index.deleted=1;
       index.stud_no=-1;
       index.relative_address=0;

       for(int i=1;i<=230;i++)
          write(index_handler,&index,index_size);
     }

void Load_IndexHash( )
     {
       int master_handler=open("data_binary",O_CREAT|O_RDONLY|O_BINARY,S_IREAD);
       int index_handler=open("IndexHash",O_CREAT|O_TRUNC|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
       int overflow_handler=open("overflow",O_CREAT|O_TRUNC|O_WRONLY|O_BINARY,S_IWRITE);

       master_record stud;
       index_record index;
       int stud_size=sizeof(master_record);
       int index_size=sizeof(index_record);

       index.deleted=1;
       index.stud_no=-1;
       index.relative_address=0;

       int i,j;
       for(i=1;i<=230;i++)
          write(index_handler,&index,index_size);

       i=0;
       while(!eof(master_handler))
            {
              read(master_handler,&stud,stud_size);
              lseek(index_handler,(stud.stud_no % 23)*10*index_size,SEEK_SET);
              j=0;
              do
               {
                read(index_handler,&index,index_size);
                j++;
               }
              while(index.deleted==0 && j<10);
              index.deleted=0;
              index.stud_no=stud.stud_no;
              index.relative_address=i*stud_size;  i++;

              if (j<10)
                 {
                  lseek(index_handler,-(long)index_size,index_size);
                  write(index_handler,&index,index_size);
                 }
              else write(overflow_handler,&index,index_size);
            }
       close(master_handler);
       close(index_handler);
       close(overflow_handler);
     }

int binary_search_in_indexfile(int index_handler,index_record &index)
   {
    index_record index1;
    int index_size=sizeof(index_record);
    int low=0;
    int high=(filelength(index_handler)/index_size)-1;
    int mid;
    long  k;
    while(low<=high)
	 {
	  mid=(low+high)/2;
          lseek(index_handler,mid*index_size,SEEK_SET);
          read(index_handler,&index1,index_size);
          if (index1.stud_no==index.stud_no)
              {
               index.relative_address=index1.relative_address;
               if (index1.deleted==1)
                   return -1;
               else return 1;
              }
	  if (index1.stud_no>index.stud_no)  high=mid-1;
	  else low=mid+1;
	 }
    return 0;
  }


int seq_search_in_trans(int trans_handler,trans_record stud)
  {
    trans_record stud1;
    int trans_size=sizeof(trans_record);
    lseek(trans_handler, 0L, SEEK_SET);
    while (!eof(trans_handler))
         {
          read(trans_handler,&stud1,trans_size);
          if (stud1.stud_no == stud.stud_no)
              {
               if (stud1.deleted==1)
                   return -1;
               else return 1;
              }
         }
    return 0;
  }

void swap(transArray_record &x,transArray_record &y)
{
  /*The procedure "swap":
  swap exchanges the values of x and y */
  transArray_record temp;
  temp=x;
  x=y;
  y=temp;
}

void bubble_sort(transArray_record A[],int n)
{
 //Bubble sort algorithm
   int flag;
   for (int i=0;i<n-1;i++)
       {
	flag=0;
	for (int j=n-1;j>=i+1;j--)
	   {
	    if (A[j].stud_no<A[j-1].stud_no)
	      {
	       swap(A[j],A[j-1]);
	       flag=1; // A swap has happened.
	      }
	   }
	if (flag==0) break;//There is no swap has happened in previous pass,
       }          	   // so the List is sorted.
}

void SortTransaction(int trans_handler,transArray_record arr[], int &n)
     {
       int trans_size = sizeof(trans_record);
       trans_record stud;
       int i=0,j=0;
       lseek(trans_handler, 0L, SEEK_SET);
       while(!eof(trans_handler))
            {
             read(trans_handler,&stud,trans_size);
             if (stud.deleted==0) //not deleted.
                {
                 arr[i].stud_no=stud.stud_no;
                 arr[i].relative_address=j*trans_size;
                 i++;
                }
             j++;
            }
       n=i;
       bubble_sort(arr,n);
     }

void Merge_Master_Trans_Rebuild_Index(int &master_handler,char master_name[],int &index_handler,char index_name[],int trans_handler,transArray_record arr[],int n)
{
 int tempMaster_handler=open("tempMaster",O_CREAT|O_TRUNC|O_WRONLY|O_BINARY,S_IWRITE);
 int tempIndex_handler=open("tempIndex",O_CREAT|O_TRUNC|O_WRONLY|O_BINARY,S_IWRITE);

 master_record stud_master;
 trans_record stud_trans;
 index_record index;

 int stud_master_size = sizeof(master_record);
 int stud_trans_size = sizeof(trans_record);
 int index_size = sizeof(index_record);

 int master_records_no = filelength(master_handler)/stud_master_size;

 int i=0,j=0,l=0; int flag1=1,flag2=1;

 lseek(index_handler,0L, SEEK_SET);
 while (i<master_records_no && j<n)
 {
   if (flag1==1)
      {
       do
        {
         read(index_handler,&index,index_size);
         i++;
        }
       while(index.deleted==1 && i<master_records_no);
       if (i>=master_records_no) break;
       read(master_handler,&stud_master,stud_master_size);
      }

   if (flag2==1)
     {
      lseek(trans_handler,arr[j].relative_address, SEEK_SET);
      read(trans_handler,&stud_trans,stud_trans_size);
     }

   if(stud_master.stud_no<stud_trans.stud_no)
     {
      write(tempMaster_handler,&stud_master,stud_master_size);
      index.stud_no=stud_master.stud_no;
      flag1=1;
      flag2=0;
     }
   else
    {
     write(tempMaster_handler,&stud_trans,stud_trans_size);
     index.stud_no=stud_trans.stud_no;
     j++;
     flag1=0;
     flag2=1;
    }
   index.relative_address=l*stud_master_size; l++;
   write(tempIndex_handler,&index,index_size);
 }

 if(i>=master_records_no)
   for (int x=j;x<n;x++)
       {
        lseek(trans_handler,arr[x].relative_address, SEEK_SET);
        read(trans_handler,&stud_trans,stud_trans_size);
        write(tempMaster_handler,&stud_trans,stud_trans_size);
        index.stud_no=stud_trans.stud_no;
        index.relative_address=l*stud_master_size;   l++;
        write(tempIndex_handler,&index,index_size);
       }
 else if(j>=n)
   while (i<master_records_no)
       {
         do
           {
            read(index_handler,&index,index_size);
            i++;
           }
         while(index.deleted==1 && i<master_records_no);
         if (index.deleted==0)
           {
            write(tempMaster_handler,&stud_master,stud_master_size);
            index.relative_address=l*stud_master_size;   l++;
            write(tempIndex_handler,&index,index_size);
           }
       }

  close(tempMaster_handler);
  close(tempIndex_handler);

  close(master_handler);
  close(index_handler);

  remove(master_name);
  remove(index_name);

  rename("tempMaster",master_name);
  rename("tempIndex",index_name);
  master_handler=open(master_name,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
  index_handler=open(index_name,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
}

void UpdateTransaction(int &trans_handler)
     {
      close(trans_handler);
      trans_handler=open("transaction",O_TRUNC|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
      //O_TRUNC	If the file exists, its length is truncated to 0. The file attributes remain unchanged.
     }

void OrganizeIndexSorted(int &master_handler,char master_name[],int &index_handler,char index_name[],int trans_handler,int &count_trans_records,int &count_deleted_records)
     {
      count_trans_records=0;
      count_deleted_records=0;
      transArray_record arr[20]; int n;
      SortTransaction(trans_handler,arr,n);
      Merge_Master_Trans_Rebuild_Index(master_handler,master_name,index_handler,index_name,trans_handler,arr,n);
      UpdateTransaction (trans_handler);
      clrscr( );
      printf ("The data has been organized.\n");
      getch ( );
     }

void AddIndexSorted(int &master_handler,char master_name[],int &index_handler,char index_name[],int trans_handler,int &count_trans_records,int &count_deleted_records)
     {
       clrscr( );
       trans_record stud;          stud.deleted=0;
       index_record index;         index.deleted=0;
       printf("Enter the new student record :\n");
       printf("Student#:");
       scanf("%ld",&stud.stud_no); index.stud_no=stud.stud_no;
       printf("\nName:");
       scanf("%s",stud.name);
       printf("\nAddress:");
       scanf("%s",stud.address);
       printf("\nGrade:");
       scanf("%f",&stud.grade);
       int binary_search_result=binary_search_in_indexfile(index_handler,index);
       if (binary_search_result==1)
          {
           printf("\nThere exists a student having the same student#.");
           getch( );
          }

       else if (binary_search_result==-1)
          {
           //don't shift downwords
           lseek(index_handler,-(long)sizeof(index_record),SEEK_CUR);
           write(index_handler,&index,sizeof(index_record));
           count_deleted_records--;
          }

       else
         {
          int seq_search_result= seq_search_in_trans(trans_handler,stud);
          if (seq_search_result == 1)
             {
              printf("\nThere exists a student having the same student#.");
              getch( );
             }

          else
             {
              if (seq_search_result == -1)
                  {
                   count_deleted_records--;
                   lseek(trans_handler,-(long)sizeof(trans_record),SEEK_CUR);
                  }
              write (trans_handler,&stud,sizeof(trans_record));
              count_trans_records ++;
              if (count_trans_records==10)
                 OrganizeIndexSorted(master_handler,master_name,index_handler,index_name,trans_handler,count_trans_records,count_deleted_records);
             }
        }
     }

void DeleteIndexSorted (int &master_handler,char master_name[],int &index_handler,char index_name[],int trans_handler,int &count_trans_records,int &count_deleted_records)
{
   clrscr( );
   trans_record stud;
   index_record index;
   printf("Enter the old student record:\n");
   printf("Student#:");
   scanf("%ld",&stud.stud_no);  index.stud_no=stud.stud_no;
   printf("\nName:");
   scanf("%s",stud.name);
   printf("\nAddress:");
   scanf("%s",stud.address);
   printf("\nGrade:");
   scanf("%f",&stud.grade);
   int binary_search_result=binary_search_in_indexfile(index_handler,index);
   if (binary_search_result==1)
      {
        index.deleted = 1;
        lseek(index_handler,-(long)sizeof(index_record),SEEK_CUR);
        write(index_handler,&index,sizeof(index_record));
        count_deleted_records++;
      }

   else if (binary_search_result==-1)
      {
        printf("This record is already deleted.\n");
        getch( );
      }

   else
      {
        int seq_search_result= seq_search_in_trans(trans_handler,stud);
        if (seq_search_result == 1)
             {
               stud.deleted = 1;
               lseek(trans_handler,-(long)sizeof(trans_record),SEEK_CUR);
               write(trans_handler,&stud,sizeof(trans_record));
               count_trans_records--;
               count_deleted_records++;
             }

        else if (seq_search_result == -1)
             {
               printf("This record is already deleted.\n");
               getch( );
             }

        else
             {
               printf("This record doesn't exist.\n");
               getch( );
             }
       }
    if (count_deleted_records==10)
        OrganizeIndexSorted(master_handler,master_name,index_handler,index_name,trans_handler,count_trans_records,count_deleted_records);
}

void UpdateIndexSorted(int &master_handler,char master_name[],int &index_handler,char index_name[],int trans_handler,int &count_trans_records,int &count_deleted_records)
     {
       DeleteIndexSorted(master_handler,master_name,index_handler,index_name,trans_handler,count_trans_records,count_deleted_records);
       AddIndexSorted(master_handler,master_name,index_handler,index_name,trans_handler,count_trans_records,count_deleted_records);
     }

void IndexSorted( )
     {
       Load_records_inMasterFile( );
       Load_records_inIndexFile( );
       clrscr( );
       printf ("Enter the master file name:\n");
       char master_name[20];
       scanf("%s", master_name);
       int master_handler=open(master_name,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
       printf ("Enter the index file name:\n");
       char index_name[20];
       scanf("%s", index_name);
       int index_handler=open(index_name,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
       int trans_handler=open("transaction",O_CREAT|O_TRUNC|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
       int choice; int count_trans_records=0;
       int count_deleted_records=0;
       while(1)
       {
        submenu( );
        scanf("%d", & choice);
        switch (choice)
          {
           case 1: AddIndexSorted(master_handler,master_name,index_handler,index_name,trans_handler,count_trans_records,count_deleted_records);
                   break;
           case 2: UpdateIndexSorted(master_handler,master_name,index_handler,index_name,trans_handler,count_trans_records,count_deleted_records);
                   break;
           case 3: DeleteIndexSorted(master_handler,master_name,index_handler,index_name,trans_handler,count_trans_records,count_deleted_records);
                   break;
           case 4: OrganizeIndexSorted(master_handler,master_name,index_handler,index_name,trans_handler,count_trans_records,count_deleted_records);
                   break;
           case 5:if(count_trans_records>0 || count_deleted_records>0)
                    OrganizeIndexSorted(master_handler,master_name,index_handler,index_name,trans_handler,count_trans_records,count_deleted_records);
          }
         if(choice==5) break;
         }
        close(master_handler);
        close(index_handler);
        close(trans_handler);
        remove("transaction");
     }

int search_in_IndexHash(int index_handler,index_record &index,int fk)
  {
    index_record index1;
    int index_size=sizeof(index_record);
    lseek(index_handler,fk*10*index_size,SEEK_SET);
    for(int i=1;i<=10;i++)
         {
          read(index_handler,&index1 ,index_size);
          if (index1.stud_no == index.stud_no  && index1.deleted==0)
             {
              index.relative_address=index1.relative_address;
              return 1;
             } 
         }
    return 0;
  }

int seq_search_in_overflow(int overflow_handler,index_record &index)
  {
    index_record index1;
    int index_size=sizeof(index_record);
    lseek(overflow_handler,0L,SEEK_SET);
    while(!eof(overflow_handler))
         {
          read(overflow_handler,&index1 ,index_size);
          if (index1.stud_no==index.stud_no)
             {
              index.relative_address=index1.relative_address;
              if(index1.deleted==1)
                return -1;
              else return 1;
             }
         }
    return 0;
  }

void AddIndexHash(int master_handler,int index_handler,int overflow_handler,int &count_deleted_records)
     {
      clrscr( );
      master_record stud;         
      index_record index;         index.deleted=0;
      printf("Enter the new student record:\n");
      printf("Student#:");
      scanf("%ld",&stud.stud_no);
        index.stud_no=stud.stud_no;  int fk=index.stud_no % 23;
      printf("\nName:");
      scanf("%s",stud.name);
      printf("\nAddress:");
      scanf("%s",stud.address);
      printf("\nGrade:");
      scanf("%f",&stud.grade);
      int index_size=sizeof(index_record);
      int search_result=search_in_IndexHash(index_handler,index,fk);
      if (search_result==1)
          {
           printf("\nThere exists a student having the same student#.");
           getch( );
          }
      else
          {
           int seq_search_result=seq_search_in_overflow(overflow_handler,index);
           if (seq_search_result==1)
              {
               printf("\nThere exists a student having the same student#.");
               getch( );
              }
           else if (seq_search_result==-1)
              {
               lseek(overflow_handler,-(long)index_size,SEEK_CUR);
               write(overflow_handler,&index,index_size);
               count_deleted_records -- ;
              }
           else
              {
               index.relative_address=filelength(master_handler);
               lseek(index_handler,fk*10*index_size,SEEK_SET); int i=0;
               index_record index1;
               do
                 {
                  read(index_handler,&index1,index_size);
                  i++;
                 }
               while(index1.deleted==0 &&  i<10);
               if (i<10)
                 {
                  lseek(index_handler,-(long)index_size,SEEK_CUR);
                  write(index_handler,&index,index_size);
                 }
               else write(overflow_handler,&index,index_size);
              }
          }
     }

void OrganizeIndexHash(int &master_handler,char master_name[],int &index_handler,char index_name[],int &overflow_handler,char overflow_name[],int &count_deleted_records)
   {
        count_deleted_records=0;
        master_record stud;
        index_record index,index1;
        int stud_size=sizeof(master_record);
        int index_size=sizeof(index_record);

        int temp1_handler=open("temporary1",O_CREAT|O_TRUNC|O_WRONLY|O_BINARY,S_IWRITE);
        int temp2_handler=open("temporary2",O_CREAT|O_TRUNC|O_WRONLY|O_BINARY,S_IWRITE);
        int temp3_handler=open("temporary3",O_CREAT|O_TRUNC|O_WRONLY|O_BINARY,S_IWRITE);
        initialize_IndexHash(temp2_handler);

        lseek(index_handler, 0L, SEEK_SET);
        lseek(overflow_handler, 0L, SEEK_SET);
        int x,y;

        for(x=0;x<23;x++)
        {
          lseek(temp2_handler,x*10*index_size,SEEK_SET);
          for(y=1;y<=10;y++)
             {
              read(index_handler,&index,index_size);
              if(index.deleted==0)
                {
                 lseek(master_handler,index.relative_address,SEEK_SET);
                 read(master_handler,&stud,stud_size);
                 index.relative_address=filelength(temp1_handler);
                 write(temp1_handler,&stud,stud_size);
                 write(temp2_handler,&index,index_size);
                }
             }
         }

        while(!eof(overflow_handler))
             {
               read(overflow_handler,&index,index_size);
               if(index.deleted==0)
                {
                 lseek(master_handler,index.relative_address,SEEK_SET);
                 read(master_handler,&stud,stud_size);
                 index.relative_address=sizeof(temp1_handler);
                 write(temp1_handler,&stud,stud_size);
                 write(temp3_handler,&index,index_size);
               }
             }

        close(temp1_handler);
        close(temp2_handler);
        close(temp3_handler);

        close(master_handler);
        close(index_handler);
        close(overflow_handler);


        remove(master_name);
        remove(index_name);
        remove(overflow_name);
        rename("temporary1",master_name);
        rename("temporary2",index_name);
        rename("temporary3",overflow_name);
        master_handler=open(master_name,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
        index_handler=open(index_name,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
        overflow_handler=open(overflow_name,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
        clrscr( );
        printf("The data had been organized.\n");
        getch( );
   }

void DeleteIndexHash(int &master_handler,char master_name[],int &index_handler,char index_name[],int &overflow_handler,char overflow_name[],int &count_deleted_records)
  {
   clrscr( );
   master_record stud;
   index_record index;
   printf("Enter the old student record:\n");
   printf("Student#:");
   scanf("%ld",&stud.stud_no);
     index.stud_no=stud.stud_no;   int fk=index.stud_no % 23;
   printf("\nName:");
   scanf("%s",stud.name);
   printf("\nAddress:");
   scanf("%s",stud.address);
   printf("\nGrade:");
   scanf("%f",&stud.grade);

   int search_result=search_in_IndexHash(index_handler,index,fk);
   if (search_result==1)
      {
        index.deleted = 1;
        lseek(index_handler,-(long)sizeof(index_record),SEEK_CUR);
        write(index_handler,&index,sizeof(index_record));
        count_deleted_records++;
        if (count_deleted_records==10)
            OrganizeIndexHash(master_handler,master_name,index_handler,index_name,overflow_handler,overflow_name,count_deleted_records);
      }

   else
      {
        int seq_search_result=seq_search_in_overflow(overflow_handler,index);
        if (seq_search_result==1)
              {
               index.deleted = 1;
               lseek(overflow_handler,-(long)sizeof(index_record),SEEK_CUR);
               write(overflow_handler,&index,sizeof(index_record));
               count_deleted_records++;
               if (count_deleted_records==10)
                  OrganizeIndexHash(master_handler,master_name,index_handler,index_name,overflow_handler,overflow_name,count_deleted_records);
              }
        else if (seq_search_result==-1)
              {
               printf("This record is already deleted.\n");
               getch( );
              }

        else
             {
              printf("This record doesn't exist.\n");
              getch( );
             }
     }
 }
 
void UpdateIndexHash(int &master_handler,char master_name[],int &index_handler,char index_name[],int &overflow_handler,char overflow_name[],int &count_deleted_records)
     {
       DeleteIndexHash(master_handler,master_name,index_handler,index_name,overflow_handler,overflow_name,count_deleted_records);
       AddIndexHash(master_handler,index_handler,overflow_handler,count_deleted_records);
     }


void IndexHash( )
     {
       Load_records_inMasterFile( );
       Load_IndexHash( );
       clrscr( );
       printf ("Enter the master file name:\n");
       char master_name[20];
       scanf("%s", master_name);
       int master_handler=open(master_name,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
       printf ("Enter the index file name:\n");
       char index_name[20];
       scanf("%s", index_name);
       int index_handler=open(index_name,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
       printf ("Enter the overflow file name:\n");
       char overflow_name[20];
       scanf("%s", overflow_name);
       int overflow_handler=open(overflow_name,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
       int choice;
       int count_deleted_records=0;
       while(1)
       {
        submenu( );
        scanf("%d", & choice);
        switch (choice)
          {
           case 1: AddIndexHash(master_handler,index_handler,overflow_handler,count_deleted_records);
                   break;
           case 2: UpdateIndexHash(master_handler,master_name,index_handler,index_name,overflow_handler,overflow_name,count_deleted_records);
                   break;
           case 3: DeleteIndexHash(master_handler,master_name,index_handler,index_name,overflow_handler,overflow_name,count_deleted_records);
                   break;
           case 4: OrganizeIndexHash(master_handler,master_name,index_handler,index_name,overflow_handler,overflow_name,count_deleted_records);
                   break;
           case 5:if(count_deleted_records>0)
                   OrganizeIndexHash(master_handler,master_name,index_handler,index_name,overflow_handler,overflow_name,count_deleted_records);
          }
         if(choice==5) break;
         }
        close(master_handler);
        close(index_handler);
        close(overflow_handler);
     }

void main()
       {
        int choice;
        while (true)
              {
                menu( );
                scanf("%d", & choice);
                switch (choice)
                  {
                   case 1: IndexSorted( );
                           break;
                   case 2: IndexHash( );
                           break;
                   default: exit(0);
                  }
              }
       }



