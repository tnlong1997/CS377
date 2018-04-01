#include "ProducerConsumer.h"
#include <iostream>
#include <fstream>

//TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *buffer = new BoundedBuffer(100);
void *consumer(void *param);
void *producer(void *param);
int ps, cs, n;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
	//TODO: constructor to initialize variables declared
	//also see instruction for implementation
  ofstream myfile;
  myfile.open("output.txt");
  pthread_t producer_thread, consumer_thread;

  for (int i = 0; i < c; i++) {
    pthread_create(&consumer_thread, NULL, consumer, NULL);
  }

  for (int i = 0; i < p; i++) {
    pthread_create(&producer_thread, NULL, producer, NULL);
  }

  ps = psleep;
  cs = csleep;
  n = items;
  myfile.close();
  pthread_exit(NULL);
}

void* producer(void* threadID){
	//TODO: producer thread, see instruction for implementation
  // int* index = (int*) threadID;

	for(int i = 0; i < n; i++){
    sleep(ps);
		printf("Producer: %d\n", i);

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
		buffer->remove();
	}

	return NULL;
}
