#include "threads_dispatch.h"

#ifdef DEBUG
#include <stdio.h>
#endif

struct timespec start_timespec;

int create_fp_thread (unsigned int priority,
		      unsigned int stack_size,
		      void* (*start_routine)(void),
		      pthread_t * tid,
		      int policy)
{
  int                sys_ret;
  pthread_attr_t     attr;
  struct sched_param param;

  sys_ret = pthread_attr_init (&attr);
  if (sys_ret != 0)
    {
#ifdef DEBUG
      printf("Error: thread attr init %d\n", sys_ret);
#endif
      return sys_ret;
    }

  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

  sys_ret = pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);
  if (sys_ret != 0)
    {
#ifdef DEBUG
      printf("Error: thread attr set scope %d\n", sys_ret);
#endif
      return sys_ret;
    }
  
  if (stack_size != 0)
    {
      // On some systems, pthread_attr_setstacksize() can fail with the error
      // EINVAL if stacksize is not a multiple of the system page size.
      sys_ret = pthread_attr_setstacksize (&attr, stack_size);
      if (sys_ret != 0)
	{
#ifdef DEBUG
          printf("Error: thread attr set stack size %d\n", sys_ret);
#endif
	  return sys_ret;
	}
    }

  sys_ret = pthread_attr_setschedpolicy(&attr, policy);
  if (sys_ret != 0)
    {
      return sys_ret;
    }

#ifdef Sol_q1
  param.sched_priority = priority;
  sys_ret = pthread_attr_setschedparam (&attr, &param);
  if (sys_ret != 0)
  {
#ifdef DEBUG
    printf("Error: thread attr set sched param %d\n", sys_ret);
#endif
    return sys_ret;
  }
#endif


  sys_ret = pthread_create (tid, &attr, (void* (*)(void*))start_routine, NULL);
  return sys_ret;

}

int init_sporadic_config (thread_config_t * config)
{
  sporadic_thread_config_t info = config->sporadic_config;
  int ret =  pthread_mutex_init(info.global_q->rez, NULL);
  ret = pthread_cond_init(info.global_q->event, NULL);
  return ret;
}

int init_periodic_config (thread_config_t * config)
{
  periodic_thread_config_t info = config->periodic_config;
  int ret;
#ifdef Sol_q1_v2
  static int next_sig;
  unsigned int ns;
  unsigned int sec;
  struct sigevent sigev;
  struct itimerspec itval;
  
  /* Initialise next_sig first time through. We can't use static
     initialisation because SIGRTMIN is a function call, not a constant */
  if (next_sig == 0)
    next_sig = SIGRTMIN;
  /* Check that we have not run out of signals */
  if (next_sig > SIGRTMAX)
    return -1;
  info.sig = next_sig;
  next_sig++;

  info.wakeups_missed = 0;
  
  /* Create the signal mask that will be used in wait_period */
  sigemptyset (&(config->periodic_config.alarm_sig));
  sigaddset (&(info.alarm_sig), info.sig);

  /* Create a timer that will generate the signal we have chosen */
  sigev.sigev_notify = SIGEV_SIGNAL;
  sigev.sigev_signo = info.sig;
  sigev.sigev_value.sival_ptr = (void *) &info.timer_id;
  ret = timer_create (CLOCK_MONOTONIC, &sigev, &info.timer_id);
  if (ret == -1)
    return ret;

  /* Make the timer periodic */
  sec = info.period/1000000;
  ns = (info.period - (sec * 1000000)) * 1000;
  itval.it_interval.tv_sec = sec;
  itval.it_interval.tv_nsec = ns;
  itval.it_value.tv_sec = sec;
  itval.it_value.tv_nsec = ns;
  ret = timer_settime (info.timer_id, 0, &itval, NULL);
#endif
  return ret;
}

#ifdef Sol_q1_v2
int mask_realtime_signals()
{
  int res;
#ifndef USE_MACH
#ifdef USE_POSIX

  sigset_t alarm_sig;
  int i;

  /* Block all real time signals so they can be used for the timers.
     Note: this has to be done in main() before any threads are created
     so they all inherit the same mask. Doing it later is subject to
     race conditions */
  sigemptyset (&alarm_sig);
  for (i = SIGRTMIN; i <= SIGRTMAX; i++)
    sigaddset (&alarm_sig, i);
  sigprocmask (SIG_BLOCK, &alarm_sig, NULL);
#endif
#endif
  return res;
}
#endif

StatusType await_periodic_dispatch(thread_config_t * config)
{
  periodic_thread_config_t * info = &config->periodic_config;
#ifdef Sol_q1
#ifdef Sol_q1_v2
  int sig;
  sigwait (&(info.alarm_sig), &sig);
  info.wakeups_missed += timer_getoverrun (info.timer_id);
#else

  struct timespec periodic_timespec;
  unsigned long sec = info->period/1000;
  unsigned long ns = (info->period*1000 - (sec * 1000000)) * 1000;
  
  info->iteration_counter++;
  periodic_timespec.tv_sec = start_timespec.tv_sec+ info->iteration_counter*sec;
  periodic_timespec.tv_nsec = start_timespec.tv_nsec+ info->iteration_counter*ns;

  if(periodic_timespec.tv_nsec >= 1000000000) {
    periodic_timespec.tv_nsec -= 1000000000;
    periodic_timespec.tv_sec++;
  }
  clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &periodic_timespec, NULL);

#endif
#endif
  return E_OK;
}

StatusType await_sporadic_dispatch(thread_queue_t * global_q)
{
  thread_config_t * config = global_q->task;
  sporadic_thread_config_t * info = &config->sporadic_config;

  StatusType status = E_OK;

#ifdef Sol_q3
  // wait next period
  if(info->timing_wait_phase)
  {
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, info->sporadic_timespec, NULL);
  }
  info->timing_wait_phase = 1;

  // wait for messages
  status = pthread_mutex_lock(info->global_q->rez);
  if (status != E_OK)
    return status;

  if(info->global_q->msg_nb==0)
  {
    info->global_q->waiting=info->global_q->waiting+1;
    pthread_cond_wait(info->global_q->event, info->global_q->rez);
    info->global_q->waiting=info->global_q->waiting-1;
  }

  struct timespec sporadic_time_interval;
  unsigned long sec = info->time_interval/1000;
  unsigned long ns = (info->time_interval*1000 - (sec * 1000000)) * 1000;

#ifndef USE_MACH
  clock_gettime(CLOCK_MONOTONIC, &sporadic_time_interval);
#else

  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  sporadic_time_interval.tv_sec = mts.tv_sec;
  sporadic_time_interval.tv_nsec = mts.tv_nsec;
#endif

  info->sporadic_timespec->tv_sec = sporadic_time_interval.tv_sec+sec;
  info->sporadic_timespec->tv_nsec = sporadic_time_interval.tv_nsec+ns;

  pthread_mutex_unlock(info->global_q->rez);
  
#endif

  return status;
}

void set_start_time()
{
  clock_gettime(CLOCK_MONOTONIC, &start_timespec);
}

void display_relative_date(char * id, unsigned long iteration)
{
  struct timespec current_timespec;
#ifdef DEBUG
  clock_gettime(CLOCK_MONOTONIC, &current_timespec);
  unsigned long sec = current_timespec.tv_sec-start_timespec.tv_sec;
  unsigned long nsec = current_timespec.tv_nsec-start_timespec.tv_nsec;
  printf("%s - date: %ld sec: %ld nsec - iteration %ld\n", id, sec, nsec, iteration);
#endif
}
