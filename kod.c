#include <semaphore.h>

#include <stdio.h>

#include <unistd.h>

#include <pthread.h>

#define CONST_DIVLJACI 16
#define CONST_OBROCI 5

sem_t prazanLonac;
sem_t punLonac;

void *funkcijaDivljaci();
void *funkcijaKuvar();

pthread_mutex_t mutex;

int obroci = CONST_OBROCI;
int ukupanBrojNahranjenihDivljaka = 0;

void *funkcijaKuvar() {
    while(1==1) {
        sem_wait(&prazanLonac);
        obroci += CONST_OBROCI;

        printf("\tKuvar je napunio lonac do vrha!\n");
        sem_post(&punLonac);
    }

    return;
}

void *funkcijaDivljaci() {

    pthread_mutex_lock(&mutex);
    sleep(2);

    if(obroci <= 0) {
        printf("\tKuvar je pozvan da napuni lonac!\n");
        sem_post(&prazanLonac);
        sem_wait(&punLonac);
    }
        
    obroci--;
        
    pthread_mutex_unlock(&mutex);

    sleep(2);
    ukupanBrojNahranjenihDivljaka++;
    printf("Divljak - nahranjen\n");
    printf("%d/%d\n",ukupanBrojNahranjenihDivljaka, CONST_DIVLJACI);
    return;
}

int main() {

    pthread_t divljakNiti[CONST_DIVLJACI];
    pthread_t kuvarNit;

    pthread_mutex_init(&mutex, NULL);

    sem_init(&prazanLonac, 0, 0);
    sem_init(&punLonac,  0, 0);

    for(int i=0; i<CONST_DIVLJACI; i++) {
        pthread_create(&divljakNiti[i], NULL, funkcijaDivljaci, NULL);
    }

    pthread_create(&kuvarNit, NULL, funkcijaKuvar, NULL);

    for(int i=0; i<CONST_DIVLJACI; i++) {
        pthread_join(divljakNiti[i], NULL);
    }

    sem_close(&prazanLonac);
    sem_close(&punLonac);
}