#include <queue>
#include <semaphore>

class ThreadSafeQueue {
public:
	ThreadSafeQueue(){
		queue_sem  = sem_open("queue", O_CREAT, 0644, 1);
		n = sem_open("n", O_CREAT, 0644, 0);
		e = sem_open("e", O_CREAT, 0644, num_threads);
	};

	~ThreadSafeQueue(){
		sem_close(mailbox_sem);
		sem_close(queue_sem);
		sem_close(n);
		sem_close(e);

		sem_unlink("mailbox");
		sem_unlink("queue");
		sem_unlink("n");
		sem_unlink("e");

		delete queue_sem;
		delete n;
		delete e;
	};

	void push(int client){
        sem_wait(e);
        sem_wait(queue_sem);
        clients_queue.push(client);
        sem_post(queue_sem);
        sem_post(n);
	};

	int pop(){
        sem_wait(n);
        sem_wait(queue_sem);
        int client = clients_queue.front();
        clients_queue.pop();
        sem_post(queue_sem);
        sem_post(e);

        return client;
	};

private:
	queue<int> clients_queue;
    sem_t *queue_sem;
    sem_t *n;
    sem_t *e;
}