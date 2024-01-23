#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32               // Sleep for Windows
    #include <windows.h>
#else                       // Sleep for Unix-like
    #include <unistd.h>
#endif

#define BUFF 255

typedef struct Client{
    char username[BUFF];
    char ime[BUFF];
    char prezime[BUFF];
    int stanje;
}Client;

typedef struct lista_Client{
    Client client;
    struct lista_Client *next;
}Lista_Client;

void registracija_Client(); //Register funkcija za dodavanje novog klienta
void logIn_Client(char * username, char * password); //Login funkcija za prepoznavanje klienta
void logOut_Client();

void load_Info();

void refresh(); // Samo cisti ekran lagano
void wait(int time);

void isplata();
void uplata();

Client cur_User = {"invalid", "invalid", "invalid", 0};

int main(int argc, char const *argv[]){
    char username[BUFF] = "invalid";
    char password[BUFF] = "invalid";
    char *ptr;

    refresh();
    logOut:
    while(1){
        if(strcmp(cur_User.ime, "invalid") != 0)break;
        printf("1.LogIn\n2.Register\n3.Exit\n");
        printf("Unesi opciju: ");

        int opcija;
        scanf("%d", &opcija);
        /*
        for(int i = 1;i <= 12;i++){         // LOADING ... Just for fun
            switch (i % 4)
            {
            case 0:
                refresh();
                printf("\\");
                break;
            case 1:
                refresh();
                printf("|");
                break;
            case 2:
                refresh();
                printf("/");
                break;
            case 3:
                refresh();
                printf("--");
                break;
            default:
                break;
            }
            #ifdef _WIN32
                Sleep(500);
            #else
                sleep(1/2);
            #endif
        }
        */
        
        switch(opcija){
            case 1: while(getchar() != '\n'); //Unos Username i Password
                    refresh();
                    printf("Username: ");                           //Uzimanje user input-a
                    if(fgets(username, sizeof(username), stdin)){   //Brisanje new line(\n), prebacivanje njega u end line char ('\0')
                        ptr = strchr(username, '\n');
                        if(ptr){
                            *ptr = '\0';
                        }
                    }
                    printf("Password: ");                           // Isto ko gore samo za password
                    if(fgets(password, sizeof(password), stdin)){
                        ptr = strchr(password, '\n');
                        if(ptr){
                            *ptr = '\0';
                        }
                    }
                    logIn_Client(&username[0], &password[0]);       //Login funkcija za prepoznavanje klienta
                    break;
            case 2: while(getchar() != '\n');
                    refresh();
                    registracija_Client();
                    break;
            case 3: return 0;

            default:
                refresh();
                printf("Unesite jednu od ponudjenih opcija!!!\n");

                wait(1);

                refresh();

                break;
        }
    } 

    if(strcmp(cur_User.ime, "invalid") == 0)return 0;

    while(1){
        printf("_____________________________________________________________\n");
        printf("Ime: %s,\n", cur_User.ime);
        printf("Prezime: %s,\n", cur_User.prezime);
        printf("Stanje: %i,\n", cur_User.stanje);
        printf("_____________________________________________________________\n\n");

        printf("1.Log out\n2.Isplata\n3.Uplata\n4.Exit\n");
        printf("Unesi opciju: ");

        int opcija;
        scanf("%d", &opcija);
        
        switch(opcija){
            case 1: while(getchar() != '\n');
                    refresh();
                    logOut_Client();
                    goto logOut;
                    break;
            case 2: while(getchar() != '\n');
                    refresh();
                    isplata();
                    wait(1);
                    refresh();
                    break;
            case 3: while(getchar() != '\n');
                    refresh();
                    uplata();
                    wait(1);
                    break;
            case 4: load_Info();
                    refresh();
                    return 0;

            default:
                refresh();
                printf("Unesite jednu od ponudjenih opcija!!!\n");
                wait(1);
                refresh();

                break;
        }
    } 
    
    return 0;
}

void refresh(){

    printf("“\e[1;1H\e[2J");

    return;
}

void wait(int time){

    #ifdef _WIN32
        Sleep(time* 1000);
    #else
        sleep(time);
    #endif

    return;
}


void logIn_Client(char * username, char * password){
    int find = 0;
    char * User;
    char * Pass;
    char line[BUFF];
    char * ptr;

    FILE *fptr = fopen(".\\DataBase\\Admins.txt", "r");
    // PROLAZ KROZ FAJL I CITANJE NALOGA
    while(fgets(line, BUFF, fptr)){
        ptr = strchr(line, '\n');

        if(ptr){
            *ptr = '\0';
        }

        ptr = strtok(line, ":");
        User = ptr;
        ptr = strtok(NULL, ":");
        Pass = ptr;

        if(strcmp(username, User) == 0 && strcmp(password,Pass) == 0){
            refresh();
            printf("Admin user!");              //TODO:Send to Admin Viewer
            wait(1);
            find = 1;
            refresh();
            break;
        }
    }
    fclose(fptr);

    fptr = fopen(".\\DataBase\\Clients.txt", "r");

    while(fgets(line, BUFF, fptr) && find != 1){
        ptr = strchr(line, '\n');

        if(ptr){
            *ptr = '\0';
        }

        ptr = strtok(line, ":");
        User = ptr;
        ptr = strtok(NULL, ":");
        Pass = ptr;

        if(strcmp(username, User) == 0 && strcmp(password,Pass) == 0){
            refresh();
            printf("Client user!");             //TODO:Send to Client Viewer
            wait(1);
            find = 1;
            refresh();
            break;
        }
    }
    fclose(fptr);

    fptr = fopen(".\\DataBase\\Data.txt", "r");

    char ime[BUFF];
    char prezime[BUFF];
    int stanje;

    while(fgets(line, BUFF, fptr) && find == 1){
        ptr = strchr(line, '\n');

        if(ptr){
            *ptr = '\0';
        }

        ptr = strtok(line, ":");
        User = ptr;
        ptr = strtok(NULL, ":");
        strcpy(ime, ptr);
        ptr = strtok(NULL, ":");
        strcpy(prezime, ptr);
        ptr = strtok(NULL, ":");
        stanje = atoi(ptr);

        if(strcmp(username, User) == 0){
            strcpy(cur_User.username, User);
            strcpy(cur_User.ime, ime);
            strcpy(cur_User.prezime, prezime);
            cur_User.stanje = stanje;
            break;
        }
    }

    fclose(fptr);
}   

