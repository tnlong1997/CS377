#include "ProducerConsumer.h"
#include <iostream>
#include <fstream>

//TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *buffer = new BoundedBuffer(100);
void *consumer(void *param);
void *producer(void *param);
int ps, cs, n;
ofstream myfile;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
	//TODO: constructor to initialize variables declared
	//also see instruction for implementation
  pthread_t producer_thread[p], consumer_thread[c];
  // freopen("output.txt","w",stdout);
  for (int i = 0; i < p; i++) {
    pthread_create(&producer_thread[i], NULL, producer, NULL);
  }

  for (int i = 0; i < c; i++) {
    pthread_create(&consumer_thread[i], NULL, consumer, NULL);
  }

  ps = psleep;
  cs = csleep;
  n = items;
  pthread_exit(NULL);
}

void* producer(void* threadID){
	//TODO: producer thread, see instruction for implementation
  // int* index = (int*) threadID;

	for(int i = 0; i < n; i++){
    sleep(ps);
    printf("Producer: %d\n", i);
		// cout << "Producer: " << i << endl;

		buffer->append(i);
	}

	return NULL;
}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
  // int* index = (int*) threadID;

	for(int i = 0; i < n; i++){
    sleep(cs);
    printf("Consumer: %d\n", i);
		// cout << "Consumer: " << i << endl;
		buffer->remove();
	}

	return NULL;
}
