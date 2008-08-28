/* Copyright 2007 Google Inc. All Rights Reserved.
 * Author: falmeida@google.com (Filipe Almeida)
 */

#ifndef __NEO_STATEMACHINE_H_
#define __NEO_STATEMACHINE_H_

#ifdef __cplusplus
namespace security_streamhtmlparser {
#endif

/* TODO(falmeida): I'm not sure about these limits, but since right now we only
 * have 24 states it should be fine */

enum {
    STATEMACHINE_ERROR = 127
};

#define STATEMACHINE_RECORD_BUFFER_SIZE 256

struct statetable_transitions_s {
  const char *condition;
  int source;
  int destination;
};

struct statemachine_ctx_s;

typedef void(*state_event_function)(struct statemachine_ctx_s *, int, char,
                                    int);

typedef struct statemachine_definition_s {
    int num_states;
    int **transition_table;
    state_event_function *in_state_events;
    state_event_function *enter_state_events;
    state_event_function *exit_state_events;
} statemachine_definition;

typedef struct statemachine_ctx_s {
    int current_state;
    int next_state;
    statemachine_definition *definition;
    char current_char;
    char record_buffer[STATEMACHINE_RECORD_BUFFER_SIZE];
    size_t record_pos;

    /* True if we are recording the stream to record_buffer. */
    int recording;

    /* Storage space for the layer above. */
    void *user;
} statemachine_ctx;

void statemachine_definition_populate(statemachine_definition *def,
                                     const struct statetable_transitions_s *tr);

void statemachine_in_state(statemachine_definition *def, int st,
                           state_event_function func);
void statemachine_enter_state(statemachine_definition *def, int st,
                                     state_event_function func);
void statemachine_exit_state(statemachine_definition *def, int st,
                                    state_event_function func);

statemachine_definition *statemachine_definition_new(int states);
void statemachine_definition_delete(statemachine_definition *def);

int statemachine_get_state(statemachine_ctx *ctx);
void statemachine_set_state(statemachine_ctx *ctx, int state);

void statemachine_start_record(statemachine_ctx *ctx);
const char *statemachine_stop_record(statemachine_ctx *ctx);
const char *statemachine_record_buffer(statemachine_ctx *ctx);

/* Returns the the number of characters currently stored in the record buffer.
 */
static inline size_t statemachine_record_length(statemachine_ctx *ctx) {
  return ctx->record_pos + 1;
}

/* Initializes a new statemachine. Receives a statemachine definition object
 * that should have been initialized with statemachine_definition_new() and a
 * user reference to be used by the caller.
 *
 * Returns NULL if initialization fails.
 *
 * Initialization failure is fatal, and if this function fails it may not
 * deallocate all previsouly allocated memory.
 */
statemachine_ctx *statemachine_new(statemachine_definition *def,
                                   void *user);

/* Returns a pointer to a context which is a duplicate of the statemachine src.
 * The statemachine definition and the user pointer have to be provided since
 * these references are not owned by the statemachine itself.
 */
statemachine_ctx *statemachine_duplicate(statemachine_ctx *ctx,
                                         statemachine_definition *def,
                                         void *user);

/* Copies the context of the statemachine pointed to by src to the statemachine
 * provided by dst.
 * The statemachine definition and the user pointer have to be provided since
 * these references are not owned by the statemachine itself.
 */
void statemachine_copy(statemachine_ctx *dst,
                       statemachine_ctx *src,
                       statemachine_definition *def,
                       void *user);

int statemachine_parse(statemachine_ctx *ctx, const char *str, int size);

void statemachine_delete(statemachine_ctx *ctx);

#ifdef __cplusplus
}  /* namespace security_streamhtmlparser */
#endif

#endif /* __NEO_STATEMACHINE_H_ */

