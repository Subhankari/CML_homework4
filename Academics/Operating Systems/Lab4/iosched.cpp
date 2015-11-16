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
#include <algorithm>

using namespace std;

class request{
    private:
	    int rid;
	    int AT;
	    int track;
	    int FT;
	    int RT;

    public:
        request(int r, int a, int t ){
            rid = r;
            AT = a;
            track = t;
        }
        void set_FT(int i){
            FT = i;
        }
        int get_track(){
            return track;
        }
        int get_AT(){
            return AT;
        }
        int get_FT(){
            return FT;
        }
        int get_RT(){
            return RT;
        }
        void set_RT(int i){
            RT = i;
        }
};
vector<request*> r;

//*******************abstract class*********************************
class iosched{
protected:
    vector<int> runqueue;
    vector<int> runqueue2;
	int active;

public:
        void set_active_runqueue(){
		active = 1;
		};
		void set_active_runqueue2(){
		active = 2;
		};
		int get_active(){
			return active;
		}
		bool is_runqueue_empty(){
			if((runqueue.size() == 0 )	&&
               (runqueue2.size() == 0 ))
 			{
 				return true;
		    }else{
		    	return false;
			};
		};

    virtual void add(int) =0;
    virtual int get_next(int) =0;

};
//*****************************************************************************
//**************************FIFO algo*******************************************
class FIFO:public iosched{

public:
    void add(int t){
        runqueue.push_back(t);
    }
    int get_next(int c){
        int t = runqueue.front();
        runqueue.erase(runqueue.begin());
        return t;
    }
};
//*********************************************************************************

//*************************Shortest seek time first algo*******************************
class SSTF:public iosched{
public:
    void add(int t){
     runqueue.push_back(t);
    }
    int get_next(int c){
        int t,smallest,pos;
        request *rq;
        if(runqueue.size() > 1){
            for(int i = 0; i < runqueue.size(); i++)
            {
                rq = r[runqueue[i]];
                t = rq->get_track();
                if(i == 0){
                    if(c >= t){
                        smallest = c - t;
                    }else{
                        smallest = t - c;
                    }
                    pos = i;
                }else{

                    if(c >= t ){
                           if(smallest > (c - t)){
                                smallest = c - t;
                                pos = i;
                           }
                    }else{
                        if(smallest > (t - c)){
                            smallest = t - c;
                            pos = i;
                        }
                    }
                }
            }

            t = runqueue[pos];
            runqueue.erase(runqueue.begin()+pos);
            return t;
        }else{
            t = runqueue.front();
            runqueue.erase(runqueue.begin());
            return t;
        }

    }
};
//*******************************************************************************

//*******************SCAN algo***************************************************
class SCAN:public iosched{
    int order; // 1 for increasing and 2 for decreasing

    public:
        SCAN(){
            order = 1;
        }
    void add(int t){
     runqueue.push_back(t);
    }

