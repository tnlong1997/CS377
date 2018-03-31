#include "BoundedBuffer.h"

BoundedBuffer::BoundedBuffer(int N){
	//TODO: constructor to initiliaze all the varibales declared in BoundedBuffer.h
	buffer_empty = PTHREAD_COND_INITIALIZER;
	buffer_lock = PTHREAD_MUTEX_INITIALIZER;
	buffer_full = PTHREAD_COND_INITIALIZER;

	buffer = (int *) malloc(N * sizeof(int));
	buffer_size = N;

	buffer_cnt = 0;
	buffer_last = 0;
}

void BoundedBuffer::append(int data){
	//TODO: append a data item to the circular buffer
	pthread_mutex_lock(&buffer_lock);
	if(buffer_cnt == buffer_size){
		pthread_cond_wait(&buffer_full, &buffer_lock);
	}
	buffer[buffer_last] = data;
	buffer_last = (buffer_last + 1) % buffer_size;
	buffer_cnt++;
	if(buffer_cnt == 1){
		pthread_cond_signal(&buffer_empty);
	}
	pthread_mutex_unlock(&buffer_lock);
}

int BoundedBuffer::remove(){
	//TODO: remove and return a data item from the circular buffer

	pthread_mutex_lock(&buffer_lock);

	if(buffer_cnt == 0){
		pthread_cond_wait(&buffer_empty, &buffer_lock);
	}

	int index = 0;
	index = (buffer_last + buffer_size);
	index = index - buffer_cnt;
	index = index % buffer_size;
	buffer_cnt--;

	if(buffer_cnt == buffer_size - 1){
		pthread_cond_signal(&buffer_full);
	}

	pthread_mutex_unlock(&buffer_lock);

	return buffer[index];
}

bool BoundedBuffer::isEmpty(){
	//TODO: check is the buffer is empty
	return buffer_cnt == 0;
}
