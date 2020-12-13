#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <ass2.h>


void
creat_a_linked_list(linked_list* queue){
  queue -> head = queue -> trail = NULL;
  queue->length = 0;
}

void
insert_at_head(linked_list* queue, node_t* current){
  (queue ->length)++;
  current -> previous = NULL;
  current -> next = NULL;
  if(queue -> head != NULL ){
    queue -> head -> previous = current;
    current -> next = queue->head;
  }
  queue -> head = current;
  if(queue -> trail == NULL){
    queue -> trail = current;
  }
}

void
insert_at_the_trail(linked_list* queue, node_t* current){
  (queue ->length)++;
  current -> next = NULL;
  current -> previous = NULL;
  if(queue -> head == NULL ){
    queue ->head = queue ->trail = current;
  }else{
    current -> previous = queue -> trail;
    queue-> trail -> next = current;
    queue -> trail = current;
  }
}


void
free_a_linked_list(linked_list* list){
  while(list -> head){
    (list->length)--;
    node_t* temple = list->head;
    list->head = list->head->next;
    free(temple);
  }
}

node_t*
generate_a_node(char* buffer){
  node_t* current = (node_t*)malloc(sizeof(node_t));
  current -> next = NULL;
  current -> previous = NULL;
  current -> last_used_time = -1; 
  current -> num_pages_in_memory = 0;
  /*conver the imformation in the buffer and insert in the node*/
  char *token = strtok(buffer," \n");      
  current -> arrivel_time =atoi(token);
  token = strtok(NULL, " \n");
  current -> id = atoi(token);
  token = strtok(NULL, " \n");
  current ->  require_size = (atoll(token)/page_size);
  token = strtok(NULL, " \n");
  current -> require_time = atoi(token);
  current -> total_time = current-> require_time;
  return current;
}

long long int
arrival_mimic(linked_list* arrived, linked_list* on_the_way,long long int clocktime){
  if(on_the_way->head == NULL){
    return 0;
  }
  while(on_the_way -> head != NULL && on_the_way->head->arrivel_time == clocktime){  
    node_t* temp = pop_a_linkedist(on_the_way);
    if(temp == NULL){
      return 0;
    }
    insert_in_order(arrived,temp);
  }
  return 1;
}

/*when the node arrived, insert it depends on its id*/
void
insert_in_order(linked_list* queue, node_t* current){
  current -> next = NULL;
  current -> previous = NULL;
  if(queue -> head == NULL ){
    queue ->head = queue ->trail = current;
    queue->length++; 
  }else{
    node_t *temp = queue -> trail;
    while (temp != NULL && current -> arrivel_time == temp ->arrivel_time && current->id < temp->id){
      temp = temp -> previous;
  }
  if(temp == NULL){
    insert_at_head(queue,current);
  }else if(temp == queue->trail){
    insert_at_the_trail(queue,current);
  }else{
    temp->next->previous = current;
    current->next = temp -> next;
    temp->next = current;
    current->previous = temp;
    queue->length++; 
    }
  }
}

/*this function mimic time, when every second pass, it check arrival*/
void
tick(long long int* CLOCK_TIME, linked_list* arrived, linked_list* on_the_way){
  (*CLOCK_TIME)++;
  arrival_mimic(arrived,on_the_way,(*CLOCK_TIME));
  
}

long long int
round_up_only(double a){
  long long int b = (long long int)a;
  if ( b < a){
    return (b + 1);
  }else{
    return b;
  }
}

/*this function put the minimum reamining time node to the head,used for cs*/
void
least_remaining_time(linked_list* queue){
  node_t* temp;
  node_t* minimum = NULL;
  long long int least_reamining_time = inifinity;
  temp = queue -> trail;
  while(temp != NULL){
    long long int temp_need = (temp->require_size - temp->num_pages_in_memory) * 2 + temp->require_time;
    if(temp_need < least_reamining_time){
      minimum = temp;
      least_reamining_time = temp_need;
    }
    temp = temp ->previous;
  }
  if(minimum == NULL){
    return;
  }
  if(minimum == queue ->head){
    return;
  }else if(minimum == queue->trail){
    queue -> trail = minimum->previous;
    queue -> trail-> next =NULL;
  }else{
    minimum ->previous ->next = minimum ->next;
    minimum -> next -> previous = minimum ->previous;
  }
  queue->length --;
  insert_at_head(queue,minimum);
}

