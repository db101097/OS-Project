/*
Name : Deion Bachus
Section 3
*/
#include<iostream>
#include<vector>
using namespace std;


struct Process
{
	int pid=-1;
	int time_q=0;
	int rank=0;
	string filename="";	
};

struct Page
{
	int page_num=0;
	Process x;
	int ts;
};

struct Processor
{
	Process current;
};

struct Ready_Queue
{
	vector<Process>lvl_0;
	vector<Process>lvl_1;
	vector<Process>lvl_2;
};

struct HDD
{
	vector<Process>hddq;
	Process curr;
};

struct Frame
{
	Page pg;
	int frame_num=0;
};

bool demote(Processor & p , Ready_Queue & q )
{
		
		if(p.current.rank==3)
		{
			//cout<<"Rank 3 no demotion necessary"<<endl;
			return false;
		}

		if(p.current.rank==1 && p.current.time_q==1)
		{
				//cout<<"Rank 1 the process will move to 2"<<endl;
				p.current.rank++;
				p.current.time_q=0;
				q.lvl_1.push_back(p.current);
				p.current.rank=0;
				return true;
		}

		else if(p.current.rank==2 && p.current.time_q==2)
		{
				//cout<<"Rank 2 the process will move to 3"<<endl;
				p.current.rank++;
				p.current.time_q=0;
				q.lvl_2.push_back(p.current);
				p.current.rank=0;
				return true;
		}


		return false;
}


void pick_process(Processor & p , Ready_Queue & q)
{

	//check level 0
	if(q.lvl_0.size()>0)
	{
		if(p.current.rank>1)
		{
			//pre empt
			if(p.current.rank==2)
			{
				q.lvl_1.emplace(q.lvl_1.begin(),p.current);
				p.current=q.lvl_0[0];
				q.lvl_0.erase(q.lvl_0.begin());
			}
		
			else 
			{
				q.lvl_2.emplace(q.lvl_2.begin(),p.current);
				p.current=q.lvl_0[0];
				q.lvl_0.erase(q.lvl_0.begin());
			}		
		 }
	
		else if(p.current.rank==1 && p.current.time_q==0)
		{
				return;
		}

		else
		{
			p.current=q.lvl_0[0];
			q.lvl_0.erase(q.lvl_0.begin());
		}
	}

	else if(q.lvl_1.size()>0)
	{
		if(p.current.rank>2)
		{
			q.lvl_2.emplace(q.lvl_2.begin(),p.current);	
			p.current=q.lvl_1[0];
			q.lvl_1.erase(q.lvl_1.begin());
		}

		else if(p.current.rank==2 && p.current.time_q<2)
		{
				return;
		}

		else
		{
			p.current=q.lvl_1[0];
			q.lvl_1.erase(q.lvl_1.begin());
		}
	}

	else if(p.current.rank==3)
	{
		return;
	}

	else if( q.lvl_2.size()!=0)
	{
		p.current=q.lvl_2[0];
		q.lvl_2.erase(q.lvl_2.begin());
	}

	else 
	return;
}	

void pick_hddq(	int n , vector<HDD> & hdd)
{
	if(hdd[n].curr.pid==-1 && hdd[n].hddq.size()>0)
	{
		hdd[n].curr=(hdd[n]).hddq[0];
		(hdd[n]).hddq.erase(hdd[n].hddq.begin());	
	}

}

void reposition(Process x , Ready_Queue & q)
{
	//cout<<"here"<<endl;
	x.time_q=0;
	if(x.rank==1)
	{
		q.lvl_0.push_back(x);
	}

	else if(x.rank==2)
	{
			q.lvl_1.push_back(x);
	}

	else
	q.lvl_2.push_back(x);
	
	
}

void insert_frame(vector<Frame> & f, Process p , int  pn)
{
			if(p.pid==-1)
			{
					cout<<"No process to allocate for"<<endl;
					return;
			}

			for(int i=0;i<f.size();i++)
			{
					if(f[i].pg.page_num==-1)
					{
							f[i].pg.page_num=pn;
							f[i].pg.x=p;
							return;
					}
			}

			f[0].pg.page_num=pn;
			f[0].pg.x=p;
			f.emplace(f.end(),f[0]);
			f.erase(f.begin());		
}

void deallocate(vector<Frame> & f, Process p)
{
	for(int i=0;i<f.size();i++)
	{
		if(f[i].pg.x.pid==p.pid)
		{
			f[i].pg.page_num=-1;
			f[i].pg.x.pid=-1;	
		}
	}
}
		
