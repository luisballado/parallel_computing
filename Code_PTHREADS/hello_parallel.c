#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

// Global variable (accessible to all threads)
int num_threads;

////////////////////////////////////////////////////////////////////////////////
void* hello( void* id ){
  long thread_id = (long)id;
  if(thread_id == -1) 
    printf("Hello from MAIN thread\n");
  else
    printf("Hello from thread %ld of %d\n", thread_id, num_threads);
  return NULL;
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){
  
  // Determine number of threads
  sscanf(argv[1], "%d", &num_threads);
  
  // Allocating threads
  pthread_t* threads;
  threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);
  
  // Creating the threads (branching, forking)
  for(long t=0; t<num_threads; t++)
    pthread_create(&threads[t], NULL, hello, (void*)t);
  
  // Do some work with main thread
  hello((void*)-1);
  
  // Terminate threads (join)
  for(long t=0; t<num_threads; t++)
    pthread_join(threads[t], NULL);
  
  // Deallocate memory
  free(threads);
  
  return 0;
}
////////////////////////////////////////////////////////////////////////////////
