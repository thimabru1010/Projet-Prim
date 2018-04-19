#ifndef __THREADS_PORTS_H
#define __THREADS_PORTS_H

#include "threads_types.h"

StatusType SendOutput_runtime(thread_queue_t * global_q, int port_id, void * value);

StatusType NextValue_runtime(thread_queue_t * global_q, int port_id, void * dst);

StatusType GetValue_runtime(thread_queue_t * global_q, int port_id, void * dst);

StatusType ReceiveInputs_runtime(thread_queue_t * global_q, int port_id);

StatusType GetValueDataPort_runtime(data_port_t * p, void * data);

StatusType PutValueDataPort_runtime(data_port_t * p, void * data);

StatusType GetCount_runtime(thread_queue_t * global_q, int port_id, int * count_res);

#endif


