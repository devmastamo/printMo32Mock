#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H
#include "Commands.h"

#define COMMAND_QUEUE_SIZE 50

typedef struct CommandQueue_t
{
    Command pCommandBuffer[COMMAND_QUEUE_SIZE];
    size_t dBufferFill;
} CommandQueue;


void CommandBufferInit(Command * rpCommandBuffer);

// Returns count after dequeue & fills commmand pointer
size_t DequeueCommand(Command * rpCommand);

// Returns count after enqueue and copies new command to queue
size_t EnqueueCommand(Command * rpNewCommand);

size_t GetQueueCount();

Command * GetCommandQueue();

#endif // COMMAND_HANDLER_H