    int get_next(int c){
        int t,smallest,pos,track;
        bool assigned = false;
        request *rq;
        if(runqueue.size() > 1){
            if(order == 0){
             for(int i = 0; i < runqueue.size(); i++)
            {
                rq = r[runqueue[i]];
                t = rq->get_track();
                if(i == 0){
                    if(c > t){
                        smallest = c - t;
                    }else{
                        smallest = t - c;
                    }
                    pos = i;
                }else{

                    if(c > t ){
                           if(smallest > (c - t)){
                                smallest = c - t;
                                pos = i;
                           }
                    }else{
                        if(smallest > (t - c)){
                            smallest = t - c;
                            pos = i;
                        }
                    }
                }
            }

            t = runqueue[pos];
            rq = r[t];
            track = rq->get_track();
            if( track > c){
                order = 1;
            }else{
                order = 2;
            }
            runqueue.erase(runqueue.begin()+pos);
            return t;
        }

            if(order == 1){
                for(int i = 0; i < runqueue.size(); i++)
                {
                    rq = r[runqueue[i]];
                    t = rq->get_track();
                    if((t >= c) && (!assigned)){
                        smallest = t - c;
                        pos = i;
                        assigned = true;
                    }else{
                        if((t >= c) && (smallest >= (t - c))){
                            smallest = t - c;
                            pos = i;
                        }
                    }
                }
                if(!assigned){
                    order = 2;
                    t = get_next(c);
                }else{
                    t = runqueue[pos];
                    runqueue.erase(runqueue.begin()+pos);
                }
                return t;
            }
            if(order == 2){
                for(int i = 0; i < runqueue.size(); i++)
                {
                    rq = r[runqueue[i]];
                    t = rq->get_track();
                    if((t <= c) && (!assigned)){
                        smallest = c - t;
                        pos = i;
                        assigned = true;
                    }else{
                        if((t <= c) && (smallest >= (c - t))){
                            smallest = c - t;
                            pos = i;
                        }
                    }
                }
                if(!assigned){
                    order = 1;
                    t = get_next(c);
                }else{
                    t = runqueue[pos];
                    runqueue.erase(runqueue.begin()+pos);
                }
                return t;
            }
        }else{
            t = runqueue.front();
            rq = r[t];
            track = rq->get_track();
            if( track > c){
                order = 1;
            }else{
                order = 2;
            }
            runqueue.erase(runqueue.begin());
            return t;
        }
    }
};
//***********************************************************************

//*********************CSCAN algo****************************************
class CSCAN:public iosched{
    vector<int> arrup;
    vector<int> arrdown;
    int order;
    public:
    CSCAN(){
        order = 1;
    }
    void add(int t){
     runqueue.push_back(t);
    }
    int get_next(int c){
        bool assigned = false;
        request *rq;
        int t,smallest,pos,track;
                    arrup.erase(arrup.begin(),arrup.end());
                    arrdown.erase(arrdown.begin(),arrdown.end());
                 for(int i = 0; i < runqueue.size(); i++){
                    rq = r[runqueue[i]];
                    t = rq->get_track();
                    if(t >= c){
                        arrup.push_back(t);
                    }
                    if(t < c){
                        arrdown.push_back(t);
                    }
                 }
                 sort(arrup.begin(),arrup.end());
                 sort(arrdown.begin(),arrdown.end());
                 if(arrup.empty()){
                            t = arrdown.front();
                            arrdown.erase(arrdown.begin());
                            arrdown.erase(arrdown.begin(),arrdown.end());
                            for(int i = 0; i < runqueue.size(); i++){
                                rq = r[runqueue[i]];
                                if (t == rq->get_track()){
                                    pos = i;
                                    break;
                                }
                           }
                            t = runqueue[pos];
                            runqueue.erase(runqueue.begin()+pos);
                               return t;
                        }

                 t = arrup.front();
                 arrup.erase(arrup.begin());
                  for(int i = 0; i < runqueue.size(); i++){
                    rq = r[runqueue[i]];
                    if (t == rq->get_track()){
                        pos = i;
                        break;
                    }
                 }
                 t = runqueue[pos];
                runqueue.erase(runqueue.begin()+pos);
                 return t;
            }
};
//************************************************************************

//*************************FSCAN algo*************************************
class FSCAN:public iosched{
    int order; // 1 for increasing and 2 for decreasing
    int active;// 1 for runqueue and 2 for runqueue2
    public:
        FSCAN(){
            order = 1;
            active = 1;
        }

    void add(int t){
        if(active == 1){
            runqueue2.push_back(t);
        }else{
            runqueue.push_back(t);
        }
    }