int main ()
{
	string input="";
	long int mem_size;
	long int pf_size;
	int hd;
	Processor p;
	Ready_Queue q;
	vector<HDD>hdd;
	vector<Frame> f;
	int current_pid=0;
	cout<<"Enter the memory size"<<endl;
	cin>>mem_size;

	cout<<"What is the size of a page/frame ?"<<endl;
	cin>>pf_size;

	cout<<"How many hard disks does the simulated computer have ? "<<endl;
	cin>>hd;
	cin.ignore();

	for(int i=0;i<hd;i++)
	{
			HDD t;
			hdd.push_back(t);
	}

	for(int j=0;j<(mem_size/pf_size);j++)
	{
			Frame temp;
			temp.pg.page_num=-1;
			temp.frame_num=j;
			f.push_back(temp);		
	}	
	

	while(input!="E")
 {
	getline(cin,input);
 
	if(input=="A")
	{
		current_pid++;
		Process x;
		x.rank=1;
		x.pid=current_pid;
		insert_frame(f,x,0);
		q.lvl_0.push_back(x);
		pick_process(p,q);
    }

	if(input=="Q")
	{
		if(p.current.rank<3)
		{
			p.current.time_q++;
			if(demote(p,q)==true)
			pick_process(p,q);
		}

	}

	if(input=="t")
	{
		p.current.rank=0;
		deallocate(f,p.current);
		pick_process(p,q);
	}
	
	if(input.length()>4)
{
	if(input.at(0)=='d')
	{
		if(input.at(1)==' ')
		{
			string num;
			int tmp=0;
			int i=2;
			while(input.at(i)!=' ')
			{
					if(i==input.length()-1)
					{
						cout<<"Invalid input"<<endl;
						break;
					}
					num+=input.at(i);
					i++;
			}
			tmp=stoi(num);
			i++;

			while(i!=input.length())
			{
				p.current.filename+=input.at(i);
				i++;
			}

			hdd[tmp].hddq.push_back(p.current);
			pick_hddq(tmp,hdd);
			p.current.rank=0;
			pick_process(p,q);
		}		
			
		else
		{
			cout<<'"'<<input<<'"'<<" is not valid "<<endl;
		}
	}
}

	if(input.length()>2)
{
	if(input.at(0)=='D')
	{
			string hold;
			int dn;
			if(input.at(1)!=' ')
			{
				cout<<"Invalid input"<<endl;
				break;
			}

			for(int i=2;i<input.length();i++)
			{
					hold+=input.at(i);
			}
			dn=stoi(hold);

			if(dn>hdd.size()-1)
			{
				cout<<"HDD "<<dn<<", does not exist"<<endl;
			}

			else
			{	
				if(hdd[dn].curr.pid!=-1)
				{	
					reposition(hdd[dn].curr,q);
					pick_process(p,q);
					hdd[dn].curr.pid=-1;
					pick_hddq(dn,hdd);
				}
			}
	}	

	if(input.at(0)=='m')
	{
			if(input.at(1)==' ')
			{
				string h;
				int pn;
				for(int x=2;x<input.size();x++)
				{
						h+=input.at(x);
				}
				pn=stoi(h)/(pf_size);
				insert_frame(f,p.current,pn);
			}

			else
			{
					cout<<"Invalid input"<<endl;
			}
	}
}
	if(input=="S r")
	{		
		cout<<endl;
		cout<<"---------------------------------------------------"<<endl;	
		if(p.current.rank==0)
		{
			cout<<"Processor is empty"<<endl;
		}

		else
		{
			cout<<"Process "<<p.current.pid<<" is using the cpu"<<endl;
		}

		cout<<"Level 0 contains "<<endl;
		for(auto x : q.lvl_0)
		{
				cout<<x.pid<<" ";
		}
		cout<<" "<<endl;
		cout<<endl;
		cout<<"Level 1 contains "<<endl;
		for(auto x : q.lvl_1)
		{
				cout<<x.pid<<" ";
		}
	
		cout<<" "<<endl;
		cout<<endl;
		cout<<"Level 2 contains "<<endl;
		for(auto x : q.lvl_2)
		{
				cout<<x.pid<<" ";
		}
		cout<<endl;
		cout<<"---------------------------------------------------"<<endl;		
	}

	if(input=="S i")
	{
		cout<<endl;
		cout<<"---------------------------------------------------"<<endl;	
		int j=0;
		for(auto x : hdd)
		{
			cout<<"Hard Disk "<<j<<endl;
			if(x.curr.pid==-1)
			{
				cout<<"Nothing is reading or writing"<<endl;
			}

			else
			cout<<"Process "<<x.curr.pid<<" is current reading/writing to "<<x.curr.filename<<endl;

			cout<<"These processes are currently waiting"<<endl;
			for ( auto i: x.hddq )
			{
				cout<<i.pid<<" "<<i.filename<<endl;
			}
			j++;
			cout<<endl;
			cout<<"---------------------------------------------------"<<endl;	
		}
	}

	if(input=="S m")
	{
		cout<<endl;
		cout<<"---------------------------------------------------"<<endl;
		for(auto i:f)
		{
			cout<<"Frame "<< i.frame_num << "| Process "<< i.pg.x.pid << "| Page "<< i.pg.page_num<<endl;
		}
		cout<<endl;
		cout<<"---------------------------------------------------"<<endl;
	}

	if(cin.eof())
	{
			return 0;
	}
 }
}
