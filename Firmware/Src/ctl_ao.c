#include "ctl_ao.h"
#include "ctl_api.h"

#include <stdint.h>

/* Posts an event to an active object */
void ctl_ao_post(ctl_active_object *const me, ctl_ao_event const *const event) {
  ctl_message_queue_post_nb(me->ao_msg_q, (void *)event); /* non blocking message post */
}

/* The event loop used by all active objects */
void ctl_ao_event_loop(void *params) {
  ctl_active_object *active = (ctl_active_object *)params; /* Passed in by constructor */

  /* Send in the init signal */
  static ctl_ao_event const first_event = {INIT_SIG};
  (*active->dispatch)(active, &first_event);

  while (1) {
    ctl_ao_event *event;

    /* Block until an event arrives in the message queue */
    ctl_message_queue_receive(active->ao_msg_q, (void *)&event, CTL_TIMEOUT_INFINITE, 0);

    /* Call event dispatch handler */
    (*active->dispatch)(active, event);
  }
}

void ctl_ao_ctor(ctl_active_object *const me, ctl_ao_dispatch_handler dispatch_handler) {
  me->dispatch = dispatch_handler;
}

void ctl_ao_start(ctl_active_object *const me,
    CTL_TASK_t *const task,
    uint8_t priority,
    uint32_t *const stk,
    uint16_t stk_size,
    CTL_MESSAGE_QUEUE_t *const msg_q,
    ctl_ao_event **const evt_q,
    uint8_t msg_q_size,
    const char *name) {

  me->ao_task = task;
  me->ao_msg_q = msg_q;

  ctl_message_queue_init(msg_q, (void *)evt_q, msg_q_size);
  ctl_task_run(task, priority, ctl_ao_event_loop, (void *)me, name, stk_size, stk, 0);
}