    int get_next(int c){
        int t,smallest,pos,track;
        bool assigned = false;
        request *rq;
        if (active == 1){
            if (runqueue.size() == 0){
                active = 2;
                order = 1;
                t = get_next(c);
                return t;
            }else{
            if(runqueue.size() > 1){
                if(order == 0){
                for(int i = 0; i < runqueue.size(); i++)
                {
                    rq = r[runqueue[i]];
                    t = rq->get_track();
                    if(i == 0){
                        if(c > t){
                            smallest = c - t;
                        }else{
                            smallest = t - c;
                        }
                        pos = i;
                    }else{
                        if(c > t ){
                           if(smallest > (c - t)){
                                smallest = c - t;
                                pos = i;
                           }
                        }else{
                            if(smallest > (t - c)){
                                smallest = t - c;
                                pos = i;
                            }
                        }
                    }
                }

                t = runqueue[pos];
                rq = r[t];
                track = rq->get_track();
                if( track > c){
                    order = 1;
                }else{
                    order = 2;
                }
                runqueue.erase(runqueue.begin()+pos);
                return t;
            }

            if(order == 1){
                for(int i = 0; i < runqueue.size(); i++)
                {
                    rq = r[runqueue[i]];
                    t = rq->get_track();
                    if((t >= c) && (!assigned)){
                        smallest = t - c;
                        pos = i;
                        assigned = true;
                    }else{
                        if((t >= c) && (smallest >= (t - c))){
                            smallest = t - c;
                            pos = i;
                        }
                    }
                }
                if(!assigned){
                    order = 2;
                    t = get_next(c);
                }else{
                    t = runqueue[pos];
                    runqueue.erase(runqueue.begin()+pos);
                }
                return t;
            }
            if(order == 2){
                for(int i = 0; i < runqueue.size(); i++)
                {
                    rq = r[runqueue[i]];
                    t = rq->get_track();
                    if((t <= c) && (!assigned)){
                        smallest = c - t;
                        pos = i;
                        assigned = true;
                    }else{
                        if((t <= c) && (smallest >= (c - t))){
                            smallest = c - t;
                            pos = i;
                        }
                    }
                }
                if(!assigned){
                    order = 1;
                    t = get_next(c);
                }else{
                    t = runqueue[pos];
                    runqueue.erase(runqueue.begin()+pos);
                }
                return t;
            }
        }else{
            t = runqueue.front();
            rq = r[t];
            track = rq->get_track();
            if( track > c){
                order = 1;
            }else{
                order = 2;
            }
            runqueue.erase(runqueue.begin());
            return t;
        }
            }
        }else{
            if (runqueue2.size() == 0){
                active = 1;
                order = 1;
                t = get_next(c);
                return t;
            }else{
            if(runqueue2.size() > 1){
                if(order == 0){
                for(int i = 0; i < runqueue2.size(); i++)
                {
                    rq = r[runqueue2[i]];
                    t = rq->get_track();
                    if(i == 0){
                        if(c > t){
                            smallest = c - t;
                        }else{
                            smallest = t - c;
                        }
                        pos = i;
                    }else{
                        if(c > t ){
                           if(smallest > (c - t)){
                                smallest = c - t;
                                pos = i;
                           }
                        }else{
                            if(smallest > (t - c)){
                                smallest = t - c;
                                pos = i;
                            }
                        }
                    }
                }

                t = runqueue2[pos];
                rq = r[t];
                track = rq->get_track();
                if( track > c){
                    order = 1;
                }else{
                    order = 2;
                }
                runqueue2.erase(runqueue2.begin()+pos);
                return t;
            }

            if(order == 1){
                for(int i = 0; i < runqueue2.size(); i++)
                {
                    rq = r[runqueue2[i]];
                    t = rq->get_track();
                    if((t >= c) && (!assigned)){
                        smallest = t - c;
                        pos = i;
                        assigned = true;
                    }else{
                        if((t >= c) && (smallest >= (t - c))){
                            smallest = t - c;
                            pos = i;
                        }
                    }
                }
                if(!assigned){
                    order = 2;
                    t = get_next(c);
                }else{
                    t = runqueue2[pos];
                    runqueue2.erase(runqueue2.begin()+pos);
                }
                return t;
            }
            if(order == 2){
                for(int i = 0; i < runqueue2.size(); i++)
                {
                    rq = r[runqueue2[i]];
                    t = rq->get_track();
                    if((t <= c) && (!assigned)){
                        smallest = c - t;
                        pos = i;
                        assigned = true;
                    }else{
                        if((t <= c) && (smallest >= (c - t))){
                            smallest = c - t;
                            pos = i;
                        }
                    }
                }
                if(!assigned){
                    order = 1;
                    t = get_next(c);
                }else{
                    t = runqueue2[pos];
                    runqueue2.erase(runqueue2.begin()+pos);
                }
                return t;
            }
        }else{
            t = runqueue2.front();
            rq = r[t];
            track = rq->get_track();
            if( track > c){
                order = 1;
            }else{
                order = 2;
            }
            runqueue2.erase(runqueue2.begin());
            return t;
        }
            }

        }
    }
};
//**********************************************************************************

