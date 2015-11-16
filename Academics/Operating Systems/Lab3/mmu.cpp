#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <cstddef>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

using namespace std;

FILE *f;
//******************************read and store random file**************************
class read_rfile{
        vector<int> randvals;
        int cur_index;
    public:
        read_rfile(string file){
            string line;
            int var;
            ifstream rfile;
            rfile.open(file.c_str());
            if(rfile.is_open()){

                getline(rfile,line);
                while(!rfile.eof())
                {
                    getline(rfile,line);
                    istringstream iss(line);
                    iss>>var;
                    randvals.push_back(var);
                };
            };
            rfile.close();
            cur_index = 0;
        };
        int get_next_randval(){
            int val;
            if (cur_index == (randvals.size() - 1)){
             cur_index = 0;
            };
            val = randvals[cur_index];
            cur_index = cur_index + 1;
            return val;
        };
};
//************************************************************************************

//***********************mmu parent class********************************************
class mmu{

 protected:
    struct pte{
        unsigned present:1;
        unsigned modified:1;
        unsigned referenced:1;
        unsigned pagedout:1;
        unsigned invalid_bits:22;
        unsigned frame:6;
    };
     struct frame{
        int i;
        pte *p;
    };

    pte pagetable[64];
    frame *frame_table;
    read_rfile *rf;
    int counter;
    int fsize;
    int inst_count;
    int stats_maps;
    int stats_unmaps;
    int stats_ins;
    int stats_outs;
    int stats_zeros;
    long long int totalcost;
    bool instr;


    public:

    void init(int n,read_rfile** rf2){
       frame_table = new frame[n];
       for(int i = 0; i < 64; i++){
            pagetable[i].present = 0;
            pagetable[i].referenced = 0;
            pagetable[i].modified = 0;
            pagetable[i].pagedout = 0;
            pagetable[i].frame = 0;
       }
       for(int i = 0; i < n; i++){
            frame_table[i].i = 0;
            frame_table[i].p = NULL;
       }
        counter = 0;
        fsize = n;
        rf = *rf2;
        inst_count = 0;
        stats_maps = 0;
        stats_unmaps = 0;
        stats_ins = 0;
        stats_outs = 0;
        stats_zeros = 0;
        totalcost = 0;
        instr = false;
    };

    void set_instr(){
        instr = true;
    }
    void add_inst_count(){
        inst_count = inst_count + 1;
    }
    void add_stats_map(){
        stats_maps = stats_maps + 1;
    }
    void add_stats_unmap(){
        stats_unmaps = stats_unmaps + 1;
    }
    void add_stats_ins(){
        stats_ins = stats_ins + 1;
    }
    void add_stats_outs(){
        stats_outs = stats_outs + 1;
    }
    void add_stats_zero(){
        stats_zeros = stats_zeros + 1;
    }

    void calculate_totalcost(){
    totalcost = (inst_count * 1) + (stats_maps * 400) +
                (stats_unmaps * 400) + (stats_ins * 3000) +
                (stats_outs * 3000) + (stats_zeros * 150);
    }

