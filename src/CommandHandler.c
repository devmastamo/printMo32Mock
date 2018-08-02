#include "CommandHandler.h"

static CommandQueue CommandBuffer;

void CommandBufferInit(Command * rpCommandBuffer)
{
    memset(&CommandBuffer, 0, sizeof(CommandQueue));
}

// Returns count after dequeue & fills commmand pointer
size_t DequeueCommand(Command * rpCommand)
{
    if(CommandBuffer.dBufferFill)
    {
        memcpy(rpCommand, &CommandBuffer.pCommandBuffer[0], sizeof(Command));

        CommandBuffer.dBufferFill--;
        memmove(&CommandBuffer.pCommandBuffer[0], &CommandBuffer.pCommandBuffer[1], CommandBuffer.dBufferFill*sizeof(Command));
    }
    else
    {
        printf("NO MORE COMMANDS WERE ON THE QUEUE!");
        memset(rpCommand,0,sizeof(Command));
    }
    return CommandBuffer.dBufferFill;
}

// Returns count after enqueue and copies new command to queue
size_t EnqueueCommand(Command * rpNewCommand)
{
    if(CommandBuffer.dBufferFill < COMMAND_QUEUE_SIZE)
    {
        memcpy(&(CommandBuffer.pCommandBuffer[CommandBuffer.dBufferFill]), rpNewCommand, sizeof(Command));
        CommandBuffer.dBufferFill++;
    }
    else
    {
        printf("NO MORE SPACE IN QUEUE FOR NEW COMMAND");
    }
    return CommandBuffer.dBufferFill;
}

size_t GetQueueCount()
{
    return CommandBuffer.dBufferFill;
}

Command * GetCommandQueue()
{
    return CommandBuffer.pCommandBuffer;
}