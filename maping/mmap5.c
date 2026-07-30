#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>


typedef struct User
{
    int id;
    char name[50];
    int age;
} User;

typedef struct
{
    int users_count;
    User users[100];
    int flag;
    int findid;            
    char foundname[50];   
    double avg;
    int maxage;
    char oldest_name[50];
} Database;


int main()
{
   
    Database *mem = mmap(NULL, sizeof(Database), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

    if (mem == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    mem->flag = 0;
    mem->findid = -1;
    mem->foundname[0] = '\0';

    
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        munmap(mem, sizeof(Database));
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
       
        while (mem->flag == 0)
        {
            sleep(1);
        }

        char choice;
        int found = 0;

        printf("[CHILD1] Search by (i)d or (n)ame? ");
        scanf(" %c", &choice);

        if (choice == 'i' || choice == 'I')
        {
            int id;
            printf("[CHILD1] Enter ID to search: \n");
            scanf("%d", &id);

            for (int i = 0; i < mem->users_count; i++)
            {
                if (mem->users[i].id == id)
                {
                    mem->findid = mem->users[i].id;
                    strcpy(mem->foundname, mem->users[i].name);
                    found = 1;
                    break;
                }
            }
        }
        else if (choice == 'n' || choice == 'N')
        {
            char name[50];
            printf("[CHILD1] Enter name to search: \n");
            scanf("%49s", name);

            for (int i = 0; i < mem->users_count; i++)
            {
                if (strcmp(mem->users[i].name, name) == 0)
                {
                    mem->findid = mem->users[i].id;
                    strcpy(mem->foundname, mem->users[i].name);
                    found = 1;
                    break;
                }
            }
        }
        else
        {
            printf("[CHILD1] Invalid choice.\n");
        }

        if (!found)
        {
            mem->findid = -1;
            strcpy(mem->foundname, "Not found");
        }

        printf("[CHILD1] Search finished\n");

        munmap(mem, sizeof(Database));
        exit(0);
    }

    
    pid_t pid1 = fork();

    if (pid1 < 0)
    {
        perror("fork");
        munmap(mem, sizeof(Database));
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0)
    {
        while (mem->flag == 0)
        {
            sleep(1);
        }

        int sum = 0;
        for (int i = 0; i < mem->users_count; i++)
        {
            sum += mem->users[i].age;
        }

        
        if (mem->users_count > 0)
        {
            mem->avg = (double)sum / mem->users_count;
        }
        else
        {
            mem->avg = 0;
        }

        printf("[CHILD2] Average age calculated\n");

        munmap(mem, sizeof(Database));
        exit(0);
    }

   
    pid_t pid2 = fork();

    if (pid2 < 0)
    {
        perror("fork");
        munmap(mem, sizeof(Database));
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0)
    {
        while (mem->flag == 0)
        {
            sleep(1);
        }

        if (mem->users_count > 0)
        {
           
            int max = mem->users[0].age;
            strcpy(mem->oldest_name, mem->users[0].name);

            for (int i = 1; i < mem->users_count; i++)
            {
                if (mem->users[i].age > max)
                {
                    max = mem->users[i].age;
                    strcpy(mem->oldest_name, mem->users[i].name);
                }
            }
            mem->maxage = max;
        }

        printf("[CHILD3] Oldest user found\n");

        munmap(mem, sizeof(Database));
        exit(0);
    }

   
    printf("Enter the count of users: ");
    scanf("%d", &mem->users_count);

    if (mem->users_count <= 0 || mem->users_count > 100)
    {
        printf("Invalid count!\n");
        munmap(mem, sizeof(Database));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < mem->users_count; i++)
    {
        printf("Enter ID, Name, Age: \n");
        scanf("%d %49s %d", &mem->users[i].id, mem->users[i].name, &mem->users[i].age);
    }

    printf("[PARENT] My data is ready\n");

    
    mem->flag = 1;

    
    wait(NULL);
    wait(NULL);
    wait(NULL);

    printf("\n[PARENT] FINAL DATABASE RESULT\n");
    if (mem->findid == -1)
        printf("Search result: %s\n", mem->foundname);
    else
        printf("Search result: ID = %d, Name = %s\n", mem->findid, mem->foundname);

    printf("Average age = %.2f\n", mem->avg);
    printf("Oldest user: %s (age %d)\n", mem->oldest_name, mem->maxage);

    munmap(mem, sizeof(Database));
    return 0;
}