#include <stdio.h>
#include <math.h>
#include <iso646.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
//librarie pour les nombres aléatoire
#include <stdlib.h>
#include <time.h>
//library pour les chaines de caractère
#include <string.h>
//library readline pour les entrées utilisateur
#include <readline/readline.h>
//library json-parser https://github.com/json-parser/json-parser
#include "cJSON.h"

#if defined(_WIN32) || defined(_WIN64)
void clear(){
    system("cls");
}
#elif defined(__linux__)
void clear(){
    system("clear");
}
#else
void clear(){
    printf("\n-------------------------\n")
}
#endif

int max(int a,int b){
    if (a > b){
        return a;
    } 
    return b;
}

int min(int a,int b){
    if (a < b){
        return a;
    } 
    return b;
}

char* intTostr(int n){
    char* buffer = malloc(12);
    snprintf(buffer,sizeof(buffer),"%d",n);
    return buffer;
}

int strsame(const char *str1,const char *str2){
    if (strlen(str1) != strlen(str2)){return 0;}

    for (int i = 0 ; i < strlen(str1);i++){

        if (str1[i] != str2[i]){return 0;}
    }
    return 1 ;
}

char* getStringElm(const cJSON * Json,const char *element) {
    cJSON *elm = cJSON_GetObjectItem(Json, element);
    if (cJSON_IsString(elm)){return elm->valuestring;}
    else if (cJSON_IsNumber(elm)){return intTostr( elm->valueint );}
    return "";
}

int findByName(const cJSON *Json,const char *name) {
    if (cJSON_IsArray(Json)) {
        for (int i = 0 ; i < cJSON_GetArraySize(Json);i++){
            cJSON *Hero = cJSON_GetArrayItem(Json,i);
            if (strsame(name,getStringElm(Hero,"name"))){
                return i;
            }
        }
    }
    return -1;
}

void printJson(const cJSON *Json) {
    if (cJSON_IsNumber(Json)) { printf("%d", Json->valueint);}
    if (cJSON_IsString(Json)) { printf("%s", Json->valuestring);}
}

void printJsonElt(const cJSON *Json,const char *element) {
    cJSON *id = cJSON_GetObjectItem(Json, element);
    if (cJSON_IsNumber(id)) {printf("%s: %d\n",element, id->valueint);}
    else if (cJSON_IsString(id)) {printf("%s: %s\n",element, id->valuestring);}
    else if (cJSON_IsArray(id)) {
        printf("%s",element);
        for (int i = 0 ; i < cJSON_GetArraySize(id);i++) {
            printf(" ");
            cJSON *value = cJSON_GetArrayItem(id,i);
            printJson(value);
        }
        printf("\n");
    }
}

void printHeroInfo(const cJSON *mainJson) {
    printJsonElt(mainJson,"id");
    printJsonElt(mainJson,"name");
    printJsonElt(mainJson,"id-api");
    printJsonElt(mainJson,"slug");
}

void printHeroPowers(const cJSON *PowerJson) {
    printJsonElt(PowerJson,"intelligence");
    printJsonElt(PowerJson,"strength");
    printJsonElt(PowerJson,"speed");
    printJsonElt(PowerJson,"durability");
    printJsonElt(PowerJson,"power");
    printJsonElt(PowerJson,"combat");

}

void printHeroAppear(const cJSON *AppearJson) {
    printJsonElt(AppearJson,"gender");
    printJsonElt(AppearJson,"race");
    printJsonElt(AppearJson,"height");

    cJSON *weightL = cJSON_GetObjectItem(AppearJson, "weight");
    if (cJSON_IsArray(weightL)){
        cJSON *weightImp = cJSON_GetArrayItem(weightL,0);
        cJSON *weightMet = cJSON_GetArrayItem(weightL,1);
        if (cJSON_IsString(weightImp)) {printf("weight: %s | ", weightImp->valuestring);}
        if (cJSON_IsString(weightMet)) {printf("%s\n", weightMet->valuestring);}
    }
    printJsonElt(AppearJson,"eyeColor");
    printJsonElt(AppearJson,"hairColor");
}

