using namespace std;
#include <string>
#include <vector>
#include "Action.h"
#include "WareHouse.h"
#include "Order.h"
        WareHouse::WareHouse(const string &configFilePath):
            isOpen(false),actionsLog(),volunteers(),pendingOrders()
            ,inProcessOrders(),completedOrders(),customers(),customerCounter(0)
            ,volunteerCounter(0),orderCounter(0){
            ifstream file(configFilePath);
            string line;

            while (getline(file, line)) {
                if(line.empty())
                    continue;
                if (line[0]=='#')
                    continue;
                istringstream iss(line);
                vector<string> words{
                istream_iterator<string>{iss},
                istream_iterator<string>{}
                };    
                if (words[0] == "customer") {
                    if (words[2] == "soldier") {
                        SoldierCustomer* newCustomer =new SoldierCustomer(customerCounter,words[1], std::stoi(words[3]), std::stoi(words[4]));
                        customerCounter++;
                        customers.push_back(newCustomer);                       
                        
                    } else if (words[2] == "civilian") {
                        CivilianCustomer* newCustomer= new CivilianCustomer(customerCounter,words[1], std::stoi(words[3]), std::stoi(words[4]));  
                        customerCounter++;                    
                        customers.push_back(newCustomer);
                        
                    }
                } 
                else  {
                    if (words[2] == "collector") {
                        CollectorVolunteer* newVolunteer=new CollectorVolunteer(volunteerCounter,words[1], std::stoi(words[3]));
                        volunteerCounter++;
                        volunteers.push_back(newVolunteer);                        
                        
                    }
                    else if (words[2] == "limited_collector") { 
                        LimitedCollectorVolunteer* newVolunteer=new LimitedCollectorVolunteer(volunteerCounter,words[1], std::stoi(words[3]), std::stoi(words[4])); 
                        volunteerCounter++; 
                        volunteers.push_back(newVolunteer);                       
                                            
                    }
                    else if (words[2] == "driver") {
                        DriverVolunteer* newVolunteer=new DriverVolunteer(volunteerCounter,words[1],std::stoi(words[3]), std::stoi(words[4]));
                        volunteerCounter++;
                        volunteers.push_back(newVolunteer);
                        
                    }
                    else if (words[2] == "limited_driver") {
                        LimitedDriverVolunteer* newVolunteer=new LimitedDriverVolunteer(volunteerCounter,words[1],std::stoi(words[3]), std::stoi(words[4]), std::stoi(words[5]));
                        volunteerCounter++;
                        volunteers.push_back(newVolunteer);                
                        
                    }
                }
                
            }
        };

        void WareHouse:: start(){
            open();
            std::cout << "Warehouse is open!\n";
            while (isOpen){
                std::string input;
                std::getline(std::cin, input);  // save the string entered in input
                std::istringstream iss(input);  // Split the input string into words
                std::vector<std::string> words;
                std::string word;
                while (iss >> word) {
                    words.push_back(word);
                };
                if(words[0] == "step" &&words.size()==2){
                    BaseAction* simulateStep=new SimulateStep(stoi(words[1]));
                    simulateStep->act(*this); 
                                    
                }
                else if (words[0]=="order" &&words.size()==2){
                    BaseAction* anotherOrder=new AddOrder(stoi(words[1]));
                    anotherOrder->act(*this); 
                    
                }
                else if (words[0]=="customer" &&words.size()==5){
                    string customer_name=words[1];
                    string customer_type=words[2];
                    int distance=stoi(words[3]);
                    int maxOrders=stoi(words[4]);
                    BaseAction* addcustomer=new AddCustomer(customer_name,customer_type,distance,maxOrders);
                    addcustomer->act(*this); 
                      
                }
                else if (words[0]=="orderStatus"&&words.size()==2){
                    BaseAction* order_status=new PrintOrderStatus(stoi(words[1]));
                    order_status->act(*this);
                    
                }
                else if (words[0]=="customerStatus"&&words.size()==2){
                    BaseAction* customer_status=new PrintCustomerStatus(stoi(words[1]));
                    customer_status->act(*this);
                    
                }
                else if (words[0]=="volunteerStatus"&&words.size()==2){
                    BaseAction* volunteer_status=new PrintVolunteerStatus(stoi(words[1]));
                    volunteer_status->act(*this);
                    
                    
                }
                else if (words[0]=="log"&&words.size()==1){
                    BaseAction* Print_Actions_Log=new PrintActionsLog();
                    Print_Actions_Log->act(*this);
                    
                }
                else if (words[0]=="close"&&words.size()==1){
                    BaseAction* close=new Close();
                    close->act(*this);
                    
                }
                else if (words[0]=="backup"&&words.size()==1){
                    BaseAction* Backup_WareHouse=new BackupWareHouse();
                    Backup_WareHouse->act(*this);
                    addAction(Backup_WareHouse);
                    
                }
                else if (words[0]=="restore"&&words.size()==1){
                    BaseAction* Restore_WareHouse=new RestoreWareHouse();
                    Restore_WareHouse->act(*this);
                    addAction(Restore_WareHouse);
                    
                }
                
                else{
                    std::cout << "Invalid input\n";
                };
                };
            };  
        
        const vector<BaseAction*> & WareHouse:: getActions() const{
            return actionsLog;
        }; 
        void WareHouse:: addOrder(Order* order){
            pendingOrders.push_back(order);
            orderCounter++;
            
        };
        void WareHouse:: addAction(BaseAction* action){
            actionsLog.push_back(action);
        };
        void WareHouse:: addCustomer(Customer* customer){//we added
            customers.push_back(customer);
            customerCounter++;
        };
        Customer& WareHouse:: getCustomer(int customerId) const{
            Customer* cust=nullptr;
            if (customerId<0||customerId>customerCounter){
                std::cout <<"Customer doesn’t exist\n";
                return *cust;
            }
            for ( Customer* C :customers){
                if (C->getId()==customerId)
                    return *C;    
            }
            std::cout <<"Customer doesn’t exist\n";
            return *cust;
        };
        

        Volunteer& WareHouse::getVolunteer(int volunteerId) const {
            Volunteer* vol=nullptr;
            if(volunteerId<0 || volunteerId>volunteerCounter){
                std::cout << "Volunteer doesn't exist\n";
                return *vol;
            }
            for (Volunteer* V : volunteers) {
                if (V->getId() == volunteerId)
                    return *V;
            }
            std::cout << "Volunteer doesn't exist\n";
            return *vol;
        };
        Order& WareHouse:: getOrder(int orderId) const{
            Order* ord=nullptr;
            if(orderId<0||orderId>orderCounter){
                cout <<"order doesn’t exist\n";
                return *ord;
            }
            else { 
                    for (  Order* pendOrd :pendingOrders){
                        if (pendOrd->getId()==orderId)
                            ord=pendOrd;                      
                    }
                    for (  Order* inPrcOrd :inProcessOrders){
                        if (inPrcOrd->getId()==orderId)
                            ord= inPrcOrd;                    
                    }
                    for (  Order* compOrd :completedOrders){
                        if (compOrd->getId()==orderId)
                            ord= compOrd;                    
                    }
                 };
            if (ord==nullptr)
                cout <<"order doesn’t exist\n";
            
            return *ord;
        };
        void  WareHouse:: close(){
            isOpen=false;
            
        };
        void  WareHouse:: open(){
            isOpen=true;
            
        };
        vector<Order*>& WareHouse::getPendingOrders()  {//we added
            return pendingOrders;
        };
        vector<Order*>& WareHouse::getInProcessOrders()  {//we added
            return inProcessOrders;
        };
        vector<Order*>& WareHouse::getCompletedOrders()  {//we added
            return completedOrders;
        };
        vector<Customer*> WareHouse:: getCustomers()const{//we added
            return customers;
        };
        vector<Volunteer*> WareHouse:: getVolunteers()const{//we added
            return volunteers;
        };
        int WareHouse:: getCustomerCounter()const{//we added
            return customerCounter;
        };
        int WareHouse:: getVolunteerCounter()const{//we added
            return volunteerCounter;
        };
        int WareHouse:: getOrderCounter()const{//we added
            return orderCounter;
        };
        WareHouse:: ~WareHouse(){//destructor rule of 5
        
            for (std::vector<Order*>::size_type i=0;i< pendingOrders.size()&&pendingOrders.size()>0;i++){
                 Order* order = pendingOrders[i];
                 if (order){              
                    delete order;
                    order = nullptr;
                 }
            }
            pendingOrders.clear();
            for (std::vector<Order*>::size_type i=0;i< inProcessOrders.size();i++){
                 Order *order = inProcessOrders[i];
                 if (order){              
                    delete order;
                    order = nullptr;
                 }
            }
            inProcessOrders.clear();
             for (std::vector<Order*>::size_type i=0;i< completedOrders.size();i++){
                 Order *order = completedOrders[i];
                 if (order){              
                    delete order;
                    order = nullptr;
                 }
            }
            completedOrders.clear();
             for (std::vector<Customer*>::size_type i=0;i< customers.size();i++){
              Customer *customer = customers[i];
             if (customer){
                delete customer;
                customer = nullptr;
            }
            }
             customers.clear();


            for (std::vector<Volunteer*>::size_type i=0;i< volunteers.size();i++){
               Volunteer *volunteer = volunteers[i];
             if (volunteer){
                delete volunteer;
                volunteer = nullptr;
            }
            }
             volunteers.clear();
            for (std::vector<BaseAction*>::size_type i=0;i< actionsLog.size();i++){
                BaseAction* action=actionsLog[i];
                if(action ){
                    delete action;
                    action = nullptr;
                }
            }
            actionsLog.clear();
        };
        WareHouse:: WareHouse(const WareHouse &other):
        isOpen(other.isOpen) ,actionsLog(),volunteers(),pendingOrders()
            ,inProcessOrders(),completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter)
        {//copy constructor rule of 5
            
            for (BaseAction* act : other.actionsLog)
                actionsLog.push_back(act->clone());
            for (Volunteer* vol : other.volunteers)
                volunteers.push_back(vol->clone());
            for (Order* ord : other.pendingOrders)
                pendingOrders.push_back(ord->Clone());
            for (Order* ord : other.inProcessOrders)
                inProcessOrders.push_back(ord->Clone());
            for (Order* ord : other.completedOrders)
                completedOrders.push_back(ord->Clone());
            for (Customer* cust : other.customers)
                customers.push_back(cust->clone());
        };
        WareHouse:: WareHouse(WareHouse &&other):
          isOpen(other.isOpen) ,actionsLog(),volunteers(),pendingOrders()
         ,inProcessOrders(),completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter)
        {//move constructor rule of 5 steals the values from other
            for (Order* ord : other.pendingOrders){
                pendingOrders.push_back(ord);
            };
            other.pendingOrders.clear();
            for (Order* ord : other.inProcessOrders){
                inProcessOrders.push_back(ord);
            };
            other.inProcessOrders.clear();
            for (Order* ord : other.completedOrders){
                completedOrders.push_back(ord);
            };
            other.completedOrders.clear();
            for (Customer* cust : other.customers){
                customers.push_back(cust);
            };
            other.customers.clear();
            for (Volunteer* vol : other.volunteers){
                volunteers.push_back(vol);
            };
            other.volunteers.clear();
            for (BaseAction* act : other.actionsLog){
                actionsLog.push_back(act);
            };
            other.actionsLog.clear();
            delete &other;
        };
        WareHouse& WareHouse::operator=(const WareHouse &other){//copy assignment rule of 5  copy the value from other
            if (this!=&other){
                isOpen=other.isOpen;
                customerCounter=other.customerCounter;
                volunteerCounter=other.volunteerCounter;
                orderCounter=other.orderCounter;
                
            for (BaseAction* action : actionsLog) {
                delete(action);
            }
                actionsLog.clear();
            for (BaseAction* action : other.actionsLog) {
                actionsLog.push_back(action->clone());
            }
            for (Volunteer* volunteer : volunteers) {
                delete(volunteer);
            }
            volunteers.clear();
            for (Volunteer* volunteer : other.volunteers) {
                volunteers.push_back(volunteer->clone());
            }
            for (Order* ord1 : pendingOrders) {
                delete(ord1);
            }
            pendingOrders.clear();
            for (Order* ord1 : other.pendingOrders) {
                pendingOrders.push_back(ord1->Clone());
            }
            for (Order* ord2 : inProcessOrders) {
                delete(ord2);
            }
            inProcessOrders.clear();
            for (Order* ord2 : other.inProcessOrders) {
                inProcessOrders.push_back(ord2->Clone());
            }
            for (Order* ord3 : completedOrders) {
                delete(ord3);
            }
            completedOrders.clear();
            for (Order* ord3 : other.completedOrders) {
                completedOrders.push_back(ord3->Clone());
            }
            for (Customer* customer : customers) {
                delete(customer);
            }
            customers.clear();
            for (Customer* customer : other.customers) {
                customers.push_back(customer->clone());
            }       
            };          
            return *this;
        };
        WareHouse& WareHouse::operator=(WareHouse &&other){//move assignment rule of 5 steals the values from other and delete other
            if (this!=&other){
                isOpen=other.isOpen;
                customerCounter=other.customerCounter;
                volunteerCounter=other.volunteerCounter;
                orderCounter=other.orderCounter;
                for (Order* ord : other.pendingOrders){
                    pendingOrders.push_back(ord);
                };
                other.pendingOrders.clear();
                for (Order* ord : other.inProcessOrders){
                    inProcessOrders.push_back(ord);
                };
                other.inProcessOrders.clear();
                for (Order* ord : other.completedOrders){
                    completedOrders.push_back(ord);
                };
                other.completedOrders.clear();
                for (Customer* cust : other.customers){
                    customers.push_back(cust);
                };
                other.customers.clear();
                for (Volunteer* vol : other.volunteers){
                    volunteers.push_back(vol);
                };
                other.volunteers.clear();
                for (BaseAction* act : other.actionsLog){
                    actionsLog.push_back(act);
                };
                other.actionsLog.clear();
                delete &other;
                
            }
            return *this;
        };
        OrderStatus WareHouse:: getOrderStatus(int orderId){
            Order* ord=nullptr; 
            for (  Order* pendOrd :pendingOrders){
                if (pendOrd->getId()==orderId)
                    ord=pendOrd;                      
                    }
            for (  Order* inPrcOrd :inProcessOrders){
                if (inPrcOrd->getId()==orderId)
                    ord= inPrcOrd;                    
                    }
            for (  Order* compOrd :completedOrders){
                 if (compOrd->getId()==orderId)
                    ord= compOrd;                    
            }
            return ord->getStatus();
        };
        