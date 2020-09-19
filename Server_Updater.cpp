#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
//#include <my_global.h>
#include <mysql/mysql.h>
char **favorites, **list, **downloaded,**stored_tags;
int favorites_size, list_size, downloaded_size,name_size,stored_tags_num;
struct doujinshi{
	char *title, *code;
	/*char **tags;
	int num_tags;/*/
};
doujinshi *manga;
void load_favorites(char file[100]);
void load_list(char file[100]);
//void load_tags(FILE *curr_file, int index);
bool search(char search[100], FILE *file);
char *get_name(FILE *file);
/*class tags{
	public:
		void read(char file[100],int index){
			char buff_char, *buff_string;
			int i,buff_size = 1;
			FILE *curr_file;
			manga[index].num_tags = 0;
			curr_file = fopen(file,"r");
			search((char*)"Tags",curr_file);
			printf("Sai!\n");
			do{
				buff_char = fgetc(curr_file);
			}while(buff_char == ' ' || buff_char == 's');
			buff_string= (char*)malloc(sizeof(char));
			manga[index].tags = NULL;
			do{
				do{
					buff_char = fgetc(curr_file);
					if(buff_char != ',' && buff_char != '\n' && buff_char != EOF){
						buff_string = (char*)realloc(buff_string,(1+buff_size)*sizeof(char));
						buff_string[buff_size] = buff_char;
						buff_size++;
					}
				}while(buff_char != ',' && buff_char != '\n' && buff_char != EOF);
				buff_string = (char*)realloc(buff_string,(2+buff_size)*sizeof(char));
				buff_string[buff_size+1] = '\0';
				for(i = 0; i < stored_tags_num; i++){
					if(strcmp(buff_string,stored_tags[i] ) == 0){
						printf("Achei uma tag\n");
						manga[index].tags = (char**)realloc(manga[index].tags,(1+manga[index].num_tags)*sizeof(*manga[index].tags));
						printf("Aqui!");
						manga[index].tags[manga[index].num_tags]=(char*)malloc((2+buff_size)*sizeof(char));
						printf("%s\n",buff_string);
						manga[index].tags[manga[index].num_tags] = buff_string;
						manga[index].tags[manga[index].num_tags][buff_size+1] = '\0';
						manga[index].num_tags++;
					}
				}
				buff_size = 0;
			 	buff_string = NULL;
			}while(buff_char != '\n' && buff_char != EOF);
			fclose(curr_file);
			printf("end of read\n");
		}
		void load(){
			FILE *tags_file;
			char buff, *buff_string;
			int string_size;
			tags_file = fopen("Files/Tags.txt","r");
			stored_tags_num = 0;
			string_size = 0;
			stored_tags = NULL;
			do{	
				string_size = 0;
				buff_string = NULL;
				do{
					buff = fgetc(tags_file);
					if(buff != ',' && buff != EOF){
						buff_string = (char*)realloc(buff_string,(1+string_size)*sizeof(char));
						buff_string[string_size] = buff;
						string_size++;
					}
				}while(buff != ',' && buff != EOF);
				stored_tags = (char**)realloc(stored_tags,(stored_tags_num+1)*sizeof(*stored_tags));
				stored_tags[stored_tags_num] = (char*)malloc((2+string_size)*sizeof(char));
				stored_tags[stored_tags_num][string_size+1] = '\0';
				stored_tags[stored_tags_num] = buff_string;
				printf("%s\n",stored_tags[stored_tags_num]);
				stored_tags_num++;
			}while(buff != EOF);
			fclose(tags_file);
			free(buff_string);
		}
};*/
int main(){
	char *buff, alpha;
	int i,j,numf,numl,id;
	bool found;
	FILE *curr_file;
	MYSQL *con = mysql_init(NULL);
//	tags tags_obj;
        system("nhentai --cookie \"csrftoken=t3CLrmYdcaiudSIlVjBmNATCThZsMkfGtj52EvS9evIXwIJeJX6Nanyo9rwjYoG2; sessionid=04ih2eq6g2tmy3qkebmo931uqmp1emrw\"");
        system("nhentai --page-range=1 -F > Files/Favorites.txt");
	load_favorites((char*)"Files/Favorites.txt");
	printf("Sem erros até aqui\n");
	load_list((char*)"Files/list.txt");
	printf("Nem aqui\n");
//	system("rm -r Files/to_download.txt");
//	system("touch Files/to_download.txt");
	downloaded = NULL;
	for(i=0;i<favorites_size;i++){
		found = false;
		numf = atoi(favorites[i]);
		for(j=0;j<list_size;j++){
			numl = atoi(list[j]);
			if(numf == numl){
					found = true;
			}
		}
		if(!found){
			printf("Achei: %d\n",numf);
			downloaded = (char**)realloc(downloaded,(downloaded_size+1)*sizeof(*downloaded));
		       	numf < 100000 ?  numl = 5 : numl = 6;
			downloaded[downloaded_size] = (char*)malloc(numl*sizeof(char));
			//for(int k = 0; k < numl; k++){
			sprintf(downloaded[downloaded_size],"%d",numf);
			//}
			downloaded_size++;
		}
	}
	printf("list:\n");
	for(i=0;i<list_size;i++){
		printf("%s\n",list[i]);
	}
	printf("favorites:\n");
	for(i=0;i<favorites_size;i++){
		printf("%s\n",favorites[i]);
	}
	printf("downloaded_size: %d\n",downloaded_size);
	printf("downloaded:\n");
	for(i = 0; i < downloaded_size; i++){
		printf("check\n");
		printf("%s\n",downloaded[i]);
	}
//	tags_obj.load();
	manga = NULL;
	manga = (doujinshi*)malloc(downloaded_size*sizeof(doujinshi));
	for(i = 0; i< downloaded_size; i++){
		buff=(char*)malloc(250*sizeof(char));
		manga[i].code = (char*)malloc(sizeof(downloaded[i]));
		manga[i].code = downloaded[i];
		sprintf(buff,"mkdir Downloaded/%s",downloaded[i]);
		system(buff);
		sprintf(buff,"nhentai -o Downloaded/Fresh --id=%s -D",downloaded[i]);
		system(buff);
		sprintf(buff,"mv  Downloaded/Fresh/*/*.png Downloaded/Fresh/*/*.jpg Downloaded/Fresh/*/*.jpeg Downloaded/%s/",downloaded[i]);
		system(buff);
		system("rm -r Downloaded/Fresh/*");
		sprintf(buff,"nhentai --id=%s -S > Downloaded/%s/info.txt",downloaded[i],downloaded[i]);
		system(buff);
		sprintf(buff,"Downloaded/%s/info.txt",downloaded[i]);
		curr_file = fopen(buff,"r");
		if(search((char*)"Doujinshi",curr_file)){
			manga[i].title = NULL;
			manga[i].title = (char*)malloc(250*sizeof(char));
			manga[i].title = get_name(curr_file);
			manga[i].title = (char*)realloc(manga[i].title,(name_size+1)*sizeof(char));
		}
		fclose(curr_file);
//		tags_obj.read(buff,i);
		//fclose(curr_file);
	}
	printf("Titulos: \n");
	for(i=0;i<downloaded_size;i++){
		printf("%s\n",manga[i].title);
	}
/*	for(i = 0 ; i < downloaded_size; i++){
		printf("Title: %s, num_tags: %d, Tags: \n",manga[i].title, manga[i].num_tags);
		for(j = 0; j < manga[i].num_tags;j++){
			printf("%s\n",manga[i].tags[j]);
		}
	}*/
	curr_file = fopen("Files/id.txt","r+");
	i = 0;
	buff = NULL;
	do{	
		alpha = fgetc(curr_file);
		if(alpha != EOF){
			buff = (char*)realloc(buff,(i+1)*sizeof(char));
			buff[i] = alpha;
			i++;
		}
	}while(alpha != EOF);
	fclose(curr_file);
	id = atoi(buff);
	system("rmdir Downloaded/Fresh"); 
	system("mv Downloaded/* /var/www/zenphoto/albums/");
	system("mkdir Downloaded/Fresh");
	mysql_real_connect(con,"localhost","root", "p0r0nt0?",NULL,0,NULL,0);
	mysql_query(con, "USE zenphoto;");
	for(i = 0 ; i < downloaded_size; i++){
		buff = (char*)malloc(1250*sizeof(char));
		id++;
		sprintf(buff, "INSERT INTO `_albums` (`id`, `parentid`, `folder`, `title`, `desc`, `date`, `updateddate`, `location`, `show`, `closecomments`, `commentson`, `thumb`, `mtime`, `sort_type`, `subalbum_sort_type`, `sort_order`, `image_sortdirection`, `album_sortdirection`, `hitcounter`, `password`, `password_hint`, `publishdate`, `expiredate`, `total_value`, `total_votes`, `used_ips`, `custom_data`, `dynamic`, `search_params`, `album_theme`, `user`, `rating`, `rating_status`, `watermark`, `watermark_thumb`, `owner`, `codeblock`, `lastchange`, `lastchangeuser`) VALUES ('%d', 'NULL', '%s', '%s', '%s', NULL, NULL, NULL, '1', '0', '1', NULL, NULL, NULL, NULL, NULL, '0', '0', '0', '', NULL, NULL, NULL, '0', '0', NULL, NULL, '0', NULL, NULL, NULL, NULL, '3', NULL, NULL, NULL, NULL, NULL, NULL)",id,manga[i].code,manga[i].title,manga[i].code);
		mysql_query(con,buff);
	}
	mysql_close(con);
	system("rm Files/id.txt");
	buff = NULL;
	buff = (char*)malloc(50*sizeof(char));
	sprintf(buff,"echo %d > Files/id.txt",id);
	system(buff);
	system("rm Files/list.txt");
	system("touch Files/list.txt");
	curr_file = fopen("Files/list.txt","a+");
	for(i=0;i<favorites_size;i++){
		sprintf(buff,",%s",favorites[i]);
		fputs(buff,curr_file);
		free(favorites[i]);
	}
	free(favorites);
	for(i=0;i<list_size;i++){
		free(list[i]);
	}
	free(list);
	buff = NULL;
	buff = (char*)malloc(7*sizeof(char));
	for(i = 0; i < downloaded_size; i++){
		free(downloaded[i]);
	}
	free(downloaded);
	fclose(curr_file);
	buff = NULL;
	free(buff);
	for(i = 0; i < stored_tags_num; i++){
		free(stored_tags[i]);
	}
	free(stored_tags);
	return (1);

}
void load_favorites(char file[100]){
	char buff_char, buff_string[6];
	FILE *curr_file;
	int i,curr_size;
	curr_file = fopen(file,"r");
	favorites_size = curr_size = 0;
	favorites = NULL;
	do{
		buff_char = fgetc(curr_file);
	}while(buff_char != '=');
	do{
		buff_char = fgetc(curr_file);
		if(buff_char == '\n'){
			curr_size++;
		}
	}while(curr_size < 3);
	do{
		curr_size = 0;
		favorites = (char**)realloc(favorites,(favorites_size+1)*sizeof(*favorites));
		for(i=0;i<6;i++){
			buff_string[i] = '\0';
		
		}
		for(i=0;i<6;i++){
			buff_char = fgetc(curr_file);
			if(isdigit(buff_char)){
				buff_string[curr_size] = buff_char;
				curr_size++;
			}
		}
		favorites[favorites_size] = (char*)malloc(curr_size*sizeof(char));
		for(i=0;i<curr_size;i++){
			favorites[favorites_size][i] = buff_string[i];
		}
		favorites_size++;
		do{
			buff_char = fgetc(curr_file);
		}while(buff_char != '\n' && buff_char != EOF);
	}while(buff_char != EOF && favorites_size < 25);
	fclose(curr_file);
	//free(buff_string);
}
void load_list(char file[100]){
	char buff_char, buff_string[6];
	FILE *curr_file;
	int i,curr_size;
	curr_file = fopen(file,"r");
	list_size = curr_size = 0;
	do{
		for(i=0;i<6;i++){
			buff_string[i] = '\0';
		}
		curr_size = 0;
		list = (char**)realloc(list,(1+list_size)*sizeof(*list));
		do{
			buff_char = fgetc(curr_file);
			if(isdigit(buff_char)){
				buff_string[curr_size] = buff_char;
				curr_size++;
			}
		}while(buff_char != ',' && buff_char != EOF);
		list[list_size] = (char*)malloc(curr_size*sizeof(char));
		for(i=0;i<curr_size;i++){
			list[list_size][i] = buff_string[i];
		}
		list_size++;
	}while(buff_char != EOF);
	fclose(curr_file);
	//free(buff_string);
}
bool search(char search[100], FILE *file){
        int i,numero_de_chars;
        char *curr_string, *search_string,buff;
	bool found;
	numero_de_chars = 0;
        for(i = 0; i < 100; i++){
                if(search[i] == '\0'){
                        numero_de_chars = i-1;
                        break;
                }
        }
	search_string = NULL;
	search_string = (char*)malloc((1+numero_de_chars)*sizeof(char));
	for(i=0;i<numero_de_chars;i++){
		search_string[i] = search[i];
	}
	search_string[numero_de_chars] = '\0';
	printf("Numero de chars: %d\n",numero_de_chars);
	printf("Search string: %s\n",search_string);
	curr_string = NULL;
        curr_string = (char*)realloc(curr_string,(1+numero_de_chars)*sizeof(char));
        for(i = 0; i < numero_de_chars; i++){
                curr_string[i] = 'a';
        }
	curr_string[numero_de_chars] = '\0';
        do{
                for(i = 0; i < numero_de_chars-1; i++){
                        curr_string[i] = curr_string[i+1];
                }
                curr_string[numero_de_chars-1] = fgetc(file);
		//printf("%s",curr_string);
		//printf("\n");	
		//printf("Ultimo char: %c\n",curr_string[1+numero_de_chars]);
                if(strcmp(curr_string,search_string) == 0){
			printf("Achei!\n");
			found = true;
			break;
                }
        }while(curr_string[numero_de_chars] != EOF && !found && buff != EOF);
	free(curr_string);
	free(search_string);
	return(found);
}
char *get_name(FILE *file){
	char *name,buff;
	printf("Getting name...\n");
	name_size = 0;
	do{
		buff = fgetc(file);
	}while(buff != ']');
	name = NULL;
	do{
		buff = fgetc(file);
		if(buff != '[' && buff != '('){
			name = (char*)realloc(name,(1+name_size)*sizeof(char));
			name[name_size] = buff;
			name_size++;
		}
	}while(buff != '[' && buff != '(' && buff != EOF);
	name = (char*)realloc(name,(2+name_size)+sizeof(char));
	name[name_size+1] = '\0';
	printf("Name: %s\n",name);
	return(name);
	//free(name);
}