    void print_sum(){
        printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu\n",
                inst_count, stats_unmaps, stats_maps, stats_ins, stats_outs, stats_zeros, totalcost);
    }

    void Map (int a, int b, long inst_count){
        add_stats_map();
        frame_table[b].p = &pagetable[a];
        frame_table[b].i = a;
        pagetable[a].present = 1;
        pagetable[a].modified = 0;
        pagetable[a].referenced = 0;
        pagetable[a].frame = b;
        if(instr == true){
            printf("%ld: MAP   %3d %3d\n", inst_count, a, b);
            fprintf(f,"%ld: MAP   %3d %3d\n", inst_count, a, b);
        }
    }

    int Unmap (int a, long inst_count){
        add_stats_unmap();
        pagetable[a].present = 0;
        if(instr == true){
            printf("%ld: UNMAP %3d %3d\n", inst_count, a, pagetable[a].frame);
            fprintf(f,"%ld: UNMAP %3d %3d\n", inst_count, a, pagetable[a].frame);
        }
        return pagetable[a].frame;
    }

    void Zero (int b, long inst_count){
        add_stats_zero();
        if(instr == true){
            printf("%ld: ZERO   %6d\n", inst_count, b);
            fprintf(f,"%ld: ZERO   %6d\n", inst_count, b);
        }
    }

    void read (int a){
        add_inst_count();
        pagetable[a].referenced = 1;
    }

    void write (int a){
        add_inst_count();
        pagetable[a].referenced = 1;
        pagetable[a].modified = 1;
    }

    bool ifpresent (int p){
        if(pagetable[p].present == 1)
            return true;
        else return false;
    }

    int get_counter(){
        return counter;
    };
    int set_counter(){
        counter = counter + 1;
    };
    int get_index(int n){
        int randv;
        randv = rf->get_next_randval();
        return (randv % n);
    }
    bool isout(int n){
        if(pagetable[n].pagedout == 1){
            return true;
        }else return false;
    }
    bool ismodified(int n){
        if(pagetable[n].modified == 1){
            return true;
        }else return false;
    }

    void out(int a, int b, int c, long inst_count){
        add_stats_outs();
        if(instr == true){
            printf("%ld: OUT   %3d %3d\n", inst_count, a, b);
            fprintf(f,"%ld: OUT   %3d %3d\n", inst_count, a, b);
        }
        if(isout(c)){
            in(c,b,inst_count);
        }else{
            Zero(b,inst_count);
        }
        pagetable[a].pagedout = 1;
        pagetable[a].referenced = 0;
        pagetable[a].modified = 0;
    }


    void in (int a, int b, long inst_count){
        add_stats_ins();
        if(instr == true){
            printf("%ld: IN    %3d %3d\n", inst_count, a, b);
            fprintf(f,"%ld: IN    %3d %3d\n", inst_count, a, b);
        }
    }

    void print_pagetable(){
        for(int i = 0; i < 64; i++){
                if(pagetable[i].present == 0){
                    if(pagetable[i].pagedout == 1){
                        printf("# ");
                        fprintf(f,"# ");
                    }else{
                        printf("* ");
                        fprintf(f,"* ");
                    }
                }else{
                    printf("%d:",i);
                    fprintf(f,"%d:",i);
                    if(pagetable[i].referenced == 1){
                        printf("R");
                        fprintf(f,"R");
                    }else{
                        printf("-");
                        fprintf(f,"-");
                    }
                    if(pagetable[i].modified == 1){
                        printf("M");
                        fprintf(f,"M");
                    }else{
                        printf("-");
                        fprintf(f,"-");
                    }
                    if(pagetable[i].pagedout == 1){
                        printf("S ");
                        fprintf(f,"S ");
                    }else{
                        printf("- ");
                        fprintf(f,"- ");
                    }
                }
        }
        printf("\n");
        fprintf(f,"\n");
    }
    void print_frame_table(){
        for(int i = 0; i < fsize; i++){
            if( !(frame_table[i].p == NULL)){
                printf("%d ",frame_table[i].i);
                fprintf(f,"%d ",frame_table[i].i);
            }else{
                printf("* ");
                fprintf(f,"* ");
            }
        }
        printf("\n");
        fprintf(f,"\n");
    }

    void reset(){
        for(int i = 0; i< 64; i++){
            if(pagetable[i].present == 1){
                pagetable[i].referenced = 0;
            }
        }
    }


   virtual int get_victim(void) =0;
   virtual void do_shift(int) =0;
};

//************************************************************************

//********************NRU algo*******************************************

class NRU:public mmu{
    vector<int> arcl0;
    vector<int> arcl1;
    vector<int> arcl2;
    vector<int> arcl3;

public:
    NRU(read_rfile **rf1, int n){
        init(n,rf1);
    };
    void do_shift(int i){
    //do nothing
    }

    int get_victim(){
        int index;

        arcl0.clear();
        arcl1.clear();
        arcl2.clear();
        arcl3.clear();

        for(int i = 0; i < 64; i++){
            if(pagetable[i].present == 1){
                if(pagetable[i].referenced == 0){
                    if(pagetable[i].modified == 0){
                        arcl0.push_back(i);
                    }else{
                        if(pagetable[i].modified == 1){
                            arcl1.push_back(i);
                        }
                    }
                }
                else{
                    if(pagetable[i].referenced == 1){
                        if(pagetable[i].modified == 0){
                            arcl2.push_back(i);
                        }else{
                            if(pagetable[i].modified == 1){
                            arcl3.push_back(i);
                            }
                        }
                    }
                }
            }
        }

        if(arcl0.size() != 0){
            index = get_index(arcl0.size());
            return arcl0[index];
        }else{
            if(arcl1.size() != 0){
                index = get_index(arcl1.size());
                return arcl1[index];
            }else{
                if(arcl2.size()!= 0){
                    index = get_index(arcl2.size());
                    return arcl2[index];
                }else{
                    if(arcl3.size() != 0){
                        index = get_index(arcl3.size());
                        return arcl3[index];
                    }
                }
            }
        }
    };

};
//*********************************************************************************