void printHeroInfoFull(const cJSON *mainJson,int n) {
    cJSON *HeroInfos = cJSON_GetArrayItem(mainJson,n);
    printHeroInfo(HeroInfos);
    printf("-powers:\n");
    cJSON *powers = cJSON_GetObjectItem(HeroInfos,"powerstats");
    printHeroPowers(powers);
    printf("-appearance:\n");
    cJSON *AppearJson = cJSON_GetObjectItem(HeroInfos,"appearance");
    printHeroAppear(AppearJson);
}

void printHerosList(const cJSON *mainJson) {
    int len = cJSON_GetArraySize(mainJson);
    for (int i = 0 ; i<len;i++) {
        printf("------------------------\n");
        cJSON *hero = cJSON_GetArrayItem(mainJson,i);
        printHeroInfo(hero);
    }
    printf("------------------------\n");
}

int Menu_Find_ByName(const cJSON *json) {
    printf("\n--------------------------\n");
    printf("Find Menu Name\n");
    printf("What is the name?\n");
    char *choice = readline("-> ");
    int id = findByName(json,choice);
    printf("le hero appelé %s est le numero %d\n\n",choice,id);
    printHeroInfoFull(json,id);
    readline("press any key to continue-> ");
}

int FindJsonValueElm(const cJSON *json,char* elm){

    if (cJSON_IsNumber(cJSON_GetObjectItem(json,elm))){

        return cJSON_GetObjectItem(json,elm) ->valueint;
    }
    
    else if (cJSON_IsArray(json)){

        for(int i = 0 ;i<cJSON_GetArraySize(json);i++){

            return FindJsonValueElm(cJSON_GetArrayItem(json,i),elm);
        }
        
    }
    else if (cJSON_IsObject(json)){

        for(int i = 0 ;i<cJSON_GetArraySize(json);i++){

            int value = FindJsonValueElm(cJSON_GetArrayItem(json,i),elm);
            if (value>0){
                return value;
            }
        }
    }
    return -1;
}

int Menu_Find_Complex_filter(cJSON *json,char* elm,char op,int number){

    int jsonLen = cJSON_GetArraySize(json) ;

    for(int i = 0 ; i<jsonLen;i++){

        int n = FindJsonValueElm(json,elm);
        if (n == -1){
            return 0;
        }  
        switch (op)
        {
        case '<':return n<number;break;
        case '>':return n>number;break;
        case '=':return n==number;break;
        default:return 0;break;
        }



    }
    return 1;
}


cJSON* loadJson(char *path){
    FILE *fp = fopen(path, "r");
    if (fp == NULL){
        printf("Error: Unable to open '%s'\n", path);
        return NULL;
    }


    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);

    char *buffer = malloc(fsize + 1);
    if (buffer == NULL){
        printf("Error: malloc failed\n");
        fclose(fp);
        return NULL;
    }


    fread(buffer, 1, fsize, fp);
    buffer[fsize] = '\0'; // termine la string
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);

    if (json == NULL){
        const char *error = cJSON_GetErrorPtr();
        if (error != NULL){
            printf("Error parsing JSON near: %s\n", error);
        }
        return NULL;
    }
    return json;
}


int stradd(const char *str1,const char *str2,char *str3){
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if (len1+len2>=strlen(str3)){return 1;}

    for (int i = 0 ; i < len1+len2;i++){
        if (i < len1){
            str3[i] = str1[i];
        }
        else {
            str3[i] = str2[i-len1];
        }
    }
    return 0 ;
}

void PrintHelp(const cJSON *json){

    printf("id\n");
    printf("id-api\n");
    printf("intelligence\n");
    printf("strength\n");
    printf("speed\n");
    printf("durability\n");
    printf("power\n");
    printf("combat\n");

}

int saveJson(cJSON *json,char *path){
    char *json_str = cJSON_Print(json);
    printf("saving '%s' ...\n",path);
    FILE *fp2 = fopen(path, "w");
    if (fp2 == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }
    fprintf(fp2, json_str);
    printf("saved\n");
    fclose(fp2);
    free(json_str);
    return 0;
}

