#include "main.h"

int main() {
    //pthread_mutex_t m_mutex;
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t tid1, tid2 ,tid3 ,tid4;
    
    data.weight = 10;
    data.last_weight = 10;
    strcpy(data.type, "apple");
    strcpy(data.last_type, "apple");

    pthread_create(&tid1, NULL, (void*)print_thread, (void*)&data);
    pthread_create(&tid2, NULL, (void*)post, (void*)&data);
    pthread_create(&tid3, NULL, (void*)hx711, (void*)&data);
    pthread_create(&tid4, NULL, (void*)syn6288, (void*)&data);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&cond);

    pthread_exit(NULL);
    return 0;
}

