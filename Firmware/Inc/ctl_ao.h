#ifndef CTL_AO_H
#define CTL_AO_H

#include "ctl_api.h"

#include <stdint.h>

/* Enums are 16 bits wide */
typedef uint16_t ctl_ao_signal;

/* Forward declare the structs */
typedef struct ctl_active_object ctl_active_object;
typedef struct ctl_ao_event ctl_ao_event;

/* Prototype for the dispatch handler of an active object */
typedef void (*ctl_ao_dispatch_handler)(ctl_active_object * const me, ctl_ao_event const * const e);

/* Private signals */
typedef enum {
  INIT_SIG = 0
}ctl_priv_sig;

/* Event base class, inherited by active object event sub class */
struct ctl_ao_event {
  ctl_ao_signal sig;
};

/* Active object. Inherited by specific active object sub classes */
struct ctl_active_object {
  CTL_TASK_t *ao_task; /* Task handle */
  CTL_MESSAGE_QUEUE_t *ao_msg_q; /* Message queue handle */

  ctl_ao_dispatch_handler dispatch;
};

void ctl_ao_post(ctl_active_object *const me, ctl_ao_event const *const event);
void ctl_ao_event_loop(void *params);
void ctl_ao_ctor(ctl_active_object *const me, ctl_ao_dispatch_handler dispatch_handler);
void ctl_ao_start(ctl_active_object *const me,
    CTL_TASK_t *const task,
    uint8_t priority,
    uint32_t *const stk,
    uint16_t stk_size,
    CTL_MESSAGE_QUEUE_t *const msg_q,
    ctl_ao_event **const evt_q,
    uint8_t msg_q_size,
    const char *name);

extern ctl_active_object test;
extern ctl_active_object *front_panel_ao;

#endif /* CTL_AO_H */