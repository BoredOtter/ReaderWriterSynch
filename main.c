#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h> 

// The program considers a possible solution to the readers and writers problem using mutex and semaphores
// In this example, 15 readers and 7 writers are used

sem_t wrt;
pthread_mutex_t mutex;  // Mutex is a synchronization mechanism where only one process can execute a specific code
int counter = 1;
int readerNumber = 0;
int y = 9;
int x = 16;

void *writer(void *wno)
{   
    sem_wait(&wrt);

    counter = counter * 2;

    clear();
    refresh();

    // Part responsible for displaying text in the console
    mvprintw(0+y,10+x,"Current counter value: ");
    attron(COLOR_PAIR(3));
    mvprintw(0+y,25+10+x,"%d",counter);
    attroff(COLOR_PAIR(3));
    mvaddstr(4+y,28,"Locking:");    
    attron(COLOR_PAIR(1));
    mvaddstr(4+y,39," READER");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));
    mvaddstr(2+y,0+x,"WRITER: ");
    attroff(COLOR_PAIR(2));
    mvprintw(2+y,8+x,"%d changes counter value to: ",(*((int *)wno)),counter);
    attron(COLOR_PAIR(3));
    mvprintw(2+y,39+x,"%d",counter);
    attroff(COLOR_PAIR(3));
    
    refresh();
    sleep(1);

    sem_post(&wrt);
}

void *reader(void *rno)
{   
    pthread_mutex_lock(&mutex);
    readerNumber++;
    if(readerNumber == 1) 
    {
        clear();
        refresh();

        // Part responsible for displaying text in the console
        mvprintw(0+y,10+x,"Current counter value: ");
        attron(COLOR_PAIR(3));
        mvprintw(0+y,25+10+x,"%d",counter);
        attroff(COLOR_PAIR(3));
        mvaddstr(4+y,28,"Locking: ");   
        attron(COLOR_PAIR(2));
        mvaddstr(4+y,38," WRITER");
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(1));
        mvprintw(2+y,-1+x,"READER: ");
        attroff(COLOR_PAIR(1));
        mvprintw(2+y,10+x,"%d reads counter as ",*((int *)rno));
        attron(COLOR_PAIR(3));
        mvprintw(2+y,36+x," %d",counter);
        attroff(COLOR_PAIR(3));
        refresh();
        sleep(1);

        sem_wait(&wrt); 
    }
    pthread_mutex_unlock(&mutex);


    pthread_mutex_lock(&mutex);
    readerNumber--;
    if(readerNumber == 0) 
    {
        sem_post(&wrt); 
    }

    pthread_mutex_unlock(&mutex);
}

int main()
{
    initscr();
    refresh();
    start_color();

    init_pair(1,COLOR_GREEN, COLOR_BLACK); // Reader
    init_pair(2,COLOR_RED,COLOR_BLACK);     // Writer
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);  // Counter
    

    pthread_t read[15],write[7];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    // Used for numbering the reader and writer
    int a[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}; 

    for(int i = 0; i < 15; i++) 
    {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }
    for(int i = 0; i < 7; i++) 
    {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    // Waiting for the readers' processes to finish
    for(int i = 0; i < 15; i++) {

        pthread_join(read[i], NULL);
    }

    // Waiting for the writers' processes to finish
    for(int i = 0; i < 7; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    clear();
    refresh();
    mvaddstr(10,10,"Program finished. Press any key to exit");
    getch();
    endwin();

    return 0;
}
