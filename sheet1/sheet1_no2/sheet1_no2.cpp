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
      printf("1.Ordered sequential file organization.\n");
      printf("2.Directory look up random (unordered).\n");
      printf("3.Exit.\n");
      printf("Enter your choice:");
     }

void sub_menu( )
     {
      clrscr( );
      printf("1.Add.\n");
      printf("2.Update.\n");
      printf("3.Delete.\n");
      printf("4.Display students having grade greater than 85 w.r.t. name.\n");
      printf("5.Organize.\n");
      printf("6.Back.\n");
      printf("Enter your choice:");
     }

struct stud_record
     {
      unsigned int deleted:1;
      unsigned long int stud_no;
      char name[30];
      char address[35];
      float grade;
     };

struct directory_record
     {
       unsigned long int stud_no;
       unsigned long int relative_address;
     };

void Load_records_inMasterFile( ) //write data in a binary file
     {
      FILE *fp = fopen ("data.txt","r");
      FILE *master_fp = fopen("data_binary","wb");
      stud_record stud; int stud_size=sizeof(stud_record);

      char stud_no[11];
      //unsigned long int: 32 bits -->from 0 to 4,294,967,295 (10 digits)
      char name[30];
      char address[35];
      char grade[11];

      int i; char ch=fgetc(fp);
      while (ch!= EOF)
            {
             stud.deleted=ch;
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

void Load_records_inDirectoryFile( )
     {
      FILE *fp = fopen ("data.txt","r");
      FILE *directory_fp = fopen("directory","wb");
      directory_record dir; int directory_size=sizeof(directory_record);
      int stud_size=sizeof(stud_record);

      char stud_no[11];
      //unsigned long int: 32 bits -->from 0 to 4,294,967,295 (10 digits)


      int i; char ch=fgetc(fp);   int j=0;
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
             dir.stud_no=atol(stud_no);


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


             dir.relative_address=j*stud_size;
             fwrite(&dir,directory_size,1,directory_fp);
             ch=fgetc(fp);
             j++;
            }
      fclose(fp);
      fclose(directory_fp);
     }


void Display_BinaryFile( )
     {
        int file_handler=open("data_binary",O_RDONLY|O_BINARY|S_IREAD);
        int stud_size=sizeof(stud_record);
        stud_record stud;
        while (!eof(file_handler))
              {
               read(file_handler,&stud,stud_size);
               printf("%d ",stud.deleted);
               printf("%ld ",stud.stud_no);
               printf("%s ",stud.name);
               printf("%s ",stud.address);
               printf("%4.2f\n",stud.grade);
              }
        getch();
        close(file_handler);
     }

void Display_DirectoryFile( )
     {
        int file_handler=open("directory",O_RDONLY|O_BINARY|S_IREAD);
        int dir_size=sizeof(directory_record);
        directory_record dir;
        while (!eof(file_handler))
              {
               read(file_handler,&dir,dir_size);
               printf("%ld ",dir.stud_no);
               printf("%ld \n",dir.relative_address);
              }
        getch();
        close(file_handler);
     }


void load_display_buffer(int file_handler)
   {
    int i=1;
    int j=filelength(file_handler)/245;  //size of the file in blocks
     //Assume that the block size is 245 bytes.
    //means that each block contains five records
    int k=490/245; //size of buffer in blocks; BUFSIZ= 490 bytes.
    int m= ceil((double)j/k); //divide the file to a set of buffers
          //Each time take k blocks and put them in buffer

    const int buffer_records_no=490/sizeof(stud_record);
    stud_record buffer[buffer_records_no];
    int no_bytes_placedin_buffer,no_records_placedin_buffer,n;
    clrscr( );
    lseek(file_handler, 0L, SEEK_SET);
    while (i<=m)
        {
         /*read next k blocks of the file into the buffer or
         if there are less than k blocks remaining,
         then read in the remaining blocks;*/
         no_bytes_placedin_buffer=read(file_handler,buffer,490);
        /*number of records placed in the buffer =
         number of bytes placed in the buffer / record size (49 bytes)*/
         no_records_placedin_buffer=no_bytes_placedin_buffer/sizeof(stud_record);
         for (n=0;n<no_records_placedin_buffer;n++)
             {
              printf("%ld ",buffer[n].stud_no);
              printf("%s ",buffer[n].name);
              printf("%s ",buffer[n].address);
              printf("%4.2f\n",buffer[n].grade);
             }
         i++;
       }
    getch();
   }

/*
void ExternalSorting(int file_handler)
     {
       //The sort-merge function for external sorting:
       int i=1;
       int j=filelength(file_handler)/245;  //size of the file in blocks
         //Assume that the block size is 245 bytes.
         //means that each block contains five records
       int k=490/245; //size of buffer in blocks; BUFSIZ= 490 bytes.
       const int m= ceil((double)j/k); //divide the file to a set of buffers
                      //Each time take k blocks and put them in buffer

       //Sort Phase
       const int buffer_records_no=490/sizeof(stud_record);
       stud_record buffer[buffer_records_no];
       int no_bytes_placedin_buffer,no_records_placedin_buffer;
       int temp_file_handler;
       while (i<=m)
       {
        /*read next k blocks of the file into the buffer or
        if there are less than k blocks remaining,
        then read in the remaining blocks;
        no_bytes_placedin_buffer=read(file_handler,buffer,490);
        /*number of records placed in the buffer =
         number of bytes placed in the buffer / record size (49 bytes)
         no_records_placedin_buffer=no_bytes_placedin_buffer/sizeof(stud_record);
         //sort the records in the buffer and write as a temporary subfile;
         quick_sort(buffer,0,no_records_placedin_buffer);
         temp_file_handler=open("temp" + i,1); //1:write.
         write(temp_file_handler,buffer,no_bytes_placedin_buffer);
         close(temp_file_handler);
         i++;
       }

       //Merge Phase: merge subfiles until only 1 remains
       i=1;
       int p=ceil(log(m)/log(k-1));
       //p is the number of passes for the merging phase
       j=m;  //total number of subfiles.
       int n,q;
       while (i<=p)   //Loop p passes for merging j subfiles into 1 file.
           {
            n=1;
            q=ceil((double)j/(k-1)); //number of subfiles to write in this pass
            while (n<=q)
                  {
                   /*read next k-1 subfiles or
                   remaining subfiles(from previous pass)one block at a time;
                   merge and write as new subfile one block at a time;


                   n++;
                  }
           j=q;
           i++;
          }
     }
*/


int binary_search_in_file(int file_handler,stud_record stud)
   {
    stud_record student;
    int stud_size=sizeof(stud_record);
    int low=0;
    int high=(filelength(file_handler)/stud_size)-1;
    int mid;
    long  k;
    while(low<=high)
	 {
	  mid=(low+high)/2;
          lseek(file_handler,mid*stud_size,SEEK_SET);
          read(file_handler,&student,stud_size);
          if (stud.stud_no==student.stud_no)
              {
               if (student.deleted==1)
                   return -1;
               else return 1;
              }
	  if (stud.stud_no<student.stud_no)  high=mid-1;
	  else low=mid+1;
	 }
    return 0;
  }

int seq_search_in_trans(int trans_handler,stud_record stud)
  {
    stud_record student;
    int stud_size=sizeof(stud_record);
    lseek(trans_handler, 0L, SEEK_SET);
    while (!eof(trans_handler))
         {
          read(trans_handler,&student,stud_size);
          if (stud.stud_no == student.stud_no)
              {
               if (student.deleted==1)
                   return -1;
               else return 1;
              }
         }
    return 0;
  }


int find_pivot(stud_record A[],int i,int j)
{
/*Determine whether the keys of A[i],..,A[j] are all the same.
"find_pivot" never finds two different keys, it returns -1.
Otherwise,it returns the index of the larger of the first two different keys,
this larger key becomes the pivot element.*/

/*The procedure "find pivot":
returns -1 if A[i],...,A[j] have identical keys(not different),
otherwise,returns the index of the larger of the left most two different keys*/
 stud_record first_key; //the value of first key found.
 int k; // runs left to right looking for a different key.
 first_key=A[i];
 for (k=i+1;k<=j;k++) // scan for different key.
     if (strcmp(A[k].name,first_key.name)>0) // select larger key.
	 return k;
     else if (strcmp(A[k].name,first_key.name)<0)
	 return i;
 return -1; // different keys were never found.
}

void swap_master(stud_record &x,stud_record &y)
{
  /*The procedure "swap":
  swap exchanges the values of x and y */
  stud_record temp;
  temp=x;
  x=y;
  y=temp;
}

int partition(stud_record A[],int i,int j,stud_record pivot)
{
/*The procedure "partition":
 partition A[i],.., A[j] so keys < pivot are at the left
 and keys >= pivot are on the right.
 Returns the beginning of the group on the right. */
 int L,R; // the cursors
 L=i;
 R=j;
 do
 {
  swap_master(A[L],A[R]);
   // now the scan phase begins
  while (strcmp(A[L].name,pivot.name)<0)
       {
	L++;
       }
  while (strcmp(A[R].name,pivot.name)>=0)
       {
	R--;
       }
 }
 while(L<=R);

 return L;

}

void quick_sort(stud_record A[],int i,int j)
{
  /*The procedure "quick_sort":
    sorts elements A[i],...,A[j] of external array A */
    stud_record pivot; // the pivot value
    int  pivot_index; // the index of an element of A where key is the pivot.
    int  k;// the beginning index for group of elements >= pivot
    pivot_index=find_pivot(A,i,j);
    if (pivot_index != -1)   // do nothing if all keys are equal
	 {
	   pivot= A[pivot_index];
	   k=partition(A,i,j,pivot);
	   quick_sort(A,i,k-1);
	   quick_sort(A,k,j);
	 }
}

void swap_directory(directory_record &x,directory_record &y)
{
  /*The procedure "swap":
  swap exchanges the values of x and y */
  directory_record temp;
  temp=x;
  x=y;
  y=temp;
}

void bubble_sort(directory_record A[],int n)
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
	       swap_directory(A[j],A[j-1]);
	       flag=1; // A swap has happened.
	      }
	   }
	if (flag==0) break;//There is no swap has happened in previous pass,
       }          	   // so the List is sorted.
}

