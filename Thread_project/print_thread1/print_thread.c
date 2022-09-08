#include "print_thread.h"

void* print_thread(void* thread_arg) 
{
    struct thread_data *my_data =  (struct thread_data *) thread_arg;


    pthread_mutex_lock(&m_mutex);

    printf("weight: %d\n",my_data->weight);
    printf("types: %s\n",my_data->type);

    pthread_mutex_unlock(&m_mutex);

    
    return NULL;
}
