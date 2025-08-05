#include <iostream>
#include "ProcessManagement.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include "../encryptDecrypt/Cryption.hpp"
#include <sys/mman.h>
#include <atomic>
#include <semaphore.h>

ProcessManagement::ProcessManagement() {
    // Clean up any existing semaphores
    sem_unlink("/items_semaphore");
    sem_unlink("/empty_slots_semaphore");
    
    itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);
    emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000);
    
    if (itemsSemaphore == SEM_FAILED || emptySlotsSemaphore == SEM_FAILED) {
        std::cerr << "Failed to create semaphores" << std::endl;
        exit(1);
    }
    
    // Clean up any existing shared memory
    shm_unlink(SHM_NAME);
    
    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shmFd == -1) {
        std::cerr << "Failed to create shared memory" << std::endl;
        exit(1);
    }
    
    ftruncate(shmFd, sizeof(SharedMemory));
    sharedMem = static_cast<SharedMemory *>(mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0));
    
    if (sharedMem == MAP_FAILED) {
        std::cerr << "Failed to map shared memory" << std::endl;
        exit(1);
    }
    
    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
}

ProcessManagement::~ProcessManagement() {
<<<<<<< HEAD
    if (sharedMem != MAP_FAILED) {
        munmap(sharedMem, sizeof(SharedMemory));
    }
    if (shmFd != -1) {
        close(shmFd);
    }
    shm_unlink(SHM_NAME);
    
    if (itemsSemaphore != SEM_FAILED) {
        sem_close(itemsSemaphore);
    }
    if (emptySlotsSemaphore != SEM_FAILED) {
        sem_close(emptySlotsSemaphore);
    }
    sem_unlink("/items_semaphore");
    sem_unlink("/empty_slots_semaphore");
=======
    munmap(sharedMem, sizeof(SharedMemory));
    shm_unlink(SHM_NAME);
>>>>>>> 463f0fb66506374217f8c0edd6b3825fe311733f
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    sem_wait(emptySlotsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);

    if (sharedMem->size.load() >= 1000) {
<<<<<<< HEAD
        sem_post(emptySlotsSemaphore);
        return false;
    }
    
    std::string taskStr = task->toString();
    if (taskStr.length() >= 256) {
        sem_post(emptySlotsSemaphore);
        return false;
    }
    
    strcpy(sharedMem->tasks[sharedMem->rear], taskStr.c_str());
=======
        return false;
    }
    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());
>>>>>>> 463f0fb66506374217f8c0edd6b3825fe311733f
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);
    lock.unlock();
    sem_post(itemsSemaphore);

    int pid = fork();
    if (pid < 0) {
        return false;
    } else if (pid == 0) {
        executeTask();
        exit(0);
    }
    return true;
}

void ProcessManagement::executeTask() {
    sem_wait(itemsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);
    char taskStr[256];
    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);
    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(1);
    lock.unlock();
    sem_post(emptySlotsSemaphore);

    executeCryption(taskStr);
}