int Menu_Find_Complex_Commands(cJSON *json,cJSON *heros,int *HerosNumber) {
    //as long as the user dont say exit to return the function continue
    while (1){
    char *input = readline("-> ");

    //help bring up the help, help
    if (strsame(input,"help")){
        PrintHelp(cJSON_GetArrayItem(heros,0));
    }
    //if the user want to filter we get the element ot filter ,the operation and the number to compare 
    else if (strsame(input,"filter")){
        printf("retire les élément quand vrai\n");
        char *elm = readline("élément a filtré \t-> ");
        char op = readline("opération ( =,>,< )\t-> ")[0];
        int nb = atoi(readline("nombre\t\t-> "));
        //for (int i = 0 ; i < HerosNumber ; i++){
        int i = 0;
        printf("\n");
        while (i < *HerosNumber){
            if (Menu_Find_Complex_filter(cJSON_GetArrayItem(heros,i),elm,op,nb)==1){
                printf("removing %s \n",cJSON_GetObjectItem(cJSON_GetArrayItem(heros,i),"name")->valuestring);
                cJSON_DeleteItemFromArray(heros,i);
                *HerosNumber -= 1;
            }
            else{
                i++;
            }
            
        }
    }
    else if (strsame(input,"list")){
        printHerosList(heros);
    }
    else if (strsame(input,"addByName")){
        char *hero = malloc(30);
        while (1){
            hero = readline("nom du heros a rajouté \t-> ");
            if (findByName(json,hero) !=-1){break;}
        }
        
        if (findByName(heros,hero) ==-1){
                int i = 0;
                //find where to place the hero so that is id is sorted
                for (;i<cJSON_GetArraySize(heros);i++){
                    if (FindJsonValueElm(cJSON_GetArrayItem(heros,i),"id")>FindJsonValueElm(cJSON_GetArrayItem(json,findByName(json,hero)),"id")){
                        break;
                    }
                }
                //insert the hero and duplicate the hero to insert because its a pointer so it crash because the hero is in 2 json
                cJSON_InsertItemInArray(heros,i,cJSON_Duplicate( cJSON_GetArrayItem(json,findByName(json,hero)),1 ));
                *HerosNumber += 1;
                printf("hero added\n");
        }
        else
        {
                printf("hero already exist\n");
        }
    }
    else if (strsame(input,"delByName")){
        char *hero = malloc(30);
        while (1){
            hero = readline("nom du heros a retiré \t-> ");
            if (findByName(json,hero) !=-1){break;}
        }
        
        if (findByName(heros,hero) !=-1){
                cJSON_DeleteItemFromArray(heros,findByName(heros,hero));
                *HerosNumber -= 1;
                printf("hero deleted\n");
        }
        else
        {printf("hero dont exist\n");}
    }
    //if the user say save we call the save function
    else if (strsame(input,"save")){
        printf("\nsaving...\n\n");
        saveJson(heros,readline("path of the saved file :"));

    }
    else if (strsame(input,"open")){
        heros = loadJson(readline("path :"));
    }
    //if user say exit we return 0 to exit the function
    else if (strsame(input,"exit")){
        return 0;
    }
    //if we dont know what the user typed
    else{
        printf("syntax unknow\n");
    }
    }
}

int Menu_Find_Complex(cJSON *json) {
    clear();
    printf("--------------------------\n");
    printf("type by what you want to filter\n");
    printf("(commands - help filter delByName addByName save open list exit )\n\n");
    cJSON *heros = cJSON_CreateArray();
    int HerosNumber = cJSON_GetArraySize(json);
    int herosValidate[HerosNumber];

    //create a new json with all heros to be filtered and saved
    for (int i = 0 ; i < HerosNumber ; i++){
        cJSON *hero = cJSON_GetArrayItem(json,i);
        cJSON_AddItemReferenceToArray(heros,hero);
    }

    //set all heros to be selected
    for (int i = 0 ; i < HerosNumber ; i++){herosValidate[i] = 1;}
    
    //call the function that let the user filter heros
    Menu_Find_Complex_Commands(json,heros,&HerosNumber);

    //after all clear the resulting json from memory
    cJSON_free(heros);
}

int Menu_Find(cJSON *json) {
    char elt ;
    char choice;
    printf("+-------------------------------+\n");
    printf("|Find Menu\t\t\t|\n");
    printf("|1-recherché par nom\t\t|\n");
    printf("|2-recherche triée\t\t|\n");
    printf("+-------------------------------+\n");
    char *input = readline("-> ");
    if (input == NULL) {
        return 1;
    }
    choice = atoi(input);
    switch(choice){
        case 1:Menu_Find_ByName(json);break;
        case 2:Menu_Find_Complex(json);break;
    }
}


