#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ass2.h>
#include <math.h>

long long int main(long long int argc, char** argv){
    long long int NUMB_OF_PROCESS = argc;
    long long int i;
    long long int MEMORY_SIZE = -1;
    long long int QUANTUM = 10;
    char ALGORITHM[3] = {'\0'};
    char ALLOCATION[3] = {'u','\0'};
    char FILENAME[500] = {'\0'};
    FILE *fp;
    char READINBUFFER[300];
    pages_t swapping;

    //generate two linked list to mimic the arrival 
    linked_list on_the_way,arrived;         
    creat_a_linked_list(&on_the_way);
    creat_a_linked_list(&arrived);

    //Get all the information from the command line
    for(i=0;i<argc;i++){
        if(argv[i][0] == '-'){
            if(strlen(argv[i]) != 2){
                printf("incorrect flag\n");
                exit(0);
            }
            switch (argv[i][1]){
            case 'f':
                strcpy(FILENAME,argv[i+1]);
                break;
            case 'a':
                strcpy(ALGORITHM,argv[i+1]);
                break;
            case 'm':
                strcpy(ALLOCATION,argv[i+1]);
                break;
            case 's':
                MEMORY_SIZE = atoi(argv[i+1]);
                break;
            case 'q':
                QUANTUM = atoi(argv[i+1]);
                break;
            default:
                printf("Invalid flag\n");
                exit(0);
                break;
            }
        }
    }

    long long int TOTAL_PAGES = MEMORY_SIZE / page_size;
    pages_t PAGES; 
    initial_a_swapping_page(&PAGES,TOTAL_PAGES);
    long long int CLOCK_TIME = 0;
    //Open  and read in the file
    fp=fopen(FILENAME, "r");
    while(fgets(READINBUFFER, 100, fp) != NULL){
        node_t* temp = generate_a_node(READINBUFFER);
        insert_at_the_trail(&on_the_way,temp);
        //insert_at_head(&on_the_way,temp);
    }
    fclose(fp);
    long long int total = on_the_way.length;
    //statistic
    // a [0                , 1                 , 2               , 3                           ,4                     ,5              ,6              ]
    // a [maximum_througput, minimum_throghput, current_interval, curren_interval_accumulation,total_tun_around_time,maxium overhead, total overhead]
    double statistic[7] = {-1, inifinity, 1, 0, 0, -1, 0};

    //Simulation here:
    while (arrived.head!= NULL || on_the_way.head != NULL){
        arrival_mimic(&arrived,&on_the_way,CLOCK_TIME);
        if(arrived.head == NULL){
           tick(&CLOCK_TIME,&arrived,&on_the_way);
        }
        process_a_node(ALGORITHM,ALLOCATION,QUANTUM,&arrived,&on_the_way,&CLOCK_TIME,&PAGES,statistic);

    }
 
    
    //cleaning the linked list
    free_a_linked_list(&on_the_way);
    free_a_linked_list(&arrived);
    free_pages(&PAGES);
    
    //print out the statistic
    double throughput_avg = total / statistic[current_interval];
    long long int throughput_avgin = (long long int)throughput_avg;
    if(throughput_avgin < throughput_avg){
        throughput_avgin++;
    } 
    printf("Throughput %lld, %.0f, %.0f\n",throughput_avgin,statistic[minimum_throghput],statistic[maximum_througput]);
    printf("Turnaround time %lld\n", myround_up(statistic[total_tun_around_time]/total));
    printf("Time overhead %.2f %.2f\n" ,statistic[maxium_overhead],(statistic[total_overhead]/total));
    printf("Makespan %lld\n",CLOCK_TIME);
    return 0;
}