void
print_occupyed_pages(node_t* temp, pages_t* page){
  long long int id = temp -> id;
  long long int is_the_first = 1;
  long long int usage = round_up_only((double)((page->CAPACTIY  - page->REMEAING_SIZE)/(double)page->CAPACTIY)*100);
  printf(", mem-usage=%lld%%, mem-addresses=[",usage);
  for(long long int i = 0; i < page -> CAPACTIY; i++){
    if(page->PAGETABEL[i] == id){
      if (is_the_first){
      printf("%lld",i);
      is_the_first = 0;
      }else{
        printf(",%lld",i);
      } 
    }     
  }
  printf("]\n");
}

void
process_a_node(char* AFLAG,char* MFLAG,long long int quantuam, linked_list* arrived,linked_list* on_the_way, long long int* CLOCK_TIME, pages_t* page,double* STASTIC){
 if((!strcmp(AFLAG,"cs"))){
    least_remaining_time(arrived);
 }
  node_t* the_one = pop_a_linkedist(arrived);
  if(the_one == NULL){
    return;
  }
  if((strcmp(MFLAG,"u"))){//if the memory manmagement option is activated 
    //here is the swapping
    if((!strcmp(MFLAG,"p"))){
   
      if(page->REMEAING_SIZE < (the_one -> require_size -(the_one -> num_pages_in_memory))){
        removing_all(arrived,(*CLOCK_TIME),(the_one ->require_size - page->REMEAING_SIZE) ,page,"v");
      }
    long long int outside_pages = the_one -> require_size - the_one ->num_pages_in_memory;
    insert_in_pages(page,(page,(the_one->require_size) - (the_one -> num_pages_in_memory)),the_one);
    printf("%lld, RUNNING, id=%lld, remaining-time=%lld",(*CLOCK_TIME),the_one->id,the_one->require_time);
    printf(", load-time=%lld",outside_pages*2);
    print_occupyed_pages(the_one,page);
    the_one -> require_time = the_one ->require_time + outside_pages*2;
    quantuam = quantuam + outside_pages*2; 
  
    }
    // Virtual memory when v, or custemrised memory allocation when cm
    if((!strcmp(MFLAG,"v")) ||(!strcmp(MFLAG,"cm")) ){

      quantuam = quantuam + virtual_memory_allocation(arrived,the_one,page,(*CLOCK_TIME),MFLAG);
    }  
  }else{
    printf("%lld, RUNNING, id=%lld, remaining-time=%lld\n",(*CLOCK_TIME),the_one->id,the_one->require_time);
  }
  while(the_one -> require_time){
    (the_one -> require_time) --;
    quantuam--;
    tick(CLOCK_TIME,arrived,on_the_way);
    //round_robin if rr is true (break when running out the quantum)
    if(strcmp(AFLAG,"rr") == 0 && quantuam == 0){
      break;
    }
  }

  //finish the process; 
  the_one ->last_used_time  = (*CLOCK_TIME);
  if(the_one->require_time == 0){
    if((strcmp(MFLAG,"u"))){
      removing_a_finished_point_and_print(page,the_one,(*CLOCK_TIME));
    } 
    printf("%lld, FINISHED, id=%lld, proc-remaining=%lld\n",(*CLOCK_TIME),the_one->id,arrived->length);
    
    /* thoughtput*/
    if((((*CLOCK_TIME)-1)/minute + 1)<= (long long int)STASTIC[current_interval]){
       STASTIC[curren_interval_accumulation] = STASTIC[curren_interval_accumulation] + 1;
    }  
    if(STASTIC[curren_interval_accumulation] > STASTIC[maximum_througput] ){
      STASTIC[maximum_througput] = STASTIC[curren_interval_accumulation];
    }
    if(STASTIC[curren_interval_accumulation] < STASTIC[minimum_throghput] ){
      STASTIC[minimum_throghput] = STASTIC[curren_interval_accumulation];
    }
    if((((*CLOCK_TIME)-1)/minute + 1)> (long long int)STASTIC[current_interval]){
      if(STASTIC[current_interval] + 1 < ((long long int)(*CLOCK_TIME - 1)/minute + 1)){
      STASTIC[minimum_throghput] = 0;
      }
      STASTIC[current_interval] = ((*CLOCK_TIME - 1)/minute + 1);
      STASTIC[curren_interval_accumulation] = 1;
    }
    /* turnaround and time overhead*/
    double turnaround_time = ((*CLOCK_TIME) - the_one->arrivel_time);
    STASTIC[total_tun_around_time] = STASTIC[total_tun_around_time] + turnaround_time;
    double time_over_head = (turnaround_time / the_one -> total_time );
    if(time_over_head > STASTIC[maxium_overhead]){
      STASTIC[maxium_overhead] = time_over_head;
    }
    STASTIC[total_overhead] = time_over_head + STASTIC[total_overhead];
    free(the_one);
  }else{
    insert_at_the_trail(arrived,the_one);
  }
}

