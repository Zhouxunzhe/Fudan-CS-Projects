#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>

static int nthread = 1;
static int round = 0;
static int r = 20000;
pthread_mutex_t lock;

struct barrier {
  pthread_mutex_t barrier_mutex;
  pthread_cond_t barrier_cond;
  int nthread;      // Number of threads that have reached this round of the barrier
  int round;     // Barrier round
} bstate;

double
now()
{
 struct timeval tv;
 gettimeofday(&tv, 0);
 return tv.tv_sec + tv.tv_usec / 1000000.0;
}

static void
barrier_init(void)
{
  assert(pthread_mutex_init(&bstate.barrier_mutex, NULL) == 0);
  assert(pthread_cond_init(&bstate.barrier_cond, NULL) == 0);
  bstate.nthread = 0;
}

static void 
barrier()
{
  // YOUR CODE HERE
  //
  // Block until all threads have called barrier() and
  // then increment bstate.round.
  // 
  pthread_mutex_lock(&bstate.barrier_mutex); // avoid one thread races around the loop before the others have exited the barrier
  bstate.nthread++;
  if(bstate.nthread==nthread){ // all threads have reached the barrier
    bstate.nthread=0;
    bstate.round++;
    pthread_cond_broadcast(&bstate.barrier_cond); // wake up every thread sleeping on cond
  }
  else
    pthread_cond_wait(&bstate.barrier_cond, &bstate.barrier_mutex); // go to sleep on cond, releasing lock mutex, acquiring upon wake up
  pthread_mutex_unlock(&bstate.barrier_mutex);
  
}

static void *
thread(void *xa)
{
  long n = (long) xa;
  long delay;
  int i;

  for (i = 0; i < r; i++) {
    int t = bstate.round;
    assert (i == t);
    barrier();
    usleep(random() % 100);
  }

  return 0;
}

int
main(int argc, char *argv[])
{
  pthread_t *tha;
  void *value;
  long i;
  double t1, t0;

  if (argc < 2) {
    fprintf(stderr, "%s: %s nthread\n", argv[0], argv[0]);
    exit(-1);
  }
  nthread = atoi(argv[1]); // set number of threads 
  tha = malloc(sizeof(pthread_t) * nthread);
  srandom(0);

  t0=now();
  barrier_init();

  for(i = 0; i < nthread; i++) {
    assert(pthread_create(&tha[i], NULL, thread, (void *) i) == 0);
  }
  for(i = 0; i < nthread; i++) {
    assert(pthread_join(tha[i], &value) == 0);
  }
  t1=now();
  printf("OK; passed; time: %.3f seconds\n", t1-t0);
}
