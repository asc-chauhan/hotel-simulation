#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUF_SIZE 100

int main()
{

    printf("Enter Waiter ID:");
    int waiter_id;
    scanf("%d", &waiter_id);

    struct shmid_ds buf_mgr;
    key_t key_mgr; // key to identify shared memory segment
    // Generate a key for the shared memory segment
    if ((key_mgr = ftok("hotelmanager.c", 'A' + waiter_id)) == -1)
    {
        perror("Error in ftok\n");
        return 1;
    }
    // printf("Key is %d\n", key_mgr);
    int shm_mgr;                                             // shared memory segment identifier
    int *shmPtr_mgr;                                         // Pointer to the shared memory segment
    shm_mgr = shmget(key_mgr, (BUF_SIZE), 0644 | IPC_CREAT); // IPC_PRIVATE as 1st arg.
    if (shm_mgr == -1)
    {
        perror("Error in shmget in creating/ accessing shared memory\n");
        return 1;
    }
    // Attach the shared memory segment to the waiter process
    shmPtr_mgr = shmat(shm_mgr, NULL, 0); // NULL means kernel will find a place for the segment
    if (shmPtr_mgr == (void *)-1)
    {
        perror("Error in shmPtr_mgr in attaching the memory segment\n");
        return 1;
    }
    // if (shmctl(shm_mgr, IPC_STAT, &buf_mgr) != -1)
    // {
    //     printf("\nSegment size is %lu\n", buf_mgr.shm_segsz);
    //     printf("\nNo. of processes attached %lu\n", buf_mgr.shm_nattch);
    // }

    /////////////////////////////////////////////////////////////////////////////////// above code is shm creation and deletion for hotel manager

    struct shmid_ds buf;
    key_t key; // key to identify shared memory segment
    // Generate a key for the shared memory segment
    if ((key = ftok("table.c", 'A' + waiter_id)) == -1)
    {
        perror("Error in ftok\n");
        return 1;
    }
    // printf("Key is %d\n", key);
    int shmid;   // shared memory segment identifier
    int *shmPtr; // Pointer to the shared memory segment

    // Create a shared memory segment
    shmid = shmget(key, (BUF_SIZE), 0644 | IPC_CREAT); // IPC_PRIVATE as 1st arg.
    if (shmid == -1)
    {
        perror("Error in shmget in creating/ accessing shared memory\n");
        return 1;
    }
    // Attach the shared memory segment to the waiter process
    shmPtr = shmat(shmid, NULL, 0); // NULL means kernel will find a place for the segment
    if (shmPtr == (void *)-1)
    {
        perror("Error in shmPtr in attaching the memory segment\n");
        return 1;
    }
    if (shmctl(shmid, IPC_STAT, &buf) != -1)
    {
        // printf("\nSegment size is %lu\n", buf.shm_segsz);
        // printf("\nNo. of processes attached %lu\n", buf.shm_nattch);
    }
    // printf("Waiter reading...\n");
    /***************************************************************************************************/
    int price[10];
    price[0] = 0;
    while (shmPtr[0] != -6)
    {
    }
    int itemCount = 1;
    while (shmPtr[itemCount] != -6)
    {
        price[itemCount] = shmPtr[itemCount];
        itemCount++;
    }
    itemCount--;
    /***************************************************************************************************/
    while (1)
    {
        while (shmPtr[0] == -1)
        {
        }

        if (shmPtr[0] == -4)
        {
            // printf("waiter has exited\n");
            shmPtr[0] = -5;
            break;
        }

        int i = 0, bill = 0;

        int isInvalid = 0;
        while (shmPtr[i] != -3)
        {
        }
        i++;
        while (shmPtr[i] != -1) // finding index of last elements of orders(-1)
        {
            if (shmPtr[i] <= 0 || shmPtr[i] > itemCount)
            {
                isInvalid = 1;
                break;
            }
            else
            {
                bill += price[shmPtr[i]];
            }
            i++;
        }
        // int j=i;
        // while(shmPtr[j]!=-6)
        // {
        //     j++;
        // }
        // int itemCount=j-i-1; //finding of size of menu
        // j=1;
        // while (shmPtr[j] != -1)
        // {

            
        // }
        if (isInvalid == 1)
        {
            // i = 0;
            shmPtr[0] = -2;
        }
        else
        {

            shmPtr[1] = bill;
            shmPtr[0] = -1;
        }
        if (isInvalid == 0)
        {

            // shmPtr[0]=-1;
            shmPtr_mgr[1] = bill;
            shmPtr_mgr[0] = -1;

            printf("Bill Amount for Table %d: %d INR\n", waiter_id, bill);
        }

        // if(isInvalid==0) printf("Bill Amount for Table %d: %d INR\n",waiter_id, bill);
    }

    // Detach the shared memory segment from table's address space
    if (shmdt(shmPtr) == -1)
    {
        perror("Error in shmdt in detaching the memory segment\n");
        return 1;
    }
    shmPtr_mgr[0] = -2;
    // Detach the shared memory segment from table's address space
    if (shmdt(shmPtr_mgr) == -1)
    {
        perror("Error in shmdt in detaching the memory segment\n");
        return 1;
    }
    return 0;
}