void
removing_a_finished_point_and_print(pages_t* page,node_t* finishend,long long int ClockTime){
  long long int gonna_Removed[page->CAPACTIY];
  int_array_initializer(gonna_Removed,page->CAPACTIY);
  removing_n_from_page_table(page,finishend,finishend->num_pages_in_memory,gonna_Removed);
  evicted_printer(ClockTime, gonna_Removed, page->CAPACTIY);   
}

node_t*
pop_a_linkedist(linked_list* queue) {
  if (queue->head != NULL) {
    (queue->length)--;
    node_t *temp = queue -> head;     
    if( temp->next == NULL){
      queue -> trail = NULL;
    }else{
        queue -> head -> next -> previous = NULL; 
    }
    queue -> head = queue -> head -> next;
    return temp;
  }
  return NULL;
}

/* pages */
/* SWAPPING STUFFS */
void
initial_a_swapping_page(pages_t* pages, long long int num_of_page){
  pages -> PAGETABEL = malloc(num_of_page*sizeof(long long int));
  long long int i = 0;
  for (long long int i = 0; i < num_of_page; i++){
    (pages -> PAGETABEL)[i] = -1;
  }
  pages -> CAPACTIY = num_of_page;
  pages -> REMEAING_SIZE = num_of_page;
}

void
free_pages(pages_t* pages){
  free(pages->PAGETABEL);
}

/*find least recenttly used node by default, of find the smallest required size node when aflag ==cm*/
node_t*
get_LCU(linked_list* queue,long long int Clock_time,char* AFLAG){
  node_t* temp;
  node_t* last = NULL;
  long long int latest = inifinity;
  long long int size =  inifinity;
  temp = queue -> trail;
  while(temp != NULL){
    if(!(strcmp(AFLAG,"cm"))){
      //my algorithm
      if((temp ->num_pages_in_memory > 0) &&(temp->require_size < size)){
        last = temp;
        size = temp->require_size;
      }       
    }
    else{
      if((temp ->num_pages_in_memory > 0) &&(temp->last_used_time < latest)){
        last = temp;
        latest  = temp->last_used_time;
      }
    }
    temp = temp ->previous;
  }
  return last;
}

void
insert_in_pages(pages_t* pages, long long int num_of_pages, node_t* temp){
  if(num_of_pages > pages -> REMEAING_SIZE){
    return;
  }
  for(long long int g = 0; g < pages->CAPACTIY; g++){
    if(num_of_pages <= 0){
      return;
    }
    if((pages -> PAGETABEL)[g] == -1){
      (pages -> PAGETABEL)[g] = temp->id;
      temp ->num_pages_in_memory ++;
      pages -> REMEAING_SIZE --;
      num_of_pages--;
    }
  }
}

/*this function only remove n pages of a node*/
void
removing_n_from_page_table(pages_t* page, node_t* temp,long long int n,long long int* gonna_removed){
  for(long long int i = 0; i < page -> CAPACTIY; i++){
    if((page->PAGETABEL)[i] == temp -> id){
      gonna_removed[i] = 1;
      (page->PAGETABEL)[i] = -1;
      page -> REMEAING_SIZE = page->REMEAING_SIZE + 1;
      temp -> num_pages_in_memory --;
      n--;
    }
    if (n <= 0){
      break;
    }
  }
}

long long int
myround_up(double a){
  double b = (double)((long long int)a);
  b = b + 0.5;
  if(a < b){
    return (long long int)a;
  }else{
    return((long long int)a + 1);
  }    
}

void
int_array_initializer(long long int* temp, long long int n){
  for(long long int i = 0; i < n; i++){
    temp[i] = -1;
  }
}

