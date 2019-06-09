#include "helpers.h" //Yazmis oldugumuz fonksiyonlar library/ klasoru altindadir.

typedef long  off_t; //Size için

	//int fd; //Dosya forZip

void zip(int argc,char **argv){
	int fd;
    short lastIndex=0;
	char *fileName;
    int sumSize=0;
    int *fileSizes;//Dosya boyutları dizisi
  	int sumFileCount=1;//Toplam Dosya sayısı
   	char * fullText;

   if(strcmp(argv[argc-1],"-o")==0)
			{
				fileName="a.sau";
				lastIndex=argc-1;
				sumFileCount=argc-3;
			}
			else
	    	{
				fileName=argv[argc-1];
	   		    lastIndex=argc-2;//Eger isim verilmisse argc-2 kadar dosya al
				sumFileCount=argc-4;
			}
			fileSizes=(int *)malloc(sizeof(int)*sumFileCount);
		//Zip
			for(int i =2;i<lastIndex;i++)
			{
			     int res = findSize(argv[i]);		    
	             if (res != -1) 
				 {
	                 sumSize+=res;
			     	 fileSizes[i-2]=res;				//Dosya boyutları diziye alındı
        
				 }
			}
			if(sumSize>209715200)
			{
			    fprintf(stderr, "Toplam dosya boyutu 200mb i geciyor:\n");
			    exit(1);
	    	}
			fd = open(fileName, O_WRONLY| O_CREAT|O_APPEND|O_TRUNC,0644);
	   	    char *record;	
		    //RECORDLAR GIRILDI
			int sumTextSize=(sumSize-sumFileCount+1);
     	
			char *strToplamBoyut=itoa(sumTextSize-1);
        	int toplamByte=strlen(strToplamBoyut);//Toplamın kac byte oldugunu aldım 10 a tamamlamak icin
 			if((10-toplamByte)!=0)
			{
 				short boslukSayisi=10-toplamByte;
 				for(int i=0;i<boslukSayisi;i++)	
 					strcat(strToplamBoyut," ");
 			} 	
		 	

			insertRecord(strToplamBoyut,fd);
			free(strToplamBoyut);


			
			fullText=(char *)malloc((sumTextSize)*sizeof(char));	
			char * c; //tek tek okunacak karakterler
			c=(char*)malloc(sizeof(char));
			int count=0;
			for(int i =2;i<lastIndex;i++)
			{
				/*Dosya  fullText e alınıyor*/
		   	    // opening the file in read mode 
    			int fp = open(argv[i], O_RDONLY); 
			    // checking if the file exist or not 
    			if (fp <0)
    			{ 
			        printf("File Not Found!=%s\n",argv[i]); 
        			exit(1);
    			} 
				while(read(fp,c,1)>0)
				{
					fullText[count]=*c;
					count++;	
				}
				count--;
				close(fp);			
				/*Dosya icerikleri fullText e alınıyor*/
		  		short kayitBoyutu=8;
		  		kayitBoyutu+=strlen(argv[i]);
		  		
				char *strBoyut;
        	 	
        	 	strBoyut = itoa(fileSizes[i-2]-1);
	         	kayitBoyutu+=strlen(strBoyut);		       //Dosya boyutunun toplam karakter sayısı kadar char dizisi olusturulacak 
          		record=(char *)calloc(sizeof(char),kayitBoyutu+1);
          		printf("%s",record);
          		strcat(record,"|");
          		strcat(record+1,argv[i]);
          		strcat(record,",");
          		
          		//PERMISSINS ------------------------------------------------------------------------------
				char * permission=getPermission(argv[i]);
      			strcat(record,permission);
          		
          		
          		//PERMISSINS ------------------------------------------------------------------------------
          		
          		strcat(record,",");
		  		strcat(record,strBoyut);
		  		strcat(record,"|");
		  		record[kayitBoyutu]='\0';
          		insertRecord(record,fd);
          		
          		free(record);
          		free(strBoyut);
          		//free(permission);
			}
			//RECORDLAR BITTI
			
			fullText[count]='\0';
			insertRecord(fullText,fd);		
			//FREE MEMORY
			free(record);
			free(c);
			free(fullText);
			free(fileSizes);
		//----------------------------------------------------- -b bolgesi SONU
		close(fd);
		printf("Dosyalar birleştirildi.\n");
}

