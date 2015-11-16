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
using namespace std;

bool verbose;

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

class process{
	private:
	    int pid;
	    int AT;
	    int TC;
	    int CB;
	    int IO;
	    int sprio;
	    int dprio;
	    int ccb;
	    int rccb;
	    int rem;
	    int FT;
	    int TT;
	    int IT;
	    int CW;
	    int last_ready_t;
	    int last_blocked_t;

	public:

	    process(int pid1,string line, int rand){
	        pid = pid1;
	        istringstream iss(line);
	        iss >> AT >> TC >> CB >> IO;
	        sprio = (rand % 4) + 1;
	        dprio = sprio - 1;
	        ccb = 0;
	        rccb = 0;
	        rem = TC;
	        FT = 0;
	        TT = 0;
	        IT = 0;
	        CW = 0;
	        last_ready_t = 0;
	        last_blocked_t = 0;
	    };

	    int get_AT(){
	        return AT;
	    };
	    int get_pid(){
	        return pid;
	    };
	    int get_CB(){
	        return CB;
	    };
	    int get_sprio(){
	        return sprio;
	    };
	    int get_dprio(){
	        return dprio;
	    };
	    int get_ccb(){
	        return ccb;
	    };
	    int get_rem(){
	        return rem;
	    };
	    int get_TC(){
	    	return TC;
		};
	    void set_ccb(int value){
	        ccb = value;
	    };
	    void set_rem(int value){
	        rem = value;
	    };
	    int set_dprio(int prio){
	        dprio = prio;
	    };
	    int set_sprio(int prio){
	        sprio = prio;
	    };
	    int set_rccb(int value){
	        rccb = value;
	    };
	    int get_rccb(){
	        return rccb;
	    };
	    int get_IO(){
	        return IO;
	    };
	    int set_FT_TT(int value){
	        FT = value;
	        TT = FT - AT;
	    };
	    void set_IT (int value){
	        IT = IT + value;
	    };
	    int get_IT(){
	    	return IT;
		}
	    int get_FT(){
	    	return FT;
		};
		int get_TT(){
			return TT;
		};
		void set_CW(int value){
			CW = CW + value;
		};
		int get_CW(){
			return CW;
		};
		void set_last_ready_t(int value){
			last_ready_t = value;
		};
		int get_last_ready_t(){
			return last_ready_t;
		};
		void set_last_blocked_t(int value){
			last_blocked_t = value;
		};
		int get_last_blocked_t(){
			return last_blocked_t;
		};
};

vector<process*> p;
//***********************************Scheduler abstract class************************************
class scheduler{

