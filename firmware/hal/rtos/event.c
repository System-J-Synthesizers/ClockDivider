#include "event.h"
#include "task.h"
#include "kernel.h"
#include "os_assert.h"
#include "../hal_chip.h"
#include <stddef.h>

static os_evtq evt_queue_pool[NUM_TASKS-1U];

/**
 * @brief      Initialise an event queue for a task
 *
 * @param[in]  index   TODO look at this
 * @param[in]  q_size  size of the queue, in words (MUST BE POWER OF 2)
 * @param      data    queue data
 *
 * @return     pointer to the event queue object
 */
os_evtq *os_evtq_init(uint8_t index, uint8_t q_size, os_event **data) {
    OS_ASSERT(index <= NUM_TASKS-1U);
    OS_ASSERT(q_size > 0U);
    OS_ASSERT((q_size & 0x01U) == 0x00U);
    OS_ASSERT(data != NULL);

    os_evtq *e = &evt_queue_pool[index];
    e->eq_head = 0U;
    e->eq_tail = 0U;
    e->eq_count = 0U;
    e->eq_size = q_size-1U;
    e->eq_data = data;

    return e;
}

os_event *os_evtq_receive(os_tcb *t) {
    OS_ASSERT(t != NULL);
    os_event *e;
    os_evtq *q = t->evt_q;

    if(q->eq_count == 0U) {
        e = (os_event *)OS_EVTQ_EMPTY;
    } else {
        chip_disable_isrs();
        e = q->eq_data[q->eq_tail];
        /* power of two wrap around trick */
        q->eq_tail = (q->eq_tail + 1U) & q->eq_size;
        q->eq_count--;
        chip_enable_isrs();
    }
    return e;
}

int32_t os_evtq_post(os_tcb *t, os_event *evt) {
    OS_ASSERT(t != NULL);
    OS_ASSERT(evt != NULL);
    
    int32_t result;
    os_evtq *q = t->evt_q;

    if(q->eq_count == q->eq_size) {
        result = OS_EVTQ_FULL;
    } else {
        chip_disable_isrs();
        q->eq_data[q->eq_head] = evt;
        /* power of two wrap around trick */
        q->eq_head = (q->eq_head + 1U) & q->eq_size;
        q->eq_count++;
        
        os_task_set_ready_flag(t->priority);
        if(t->priority < os_get_current_task()->priority) {
          os_pend_context_switch();
        }

        chip_enable_isrs();
        result = OS_EVTQ_OK;
    }

    return result;
}
