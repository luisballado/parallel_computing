#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#define NUM_THREADS 5

char *messages[NUM_THREADS];

//Estructura que sera pasada en la funcion paralelizada
struct thread_data{
  int thread_id;
  int sum;
  char *message;
};

//se hacen los structs respecto al NUM_THREADS
struct thread_data thread_data_array[NUM_THREADS];

//funcion paralelizable
void *print_hola(void *threadarg){
  
  int  taskid;
  int  sum;
  char *hello_msg;
  
  struct thread_data *my_data;
  
  sleep(1);
  
  //cachar la informacion
  my_data = (struct thread_data *) threadarg;
  taskid = my_data->thread_id;
  sum = my_data->sum;
  hello_msg = my_data->message;
  
  printf("Hilo %d: %s SUM=%d\n",taskid,hello_msg,sum);
  pthread_exit(NULL);
  
}

int main(int argc, char *argv[]){

  pthread_t threads[NUM_THREADS];
  int *taskids[NUM_THREADS];
  int rc,t,sum;
  
  sum = 0;
  
  messages[0] = "English: Hello World!";
  messages[1] = "French: Bonjour, le monde!";
  messages[2] = "Spanish: Hola al mundo";
  messages[3] = "Klingon: Nuq neH!";
  messages[4] = "German: Guten Tag, Welt!"; 
  messages[5] = "Russian: Zdravstvytye, mir!";
  messages[6] = "Japan: Sekai e konnichiwa!";
  messages[7] = "Latin: Orbis, te saluto!";
  
  for(t=0;t<NUM_THREADS;t++){
    
    sum = sum + t;
    thread_data_array[t].thread_id = t;
    thread_data_array[t].sum = sum;
    thread_data_array[t].message = messages[t];
    
    printf("In main: creando el hilo %d\n",t);

    rc = pthread_create(&threads[t],NULL,print_hola,(void *) &thread_data_array[t]);
    
    if(rc){
      printf("ERROR; return code from pthread_create() is %d\n",rc);
      exit(-1);
    }
  }
  
  //Last thing that main() should do
  pthread_exit(NULL);
  
}
