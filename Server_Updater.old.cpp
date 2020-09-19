#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char **codes;
bool search(char *search, char *site);
int num_codes = 0;
int main(){
	char **list, buff, **to_download;
	FILE *list_file;
	int i, j,list_size = 0, code_size = 0, download_size = 0;
	bool found,done = false;
	system("/usr/bin/firefox --new-window https://nhentai.net/favorites/ & ./save_html");
	//system("nhentai --cookie \"csrftoken=t3CLrmYdcaiudSIlVjBmNATCThZsMkfGtj52EvS9evIXwIJeJX6Nanyo9rwjYoG2; sessionid=04ih2eq6g2tmy3qkebmo931uqmp1emrw\"");
	//system("nhentai -F > Archives/Favorites.txt");
	search((char*)("<div class=\"gallery-favorite\" data-id=\""),(char*)("Site/site.txt"));
	list_file = fopen("Files/list.txt", "a+");
	list = NULL;
	do{	
		list = (char**)realloc(list,(1+list_size)*sizeof(*list));
		list[list_size] = NULL;
		list[list_size] = (char*)malloc(6*sizeof(char));
		do{
			buff = fgetc(list_file);
			if(buff == '\n' || buff == EOF){
				code_size = 0;
			}
			else{
				list[list_size][code_size] = buff;
				code_size++;
			}
		}while(buff != EOF && buff != '\n');
		
		list_size++;
	}while(buff != EOF);
	fclose(list_file);

	printf("\nCodes: ");
	for(i=0;i<num_codes;i++){
		printf("%s, ",codes[i]);
	}
	printf("\nList: ");
	for(i=0;i<list_size;i++){
		printf("%s, ",list[i]);
	}
	//system("rm -f Archives/to_download.txt");
	//system("touch Archives/to_download.txt");
	printf("\n");

	to_download = NULL;
	for(i=0;i<num_codes;i++){
		found=false;
		for(j=0;j<list_size;j++){
			if(strcmp(list[j],codes[i]) == 0){
					found = true;
			}
		}
		if(!found){
			to_download =(char**)realloc(to_download,(1+download_size)*sizeof(*to_download));
			to_download[download_size] = (char*)malloc(6*sizeof(char));	
			to_download[download_size] = codes[i];
			download_size++;
		}
	}
	printf("\nDownload: ");
	for(i=0;i<download_size;i++){
		printf("%s, ",to_download[i]);
	}
	for(i=0;i<list_size;i++){
		list[i] = NULL;
		free(list[i]);
	}
	for(i=0;i<num_codes;i++){
		codes[i] = NULL;
		free(codes[i]);
	}
	for(i=0;i<download_size;i++){
		to_download[i] = NULL;
		free(to_download[i]);
	}
	return(1);
}
bool search(char search[100], char site[100]){
	int i,char_num = 0,num_char=0;
	char *curr_string, buff, hold[6];
	FILE *site_file;
	for(i = 0; i < 100; i++){
		if(search[i] == '\0'){
			char_num = i-1;
			break;
		}
	}
	curr_string = (char*)malloc((char_num)*sizeof(char));
	for(i = 0; i < char_num; i++){
		curr_string[i] = 'a';
	}
	site_file = fopen(site, "r");
	codes = NULL;
	do{
		for(i = 0; i < char_num; i++){
			curr_string[i] = curr_string[i+1];
		}
		curr_string[char_num] = fgetc(site_file);
		if(strcmp(curr_string,search) == 0){
			codes = (char**)realloc(codes,(1+num_codes)*sizeof(*codes));
			codes[num_codes] = (char*)malloc(6*sizeof(char));
			for(i=0;i<6;i++){
				buff = fgetc(site_file);
				if(buff == '\"'){
					buff = '\0';
					break;
				}
				else{	
					codes[num_codes][i]=buff;
				}
			}
			num_codes++;
		}
	}while(curr_string[char_num] != EOF);
	fclose(site_file);
	return true;
}