struct event{
    int timestamp;
    int inp_num;
    string operation;
    int track;
};

class simulator{


    struct eventq{
        event data;
        eventq *next;
    };

    eventq *head;

public:

    void initq(event et1)
    {
      head->data = et1;
      head->next = NULL;
    };
    void add_to_eventq(event et)
     {
    	eventq **hd = &head;
       	eventq *newNode = new eventq;
	    newNode->data = et;
	    newNode->next = NULL;
	    eventq *cur = *hd;
	    eventq *prev = NULL;

		if (*hd == NULL){
			*hd = newNode;
		}
		else{

       if (cur->data.timestamp > newNode->data.timestamp)
       {
           newNode->next = *hd;
           *hd = newNode;
       }
       else{
            while(cur->data.timestamp <= newNode->data.timestamp)
                {
                    if(cur->next == NULL)
                    {
                      cur->next = newNode;
                      return;
                    }
                    else{
                       prev = cur;
                       cur = cur->next;
                    };
            };
            if(prev != NULL){
                prev->next = newNode;
                newNode->next = cur;
            };
	        };
   		 };
       };
       event eventq_front(){
           eventq *tmpnode = head;
           return tmpnode->data;
       };

       void eventq_pop(){
            eventq **hd1;
            eventq *tmpnode;
            hd1 = &head;
            tmpnode = *hd1;
            *hd1 = tmpnode->next;
            delete tmpnode;
       };
       bool check_eventq_is_empty(){
        if(head == NULL){
            return true;
        }
        else{
            return false;
        }
       };

    simulator(string in){
        string input = in;
        string line;
        event e;
        int time,track,counter = 0;
        ifstream infile;
        infile.open(input.c_str());
        if(infile.is_open()){
            while(!infile.eof())
            {
                getline(infile,line);
                if (line[0] == '#'){
                    continue;
                }
                if(line.length() == 0){
                    continue;
                }
                istringstream iss(line);
                iss>>time>>track;

                request *rq = new request(counter,time,track);
                r.push_back(rq);

                e.timestamp = time;
                e.track = track;
                e.inp_num = counter;
                e.operation = "add";
                if(counter == 0){
                    head = new eventq;
                    initq(e);
                }
                else{
                    add_to_eventq(e);
                }

                counter = counter + 1;
            }
            infile.close();
        }
    }

event get_event(){
    event ev1;
    if(!(head == NULL))
    {
        ev1 = eventq_front();
        eventq_pop();
        return ev1;
    }
};

void put_event(int rid, int timestamp, string op){
        event e;
        request *rq = r[rid];
        e.inp_num = rid;
        e.timestamp = timestamp;
        e.operation = op;
        e.track = rq->get_track();
		add_to_eventq(e);
    };


};

class runtime{
    simulator *d2;
    iosched *s2;
    int current_time;
    int ctrack;
    bool ctrack_assigned;
    event e;

    int total_time;
    int total_tracks;
    int total_request;
    int total_turnaround;
    int total_waitime;
    int max_waitime;
    double avg_turn;
    double avg_wait;

public:
    runtime(simulator **d1, iosched **s1){
	    d2   = *d1;
	    s2   = *s1;
	    current_time = 0;
	    ctrack = 0;
	    total_tracks = 0;
	    total_time = 0;
	    total_request = 0;
	    total_turnaround = 0;
	    total_waitime = 0;
	    max_waitime = 0;
	    ctrack_assigned = false;
    };