//****************FIFO algo*******************************************************
class FIFO:public mmu{
    vector<frame*>arr;

public:
    FIFO(read_rfile **rf1, int n){
        init(n,rf1);
        for(int i = 0; i < fsize; i++){
          arr.push_back(&frame_table[i]);
        };
    };

    void do_shift(int i){
    //do nothing
    }
    int get_victim(){
         frame* fr0;

        fr0 = arr.front();
        arr.erase(arr.begin());
        arr.push_back(fr0);

        return fr0->i;
    }
};
//**************************************************************************

//***********SECOND CHANCE algo*********************************************
class SECOND_CHANCE:public mmu{
    vector<frame*>arr;

public:
    SECOND_CHANCE(read_rfile **rf1, int n){
       init(n,rf1);
        for(int i = 0; i < fsize; i++){
          arr.push_back(&frame_table[i]);
        }
    }

    void do_shift(int i){
    //do nothing
    }

    int get_victim(){
        frame* fr0;
        for(int i = 0; i < fsize; i++){
            fr0 = arr.front();
            arr.erase(arr.begin());
            if(fr0->p->referenced == 0){
                arr.push_back(fr0);
                return fr0->i;
            }else{
                fr0->p->referenced = 0;
                arr.push_back(fr0);
            }
        }

        fr0 = arr.front();
        arr.erase(arr.begin());
        arr.push_back(fr0);

        return fr0->i;
    }
};
//*****************************************************************************************

//******************CLOCK algo**********************************************************
class CLOCK:public mmu{
    struct arr{
        int val;
        arr *next;
    };
    arr *head;
    arr *curr;
    char md;

public:

    void do_shift(int i){
    //do nothing
    }
   void initListp(){
       head = NULL;
    };
    void add_to_listp(int i){
        arr **hd = &head;
        arr **cur = &curr;
        arr *newnode = new arr;
        newnode->val = i;
        newnode->next = NULL;
        arr *cur1 = *hd;
        arr *cur2 = *cur;

         if(*hd == NULL){
            *hd = newnode;
            *cur = newnode;
        }
        cur2 = *cur;

        if(cur2->val == newnode->val){
            *cur = newnode;
        }else{
            cur2->next = newnode;
            *cur = newnode;
        }
    };

    void add_lastp(){
        arr **cur = &curr;
        arr *cur1 = *cur;
        arr **hd = &head;
        cur1->next = *hd;
        *cur = cur1->next;
    };

    CLOCK(read_rfile **rf1, int n, char mode){
        init(n,rf1);
        md = mode;
        if(md == 'c'){
            initListp();
            for(int i = 0; i < fsize; i++){
                    add_to_listp(i);
            };
            add_lastp();
        }else{
            if(md == 'X'){
                initListp();
                for(int i = 0; i < 64; i++){
                    add_to_listp(i);
                }
                add_lastp();
            }
        }
    };

    int get_victim(){

        arr **cur = &curr;
        arr* cur1 = *cur;
        int victim;
        if (md == 'c'){
             for(int i = 0; i < fsize; i++){
                cur1 = *cur;
                if(frame_table[cur1->val].p->present == 1){
                    if(frame_table[cur1->val].p->referenced == 0){
                        victim = frame_table[cur1->val].i;
                        *cur = cur1->next;
                        return victim;
                    }
                }

                frame_table[cur1->val].p->referenced = 0;
                *cur = cur1->next;
            }
            cur1 = *cur;
            victim = frame_table[cur1->val].i;
            *cur = cur1->next;
            return victim;


        }else{
            if(md == 'X'){
                for(int i = 0; i < 64; i++){
                    cur1 = *cur;
                    if (pagetable[cur1->val].present == 1){
                            if(pagetable[cur1->val].referenced == 0){
                                victim = frame_table[pagetable[cur1->val].frame].i;
                                *cur = cur1->next;
                                return victim;
                                }
                    }

                    pagetable[cur1->val].referenced = 0;
                    *cur = cur1->next;
                }
                for(int i = 0; i < 64; i++){
                    cur1 = *cur;
                    if (pagetable[cur1->val].present == 1){
                        victim = frame_table[pagetable[cur1->val].frame].i;
                        *cur = cur1->next;
                        return victim;
                    }
                    *cur = cur1->next;
                }
            }
        }
    }
};
//*********************************************************************************