    protected:
    vector<int> runqueue[4];
    vector<int> runqueue2[4];
	int active;
	string algo_name;
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
			if((runqueue[3].size() == 0 ) &&
			(runqueue[2].size() == 0 ) 	&&
			(runqueue[1].size() == 0 ) 	&&
 			(runqueue[0].size() == 0 )	&&
 			(runqueue2[3].size() == 0 )	&&
 			(runqueue2[2].size() == 0 )	&&
 			(runqueue2[1].size() == 0 )	&&
 			(runqueue2[0].size() == 0 ))
 			{
 				return true;
		    }else{
		    	return false;
			};
		};
		virtual string get_name(void) =0;
		virtual void add_process(int , int) =0;
        virtual int get_next_process(void) =0;
};
//***********************************************************************************************************
//***************************************FCFS implementation************************************************
class FCFS: public scheduler{
private:
    string algo_name;
public:
	FCFS(){
	    algo_name = "FCFS";
		set_active_runqueue();
		};
    string get_name(){
    return algo_name;
    };
	void add_to_expired_queue(int dp1, int pid1){
		int ac;
		ac  = get_active();

		if (ac == 1)
		{
				switch(dp1){
            		case 0:
                    runqueue2[0].push_back(pid1);
                    break;
                case 1:
                    runqueue2[1].push_back(pid1);
                	break;
                case 2:
                  runqueue2[2].push_back(pid1);
                  break;
                case 3:
                    runqueue2[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};


		}else{
				switch(dp1){
        		case 0:
	                runqueue[0].push_back(pid1);
	                break;
                case 1:
                    runqueue[1].push_back(pid1);
                	break;
                case 2:
                  runqueue[2].push_back(pid1);
                  break;
                case 3:
                    runqueue[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};
		};
	};


	void add_to_active_queue(int dp1, int pid1){
		int ac;
		ac  = get_active();

		if (ac == 1)
		{
				switch(dp1){
            	case 0:
                    runqueue[0].push_back(pid1);
                    break;
                case 1:
                    runqueue[1].push_back(pid1);
                	break;
                case 2:

                 	runqueue[2].push_back(pid1);
                  	break;
                case 3:
                    runqueue[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};


		}else{
				switch(dp1){
        		case 0:
	                runqueue2[0].push_back(pid1);
	                break;
                case 1:
                    runqueue2[1].push_back(pid1);
                	break;
                case 2:
                  runqueue2[2].push_back(pid1);
                  break;
                case 3:
                    runqueue2[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};
		};

	};

	bool check_swap(){
		int ac1 = get_active();

		if(ac1 == 1){
			if ((runqueue[3].size() == 0 ) &&
				(runqueue[2].size() == 0) &&
				(runqueue[1].size() == 0) &&
			    (runqueue[0].size() == 0 )){
			    return true;
				}else{
			return false;
		}
	}else{
			if ((runqueue2[3].size() == 0 ) &&
				(runqueue2[2].size() == 0) &&
				(runqueue2[1].size() == 0) &&
			    (runqueue2[0].size() == 0 )){
			    return true;
				}else{
			return false;
		};
	};
};

	void swap_queue(){
		int ac1 = get_active();
		if (ac1 == 1){
			set_active_runqueue2();
		}else{
			set_active_runqueue();
		}

	};


    void add_process(int pid,int state){

         process *p1 = p[pid];
         int dp;
         int sp;

            if(state == 0){
            	sp = p1->get_sprio();
            	dp = sp - 1;
            	p1->set_dprio(dp);
            }else{
            dp = p1->get_dprio();
            dp = dp - 1;
            p1->set_dprio(dp);
            };

            dp = 3;
            if (dp == -1){
            	sp = p1->get_sprio();
            	dp = sp - 1;
            	p1->set_dprio(dp);
				add_to_expired_queue(dp,pid);
				if(check_swap()){
           		 	swap_queue();
				};
			}
			else{
           add_to_active_queue(dp,pid);
        };
    };

  int get_next_process(void){
  	int q;
  	int ac2 = get_active();

  	if (ac2 == 1){
  		if (runqueue[3].size() != 0)
		{
			q = runqueue[3].front();
			runqueue[3].erase(runqueue[3].begin());
		}else{
			if (runqueue[2].size() != 0){
				q = runqueue[2].front();
				runqueue[2].erase(runqueue[2].begin());
			}else{
				if(runqueue[1].size() != 0){
					q = runqueue[1].front();
					runqueue[1].erase(runqueue[1].begin());
				}else{
					if(runqueue[0].size() != 0){
						q = runqueue[0].front();
						runqueue[0].erase(runqueue[0].begin());
					};
				};
			};

		};

	  }else{
	  	if (runqueue2[3].size() != 0)
		{
			q = runqueue2[3].front();
			runqueue2[3].erase(runqueue2[3].begin());
		}else{
			if (runqueue2[2].size() != 0){
				q = runqueue2[2].front();
				runqueue2[2].erase(runqueue2[2].begin());
			}else{
				if(runqueue2[1].size() != 0){
					q = runqueue2[1].front();
					runqueue2[1].erase(runqueue2[1].begin());
				}else{
					if(runqueue2[0].size() != 0){
						q = runqueue2[0].front();
						runqueue2[0].erase(runqueue2[0].begin());
					};
				};
			};

		};

	  };
		return q;
    };
};
//******************************************************************************************************************

//********************************************LCFS implementation**************************************************
class LCFS: public scheduler{
private:
    string algo_name;
public:
	LCFS(){
	    algo_name = "LCFS";
		set_active_runqueue();
	};
	string get_name(){
        return algo_name;
	};
		void add_to_expired_queue(int dp1, int pid1){
		int ac;
		ac  = get_active();

		if (ac == 1)
		{
				switch(dp1){
            		case 0:
                    runqueue2[0].push_back(pid1);
                    break;
                case 1:
                    runqueue2[1].push_back(pid1);
                	break;
                case 2:
                  runqueue2[2].push_back(pid1);
                  break;
                case 3:
                    runqueue2[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};


		}else{
				switch(dp1){
        		case 0:
	                runqueue[0].push_back(pid1);
	                break;
                case 1:
                    runqueue[1].push_back(pid1);
                	break;
                case 2:
                  runqueue[2].push_back(pid1);
                  break;
                case 3:
                    runqueue[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};
		};
	};


	void add_to_active_queue(int dp1, int pid1){
		int ac;
		ac  = get_active();

		if (ac == 1)
		{
				switch(dp1){
            	case 0:
                    runqueue[0].push_back(pid1);
                    break;
                case 1:
                    runqueue[1].push_back(pid1);
                	break;
                case 2:
                 	runqueue[2].push_back(pid1);
                  	break;
                case 3:
                    runqueue[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};


		}else{
				switch(dp1){
        		case 0:
	                runqueue2[0].push_back(pid1);
	                break;
                case 1:
                    runqueue2[1].push_back(pid1);
                	break;
                case 2:
                  runqueue2[2].push_back(pid1);
                  break;
                case 3:
                    runqueue2[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};
		};

	};

	bool check_swap(){
		int ac1 = get_active();

		if(ac1 == 1){
			if ((runqueue[3].size() == 0 ) &&
				(runqueue[2].size() == 0) &&
				(runqueue[1].size() == 0) &&
			    (runqueue[0].size() == 0 )){
			    return true;
				}else{
			return false;
		}
	}else{
			if ((runqueue2[3].size() == 0 ) &&
				(runqueue2[2].size() == 0) &&
				(runqueue2[1].size() == 0) &&
			    (runqueue2[0].size() == 0 )){
			    return true;
				}else{
			return false;
		};
	};
};

	void swap_queue(){
		int ac1 = get_active();
		if (ac1 == 1){
			set_active_runqueue2();
		}else{
			set_active_runqueue();
		}

	};

   void add_process(int pid,int state){

         process *p1 = p[pid];
         int dp;
         int sp;

            if(state == 0){
            	sp = p1->get_sprio();
            	dp = sp - 1;
             	p1->set_dprio(dp);
            }else{
	            dp = p1->get_dprio();
	            dp = dp - 1;
	            p1->set_dprio(dp);
            };

            dp = 3;
            if (dp == -1){
            	sp = p1->get_sprio();
            	dp = sp - 1;
            	p1->set_dprio(dp);
				add_to_expired_queue(dp,pid);

				if(check_swap()){
           		 	swap_queue();
				};
			}
			else{
           add_to_active_queue(dp,pid);
        };

    };

int get_next_process(void){
  	int q;
  	int ac2 = get_active();

  	if (ac2 == 1){
  		if (runqueue[3].size() != 0)
		{
			q = runqueue[3].back();
			runqueue[3].pop_back();
		}else{
			if (runqueue[2].size() != 0){
				q = runqueue[2].back();
				runqueue[2].pop_back();
			}else{
				if(runqueue[1].size() != 0){
					q = runqueue[1].back();
					runqueue[1].pop_back();
				}else{
					if(runqueue[0].size() != 0){
						q = runqueue[0].back();
						runqueue[0].pop_back();
					};
				};
			};

		};

	  }else{
	  	if (runqueue2[3].size() != 0)
		{
			q = runqueue2[3].back();
			runqueue2[3].pop_back();
		}else{
			if (runqueue2[2].size() != 0){
				q = runqueue2[2].back();
				runqueue2[2].pop_back();
			}else{
				if(runqueue2[1].size() != 0){
					q = runqueue2[1].back();
					runqueue2[1].pop_back();
				}else{
					if(runqueue2[0].size() != 0){
						q = runqueue2[0].back();
						runqueue2[0].pop_back();
					};
				};
			};

		};

	  };
		return q;

    };
};
//***********************************************************************************************************************

//************************************SJF implementation****************************************************************
class SJF: public scheduler{
private:
    string algo_name;
public:
	SJF(){
	    algo_name = "SJF";
		set_active_runqueue();
	};
	string get_name(){
        return algo_name;
	};
		int find_pos(int dp2,int pid2){
			int ac = get_active();
			process *p1;
			int pr;
			process *cur_p = p[pid2];
			int remtime = cur_p->get_rem();
			int i = 0;

				if (ac == 2)
				{
					if (dp2 == 0)
		            {
		            		while(i < runqueue2[0].size()){

			              		pr = runqueue2[0].at(i);
			                    p1 = p[pr];
			                    if(remtime < p1->get_rem()){
			                    	return i;
		               			}
		               			i++;
		               		}
		               		if (i == runqueue2[0].size()){
		               			return i;
		               		};
		              };
		            if(dp2 == 1)
		            {
					      while(i < runqueue2[1].size()){

			                    pr = runqueue2[1].at(i);
			                    p1 = p[pr];
			                    if(remtime < p1->get_rem()){
			                   	 return i;
			               		}
			               		i++;
		               		}
		               		if (i == runqueue2[1].size()){
		               			return i;
		               		};
		            };

		            if(dp2 == 2){

		                  	while(i < runqueue2[2].size()){

			                    pr = runqueue2[2].at(i);
			                    p1 = p[pr];
			                    if(remtime < p1->get_rem()){
			                    	return i;
			               		}
			               		i++;
		               		}
		               		if (i == runqueue2[2].size()){
		               			return i;
		               		};
		            };
		            if(dp2 == 3){

			            while(i < runqueue2[3].size()){

				            pr = runqueue2[3].at(i);
				            p1 = p[pr];
				            if(remtime < p1->get_rem()){
				           	 return i;
				       		}
				       		i++;
			       		}
			       		if (i == runqueue2[3].size()){
			       			return i;
			       		};
			       	};


				}else{
					if (dp2 == 0)	//	switch(dp2){
	           		 {
	            		while(i < runqueue[0].size()){

		              		pr = runqueue2[0].at(i);
		                    p1 = p[pr];
		                    if(remtime < p1->get_rem()){
		                   	 return i;
		               		}
		               		i++;
		               		}
	               		if (i == runqueue[0].size()){
	               			return i;
	               		};
	              };
	            if(dp2 == 1)
	            {
				      while(i < runqueue[1].size()){

	                    pr = runqueue[1].at(i);
	                    p1 = p[pr];
	                    if(remtime < p1->get_rem()){
	                   	 return i;
	               		}
	               		i++;
	               		}
	               		if (i == runqueue[1].size()){
	               			return i;
	               		};
	            };

	            if(dp2 == 2){

	                  	while(i < runqueue[2].size()){

		                    pr = runqueue[2].at(i);
		                    p1 = p[pr];
		                    if(remtime < p1->get_rem()){
		                   	 return i;
		               		}
		               		i++;
		               		}
	               		if (i == runqueue[2].size()){
	               			return i;
	               		};
	            };
	            if(dp2 == 3){

		            while(i < runqueue[3].size()){

			            pr = runqueue[3].at(i);
			            p1 = p[pr];
			            if(remtime < p1->get_rem()){
			           	 return i;
			       		}
			       		i++;
		       		}
		       		if (i == runqueue[3].size()){
		       			return i;
		       		};
		       	};
			};
		};

		void add_to_expired_queue(int dp1, int pid1){
		int ac;
		vector<int>::iterator it;
		int pos;
		pos = find_pos(dp1, pid1);
		ac  = get_active();

		if (ac == 1)
		{
				switch(dp1){
            		case 0:
            		if (pos >= runqueue2[0].size())	{
            			runqueue2[0].push_back(pid1);
					}else{
            			it = runqueue2[0].begin();
                    	runqueue2[0].insert(it+pos,pid1);
                	};
                    break;
                case 1:
                	if (pos >= runqueue2[1].size())	{
            			runqueue2[1].push_back(pid1);
					}else{
                    	it = runqueue2[1].begin();
                    	runqueue2[1].insert(it+pos,pid1);
                	};
                	break;
                case 2:
                	if (pos >= runqueue2[2].size())	{
            			runqueue2[2].push_back(pid1);
					}else{
                  		it = runqueue2[2].begin();
                    	runqueue2[2].insert(it+pos,pid1);
                	};
                  break;
                case 3:
                   if (pos >= runqueue2[3].size())	{
            			runqueue2[3].push_back(pid1);
					}else{
				  	 	it = runqueue2[3].begin();
                    	runqueue2[3].insert(it+pos,pid1);
               		 }
                    break;
                default:
                    break;
                      //do nothing
            	};


		}else{
				switch(dp1){
        		case 0:
        			if (pos >= runqueue[0].size())	{
            			runqueue[0].push_back(pid1);
					}else{
	                	it = runqueue[0].begin();
                    	runqueue[0].insert(it+pos,pid1);
                	};
	                break;
                case 1:
                	if (pos >= runqueue[1].size())	{
            			runqueue[1].push_back(pid1);
					}else{
                    	it = runqueue[1].begin();
                    	runqueue[1].insert(it+pos,pid1);
               		 };
                	break;
                case 2:
                	if (pos >= runqueue[2].size())	{
            			runqueue[2].push_back(pid1);
					}else{
                 	 	it = runqueue[2].begin();
                    	runqueue[2].insert(it+pos,pid1);
                	};
                  break;
                case 3:
                	if (pos >= runqueue[3].size())	{
            			runqueue[3].push_back(pid1);
					}else{
                    	it = runqueue[3].begin();
                    	runqueue[3].insert(it+pos,pid1);
                	};
                    break;
                default:
                    break;
                      //do nothing
            	};
		};
	};


	void add_to_active_queue(int dp1, int pid1){
			int ac;
		vector<int>::iterator it;
		int pos;
		pos = find_pos(dp1, pid1);
		ac  = get_active();
		if (ac == 1)
		{
				switch(dp1){
        		case 0:
        			if (pos >= runqueue[0].size())	{
            			runqueue[0].push_back(pid1);
					}else{
	     	           it = runqueue[0].begin();
            	        runqueue[0].insert(it+pos,pid1);
                	};
	                break;
                case 1:
                	if (pos >= runqueue[1].size())	{
            			runqueue[1].push_back(pid1);
					}else{
                	    it = runqueue[1].begin();
                    	runqueue[1].insert(it+pos,pid1);
               		 };
                	break;
                case 2:
                	if (pos >= runqueue[2].size())	{
            			runqueue[2].push_back(pid1);
					}else{
                 	 	it = runqueue[2].begin();
                    	runqueue[2].insert(it+pos,pid1);
                	};
                  break;
                case 3:
                	if (pos >= runqueue[3].size())	{
             			runqueue[3].push_back(pid1);
					}else{
                    	it = runqueue[3].begin();
                    	runqueue[3].insert(it+pos,pid1);
                	};
                    break;
                default:
                    break;
                      //do nothing
            	};


		}
		else{

				switch(dp1){
            		case 0:
            		if (pos >= runqueue2[0].size())	{
            			runqueue2[0].push_back(pid1);
					}else{
            			it = runqueue2[0].begin();
                    	runqueue2[0].insert(it+pos,pid1);
                	};
                    break;
                case 1:
                	if (pos >= runqueue2[1].size())	{
            			runqueue2[1].push_back(pid1);
					}else{
                    	it = runqueue2[1].begin();
                    	runqueue2[1].insert(it+pos,pid1);
                	};
                	break;
                case 2:
                	if (pos >= runqueue2[2].size())	{
            			runqueue2[2].push_back(pid1);
					}else{
                  		it = runqueue2[2].begin();
                    	runqueue2[2].insert(it+pos,pid1);
                	};
                  break;
                case 3:
                   if (pos >= runqueue2[3].size())	{
            			runqueue2[3].push_back(pid1);
					}else{
				  	 	it = runqueue2[3].begin();
                    	runqueue2[3].insert(it+pos,pid1);
               		 }
                    break;
                default:
                    break;
                      //do nothing
            	};
		};

	};


	bool check_swap(){
		int ac1;
		ac1 = get_active();

		if(ac1 == 1){
			if ((runqueue[3].size() == 0 ) &&
				(runqueue[2].size() == 0) &&
				(runqueue[1].size() == 0) &&
			    (runqueue[0].size() == 0 )){
			    return true;
				}else{
			return false;
		}
	}else{
			if ((runqueue2[3].size() == 0 ) &&
				(runqueue2[2].size() == 0) &&
				(runqueue2[1].size() == 0) &&
			    (runqueue2[0].size() == 0 )){
			    return true;
				}else{
			return false;
		};
	};
};

	void swap_queue(){
		int ac1 = get_active();
		if (ac1 == 1){
			set_active_runqueue2();
		}else{
			set_active_runqueue();
		}

	};

   void add_process(int pid,int state){

         process *p1 = p[pid];
         int dp;
         int sp;

            if(state == 0){
            	sp = p1->get_sprio();
            	dp = sp - 1;
              	p1->set_dprio(dp);
            }else{
            dp = p1->get_dprio();
            dp = dp - 1;
            p1->set_dprio(dp);
            };

            dp = 3;
            if (dp == -1){
            	sp = p1->get_sprio();
            	dp = sp - 1;
            	p1->set_dprio(dp);
				add_to_expired_queue(dp,pid);

				if(check_swap()){
           		 	swap_queue();
				};
			}
			else{
	           add_to_active_queue(dp,pid);
        };

    };

int get_next_process(void){
  	int q;
  	int ac2 = get_active();

  	if (ac2 == 1){
  		if (runqueue[3].size() != 0)
		{
			q = runqueue[3].front();
			runqueue[3].erase(runqueue[3].begin());
		}else{
			if (runqueue[2].size() != 0){
				q = runqueue[2].front();
				runqueue[2].erase(runqueue[2].begin());
			}else{
				if(runqueue[1].size() != 0){
					q = runqueue[1].front();
					runqueue[1].erase(runqueue[1].begin());
				}else{
					if(runqueue[0].size() != 0){
						q = runqueue[0].front();
						runqueue[0].erase(runqueue[0].begin());
					};
				};
			};

		};

	  }else{
	  	if (runqueue2[3].size() != 0)
		{
			q = runqueue2[3].front();
			runqueue2[3].erase(runqueue2[3].begin());
		}else{
			if (runqueue2[2].size() != 0){
				q = runqueue2[2].front();
				runqueue2[2].erase(runqueue2[2].begin());
			}else{
				if(runqueue2[1].size() != 0){
					q = runqueue2[1].front();
					runqueue2[1].erase(runqueue2[1].begin());
				}else{
					if(runqueue2[0].size() != 0){
						q = runqueue2[0].front();
						runqueue2[0].erase(runqueue2[0].begin());
					};
				};
			};

		};

	  };
		return q;
    };
};

//******************************************************************************************************************************************
//***********************************Round Robin implementation****************************************************************************
class RR: public scheduler{
private:
    string algo_name;
public:
		RR(){
		    algo_name = "RR";
		set_active_runqueue();
	};
	string get_name(){
        return algo_name;
	};
		void add_to_expired_queue(int dp1, int pid1){
		int ac;
		ac  = get_active();

		if (ac == 1)
		{
				switch(dp1){
            		case 0:
                    runqueue2[0].push_back(pid1);
                    break;
                case 1:
                    runqueue2[1].push_back(pid1);
                	break;
                case 2:
                  runqueue2[2].push_back(pid1);
                  break;
                case 3:
                    runqueue2[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};


		}else{
				switch(dp1){
        		case 0:
	                runqueue[0].push_back(pid1);
	                break;
                case 1:
                    runqueue[1].push_back(pid1);
                	break;
                case 2:
                  runqueue[2].push_back(pid1);
                  break;
                case 3:
                    runqueue[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};
		};
	};


	void add_to_active_queue(int dp1, int pid1){
		int ac;
		ac  = get_active();
		if (ac == 1)
		{
				switch(dp1){
            	case 0:
                    runqueue[0].push_back(pid1);
                    break;
                case 1:
                    runqueue[1].push_back(pid1);
                	break;
                case 2:

                 	runqueue[2].push_back(pid1);
                  	break;
                case 3:
                    runqueue[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};


		}else{
				switch(dp1){
        		case 0:
	                runqueue2[0].push_back(pid1);
	                break;
                case 1:
                    runqueue2[1].push_back(pid1);
                	break;
                case 2:
                  runqueue2[2].push_back(pid1);
                  break;
                case 3:
                    runqueue2[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};
		};

	};

	bool check_swap(){
		int ac1 = get_active();

		if(ac1 == 1){
			if ((runqueue[3].size() == 0 ) &&
				(runqueue[2].size() == 0) &&
				(runqueue[1].size() == 0) &&
			    (runqueue[0].size() == 0 )){
			    return true;
				}else{
			return false;
		}
	}else{
			if ((runqueue2[3].size() == 0 ) &&
				(runqueue2[2].size() == 0) &&
				(runqueue2[1].size() == 0) &&
			    (runqueue2[0].size() == 0 )){
			    return true;
				}else{
			return false;
		};
	};
};

	void swap_queue(){
		int ac1 = get_active();
		if (ac1 == 1){
			set_active_runqueue2();
		}else{
			set_active_runqueue();
		}

	};

   void add_process(int pid,int state){

         process *p1 = p[pid];
         int dp;
         int sp;

           // if(state == 0){
                //cout<<"add_process";
            	//dp = 3;
            	sp = p1->get_sprio();
            	dp = sp - 1;
              //p1->set_sprio(4);
              p1->set_dprio(dp);
             // cout<<"set";

            //}else{
        // process *p1 = p[pid];
//        sprio = p1->get_sprio();
            //dp = p1->get_dprio();
            //dp = dp - 1;
            //p1->set_dprio(dp);
            //};

            dp = 3;
            if (dp == -1){
            	sp = p1->get_sprio();
            	dp = sp - 1;
            	p1->set_dprio(dp);
				add_to_expired_queue(dp,pid);

				if(check_swap()){
           		 	swap_queue();
				};
			}
			else{
	           add_to_active_queue(dp,pid);
        };

    };

int get_next_process(void){
  	int q;
  	int ac2 = get_active();

  	if (ac2 == 1){
  		if (runqueue[3].size() != 0)
		{
			q = runqueue[3].front();
			runqueue[3].erase(runqueue[3].begin());
		}else{
			if (runqueue[2].size() != 0){
				q = runqueue[2].front();
				runqueue[2].erase(runqueue[2].begin());
			}else{
				if(runqueue[1].size() != 0){
					q = runqueue[1].front();
					runqueue[1].erase(runqueue[1].begin());
				}else{
					if(runqueue[0].size() != 0){
						q = runqueue[0].front();
						runqueue[0].erase(runqueue[0].begin());
					};
				};
			};

		};

	  }else{
	  	if (runqueue2[3].size() != 0)
		{
			q = runqueue2[3].front();
			runqueue2[3].erase(runqueue2[3].begin());
		}else{
			if (runqueue2[2].size() != 0){
				q = runqueue2[2].front();
				runqueue2[2].erase(runqueue2[2].begin());
			}else{
				if(runqueue2[1].size() != 0){
					q = runqueue2[1].front();
					runqueue2[1].erase(runqueue2[1].begin());
				}else{
					if(runqueue2[0].size() != 0){
						q = runqueue2[0].front();
						runqueue2[0].erase(runqueue2[0].begin());
					};
				};
			};

		};

	  };
		return q;
    };
};
//******************************************************************************************************************

//*******************************Priority scheduler*****************************************************************
class P: public scheduler{
    string algo_name;
public:
		P(){
        algo_name = "PRIO";
		set_active_runqueue();
	};
        string get_name(){
            return algo_name;
        };
		void add_to_expired_queue(int dp1, int pid1){
		int ac;
		ac  = get_active();

		if (ac == 1)
		{
				switch(dp1){
            		case 0:
                    runqueue2[0].push_back(pid1);
                    break;
                case 1:
                    runqueue2[1].push_back(pid1);
                	break;
                case 2:
                  runqueue2[2].push_back(pid1);
                  break;
                case 3:
                    runqueue2[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};


		}else{
				switch(dp1){
        		case 0:
	                runqueue[0].push_back(pid1);
	                break;
                case 1:
                    runqueue[1].push_back(pid1);
                	break;
                case 2:
                  runqueue[2].push_back(pid1);
                  break;
                case 3:
                    runqueue[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};
		};
	};


	void add_to_active_queue(int dp1, int pid1){
		int ac;
		ac  = get_active();

		if (ac == 1)
		{
				switch(dp1){
            	case 0:
                    runqueue[0].push_back(pid1);
                    break;
                case 1:
                    runqueue[1].push_back(pid1);
                	break;
                case 2:

                 	runqueue[2].push_back(pid1);
                  	break;
                case 3:
                    runqueue[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};


		}else{
				switch(dp1){
        		case 0:
	                runqueue2[0].push_back(pid1);
	                break;
                case 1:
                    runqueue2[1].push_back(pid1);
                	break;
                case 2:
                  runqueue2[2].push_back(pid1);
                  break;
                case 3:
                    runqueue2[3].push_back(pid1);
                    break;
                default:
                    break;
                      //do nothing
            	};
		};

	};

	bool check_swap(){
		int ac1 = get_active();

		if(ac1 == 1){
			if ((runqueue[3].size() == 0 ) &&
				(runqueue[2].size() == 0) &&
				(runqueue[1].size() == 0) &&
			    (runqueue[0].size() == 0 )){
			    return true;
				}else{
			return false;
		}
	}else{
			if ((runqueue2[3].size() == 0 ) &&
				(runqueue2[2].size() == 0) &&
				(runqueue2[1].size() == 0) &&
			    (runqueue2[0].size() == 0 )){
			    return true;
				}else{
			return false;
		};
	};
};

	void swap_queue(){
		int ac1 = get_active();
		if (ac1 == 1){
			set_active_runqueue2();
		}else{
			set_active_runqueue();
		}

	};

   void add_process(int pid,int state){

        process *p1 = p[pid];
         int dp;
         int sp;

            if(state == 0){
            	sp = p1->get_sprio();
            	dp = sp - 1;
              p1->set_dprio(dp);
            }else{
            dp = p1->get_dprio();
            dp = dp - 1;
            p1->set_dprio(dp);
            };

			dp = p1->get_dprio();
            if (dp == -1){
            	sp = p1->get_sprio();
            	dp = sp - 1;
            	p1->set_dprio(dp);
				add_to_expired_queue(dp,pid);
				if(check_swap()){
           		 	swap_queue();
				};

			}
			else{
           add_to_active_queue(dp,pid);
        };



        };

int get_next_process(void){
  	int q;
  	if(check_swap())
  	{
	 swap_queue();
	  };
  	int ac2 = get_active();

  	if (ac2 == 1){
  		if (runqueue[3].size() != 0)
		{
			q = runqueue[3].front();
			runqueue[3].erase(runqueue[3].begin());
		}else{
			if (runqueue[2].size() != 0){
				q = runqueue[2].front();
				runqueue[2].erase(runqueue[2].begin());
			}else{
				if(runqueue[1].size() != 0){
					q = runqueue[1].front();
					runqueue[1].erase(runqueue[1].begin());
				}else{
					if(runqueue[0].size() != 0){
						q = runqueue[0].front();
						runqueue[0].erase(runqueue[0].begin());
					};
				};
			};

		};

	  }else{
	  	if (runqueue2[3].size() != 0)
		{
			q = runqueue2[3].front();
			runqueue2[3].erase(runqueue2[3].begin());
		}else{
			if (runqueue2[2].size() != 0){
				q = runqueue2[2].front();
				runqueue2[2].erase(runqueue2[2].begin());
			}else{
				if(runqueue2[1].size() != 0){
					q = runqueue2[1].front();
					runqueue2[1].erase(runqueue2[1].begin());
				}else{
					if(runqueue2[0].size() != 0){
						q = runqueue2[0].front();
						runqueue2[0].erase(runqueue2[0].begin());
					};
				};
			};

		};

	  };
		return q;
    };
};
//*****************************************************************************************************************

    struct event{
        int timestamp;
        int pid;
        string oldstate;
        string newstate;
    };
 //*****************************************DES implementation********************************************************

class des{

    struct eventq {
        event data;
        eventq *next;
    };
	eventq *head;
	read_rfile *rf4;
	process *cp;
	int ctime;
	int max_quant;

public:

    void set_max_quant(int quant){
        max_quant = quant;
    };
    int get_max_quant(){
        return max_quant;
    };

    void set_cp(event e){
        cp = p[e.pid];
        };

    void set_null(){
        cp = NULL;
        };

    void set_ctime(event e){
        ctime  = e.timestamp;
    };
    eventq *get_head(){
        return head;
    };

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

    des(string infile,read_rfile **rf3){
        string line;
        int i = 0;
        int rv;
        rf4 = *rf3;
		max_quant = 0;
        ifstream input;
         try{
         	input.open(infile.c_str());
            if (input.is_open())
            {
                while(!input.eof()){

                       getline(input,line);
                      if(line.length() != 0){
                      rv = rf4->get_next_randval();
                      process *p1 = new process(i,line,rv);
                      p.push_back(p1);
                      i++;
                      };
            };

        };
        input.close();
        }
        catch(int error){
            cout<<"file cannot be opened";
        };
        //create initial events

        for(int i = 0; i < p.size(); i++){
            event ev;
            process *p1 = p[i];
            ev.timestamp = p1->get_AT();
            ev.pid       = p1->get_pid();
            ev.oldstate  = "CREATED";
            ev.newstate  = "READY";

            if(i == 0){
                head = new eventq;
                initq(ev);
            }else{
                add_to_eventq(ev);
            }
            if(max_quant < p1->get_CB()){
                max_quant = p1->get_CB();
            };
        };
    };

    event get_event(){
    event ev1;
    if(!(head == NULL))
    {
        ev1 = eventq_front();
        eventq_pop();
        return ev1;
    };
};

void put_event(int pid, int timestamp, string oldstate, string newstate){
        event e;
        e.pid = pid;
        e.timestamp = timestamp;
        e.oldstate = oldstate;
        e.newstate = newstate;
		add_to_eventq(e);
    };
};

//************************************************************************************************************************************************
//********************************* runtime implementation***************************************************************************************
class runtime{
    des *d2;
    scheduler *s2;
    process *cp;
    read_rfile *rf2;
    int current_time;
    event e;
    int IO_busy, last_IO, last_ib, num_p_io;
public:
    runtime(des **d1, scheduler **s1, read_rfile **rf1){
	    d2   = *d1;
	    s2   = *s1;
	    rf2  = *rf1;
	    cp = NULL;
	    current_time = 0;
	    last_IO = 0;
	    last_ib = 0;
	    num_p_io = 0;
	    IO_busy = 0;
    };
    void set_current_time(int t){
    	current_time = t;
	};
	int get_current_time(){
		return current_time;
	};

    int calculate_burst(int max_burst){
	    int randval;
	    randval = rf2->get_next_randval();
	    return ((randval % max_burst) + 1);
    };

    void execute_event(event e1){
        int ib;
        int cib;
        int rem;
		int pid;
        int quant;
        int ccb;
        int rccb;
        int CB;
        int time;
        int t2;
        int TC;

	    process *p1;
        t2 = get_current_time();
		e.pid = e1.pid;
		pid = e.pid;
        p1 = p[e.pid];
        ib = p1->get_IO();
        TC = p1->get_TC();
        rem = p1->get_rem();
        if (verbose){
         cout<<"\n"<<e.timestamp<<"\t"<<e.pid;
        };
        if (rem > 0){
            if (verbose){
                cout<<"\t"<<e.oldstate<<"->"<<e.newstate;
            };
	        e.oldstate = e1.newstate;

        if (e1.newstate == "READY")
        {
        	p1->set_last_ready_t(t2);

            if((e1.oldstate == "CREATED") || (e1.oldstate == "BLOCK")){
				s2->add_process(e1.pid,0);
				if (e1.oldstate == "BLOCK"){
					time = t2 - p1->get_last_blocked_t();
					p1->set_IT(time);
					if(num_p_io == 1){
						IO_busy = IO_busy + (t2 - last_IO);

					};
					num_p_io --;
				};
            }else{
            	cp = NULL;
                    if (verbose){
            		 cout<<"\t"<<"ccb: "<<p1->get_rccb();
               		 cout<<"\t"<<"rem:"<<rem;
               		 cout<<"\t"<<"prio"<<p1->get_dprio();
                    };
                s2->add_process(e1.pid,1);
            };
        }
        else{
			if(e1.newstate == "RUNNING"){
				time = t2 - p1->get_last_ready_t();
				p1->set_CW(time);
				cp = p[e1.pid];
         		rccb = p1->get_rccb();
          		quant = d2->get_max_quant();
        	    CB = p1->get_CB();
            	if(rccb == 0)
            	{
               		 ccb = calculate_burst(CB);
               		 if (ccb > rem)
               			 {
							ccb = rem;
               			 };
                    if (verbose){
               		 cout<<"\t"<<"ccb: "<<ccb;
               		 cout<<"\t"<<"rem:"<<rem;
               		 cout<<"\t"<<"prio"<<p1->get_dprio();
               		 };
           		     p1->set_ccb(ccb);
                	if(quant < ccb){
                		time = t2 + quant;
                    	d2->put_event(pid, time, "RUNNING", "READY");
                  	  rccb = ccb - quant;
                    	rem = rem - quant;
                  	  p1->set_rem(rem);
                    	p1->set_rccb(rccb);
             	   }else{
             	   		time = t2 + ccb;
                	    d2->put_event(pid, time, "RUNNING", "BLOCK");
                   		 rem = rem - ccb;
                   	 p1->set_rem(rem);
                    	p1->set_rccb(0);
                	};
            	}
          		  else{
                    if (verbose){
          		  	cout<<"\t"<<"ccb: "<<rccb;
               		 cout<<"\t"<<"rem:"<<rem;
               		 cout<<"\t"<<"prio"<<p1->get_dprio();
                    };
               		 if (rccb <= quant){
               		 	quant = rccb;
               		 	time = t2 + quant;
               		 	d2->put_event(pid, time, "RUNNING", "BLOCK");
						}else{
							time = t2 + quant;
							d2->put_event(pid, time, "RUNNING", "READY");
						}
               		 rccb = rccb - quant;
               		 rem = rem - quant;
                	p1->set_rem(rem);
                	p1->set_rccb(rccb);
            	};

			}else{
			     if(e1.newstate == "BLOCK"){
			     	p1->set_last_blocked_t(t2);
			    	cp = NULL;
            		if (rem > 0){
            			cib = calculate_burst(ib);
					  if (verbose){
					    cout<<"\t"<<"ib: "<<cib;
					    cout<<"\t"<<"rem: "<<rem;
					  };
					   		num_p_io++;
					   		if(num_p_io == 1){
					   			last_IO = t2;
					   		};
					    time = t2 + cib;
                   		d2->put_event(e.pid, time, "BLOCK", "READY");
               		 };
           		 };
                };
       		 };

   		 }else{
   		 	cp = NULL;
   		 	p1->set_FT_TT(t2);
   		 	if (verbose){
   		 	cout<<"\t"<<"done.";
   		 	};
			};
	};

    void start_simulation(){
        int pid;
        int quant;
        int ccb;
        int rccb;
        int rem;
        int CB;
        int t1;

        cp = NULL;
        int i = 0;
        int j = 0;
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

        	if(cp == NULL)
       		 {
       		 if(!s2->is_runqueue_empty()){
            	pid = s2->get_next_process();
            	d2->put_event(pid, t1,"READY","RUNNING");
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
  		 };

  		 void print_result(){
  		 	int i = 0;
  		 	double total_IT = 0;
  		 	double total_TC = 0;
			double total_CW = 0;
			double total_TT = 0;
			double num_proc;
			int FT;
			double IO_busy1;
  		 	double CPU_U;
  		 	double IO_U;
  		 	double av_TT;
  		 	double av_CW;
  		 	double throughput;
  		 	string name;

  		 	process *p1;
  		 	name = s2->get_name();
            cout << name;
            if (name == "RR" || name == "PRIO"){
              cout << " " << d2->get_max_quant();
            };
            cout<<"\n";
  		 	for(i = 0; i < p.size(); i++){
  		 		p1 = p[i];
  		 		printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
			       p1->get_pid(),
			       p1->get_AT(), p1->get_TC(), p1->get_CB() , p1->get_IO(), p1->get_sprio(),
			       p1->get_FT(),
			       p1->get_TT(),
			       p1->get_IT(),
			       p1->get_CW());
			       total_TT = total_TT + p1->get_TT();
			       total_IT = total_IT + p1->get_IT();
			       total_TC = total_TC + p1->get_TC();
			       total_CW = total_CW + p1->get_CW();
			       if (FT < p1->get_FT()){
			       FT = p1->get_FT();
			   		};
			   };

			   num_proc = p.size();
			   IO_busy1 = IO_busy;
			   CPU_U = ( total_TC / FT ) * 100;
			   IO_U = ( IO_busy1 / FT ) * 100;
			   av_TT = total_TT / num_proc;
			   av_CW = total_CW / num_proc;
			   throughput = ( num_proc / FT ) * 100;

			printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
	       FT,
	       CPU_U,
	       IO_U,
	       av_TT,
	       av_CW,
	       throughput);

		   };
};
//*******************************************************************************************************************************
//*********************main method**********************************************************************************************
int main(int argc, char **argv){
	int c;
	verbose = false;
	string schedval;
	string infile;
	string rand_file;
	bool infile_set = false;
	int quant = 0;
	char schedspec;
	string argument;
	int index;
	int counter;

	 while ((c = getopt (argc, argv, "vs:")) != -1)
    switch (c)
      {
      case 'v':
        verbose = true;
        break;
      case 's':
        schedval = optarg;
        break;
      default:
        abort();
      }
      for (index = optind; index < argc; index++){
        infile = argv[index];
        index++;
        rand_file = argv[index];
        }

counter = schedval.length();
schedspec = schedval[0];
if (counter > 1){

	schedval = schedval.substr(1,counter);
	istringstream ss(schedval);
	ss >> quant;
};

//instantiate scheduler
scheduler * s;

read_rfile *rfile = new read_rfile(rand_file);
//instantiate DES
des *d = new des(infile,&rfile);
//pass the scheduler object
if (schedspec == 'F')
{
    FCFS *fsched = new FCFS;
    s = fsched;
}
else{
	if (schedspec == 'L'){
		LCFS *lsched = new LCFS;
		s = lsched;
	}else{
		if(schedspec == 'S'){
			SJF *ssched = new SJF;
			s = ssched;
		}else{

		if(schedspec == 'R'){
			RR *rsched =  new RR;
			s = rsched;
			d->set_max_quant(quant);
		}else{
			if(schedspec == 'P'){
				P *psched = new P;
				s = psched;
				d->set_max_quant(quant);
			};
		};
	};
	};

}
//insantiate runtime
runtime *r = new runtime(&d,&s,&rfile);
r->start_simulation();
r->print_result();

return 0;
};
//**************************************************************************************************************************************