    void set_current_time(int t){
    	current_time = t;
	};
	int get_current_time(){
		return current_time;
	};
	void print_sum(){
	avg_turn = (double)total_turnaround / total_request;
	avg_wait = (double)total_waitime / total_request;
	printf("SUM: %d %d %.2lf %.2lf %d\n",
        total_time,
        total_tracks,
        avg_turn,
        avg_wait,
        max_waitime);

	}

	 void execute_event(event e1){
	     int t;
         event e;
	     int time = get_current_time();

	     //cout<< e1.timestamp<<": ";
	     if(e1.operation == "add"){
           //     printf(" %4d add %d\n",e1.inp_num,e1.track);
              //  cout<< e1.timestamp<<": "<<e1.inp_num <<" "<<e1.operation<<" "<<e1.track
                s2->add(e1.inp_num);
                total_request = total_request + 1;

	     }else{
	         if(e1.operation == "issue"){
             //       printf(" %4d issue %d %d\n",e1.inp_num,e1.track, ctrack);
                    request *rq =  r[e1.inp_num];
                    total_waitime = total_waitime + (time - (rq->get_AT()));
                    if (max_waitime < (time - (rq->get_AT())))
                    {
                        max_waitime = time - (rq->get_AT());
                    }
                    //e.inp_num = e1.inp_num;
                    //e.operation = "finish";
                    if(ctrack > e1.track){
                    t = time + (ctrack - e1.track);
                    total_tracks = total_tracks + (ctrack - e1.track);
                    }else{
                        t = time + (e1.track - ctrack);
                        total_tracks = total_tracks + (e1.track - ctrack);
                    }
                    //e.track = e1.track;
                    d2->put_event(e1.inp_num,t,"finish");
                   // ptrack = ctrack;
                    ctrack = e1.track;
                    ctrack_assigned = true;


	         }else{
                if(e1.operation == "finish"){
                    //ctrack = 0;
                    ctrack_assigned = false;
                    request *rq =  r[e1.inp_num];
               //     printf(" %4d finish %d\n",e1.inp_num,(time - (rq->get_AT())));
                    total_turnaround = total_turnaround + (time - (rq->get_AT()));
                    rq->set_FT(time);
	         }
	     }
	     }
	};

	void start_simulation(){
        int rid;
        int t1;
        event e;
        t1 = get_current_time();
        e = d2->eventq_front();
        t1 = e.timestamp;
        set_current_time(t1);
    while(!d2->check_eventq_is_empty()){

      while(!d2->check_eventq_is_empty()){

                e = d2->eventq_front();
                if(t1 != e.timestamp)
                {
                    break;
				};
                e = d2->get_event();
                t1 = e.timestamp;
                set_current_time(t1);
                execute_event(e);
      		};

        	if(ctrack_assigned == false)
       		 {
       		 if(!s2->is_runqueue_empty()){
            	rid = s2->get_next(ctrack);
            	d2->put_event(rid, t1,"issue");
            }else{
            	t1 = e.timestamp;
      		  	set_current_time(t1);
			};

       		 }else{
       		 	e = d2->eventq_front();
       		 	t1 = e.timestamp;
      		  	set_current_time(t1);
			};
			};

			total_time = t1;
			print_sum();
  		 };

};


int main(int argc, char **argv){

int c,index;
string alg, infile;

    while ((c = getopt (argc, argv, "s:")) != -1)
    switch (c)
      {
      case 's':
        alg = optarg;
        break;
      default:
        abort();
      }
    for (index = optind; index < argc; index++){
        infile = argv[index];
        }

char algo = alg[0];
iosched * sh;


if(algo == 'i'){
    FIFO *f = new FIFO;
    sh = f;
}else{
    if(algo == 'j'){
            SSTF * j = new SSTF;
            sh = j;
    }else{
        if(algo == 's'){
            SCAN *s = new SCAN;
            sh = s;
        }else{
           if(algo == 'c'){
                CSCAN *c = new CSCAN;
                sh = c;
            }else{
                if(algo == 'f'){
                    FSCAN *f = new FSCAN;
                    sh = f;
                }
            }
        }
    }

}

    simulator *d = new simulator(infile);
    runtime *r = new runtime(&d,&sh);
    r->start_simulation();
    return 0;
}