//*******************LRU algo******************************************************
class LRU:public mmu{
vector<int> arr;
public:
    LRU(read_rfile **rf1, int n){
        init(n,rf1);
    }
    void do_shift(int i){
        arr.push_back(i);
        //pagetable[i].referenced = 1;
        for(int j = 0; j < (arr.size() - 1); j++){
            if(arr[j] == i){
                arr.erase(arr.begin()+j);
            }
        }
    }

    int get_victim(){
        int victim = arr.front();
        arr.erase(arr.begin());
        return victim;
    }

};
//********************************************************************************

//***********************RANDOM algo**********************************************
class RANDOM:public mmu{
public:
    RANDOM(read_rfile **rf1, int n){
        init(n,rf1);
    };
    void do_shift(int i){
    //do nothing
    }
    int get_victim(){
        return frame_table[get_index(fsize)].i;
    }
};
//******************************************************************************

//***********************AGING algo*********************************************
class AGING:public mmu{
    unsigned int *arr;
    char md;

public:
    AGING(read_rfile **rf1, int n, char c){
        init(n,rf1);
        md = c;
        if(md == 'a'){
             arr = new unsigned int[fsize];
            for(int i = 0; i < fsize; i++){
                    arr[i] = 0;
            };
        }else{
            if(md == 'Y'){
                arr = new unsigned int[64];
                for(int i = 0; i < 64; i++){
                    arr[i] = 0;
                }
            }
        }
    };
    void do_shift(int i){
    //do nothing
    };

    int get_victim(){
        unsigned int smallest;
        int pos;
        if(md == 'a'){
            for(int j = 0; j < fsize; j++){
                    arr[j] = arr[j] >> 1;
                if(frame_table[j].p->referenced == 1){
                      arr[j] = arr[j] |= 1 << 31;
                      frame_table[j].p->referenced = 0;
                }
                if (j == 0){
                    smallest = arr[j];
                    pos = j;
                }else{
                    if(smallest > arr[j]){
                        smallest = arr[j];
                        pos = j;
                    }
                }
            }
            arr[pos] = 0;
            return frame_table[pos].i;


        }else{
            if(md == 'Y'){
                smallest = NULL;
                for(int j = 0; j < 64; j++){
                 arr[j] = arr[j] >> 1;
                 if(pagetable[j].present == 1){
                        if(pagetable[j].referenced == 1){
                            arr[j] = arr[j] |= 1 << 31;
                            pagetable[j].referenced = 0;
                        }
                        if (smallest == NULL){
                            smallest = arr[j];
                            pos = j;
                        }else{
                            if(smallest > arr[j]){
                                smallest = arr[j];
                                pos = j;
                            }
                        }
                    }
                }
                arr[pos] = 0;
                return pos;
            }
        }
          return 0;
    }

};
//*******************************************************************************