void unZip(int argc,char **argv){
	int fileNameSize=0; //sauFileCheck params
	char * c,* file;//sauFileRead param
	int fileSauSize=0,fileCount=0,fdSau;//sauFileReadParam
	char cwd[maxSize],result[maxSize]; // current working directory
	char *fileName,*filePermission,*fileSize; //Yeni acilacak dosyaların isimlerini , izinlerini , boyutlarını tutan değişkenler
	int maxFileName=0,maxFilePermission=0,maxFileSize=0;//Yukarıdaki değişkenkenlere ne kadar yer alınacağının belirlendiği değişkenler
	int fp,i,l=0,pipeCount=0,startPoint=0,endPoint=0;//startPoint icerik baslangıcı endPoint icerik bitisi 
	int j=0,k=0;//fileDetail dizisiyle arşiv dosyasının içeriğinde | işaretleri arasında bulunan dosya adi,dosya izni,dosya boyutunun kaç haneli olduğu bilgisi saklanmaktadır,j,k fileDetail dizisinin indisleri
	

	parameterCheck(argc,argv);
	sauFileCheck(&fileNameSize,argv);
	//Arşiv dosyasını okur ve içeriğini file isimli char dizisine atar
	fdSau=open(argv[2],O_RDONLY);
	if(fdSau<0)
	{
		fprintf(stderr,"%s\n","Dosya Acilamadi !");
		exit(0);
	}
	c=malloc(1*sizeof(char));
	//.sau uzantılı dosyanın boyutu ve icerisinde kaç dosya barındırıyor
	while(read(fdSau,c,1)>0)
	{
		fileSauSize++;
		if(*c=='|')
			fileCount++;
	}
	fileCount/=2;
	int fileDetail[fileCount][3];
	//.sau uzantılı dosya icerigini tutacak dizi
	file=malloc(fileSauSize * sizeof(char));
	lseek(fdSau,10,SEEK_SET);
	for(i=0;read(fdSau,c,1)>0;i++)
	{
		file[i]=*c;
	}
	file[i]='\0';
	
	close(fdSau);
	free(c);
	for(i=0;i<fileCount;i++)
	{
		for(j=0;j<3;j++)
		{
			fileDetail[i][j]=0;
		}
	}
	pipeCount=fileCount*2;
	for (i=0;pipeCount!=0;i++)
	{
		if(file[i]=='|')
			pipeCount--;
		startPoint++;
	}
	//fileDetail indisler
	j=k=0;
	//| count
	pipeCount=fileCount*2;
	for(i=0;pipeCount!=0;i++)
	{
		if(file[i]=='|')
			pipeCount--;	
		if(pipeCount%2==0 && pipeCount!=0) 
		{
			k=0;
			j++;		
		}
		if(file[i]==',')
			k++;
		else if(file[i]!=',' && file[i]!='|')
			fileDetail[j][k]++;
	}
	
	for(i=0;i<fileCount;i++)
	{
		if(fileDetail[i][0]>maxFileName)
			maxFileName=fileDetail[i][0];
		if(fileDetail[i][1]>maxFilePermission)
			maxFilePermission=fileDetail[i][1];
		if(fileDetail[i][2]>maxFileSize)
			maxFileSize=fileDetail[i][2];
	}
	fileName=malloc((maxFileName)*sizeof(char));
	strcat(cwd,fileName);
	filePermission=malloc((maxFilePermission)*sizeof(char));
	fileSize=malloc((maxFileSize)*sizeof(char));
	strcpy(result,"");
	//fileDetail indisler
	j=k=0;
	//| count
	pipeCount=fileCount*2;
	for(i=0;pipeCount!=0;i++)
	{
		if(file[i]=='|')
			pipeCount--;	
		if(pipeCount%2==0 && pipeCount!=0) 
		{
			k=0;
			l=0;
			j++;
			
			//DOSYA İSLEMLERİ
			if(argc==4 && argv[3]!="")
				chdir(argv[3]);
			fp=open(fileName,O_WRONLY | O_CREAT | O_TRUNC);
			if(fp<0)
			{
				fprintf(stderr,"%s\n","Dosya Olusturulamadi !");
				exit(0);
			}
			endPoint=startPoint+atoi(fileSize);
			getcwd(cwd, sizeof(cwd));
			strcat(cwd,"/");
			strcat(cwd,fileName);
			strcat(result,fileName);
			strcat(result," ");
			c=malloc(1*sizeof(char));
			while(startPoint != endPoint && file[startPoint]!='\0')
			{
				*c=file[startPoint];
				write(fp,c,1);
				startPoint++;
			}
			if (chmod (cwd,strtol(filePermission, 0, 8)) < 0)
		    	{
				fprintf(stderr,"%s\n","İzin verilemedi !");
				exit(1);
		    	}
			free(c);
		}
		if(pipeCount==0)
		{
			//DOSYA İSLEMLERİ
			if(argc==4 && argv[3]!="")
				chdir(argv[3]);
			fp=open(fileName,O_WRONLY | O_CREAT | O_TRUNC);
			if(fp<0)
			{
				fprintf(stderr,"%s\n","Dosya Olusturulamadi !");
				exit(0);
			}
			endPoint=startPoint+atoi(fileSize);
			//Calıstıgımız directory
			getcwd(cwd, sizeof(cwd));
			strcat(cwd,"/");
			strcat(cwd,fileName);
			strcat(result,fileName);
			strcat(result," ");
			c=malloc(1*sizeof(char));
			while(startPoint != endPoint && file[startPoint]!='\0')
			{
				*c=file[startPoint];
				write(fp,c,1);
				startPoint++;
			}
		    	if (chmod (cwd,strtol(filePermission, 0, 8)) < 0)
		    	{
				fprintf(stderr,"%s\n","İzin verilemedi !");
				exit(1);
		    	}
			free(c);
		
		}
		if(file[i]==',')
		{
			l=0;
			k++;
		}			
		else if(file[i]!=',' && file[i]!='|')
		{
			if(k==0)
			{
				fileName[l]=file[i];
				l++;
				fileName[l]='\0';
			}
			if(k==1)
			{
				filePermission[l]=file[i];
				l++;
				filePermission[l]='\0';
			}
			if(k==2)
			{
				fileSize[l]=file[i];
				l++;
				fileSize[l]='\0';
			}
		}
	}
		
	strcat(result,"dosyaları ");
	if(argc==4)
	{
		strcat(result,argv[3]);
		strcat(result," dizinine açıldı");
	}
	else
		strcat(result,("açıldı"));	
	printf("%s\n",result);
	free(fileName);
	free(filePermission);
	free(fileSize);
	free(file);
}


