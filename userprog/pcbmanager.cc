#include "pcbmanager.h"


PCBManager::PCBManager(int maxProcesses):maxProcesses(maxProcesses) {
    this->maxProcesses = maxProcesses;
    bitmap = new BitMap(maxProcesses);
    pcbs = new PCB*[maxProcesses];
    pcbManagerLock = new Lock("PCBManagerLock");
    for(int i = 0; i < maxProcesses; i++) {
        pcbs[i] = NULL;
    }

}


PCBManager::~PCBManager() {
 for (int i = 0; i < maxProcesses; i++) {
        delete pcbs[i];
    }
     delete[] pcbs;
    delete bitmap;

    

}


PCB* PCBManager::AllocatePCB() {

    // Aquire pcbManagerLock
    pcbManagerLock->Acquire();
    //printf("Made it to Acquire Allocate\n");
    int pid = bitmap->Find();

    // Release pcbManagerLock
    pcbManagerLock->Release();
    //printf("Made it to Release allocate\n");
    ASSERT(pid != -1);

    pcbs[pid] = new PCB(pid);

    return pcbs[pid];

}


int PCBManager::DeallocatePCB(PCB* pcb) {

    // Check is pcb is valid -- check pcbs for pcb->pid
    int pid = pcb->pid;
if(pid < 0 || pid >= maxProcesses) {
    printf("Invalid PID\n");
    return -1;
}
      //Aquire pcbManagerLock
    pcbManagerLock->Acquire();
    //printf("Made it to acquire deallocate\n");
    bitmap->Clear(pid);

    // Release pcbManagerLock
    pcbManagerLock->Release();
     //printf("Made it to release\n");

    delete pcbs[pid];

    pcbs[pid] = NULL;

    return 0;

}

PCB* PCBManager::GetPCB(int pid) {
    return pcbs[pid];
}