//************************main method****************************************
int main(int argc, char **argv)
{
     //cout<<"here";
    int inst, vp, victim,ppg, i, fault_count, n;
    string Ovalue ;
    string Avalue ;
    string Fvalue ;
    string in;
    string r_file;
    char algo;
    bool aval = false, oval = false, fval = false;
    bool O = false,P = false,F = false,S = false;
    int c, index;
    long instr_count = 0;

   while ((c = getopt (argc, argv, "a:o:f:")) != -1)
    switch (c)
      {
      case 'a':
        Avalue = optarg;
        aval = true;
        break;
      case 'o':
        Ovalue = optarg;
        oval = true;
        break;
      case 'f':
        Fvalue = optarg;
        fval = true;
        break;
      default:
        abort();
      }

      for (index = optind; index < argc; index++){
        in = argv[index];
        index++;
        r_file = argv[index];
        }

      if(aval == false){
        algo = 'l';
      }else{
        algo = Avalue[0];
      }

      if(oval == true){
        for(int i = 0; i < Ovalue.length(); i++){
            if(Ovalue[i] =='O'){
                O = true;
            };
            if(Ovalue[i] == 'P'){
                P = true;
            };
            if(Ovalue[i] == 'F'){
                F = true;
            };
            if(Ovalue[i] == 'S'){
                S = true;
            }
        }
    }

    if(fval == false){
        n = 32;
    }else{
        int length;
        istringstream iss(Fvalue);
        iss>>n;
    }

    string input = in;
    string rand_file = r_file;

    mmu *m;
    f = fopen("file.txt", "w");
    if (f == NULL)
    {
    printf("Error opening file!\n");
    exit(1);
    }
    read_rfile *rfile = new read_rfile(rand_file);

    ifstream infile;
    string line;

    if (algo == 'N'){
        NRU *nrualgo = new NRU(&rfile,n);
        m = nrualgo;
    }else{
        if(algo == 'f'){
            FIFO *fifoalgo = new FIFO(&rfile,n);
            m = fifoalgo;
        }else{
            if(algo == 's'){
                SECOND_CHANCE *secondalgo = new SECOND_CHANCE(&rfile,n);
                m = secondalgo;
            }else{
                if(algo == 'c'){
                    CLOCK *clockalgo1 = new CLOCK(&rfile,n,'c');
                    m = clockalgo1;
                }else{
                    if(algo == 'X'){
                        CLOCK *clockalgo2 = new CLOCK(&rfile,n, 'X');
                        m = clockalgo2;
                    }else{
                        if(algo == 'l'){
                            LRU *lrualgo = new LRU(&rfile,n);
                            m = lrualgo;
                            }else{
                                if (algo == 'r'){
                                    RANDOM *ralgo = new RANDOM(&rfile,n);
                                    m = ralgo;
                                    }else{
                                        if(algo == 'a'){
                                            AGING *aalgo = new AGING(&rfile, n, 'a');
                                            m = aalgo;
                                        }else{
                                            if(algo == 'Y'){
                                                AGING *aalgo1 = new AGING(&rfile, n , 'Y');
                                                m = aalgo1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

    fault_count = 0;

    infile.open(input.c_str());

    if(infile.is_open()){
        while(!infile.eof()){
            getline(infile,line);
            if (line[0] == '#'){
                continue;
            }
            if(line.length() == 0){
                continue;
            }
            istringstream iss(line);
            iss>>inst>>vp;
            if(O == true){
                m->set_instr();
                printf("==> inst: %1d %1d\n", inst, vp);
                fprintf(f,"==> inst: %1d %1d\n", inst, vp);
            };
            i = m->get_counter();

            if (m->ifpresent(vp)){
                   if(inst == 1){
                        m->write(vp);
                        m->do_shift(vp);
                    }else{
                        m->read(vp);
                        m->do_shift(vp);
                    }
            }else{
                if (i < n){
                    m->Zero(i, instr_count);
                    m->Map(vp, i, instr_count);
                    m->set_counter();
                }else {
                    victim = m->get_victim();
                    if (algo == 'N'){
                    fault_count = fault_count + 1;
                    if (fault_count == 10){
                            fault_count = 0;
                            m->reset();
                        }
                    }
                    ppg = m->Unmap(victim,instr_count);
                    if(m->ismodified(victim)){
                        m->out(victim,ppg,vp,instr_count);
                    }else{
                        if(m->isout(vp)){
                            m->in(vp,ppg,instr_count);
                        }else{
                            m->Zero(ppg,instr_count);
                        }
                    }
                    m->Map(vp,ppg,instr_count);
                }
                if(inst == 1){
                    m->write(vp);
                    m->do_shift(vp);
                }else{
                    m->read(vp);
                    m->do_shift(vp);
                }
            }
            instr_count = instr_count + 1;
        }
    if(P == true){
        m->print_pagetable();
    }
    if(F == true){
        m->print_frame_table();
    }
    if(S == true){
        m->calculate_totalcost();
        m->print_sum();
    }

    fclose(f);
    }
    return 0;
}
//***********************************************************************************
