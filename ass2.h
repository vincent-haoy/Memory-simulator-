#define maximum_througput 0
#define minimum_throghput 1
#define current_interval 2
#define curren_interval_accumulation 3
#define total_tun_around_time 4
#define maxium_overhead 5
#define total_overhead 6
#define page_size 4
#define inifinity 999999999
#define minute 60

typedef struct node node_t;
struct node{
    long long int arrivel_time;
    long long int id;
    long long int require_size;
    long long int require_time;
    long long int num_pages_in_memory;
    long long int total_time;
    long long int last_used_time;
    node_t* next;
    node_t* previous;

};

typedef struct{
  node_t* head;
  node_t* trail;
  long long int length;
}linked_list;

typedef struct{
  long long int* PAGETABEL;
  long long int CAPACTIY;
  long long int REMEAING_SIZE;
}pages_t;


void free_pages(pages_t* pages);
long long int myround_up(double a);
long long int round_up_only(double a);
node_t* generate_a_node(char* buffer);
void free_a_linked_list(linked_list* list);
node_t* pop_a_linkedist(linked_list* queue);
void creat_a_linked_list(linked_list* queue);
void least_remaining_time(linked_list* queue);
void print_occupyed_pages(node_t* temp, pages_t* page);
void insert_at_head(linked_list* queue, node_t* current);
void insert_in_order(linked_list* queue, node_t* current);
void assign_all_avaliable_pages(node_t* temp, pages_t* page);
void insert_at_the_trail(linked_list* queue, node_t* current);
void int_array_initializer(long long int* temp, long long int n);
void initial_a_swapping_page(pages_t* pages, long long int num_of_page);
node_t* get_LCU(linked_list* queue,long long int Clock_time,char* AFLAG);
void insert_in_pages(pages_t* pages, long long int num_of_pages, node_t* temp);
void tick(long long int* CLOCK_TIME, linked_list* arrived, linked_list* on_the_way);
void evicted_printer(long long int CLOCKTIME, long long int* gonna_print,long long int length);
void removing_a_finished_point_and_print(pages_t* page,node_t* finishend,long long int ClockTime);
long long int arrival_mimic(linked_list* arrived, linked_list* on_the_way,long long int clocktime);
void removing_n_from_page_table(pages_t* page, node_t* temp,long long int n,long long int* is_first_time);
void remove_n_pages(linked_list* queue,pages_t*page,long long int CLOCK_TIME, long long int n,char* AFLAG);
void removing_all(linked_list* queue, long long int Clock_time,long long int pages_to_remove, pages_t* page,char* AFLAG);
long long int virtual_memory_allocation(linked_list* queue,node_t* temp,pages_t* page,long long int CLOCK_TIME,char* AFLAG);
void process_a_node(char* AFLAG,char* MFLAG,long long int quantuam, linked_list* arrived,linked_list* on_the_way, long long int* CLOCK_TIME, pages_t* page,double* STASTIC);