void SortTransaction(int trans_handler,directory_record arr[], int &n)
     {
       int stud_size = sizeof(stud_record);
       stud_record trans_record;
       int i=0,j=0;
       lseek(trans_handler, 0L, SEEK_SET);
       while(!eof(trans_handler))
            {
             read(trans_handler,&trans_record,stud_size);
             if (trans_record.deleted==0) //not deleted.
                {
                 arr[i].stud_no=trans_record.stud_no;
                 arr[i].relative_address=j*stud_size;
                 i++;
                }
             j++;
            }
       n=i;
       bubble_sort(arr,n);
     }

void Merge2Files(int &master_handler,char name[],int trans_handler,directory_record arr[],int n)
{
 int i=0,j=0;
 int temp_handler=open("temporary",O_CREAT|O_TRUNC|O_WRONLY|O_BINARY,S_IWRITE);
 stud_record master_record,trans_record;
 int stud_size = sizeof(stud_record);
 lseek(master_handler,0L, SEEK_SET);
 int master_records_no=filelength(master_handler)/stud_size;
 int flag1=1;  int flag2=1;
 while (i<master_records_no && j<n)
 {
   if (flag1==1)
    do
     {
      read(master_handler,&master_record,stud_size);
      i++;
     }
    while(master_record.deleted==1 && i<master_records_no);
   if (i>=master_records_no) break;

   if (flag2==1)
     {
      lseek(trans_handler,arr[j].relative_address, SEEK_SET);
      read(trans_handler,&trans_record,stud_size);
     }

   if(master_record.stud_no<trans_record.stud_no)
     {
      write(temp_handler,&master_record,stud_size);
      flag1=1;
      flag2=0;
     }
   else
    {
     write(temp_handler,&trans_record,stud_size);
     j++;
     flag1=0;
     flag2=1;
    }
 }
 if(i>=master_records_no)
   for (int x=j;x<n;x++)
       {
        lseek(trans_handler,arr[x].relative_address, SEEK_SET);
        read(trans_handler,&trans_record,stud_size);
        write(temp_handler,&trans_record,stud_size);
       }
 else if(j>=n)
   while (i<master_records_no)
       {
         do
           {
            read(master_handler,&master_record,stud_size);
            i++;
           }
         while(master_record.deleted==1 && i<master_records_no);
         if (master_record.deleted==0)
             write(temp_handler,&master_record,stud_size);
       }

  close(temp_handler);

  close(master_handler);
  remove(name);
  rename("temporary",name);
  master_handler=open(name,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
}

void UpdateTransaction(int &trans_handler)
     {
      close(trans_handler);
      trans_handler=open("transaction",O_TRUNC|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
      //O_TRUNC	If the file exists, its length is truncated to 0. The file attributes remain unchanged.
     }

void merge_func(int &master_handler,char name[ ],int &trans_handler,int &count_trans_records,int &count_deleted_records)
     {
      count_trans_records=0;
      count_deleted_records=0;
      directory_record arr[20]; int n;
      SortTransaction(trans_handler,arr,n);
      Merge2Files(master_handler,name,trans_handler,arr,n);
      UpdateTransaction (trans_handler);
      clrscr( );
      printf ("The data has been organized.\n");
      getch ( );
     }

void AddSeqOrdered(int &master_handler,char name[],int &trans_handler,int &count_trans_records,int &count_deleted_records)
     {
       clrscr( );
       stud_record stud;
       printf("Enter the new student record :\n");
       stud.deleted=0;
       printf("Student#:");
       scanf("%ld",&stud.stud_no);
       printf("\nName:");
       scanf("%s",stud.name);
       printf("\nAddress:");
       scanf("%s",stud.address);
       printf("\nGrade:");
       scanf("%f",&stud.grade);
       int binary_search_result=binary_search_in_file(master_handler,stud);
       if (binary_search_result==1)
          {
           printf("\nThere exists a student having the same student#.");
           getch( );
          }

       else if (binary_search_result==-1)
          {
           //don't shift downwords
           lseek(master_handler,-(long)sizeof(stud_record),SEEK_CUR);
           write(master_handler,&stud,sizeof(stud_record));
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
                   lseek(trans_handler,-(long)sizeof(stud_record),SEEK_CUR);
                  }
              write (trans_handler,&stud,sizeof(stud_record));
              count_trans_records ++;
              if (count_trans_records==10)
                  merge_func(master_handler,name,trans_handler,count_trans_records,count_deleted_records);
             }
        }
     }