void main(int argc,char **argv)
{

    if(argc==1) { 
		fprintf(stderr, "Hatalı kullanım parametrelerinizi kontrol ediniz:\n"); 
		exit(1); 
	}	
	if(strcmp(argv[1],"-b")==0)
	{
		
		if ( argc < 4 
		|| (strcmp(argv[argc-1],"-o")!=0 && strcmp(argv[argc-2],"-o")!=0) 
		|| argc>36)
		{ 
	   	    fprintf(stderr, "Hatalı kullanım parametrelerinizi kontrol ediniz2:\n"); 
		    exit(1); 
		}
		else 
		{
			for(int i=2; strcmp(argv[i],"-o")!=0;i++){
			 if(checkAscii(argv[i])==0) { printf("Girilen dosyalardan biri veya birkaçı metin belgesi değil.\n"); exit(1); }
			}
			//----------------------------------------------------- -b bolgesi
			zip(argc,argv);
	  }		
	}
	
  else	if(strcmp(argv[1],"-a")==0)
	{
		//UnZip
		//----------------------------------------------------- -a bolgesi
		unZip(argc,argv);
	}
		//----------------------------------------------------- -a bolgesi Sonu
	else 
	{
		fprintf(stderr, "Hatalı kullanım parametrelerinizi kontrol ediniz2:\n"); 
		    exit(1); 
	}
  	exit(0);
}