int strinsert(char* a , char* b,int* n){
    for (int i = 0 ; i<strlen(b);i++){
        a[*n+i] = b[i];
    }
    return *n+strlen(b);
}

char* FindJsonStrElm(const cJSON *json,char* elm){

    cJSON *elmJson = cJSON_GetObjectItem(json,elm);

    if (cJSON_IsNumber(elmJson)){
        return intTostr(elmJson->valueint);
    }

    else if (cJSON_IsString(elmJson)){
        return elmJson->valuestring;
    }
    
    else if (cJSON_IsArray(elmJson)){
        char* buffer = malloc(100);
        int neededLen = 0;
        for (int i=0;i<cJSON_GetArraySize(elmJson);i++){

            if (cJSON_IsNumber( cJSON_GetArrayItem(elmJson,i) )){
                //return intTostr( cJSON_GetArrayItem(elmJson,i)->valueint );
                neededLen = strinsert(buffer,intTostr( cJSON_GetArrayItem(elmJson,i)->valueint ),&neededLen);
            }
            else if (cJSON_IsString( cJSON_GetArrayItem(elmJson,i) )){
                //return cJSON_GetArrayItem(elmJson,i)->valuestring;
                neededLen = strinsert(buffer,cJSON_GetArrayItem(elmJson,i)->valuestring ,&neededLen);
            }
            if (i<cJSON_GetArraySize(elmJson)-1){
                neededLen = strinsert(buffer,"/",&neededLen);
            }
        }

        return buffer;
    }

    else if (cJSON_IsArray(json)){
        for(int i = 0 ;i<cJSON_GetArraySize(json);i++){
            return FindJsonStrElm(cJSON_GetArrayItem(json,i),elm);
        }  
    }

    else if (cJSON_IsObject(json)){
        for(int i = 0 ;i<cJSON_GetArraySize(json);i++){
            char* result = FindJsonStrElm(cJSON_GetArrayItem(json,i),elm);
            if(strlen(result)!=0){
                return result;
            }
        }
    }
    return "";
}

int ComparaisonHeros(cJSON *json){
    int hero1id;
    int hero2id;

    while (1){
        //demande le nom du hero
        char* hero1 = readline("name of the first hero -> ");
        //si le hero existe sort de la boucle
        hero1id = findByName(json,hero1);
        if (hero1id != -1){break;}
    }
    while (1){
        //demande le nom du hero
        char* hero2 = readline("name of the second hero -> ");
        //si le hero existe sort de la boucle
        hero2id = findByName(json,hero2);
        if (hero2id != -1){break;}
    }
    
    

    cJSON *hero1Json = cJSON_GetArrayItem(json,hero1id);
    cJSON *hero2Json = cJSON_GetArrayItem(json,hero2id);

    //------|Superman               |               Batman|
    printf("+-----------------------------------------------+\n");
    char *key[14] = {
        "name",
        "id",
        "intelligence",
        "strength",
        "speed",
        "durability",
        "power",
        "combat",
        "gender",
        "race",
        "height",
        "weight",
        "eyeColor",
        "hairColor"
    };
    for(int i = 0 ; i<14 ; i++){
    char* str1 = FindJsonStrElm(hero1Json,key[i]);
    char* str2 = FindJsonStrElm(hero2Json,key[i]);
    printf("%s : '%s' vs '%s'\n",key[i],str1,str2);
    }
    printf("+-----------------------------------------------+\n");
}

