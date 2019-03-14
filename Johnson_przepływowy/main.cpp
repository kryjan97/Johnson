#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;
int job_amount=0;
int machine_amount=0;
/* Klasa przechowuje zadanie i informacje o nim */
class jobs{
private:
    /*Numer zadania*/
    int no_job;
    /*Numer maszyny na której zadanie jest wykonywane*/
    int no_machine;
    /*Czas wykonania zadania na danej maszynie*/
    int execution_time;

public:

    jobs(int no_job, int no_machine, int execution_time){
    this->no_job=no_job;
    this->no_machine=no_machine;
    this->execution_time=execution_time;
    };

    int get_job() {return no_job;}
    int get_machine(){return no_machine;}
    int get_time(){return execution_time;}
    int set_job(int no_job) {this->no_job=no_job;}
    int set_machine(int no_machine){this->no_machine=no_machine;}
    int set_time(int execution_time){this->execution_time=execution_time;}
    void show_job(){
        cout<<"nr. zadania: "<<no_job<<"  ";
        cout<<"czas wykonania: "<<execution_time<<"  ";
        cout<<"nr. maszyny: "<<no_machine<<endl;
    }


};
/*Funkcja liczy czas całkowity dla 2 maszyn*/
int calculate_two(vector<jobs> machine1, vector<jobs> machine2);
/*Funkcja wypisuje elementy z wektora*/
void show_vector(vector<jobs> jobs);
/*Funkcja ładuje dane z pliku*/
vector<jobs> load_jobs_two_machines();
/*Algorytm Johnsona*/
void Johnson_rule(vector<jobs> job);
/*Przegląd zupełny*/
void permutation(vector<jobs> job);




void show_vector(vector<jobs> jobs){

    for(int i=0;i<jobs.size();i++)
        jobs[i].show_job();

}



vector<jobs> load_jobs_two_machines() {
    int a, b;
    vector<jobs> jobs_container;
    ifstream file;
    file.open("data");
    file >> job_amount >> machine_amount;

    for (int i = 0; i < job_amount; i++) {
        file >> a >> b;
        jobs_container.push_back(jobs(i + 1, 1, a));
        jobs_container.push_back(jobs(i + 1, 2, b));
    }
    file.close();

    return jobs_container;
}


void Johnson_rule(vector<jobs> job){
    vector<jobs>first,second,machine1,machine2;
    vector<jobs>save=job;
    int min=1000,task;
    bool conti;
    while(!job.empty()) {
        conti=false;
        //szukanie minimalnej wartości
        for (int i = 0; i < job.size(); i++) {
            if (job[i].get_time() < min)
                min = job[i].get_time();
        }
        //przestawienie jej do odpowiedniego kontenera
        for(int j=0;j < job.size();j++){
            if(job[j].get_time()==min){
                if(job[j].get_machine()==1){
                    first.push_back(job[j]);
                }
                else if(job[j].get_machine()==2){
                    second.insert(second.begin(),job[j]);
                }
                task=job[j].get_job();
                job.erase(job.begin()+j);

                for(int k=0;k<job.size();k++){

                    if(task==job[k].get_job()){
                        job.erase(job.begin()+k);
                        min=1000;
                        conti=true;
                        break;
                    } //usuwam duplikat zadania

                }

            }
            if(conti) break;
        }
    }


    //łącze w całość
    while(!second.empty()){
        first.push_back(second[0]);
        second.erase(second.begin());
    }



    //podział na dwie maszyny w optymalnej kolejności
   int j=0;


            for(int j=0;j<first.size();j++){
                for(int i=0;i<save.size();i++){
                    if(save[i].get_job()==first[j].get_job() ){
                        if( save[i].get_machine()==1){
                            machine1.push_back(save[i]);
                        }
                        else{
                            machine2.push_back(save[i]);
                        }

                    }
                }
            }
cout<<"METODA JOHNSONA"<<endl;
    int wynik=calculate_two(machine1,machine2);
    cout<<endl<<"Min wynik to: "<<wynik<<endl;
cout<<"Dla kolejnosci: ";
            for(int i=0; i<first.size();i++)
                cout<<first[i].get_job()<<" ";

}




int calculate_two(vector<jobs> machine1, vector<jobs> machine2){
    int time1=0,time2=0;
    int tmp=0;

    for(int i = 0; i < machine1.size(); i++){
        time1 = time1 + machine1[i].get_time();
        tmp = 0;
        while(machine1[i].get_job() != machine2[tmp].get_job()){
            tmp++; }

        if(i == 0) time2 = machine1[i].get_time() + machine2[tmp].get_time();

        if (i != 0 && time1 <= time2){
            time2 = time2 + machine2[tmp].get_time();
        }
        else if(i != 0 && time1 > time2) {
            time2 = time2 + (time1 - time2);
            time2 = time2 + machine2[tmp].get_time();
        }

    }

    return time2;

}




void permutation(vector<jobs> job){
   vector<int> amount;
   vector<int> save_order;
   int min_wynik=200000;
   int wynik=0;
   vector<jobs> machine1,machine2;
   for(int i=0; i<job_amount;i++){
       amount.push_back(i+1);
   }

    do{

        for(int j=0;j<amount.size();j++){
            for(int i=0;i<job.size();i++){
                if(job[i].get_job()==amount[j] ){
                    if( job[i].get_machine()==1){
                        machine1.push_back(job[i]);
                    }
                    else{
                        machine2.push_back(job[i]);
                    }

                }
            }
        }
        wynik=calculate_two(machine1,machine2);
        cout<<"Dla kolejnosci: ";
        for(int i=0;i<amount.size();i++){
            cout<<amount[i]<<" ";
        }
        cout<<"   Wynik to: "<<wynik<<endl;
        
        machine1.clear();
        machine2.clear();
        if(wynik < min_wynik){
            min_wynik=wynik;
            save_order=amount;
        }
    }
    while(std::next_permutation(amount.begin(),amount.end()));

    //cout<<endl<<"METODA PERMUTACJI !"<<endl;
    cout<<"Min wynik to: "<<min_wynik<<endl<<"Dla kolejnosci: ";
    for(int i=0;i<save_order.size();i++){
        cout<<save_order[i]<<" ";
    }
    cout<<endl<<endl<<endl;
}

int main() {
    vector<jobs> jobs_container=load_jobs_two_machines();
    permutation(jobs_container);
    Johnson_rule(jobs_container);

   return 0;
}