/*this function prints the evicted process*/
void
evicted_printer(long long int CLOCKTIME, long long int* gonna_print, long long int length){
  long long int is_first = 1;
  printf("%lld, EVICTED, mem-addresses=[",CLOCKTIME);
  for(long long int i = 0; i < length;i++){
    if(gonna_print[i] == 1){
      if(is_first){
        printf("%lld",i);
        is_first = 0;
      }else{
      printf(",%lld",i);
      }
    }
  }
  printf("]\n");
}
/*this function will remove all the pages of nodes until the spages needed is reached, used for swapping*/
void
removing_all(linked_list* queue, long long int Clock_time,long long int pages_to_remove, pages_t* page,char* AFLAG){
  node_t* temp = get_LCU(queue,Clock_time,AFLAG);
  long long int gonna_Removed[page->CAPACTIY];
  int_array_initializer(gonna_Removed,page->CAPACTIY);
  long long int remaing_pages = pages_to_remove;  
  while(remaing_pages > 0){
    if(temp == NULL){
    printf("NOT ENOUGH SPACE !!!! \n");
    exit(0);
    }
    remaing_pages = remaing_pages - (temp -> num_pages_in_memory);
    removing_n_from_page_table(page,temp,temp->num_pages_in_memory,gonna_Removed);
    temp = get_LCU(queue,Clock_time,AFLAG);
  }
  evicted_printer(Clock_time, gonna_Removed, page->CAPACTIY);     
}

/*this function mimic virtual memory and my cm*/
long long int
virtual_memory_allocation(linked_list* queue,node_t* temp,pages_t* page,long long int CLOCK_TIME,char* AFLAG){
  long long int page_to_load = temp ->num_pages_in_memory;
  assign_all_avaliable_pages(temp,page);
  page_to_load = temp->num_pages_in_memory - page_to_load;
  if(temp->num_pages_in_memory >= page_size  || temp->num_pages_in_memory == temp->require_size){
  }else{
    if(temp->require_size > page_size){
      page_to_load = page_size - temp -> num_pages_in_memory;
    }else if(temp -> require_size < page_size){
      page_to_load = temp->require_size - temp -> num_pages_in_memory;
    }else{
      printf("logical error");
      exit(0);
    }
    remove_n_pages(queue,page,CLOCK_TIME,page_to_load,AFLAG);
    assign_all_avaliable_pages(temp,page);
  }
  long long int page_fault = temp->require_size - temp ->num_pages_in_memory;
  //add page fault to the remaning time
  temp ->require_time = temp->require_time + page_fault;
  printf("%lld, RUNNING, id=%lld, remaining-time=%lld",(CLOCK_TIME),temp->id,temp->require_time);
  printf(", load-time=%lld",page_to_load*2);
  print_occupyed_pages(temp,page);
  // add payload to the remaning time
  temp ->require_time = temp->require_time + page_to_load*2;
  return page_to_load*2;
}

/*this function will remove n page from the ram*/
void
remove_n_pages(linked_list* queue,pages_t*page,long long int CLOCK_TIME, long long int n,char* AFLAG){
  node_t* temp = get_LCU(queue,CLOCK_TIME,AFLAG);
  long long int gonna_Removed[page->CAPACTIY];
  int_array_initializer(gonna_Removed,page->CAPACTIY);
  if(temp == NULL){
    exit(0);
  }
  long long int is_first = 1;
  while(n > 0){
    if(temp ->num_pages_in_memory > n){
      removing_n_from_page_table(page,temp,n,gonna_Removed);
      n = 0;
    }else{
      n = n - temp->num_pages_in_memory;
      removing_n_from_page_table(page,temp,temp->num_pages_in_memory,gonna_Removed);
      temp = get_LCU(queue,CLOCK_TIME,AFLAG);
      }
    }
    evicted_printer(CLOCK_TIME, gonna_Removed, page->CAPACTIY);   
}  

/*this function will assign all avaliable pages to the node*/
void
assign_all_avaliable_pages(node_t* temp, pages_t* page){
  long long int temp_needed = temp->require_size - temp->num_pages_in_memory;
  long long int i = 0;
  while(i < page->CAPACTIY && temp_needed >0 && page->REMEAING_SIZE >0){
    if(page->PAGETABEL[i]==-1){
      temp_needed--;
      page->PAGETABEL[i] = temp->id;
      temp->num_pages_in_memory++;
      page->REMEAING_SIZE--;
    }
    i++;
  }
}