int QuizzMenu(cJSON *json){
    int heroId = rand() % cJSON_GetArraySize(json);
    cJSON *hero = cJSON_Duplicate( cJSON_GetArrayItem(json,heroId-1) ,1);
    char* guess = malloc(30);
    int win = 0;
    char *prop[] = {
        "intelligence",
        "strength",
        "speed",
        "durability",
        "power",
        "combat",
        "gender",
        "race",
        "height",
        "weight",
        "eyeColor",
        "hairColor"
    };
    int propNum = 12;
    //int is 4 bytes in size
    int* propUsed = malloc(4*propNum);
    //set all hints to be not used
    for(int i=0;i<propNum;i++){propUsed[i]=0;}
    int randomHint = rand() % propNum;
    int hints = 0;
    printf("find hero name in the less hints possible \n");
    while (hints<12){

        
        while (propUsed[randomHint] != 0){
            randomHint = rand() % propNum;
            
        }
        propUsed[randomHint] = 1;
        hints++;
        
        printf("the hint n°%d is : %s = %s\n",hints,prop[randomHint],FindJsonStrElm(hero,prop[randomHint]));


        printf("guess the hero\n");
        guess = readline("->");
        if (strsame(FindJsonStrElm(hero,"name"),guess)){
            win = 1;break;
        }
        
    }
    if (win){
        printf("bravo le heros était bien %s, trouvé en %d hints\n",guess,guess);
    }
    else{
        printf("le heros était %s\n",FindJsonStrElm(hero,"name"));
    }
    free(propUsed);
    cJSON_free(hero);
    return 0;
}

int LinkMenu(){
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║              RESSOURCES EXTERNES - SUPER-HEROS               ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  [1] Wikipedia - Super-heros (FR)                            ║\n");
    printf("║      https://fr.wikipedia.org/wiki/Super-h%%C3%%A9ros          ║\n");
    printf("║                                                              ║\n");
    printf("║  [2] Vikidia - Superheros                                    ║\n");
    printf("║      https://fr.vikidia.org/wiki/Superh%%C3%%A9ros             ║\n");
    printf("║                                                              ║\n");
    printf("║  [3] Wikipedia - La Legion des super-heros                   ║\n");
    printf("║      https://fr.wikipedia.org/wiki/La_L%%C3%%A9gion_des_       ║\n");
    printf("║      super-h%%C3%%A9ros                                        ║\n");
    printf("║                                                              ║\n");
    printf("║  [4] Wikipedia - Puissants Vengeurs                          ║\n");
    printf("║      https://fr.wikipedia.org/wiki/Puissants_Vengeurs        ║\n");
    printf("║                                                              ║\n");
    printf("║  [5] Wiktionary - Definition super-heros                     ║\n");
    printf("║      https://fr.wiktionary.org/wiki/super-h%%C3%%A9ros         ║\n");
    printf("║                                                              ║\n");
    printf("║  [6] Alucare - Liste complete des super-heros                ║\n");
    printf("║      https://www.alucare.fr/la-liste-complete-des-super-     ║\n");
    printf("║      heros/                                                  ║\n");
    printf("║                                                              ║\n");
    printf("║  [7] YouTube - Video super-heros                             ║\n");
    printf("║      https://youtu.be/kLo_t7RI9pE?si=3_wOoBfD5TNC-km7        ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    readline("press any key to continue-> ");
}

int Menu(cJSON *json) {
    char choice;
        printf("+-------------------------------+\n");
        printf("|Main Menu\t\t\t|\n");
        printf("|1-base de données complete\t|\n");
        printf("|2-recherche\t\t\t|\n");
        printf("|3-comparaison\t\t\t|\n");
        printf("|4-quizz\t\t\t|\n");
        printf("|5-resource\t\t\t|\n");
        printf("|6-quitter\t\t\t|\n");
        printf("+-------------------------------+\n");
        printf("\n");

        char *input = readline("-> ");

        choice = atoi(input);
        switch(choice){
            case 1:
                clear();
                printf("\n");
                int HerosNumbers = cJSON_GetArraySize(json);
                for (int i = 0 ; i < HerosNumbers; i++) {
                    printHeroInfoFull(json,i);
                    printf("-------------------\n");
                }
                readline("press any key to continue-> ");
            break;

            case 2:
                clear();
                Menu_Find(json);
            break;

            case 3:
                clear();
                ComparaisonHeros(json);
                readline("press any key to continue-> ");
            break;
            case 4:
                clear();
                QuizzMenu(json);
                readline("press any key to continue-> ");
            break;
            case 5:
                clear();
                LinkMenu();
            break;
            case 6:
                clear();
                return 0;
            break;
        
    }
    return 1;
}

int main(void) {
    srand( time( NULL ) );
    cJSON *json = loadJson("./json/SuperHeros.json");
    if (json == NULL){ return -1; }

    while (Menu(json))
    {
        clear();
    }
    
    cJSON_free(json);
    return 0;
}
