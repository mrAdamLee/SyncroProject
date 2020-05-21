#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
sem_t tobacco;

sem_t paper;

sem_t matches;

sem_t agent;
bool isTobacco = false;
bool isPaper = false; 
bool isMatches = false;



void *AgentTobaccoPaper(void *ar){
    sem_wait(&agent);
    sem_post(&tobacco);
    sem_post(&paper);
}
void *AgentTobaccoMatches(void *ar){
    sem_wait(&agent);
    sem_post(&tobacco);
    sem_post(&matches);
}
void *AgentMatchesPaper(void *ar){
    sem_wait(&agent);
    sem_post(&matches);
    sem_post(&paper);
}
void *pushtTandP(void *arg){
  sem_wait(&matches);
  sem_wait(&agent);
  if (isPaper){
      isPaper = false; 
      sem_post(&tobacco);
  }
  else if(isTobacco){
      isTobacco = false;
      sem_post(&paper);
  }
  else{
      isMatches = true;
  }
  sem_post(&agent);
}
void *pushMandP(void *arg){
    sem_wait(&tobacco);
    sem_wait(&agent);
    if(isPaper){
        isPaper = false;
        sem_post(&matches);
    }
    else if(isMatches){
        isMatches = false;
        sem_post(&paper);
    }
    else{
        isTobacco = true;
    }
    sem_post(&agent);
}
void *pushTandM(void *arg){
    sem_wait(&paper);
    sem_wait(&agent);
    if(isTobacco){
        isTobacco = false;
        sem_post(&matches);
    }
    else if(isMatches){
        isMatches = false;
        sem_post(&tobacco);
    }
    else{
        isPaper = true;
    }
    sem_post(&agent);

}
//tobacco
void *smokers1(void *arg){
    while(true){
 nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
 printf("smoker 1 is waiting on paper and matches \n");
 sem_wait(&tobacco);
 printf("smoker 1 got paper and matches and are smoking \n");
 sem_post(&agent);
}
}
//paper

void *smokers2(void *arg){
    while(true){
 nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
 printf("    smoker 2 is waiting on tobacco and matches \n");
 sem_wait(&paper);
 printf("    smoker 2 got tobacco and matches and are smoking \n");
 sem_post(&agent);
}
}

//matches

void *smokers3(void *arg){
    while(true){
        nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
        printf("        smoker 3 is waiting on paper and tobacco \n");
        sem_wait(&matches);
        printf("        smoker 3 got paper and tobacco and are smoking \n ");
        sem_post(&agent);
}
}



int main(int argc, char *arvg[]){
srand(time(NULL));
//init everything

printf("initilaizing \n");
pthread_t smokerthread1;
pthread_t smokerthread2;
pthread_t smokerthread3;
pthread_t AgentTPthread;
pthread_t AgentMPthread;
pthread_t AgentTMthread;
pthread_t PushTMthread;
pthread_t PushMPthread;
pthread_t PushTPthread;
printf("initiliazing semaphores \n");
sem_init(&tobacco, 0, 0);
sem_init(&paper, 0, 0);
sem_init(&matches, 0, 0);
sem_init(&agent, 0, 1);

pthread_create(&smokerthread1, NULL, smokers1, NULL);
pthread_create(&smokerthread2, NULL, smokers2, NULL);
pthread_create(&smokerthread3, NULL, smokers3, NULL);

//tobacco paper agent thread
pthread_create(&AgentTPthread, NULL, AgentTobaccoPaper, NULL);

//Matches and paper agent thread
pthread_create(&AgentMPthread, NULL, AgentMatchesPaper, NULL);
//Tobacco matches thread
pthread_create(&AgentTMthread, NULL, AgentTobaccoMatches, NULL);

//pusher threads

//Tobacco and matches  pusher thread 
pthread_create(&PushTMthread, NULL, pushTandM, NULL);
//matches and paper pusher thread
pthread_create(&PushMPthread, NULL, pushMandP, NULL);
//tobacco and paper pusher thread
pthread_create(&PushTPthread, NULL, pushtTandP, NULL);
//pthread_create(smoker)
printf("got to line 162");
/*
while(true){
    sem_wait(&agent);
    int randnum = rand () % 3;
    if(randnum == 0){
        
        sem_post(&smoker1);
    }
    else if(randnum == 1){
    
        sem_post(&smoker2);
    }
    else if(randnum == 2){
        
        sem_post(&smoker3);
    }
}
*/
pthread_join(AgentTPthread, NULL);
pthread_join(AgentTMthread, NULL);
pthread_join(AgentMPthread, NULL);
pthread_join(PushTPthread, NULL);
pthread_join(PushTMthread, NULL);
pthread_join(PushMPthread, NULL);
pthread_join(smokerthread1, NULL);
pthread_join(smokerthread2, NULL);
pthread_join(smokerthread3, NULL);
}

