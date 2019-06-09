#include "helpers.h"
typedef long  off_t; //Size için
	int fd; //Dosya forZip
	int convertDecimalToOctal(unsigned short decimalNumber){
    	int octalNumber = 0, i = 1;
    	while (decimalNumber != 0)
    	{
        	octalNumber += (decimalNumber % 8) * i;
        	decimalNumber /= 8;
        	i *= 10;
    	}

    return octalNumber;
	}
	
	short checkAscii(char * fileName){
		char *c;
		int fd;
		fd=open(fileName,O_RDONLY);
		c=(char *)malloc(sizeof(char));
	if(fd<0){
		printf("Dosya Bulunamadı yada açılamadı \n");
		exit(1);
	}
	while(read(fd,c,1)>0){
		
		if(*c<0 || *c>127) return 0;
			
	}
	return 1;
	
	}
	
	char * itoa(int number){
		char *toStr;
			toStr=(char*)malloc(sizeof(char)*10);
    	 	sprintf(toStr, "%d", number);
    	 	toStr[9]='\0';
    	 	return toStr;
	}


	char * getPermission(char file_name[]){
			 int i;
 			 struct stat buf;
  			 int exists;

    		exists = stat(file_name, &buf);
   		    if (exists < 0) {
      			fprintf(stderr, "%s not found\n", file_name);
      			exit(1);
    		}
    		 else {
			int octalType=convertDecimalToOctal(buf.st_mode);
			
      		char * permission;
      		permission=itoa(octalType);
      		permission[7]='\0';		
       		return (permission+2);	
    }
	}
	
	
	off_t findSize(char file_name[]) 
	{ 
	    int i;
  	 	struct stat buf;
  		int exists;
	 	
	        exists = stat(file_name, &buf);
            if (exists < 0) {
	    	    fprintf(stderr, "%s not found\n", file_name);
	    		exit(1);
	    	} 
	    else return buf.st_size;	    
  
	} 
	void insertRecord(char record[],int fd)
	{
   		if(fd<0) { fprintf(stderr, "HATA \n");}
   		write(fd,record,strlen(record));
	}
	//------------------------------------------------------------------
/*
	Ekrandan girilen parametre sayısı 3 ya da 4 olmadıgında hata verir ve çıkış yapar . 
	Ekrandan girilen parametre sayısı 4 ve 4. parametre (dosya adı) boş değilse istenilen klasörü oluşturur.
*/
void parameterCheck(int parameterCount,char ** params)
{
	if(parameterCount!=3 && parameterCount!=4){
		fprintf(stderr,"%s\n","Eksik ya da hatalı giriş yaptınız.");
		exit(0);
	}
	if(parameterCount==4 && params[3]!=""){
		if(mkdir(params[3],0777)==1)
		{
			fprintf(stderr,"%s\n","Dizin olusturulamadi.");
			exit(0);
		}
	}
}

/*
	Ekrandan parametre olarak girilen arşiv dosyasının uzantısı .sau uzantılı değilse hata verir ve çıkış yapar.
*/
void sauFileCheck(int * fileNameSize,char ** params)
{
	int i,sauFlag;
	//.sau uzantılı dosyanın dosya ismi uzunluğu
	for(i=0;params[2][i]!='\0';i++){
		*fileNameSize=(*fileNameSize)+1;
	}
	//dosya .sau uzantılımı ? .sau uzantılı ise sauFlag=1
	for(i=0;i<(*fileNameSize)-2;i++){
		if(params[2][i-1]=='.' && params[2][i]=='s'&& params[2][i+1]=='a'&& params[2][i+2]=='u' && i==(*fileNameSize)-3)
			sauFlag=1;
	}
	//dosya .sau uzantılı değilse hata ver ve çıkış yap
	if(sauFlag==0){
		fprintf(stderr,"%s\n","Arşiv dosyası uygun değil veya bozuk!.");
		exit(0);
	}
}