void registracija_Client(){

    char username[BUFF];
    char password[BUFF], conform_pass[BUFF];
    char ime[BUFF];
    char prezime[BUFF];
    char user_in[BUFF];
    char *ptr;
    
    FILE *fptr = fopen(".\\DataBase\\Clients.txt", "a");
    while(1){
        printf("Unesi username: ");
        if(fgets(username, BUFF, stdin)){
            ptr = strchr(username, '\n');
            if(ptr){
                *ptr = '\0';
            }
        }

        printf("Unesi password: ");
        if(fgets(password, BUFF, stdin)){
            ptr = strchr(password, '\n');
            if(ptr){
                *ptr = '\0';
            }
        }

        printf("Unesi potvrdi password: ");
        if(fgets(conform_pass, BUFF, stdin)){
            ptr = strchr(conform_pass, '\n');
            if(ptr){
                *ptr = '\0';
            }
        }

        if(strcmp(password,conform_pass)){
            printf("Vasa sifra nije potvrdjena!!!\n");
            refresh();
            continue;
        }

        printf("Unesi ime: ");
        if(fgets(ime, BUFF, stdin)){
            ptr = strchr(ime, '\n');
            if(ptr){
                *ptr = '\0';
            }
        }

        printf("Unesi prezime: ");
        if(fgets(prezime, BUFF, stdin)){
            ptr = strchr(prezime, '\n');
            if(ptr){
                *ptr = '\0';
            }
        }
        
        ponovo:

        printf("Da li ste zadovolji unesenim podacima (Y/N):");
        if(fgets(user_in, BUFF, stdin)){
            ptr = strchr(user_in, '\n');
            if(ptr){
                *ptr = '\0';
            }
        }
        if(user_in[0] == 'N'|| user_in[0] == 'n'){
            refresh();
            continue;
        }else if(user_in[0] == 'Y'|| user_in[0] == 'y'){
            refresh();
            break;
        }else{
            goto ponovo;
        }
    }

    fprintf(fptr, "\n");
    fprintf(fptr, username);
    fprintf(fptr, ":");
    fprintf(fptr, password);
    
    fclose(fptr);

    fptr = fopen(".\\DataBase\\Data.txt", "a");

    fprintf(fptr, "\n");
    fprintf(fptr, username);
    fprintf(fptr, ":");
    fprintf(fptr, ime);
    fprintf(fptr, ":");
    fprintf(fptr, prezime);
    fprintf(fptr, ":");
    fprintf(fptr, "0");
    

    fclose(fptr);
}

void logOut_Client(){

    strcpy(cur_User.ime, "invalid");
    strcpy(cur_User.prezime, "invalid");
    cur_User.stanje = 0;

    return;
}

void load_Info(){

    FILE *ptr = fopen(".\\DataBase\\Data.txt", "r");
    FILE *temp = fopen(".\\DataBase\\temp.txt", "w");

    char user[BUFF], ime[BUFF], prezime[BUFF];
    int stanje;

    if(ptr == NULL && temp == NULL){
        fprintf(stderr, "Error u citanju fajlova u load_Info() funkciji\n");
        return;
    }

    while(fscanf(ptr,"%[^:]:%[^:]:%[^:]:%d", user, ime, prezime, &stanje) == 4){
        if(strcmp(user, cur_User.username) == 0 && stanje != cur_User.stanje){
            fprintf(temp, "%s:%s:%s:%d", user, ime, prezime, cur_User.stanje);
            continue;
        }
        
        fprintf(temp, "%s:%s:%s:%d", user, ime, prezime, stanje);
    }

    fclose(ptr);
    fclose(temp);

    remove(".\\DataBase\\Data.txt");
    rename(".\\DataBase\\temp.txt", ".\\DataBase\\Data.txt");

    return;
}

void isplata(){
    refresh();

    int isplati;

    fprintf(stdout, "Unesite kolicinu koliko zelite da isplatite.\n");
    scanf("%d", &isplati);

    if(isplati > cur_User.stanje){
        fprintf(stdout, "Nemate dovoljno kredita na racunu.\n");
        wait(3);
        return;
    }

    cur_User.stanje -= isplati;

    fprintf(stdout, "Isplaceno: %d\nTrenutno stanje: %d\n", isplati, cur_User.stanje);
    wait(3);
    refresh();
    return;
}

void uplata(){
    refresh();

    int uplati;

    fprintf(stdout, "Unesite kolicinu koliko zelite da uplatite.\n");
    scanf("%d", &uplati);

    cur_User.stanje += uplati;

    fprintf(stdout, "Uplaceno: %d\nTrenutno stanje: %d", uplati, cur_User.stanje);
    wait(3);
    refresh();
    return;
}