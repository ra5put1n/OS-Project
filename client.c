#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
 struct data{
	int service;
	char * string[20];  //service 1
	char * matrix[3][3];  //service 2
	char * factorial;  //service 3
	int shared_mem_id;
	int client_id;
}; 	

struct data_queue {
	struct data queue[100];
	int num;
};

struct ANS{
  int answer;
}*myans;


void my_handler(){}

int main()
{
	int pid = getpid();
	signal(SIGUSR1,my_handler);
	printf(" Client program has started \n");
	// Creatring Shared Memory for queue
		key_t keyForQueue = ftok("buffer.txt",10);
		if(keyForQueue<0){
			perror("errorKeyQueueClient: ");
			exit(0);
		}
		int shmForQueue= shmget(keyForQueue,sizeof(struct data_queue),IPC_CREAT | 0666);
		if(shmForQueue<0){
			perror("errorShmClient ");
			exit(0);
		} 
		struct data_queue* q = (struct data_queue*)shmat(shmForQueue,NULL,0);
		if(q==(void*)-1){
			perror("errorQueueClient: ");
			exit(0);
		}
	// shared memory created for queue

	// getting pid of client process and storing it into queue
	// server will use this key for kill(pid,SIGUSR1);
	q->queue[0].client_id = getpid();
	printf("Line 46 of client program\n");
	int num = q->num;
	printf("q->num updated by the server : %d\n",q->num);
	int c;
  printf("Enter 1 to check whether a string is palindrome or not?\n" );
  printf("Enter 2 to find determinant of a 3x3 matrix.\n" );
  printf("Enter 3 to find factorial of an integer?\n" );
  printf("Choose the service you want to avail (1/2/3) or -1 to exit: ");
	scanf("%d",&c);

	//cs
	q->num++;
	printf("q->num incremented : %d\n",q->num);
	//cd end

	key_t key1 = ftok("answer.txt", pid);
  if(key1==-1){
    perror("error0:");
    exit(1);
  }
  int shmid = shmget(key1, sizeof(struct ANS), IPC_CREAT | 0666);
  if(shmid<0){
    perror("error1:");
    exit(1);
  }
  myans=(struct ANS *)shmat(shmid,NULL,0);
  if(myans == (void *) -1){
    perror("error2:");
    exit(1);
  }

	pause();
	//cs
	int final_ans = myans->answer;
	//cs
	
  if(c==1)
  {
    if(final_ans == 0)
      printf("\nNOT palindrome");
    else
      printf("\nPalindrome");
  }
  else
    printf("\nAnswer is: %d",final_ans);
      
  exit(0);

}