void DeleteSeqOrdered(int &master_handler,char name[],int &trans_handler,int &count_trans_records,int &count_deleted_records)
  {
   clrscr( );
   stud_record stud;
   printf("Enter the old student record:\n");
   printf("Student#:");
   scanf("%ld",&stud.stud_no);
   printf("\nName:");
   scanf("%s",stud.name);
   printf("\nAddress:");
   scanf("%s",stud.address);
   printf("\nGrade:");
   scanf("%f",&stud.grade);
   int binary_search_result=binary_search_in_file(master_handler,stud);
   if (binary_search_result==1)
      {
        stud.deleted = 1;
        lseek(master_handler,-(long)sizeof(stud_record),SEEK_CUR);
        write(master_handler,&stud,sizeof(stud_record));
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
               lseek(trans_handler,-(long)sizeof(stud_record),SEEK_CUR);
               write(trans_handler,&stud,sizeof(stud_record));
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
        merge_func(master_handler,name,trans_handler,count_trans_records,count_deleted_records);
  }

void UpdateSeqOrdered(int &master_handler,char name[],int &trans_handler,int &count_trans_records,int &count_deleted_records)
     {
       DeleteSeqOrdered(master_handler,name,trans_handler,count_trans_records,count_deleted_records);
       AddSeqOrdered(master_handler,name,trans_handler,count_trans_records,count_deleted_records);
     }

void Display_studs_grade_greater85_SeqOrdered(int master_handler,int trans_handler)
     {
        stud_record records_arr[20]; int n=0;
        int stud_size=sizeof(stud_record);
        stud_record stud;
        lseek(master_handler, 0L, SEEK_SET);
        while (!eof(master_handler))
              {
               read(master_handler,&stud ,stud_size);
               if (stud.deleted==0 && stud.grade > 85.00)
                  {
                   records_arr[n]=stud;
                   n++;
                  }
              }
        lseek(trans_handler, 0L, SEEK_SET);
        while (!eof(trans_handler))
              {
               read(trans_handler,&stud ,stud_size);
               if (stud.deleted==0 && stud.grade > 85.00)
                  {
                   records_arr[n]=stud;
                   n++;
                  }
              }
        quick_sort(records_arr,0,n-1);
        clrscr( );
        for (int i=0;i<n;i++)
            {
               printf("%ld ",records_arr[i].stud_no);
               printf("%s ",records_arr[i].name);
               printf("%s ",records_arr[i].address);
               printf("%4.2f\n",records_arr[i].grade);
            }
        getch();
     }


void seq_ordered( )
     {
       Load_records_inMasterFile( );
       char master[20];
       clrscr( );
       printf ("Enter the file name:\n");
       scanf("%s", master);
       int master_handler=open(master,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
       int trans_handler=open("transaction",O_CREAT|O_TRUNC|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
       int choice; int count_trans_records=0;
       int count_deleted_records=0;
       while(1)
       {
        sub_menu( );
        scanf("%d", & choice);
        switch (choice)
          {
           case 1: AddSeqOrdered(master_handler,master,trans_handler,count_trans_records,count_deleted_records);
                   break;
           case 2: UpdateSeqOrdered(master_handler,master,trans_handler,count_trans_records,count_deleted_records);
                   break;
           case 3: DeleteSeqOrdered(master_handler,master,trans_handler,count_trans_records,count_deleted_records);
                   break;
           case 4: Display_studs_grade_greater85_SeqOrdered(master_handler,trans_handler);
                   break;
           case 5: merge_func(master_handler,master,trans_handler,count_trans_records,count_deleted_records);
                   break;
           case 6:if(count_trans_records>0 || count_deleted_records>0)
                   merge_func(master_handler,master,trans_handler,count_trans_records,count_deleted_records);
          }
         if(choice==6) break;
         }
        close(master_handler);
        close(trans_handler);
     }

int seq_search_in_directory(int master_handler,int directory_handler,stud_record stud)
  {
    directory_record dir_record;
    stud_record student;
    int directory_size=sizeof(directory_record);
    int stud_size=sizeof(stud_record);
    lseek(directory_handler, 0L, SEEK_SET);
    while (!eof(directory_handler))
         {
          read(directory_handler,&dir_record ,directory_size);
          if (stud.stud_no == dir_record.stud_no)
              {
               lseek(master_handler,dir_record.relative_address,SEEK_SET);
               read(master_handler,&student,stud_size);
               if (student.deleted==1)
                   return -1;
               else return 1;
              }
         }
    return 0;
  }

void AddDirectUnordered(int master_handler,int directory_handler,int &count_deleted_records)
     {
      clrscr( );
      stud_record stud;
      printf("Enter the new student record:\n");
      printf("Student#:");
      scanf("%ld",&stud.stud_no);
      printf("\nName:");
      scanf("%s",stud.name);
      printf("\nAddress:");
      scanf("%s",stud.address);
      printf("\nGrade:");
      scanf("%f",&stud.grade);
      directory_record dir_record,temp_record;
      int seq_search_result=seq_search_in_directory(master_handler,directory_handler,stud);
      if (seq_search_result==1)
          {
           printf("\nThere exists a student having the same student#.");
           getch( );
          }

      else if (seq_search_result==-1)
          {
           //don't shift downwords
           lseek(master_handler,-(long)sizeof(stud_record),SEEK_CUR);
           write(master_handler,&stud,sizeof(stud_record));
           count_deleted_records -- ;
          }

      else
          {
           dir_record.stud_no=stud.stud_no;
           dir_record.relative_address=filelength(master_handler);
           write(directory_handler,&dir_record,sizeof(directory_record));
           write(master_handler,&stud,sizeof(stud_record));
          }

     }

void OrganizeDirectUnordered(int &master_handler,char master[],int &directory_handler,char directory[],int &count_deleted_records)
   {
        count_deleted_records=0;
        stud_record stud;
        directory_record dir_record;
        int stud_size=sizeof(stud_record);
        int directory_size=sizeof(directory_record);

        int temp1_handler=open("temporary1",O_CREAT|O_TRUNC|O_WRONLY|O_BINARY,S_IWRITE);
        int temp2_handler=open("temporary2",O_CREAT|O_TRUNC|O_WRONLY|O_BINARY,S_IWRITE);

        lseek(master_handler, 0L, SEEK_SET);
        lseek(directory_handler, 0L, SEEK_SET);
        while(!eof(master_handler))
             {
              read(master_handler,&stud,stud_size);
              read(directory_handler,&dir_record,directory_size);
              if(stud.deleted==0)
                {
                 write(temp1_handler,&stud,stud_size);
                 write(temp2_handler,&dir_record,directory_size);
                }
             }
        close(temp1_handler);
        close(temp2_handler);
        close(master_handler);
        close(directory_handler);


        remove(master);
        remove(directory);
        rename("temporary1",master);
        rename("temporary2",directory);
        master_handler=open(master,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
        directory_handler=open(directory,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
        clrscr( );
        printf("The data had been organized.\n");
        getch( );

   }

void DeleteDirectUnordered(int &master_handler,char master[],int &directory_handler,char directory[],int &count_deleted_records)
  {
   clrscr( );
   stud_record stud;
   printf("Enter the old student record:\n");
   printf("Student#:");
   scanf("%ld",&stud.stud_no);
   printf("\nName:");
   scanf("%s",stud.name);
   printf("\nAddress:");
   scanf("%s",stud.address);
   printf("\nGrade:");
   scanf("%f",&stud.grade);
   directory_record dir_record;
   int seq_search_result=seq_search_in_directory(master_handler,directory_handler,stud);
   if (seq_search_result==1)
      {
        stud.deleted = 1;
        lseek(master_handler,-(long)sizeof(stud_record),SEEK_CUR);
        write(master_handler,&stud,sizeof(stud_record));
        count_deleted_records++;
        if (count_deleted_records==10)
          OrganizeDirectUnordered(master_handler,master,directory_handler,directory,count_deleted_records);
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

void UpdateDirectUnordered(int &master_handler,char master[],int &directory_handler,char directory[],int &count_deleted_records)
     {
       DeleteDirectUnordered(master_handler,master,directory_handler,directory,count_deleted_records);
       AddDirectUnordered(master_handler,directory_handler,count_deleted_records);
     }

void Display_studs_grade_greater85_DirectUnordered(int master_handler,int directory_handler)
     {
       stud_record records_arr[20]; int n=0;
       stud_record stud;
       directory_record dir_record;
       int stud_size=sizeof(stud_record);
       int directory_size=sizeof(directory_record);
       lseek(master_handler, 0L, SEEK_SET);
       lseek(directory_handler, 0L, SEEK_SET);
       while (!eof(directory_handler))
              {
               read(directory_handler,&dir_record,directory_size);
               read(master_handler,&stud,stud_size);
               if (stud.deleted==0 && stud.grade > 85.00)
                  {
                   records_arr[n]=stud;
                   n++;
                  }
              }
        quick_sort(records_arr,0,n-1);
        clrscr( );
        for (int i=0;i<n;i++)
            {
               printf("%ld ",records_arr[i].stud_no);
               printf("%s ",records_arr[i].name);
               printf("%s ",records_arr[i].address);
               printf("%4.2f\n",records_arr[i].grade);
            }
        getch();
     }

void directory_unordered( )
     {
       Load_records_inMasterFile( );
       Load_records_inDirectoryFile( );
       clrscr( );
       printf ("Enter the master file name:\n");
       char master[20];
       scanf("%s", master);
       int master_handler=open(master,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
       printf ("Enter the directory file name:\n");
       char directory[20];
       scanf("%s", directory);
       int directory_handler=open(directory,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
       int choice;
       int count_deleted_records=0;
       while(1)
       {
        sub_menu( );
        scanf("%d", & choice);
        switch (choice)
          {
           case 1: AddDirectUnordered(master_handler,directory_handler,count_deleted_records);
                   break;
           case 2: UpdateDirectUnordered(master_handler,master,directory_handler,directory,count_deleted_records);
                   break;
           case 3: DeleteDirectUnordered(master_handler,master,directory_handler,directory,count_deleted_records);
                   break;
           case 4: Display_studs_grade_greater85_DirectUnordered(master_handler,directory_handler);
                   break;
           case 5: OrganizeDirectUnordered(master_handler,master,directory_handler,directory,count_deleted_records);
                   break;
           case 6:if(count_deleted_records>0)
                   OrganizeDirectUnordered(master_handler,master,directory_handler,directory,count_deleted_records);
          }
         if(choice==6) break;
         }
        close(master_handler);
        close(directory_handler);
     }

void main()
       {

    //  Display_BinaryFile( );
    // Display_DirectoryFile( );
        

         //int file_handler=open("data_binary",0);
        //load_display_buffer( );
        // close(file_handler);

       /*int file_handler=open("data_binary",0);
        stud_record stud={8,"Mohamed","Cairo",80};
        if (binary_search_in_file(file_handler,stud)==1)
           printf("found.\n");
        else printf("Not found.\n");
        getch( );
        close(file_handler);*/

        int choice;
        while (true)
              {
                menu( );
                scanf("%d", & choice);
                switch (choice)
                  {
                   case 1: seq_ordered( );
                           break;
                   case 2: directory_unordered( );
                           break;
                   default: exit(0);
                  }
              }
       }




