#include "WareHouse.h"
#include "Action.h"
using namespace std;
using std::vector;  
                                                                                                                                                                             

        BaseAction::BaseAction():errorMsg(""),status(ActionStatus::ERROR){};
        ActionStatus BaseAction::getStatus() const{
            return status;
        };
      
       string BaseAction::CustomerTypeToString(CustomerType type) const {//we added
            switch (type) {
            case CustomerType::Soldier: return "soldier";
            case CustomerType::Civilian: return "civilian";
            default: return "unknown";
            }
        };
        string BaseAction::OrderStatusToString(OrderStatus Status) const {//we added
            switch (Status) {
            case OrderStatus::PENDING: return "PENDING";
            case OrderStatus::COLLECTING: return "COLLECTING";
            case OrderStatus::DELIVERING: return "DELIVERING";
            case OrderStatus::COMPLETED: return "COMPLETED";
            default: return "unknown";
            } 
                        
        }
        string BaseAction::VolunteerTypeToString(VolunteerType type)const {//we added
            switch (type) {
            case VolunteerType::collector: return "collector";
            case VolunteerType::limited_collector: return "limited_collector";
            case VolunteerType::driver: return "driver";
            case VolunteerType::limited_driver: return "limited_driver";
            default: return "unknown";
            } 
                         
        }
      

        string BaseAction::ActionStatusToString(ActionStatus status)const { // we added
            switch (status) {
            case ActionStatus::COMPLETED: return "COMPLETED";
            case ActionStatus::ERROR: return "ERROR";
            default: return "ERROR";
            }
           
        }
        void BaseAction:: eraseElementO(vector<Order*> vec, Order* order){
            auto it = std::find(vec.begin(), vec.end(), order);
            if (it != vec.end()) {
                vec.erase(it);
            };
        };
        void BaseAction:: eraseElementV(vector<Volunteer*> vec, Volunteer* volunteer){
            auto it = std::find(vec.begin(), vec.end(), volunteer);
            if (it != vec.end()) {
                vec.erase(it);
            };
        };

        void BaseAction::complete(){
            status = ActionStatus::COMPLETED;
        };
        void BaseAction::error(std::string errorMsg){
            status = ActionStatus::ERROR;
            this->errorMsg = errorMsg;
            std::cout << "Error: " << errorMsg ;
        };
        string BaseAction:: getErrorMsg() const{
            return errorMsg;
        };


       SimulateStep:: SimulateStep(int numOfSteps):numOfSteps(numOfSteps){};
        void  SimulateStep::act(WareHouse &wareHouse) {
            for(int m=0; m<numOfSteps; m++){
                //phase 1
                for (std::vector<Order*>::size_type i=0;i<(wareHouse.getPendingOrders()).size();i++){
                    Order* order=wareHouse.getPendingOrders()[0];
                   
                    if(order->getStatus() == OrderStatus::COLLECTING){
                        
                            for(std::vector<Volunteer*>::size_type k=0;k<(wareHouse.getVolunteers()).size();k++){
                                 if(wareHouse.getVolunteers()[k]->isBusy()==false ){      
                                    if(wareHouse.getVolunteers()[k]->canTakeOrder(*order)){
                                      if ((typeid(*(wareHouse.getVolunteers()[k])) == typeid(DriverVolunteer)) || (typeid(*(wareHouse.getVolunteers()[k])) == typeid(LimitedCollectorVolunteer) )){
                                            wareHouse.getVolunteers()[k]->acceptOrder(*order);
                                            order->setDriverId(wareHouse.getVolunteers()[k]->getId());
                                            wareHouse.getInProcessOrders().push_back(order);
                                            auto it = std::find(wareHouse.getPendingOrders().begin(), wareHouse.getPendingOrders().end(), order);
                                            if (it != wareHouse.getPendingOrders().end()) {
                                                wareHouse.getPendingOrders().erase(it);  
                                            };
                                            order->setStatus(OrderStatus::DELIVERING);
                                            k--; 
                                        } 
                                           
                                    }
                                  
                                }
                               
                            }
                    }                          
                    else if (order->getStatus() == OrderStatus::PENDING){
                        
                         for(std::vector<Volunteer*>::size_type j=0;j<(wareHouse.getVolunteers()).size();j++){
                             if(wareHouse.getVolunteers()[j]->isBusy()==false ){ 
                                    if (typeid(*wareHouse.getVolunteers()[j]) == typeid(CollectorVolunteer) || typeid(*wareHouse.getVolunteers()[j]) == typeid(LimitedCollectorVolunteer)){
                                        if(wareHouse.getVolunteers()[j]->canTakeOrder(*order)){
                                            wareHouse.getVolunteers()[j]->acceptOrder(*order);
                                            order->setCollectorId(wareHouse.getVolunteers()[j]->getId());
                                            wareHouse.getInProcessOrders().push_back(order);
                                            auto it = std::find(wareHouse.getPendingOrders().begin(), wareHouse.getPendingOrders().end(), order);
                                            if (it != wareHouse.getPendingOrders().end()) {
                                              wareHouse.getPendingOrders().erase(it);
                                            };
                                            order->setStatus(OrderStatus::COLLECTING);
                                            i--;
                                            break;
                                        }
                                    } 
                             }                               
                         }   
                        
                    }    
                  
                }
                //phase 2
                
                    for(std::vector<Volunteer*>::size_type i=0 ;i<(wareHouse.getVolunteers()).size();i++){
                        wareHouse.getVolunteers()[i]->step();
                    }
                
                //phase 3 -check for completed orders
                for (std::vector<Order*>::size_type i=0;i<(wareHouse.getInProcessOrders()).size();i++){
                    if(wareHouse.getInProcessOrders()[i]->getStatus() ==OrderStatus::COLLECTING && (wareHouse.getVolunteer(wareHouse.getInProcessOrders()[i]->getCollectorId())).getActiveOrderId()==NO_ORDER){
                        wareHouse.getPendingOrders().push_back(wareHouse.getInProcessOrders()[i]);
                        auto it = std::find(wareHouse.getInProcessOrders().begin(), wareHouse.getInProcessOrders().end(), wareHouse.getInProcessOrders()[i]);
                            if (it != wareHouse.getInProcessOrders().end()) {
                                wareHouse.getInProcessOrders().erase(it);
                                i--;
                            };
                            

                    }
                    else if (wareHouse.getInProcessOrders()[i]->getStatus() ==OrderStatus::DELIVERING && (wareHouse.getVolunteer(wareHouse.getInProcessOrders()[i]->getDriverId())).getActiveOrderId()==NO_ORDER){      
                
                        for(std::vector<Volunteer*>::size_type i=0;i<(wareHouse.getVolunteers()).size();i++){
                            if((wareHouse.getVolunteers()[i]->isBusy()==false)&&( wareHouse.getVolunteers()[i]->getCompletedOrderId()!=NO_ORDER )){
                                if(typeid(*wareHouse.getVolunteers()[i]) == typeid(DriverVolunteer)||typeid(*wareHouse.getVolunteers()[i]) == typeid(LimitedDriverVolunteer)){
                                    Order& order1=wareHouse.getOrder(wareHouse.getVolunteers()[i]->getCompletedOrderId());
                                    if (order1.getStatus()==OrderStatus::DELIVERING){
                                        wareHouse.getCompletedOrders().push_back(&order1);
                                        auto it = std::find(wareHouse.getInProcessOrders().begin(), wareHouse.getInProcessOrders().end(), &order1);
                                        if (it != wareHouse.getInProcessOrders().end()) {
                                            wareHouse.getInProcessOrders().erase(it);
                                            
                                        };
                            
                                    wareHouse.getOrder(wareHouse.getVolunteers()[i]->getCompletedOrderId()).setStatus(OrderStatus::COMPLETED);
                                    wareHouse.getVolunteers()[i]->resetCompletedOrderId(); 
                                     i--;
                                }
                                }
                            }
                            
                        }
                    }
                }
                 //phase 4-delete volunteers with no orders left
                 for(std::vector<Volunteer*>::size_type i=0;i<(wareHouse.getVolunteers()).size();i++){
                    if(typeid(wareHouse.getVolunteers()[i]) == typeid(LimitedCollectorVolunteer)||typeid(wareHouse.getVolunteers()[i]) == typeid(LimitedDriverVolunteer)){
                        if(wareHouse.getVolunteers()[i]->hasOrdersLeft()==false&& wareHouse.getVolunteers()[i]->getActiveOrderId()==NO_ORDER){
                            auto it = std::find(wareHouse.getVolunteers().begin(), wareHouse.getVolunteers().end(), wareHouse.getVolunteers()[i]);
                                    if (it != wareHouse.getVolunteers().end()) {
                                        wareHouse.getVolunteers().erase(it);
                                        delete wareHouse.getVolunteers()[i];
                                        i--;
                                    };
                           
                        }
                    }
                }
                 
            for(Order* order : wareHouse.getCompletedOrders()){
                order->setStatus(OrderStatus::COMPLETED);
            }     
            
            
        };
            wareHouse.addAction(this);
            complete();
        }
        string SimulateStep:: toString() const {
            return "SimulateStep " + std::to_string(numOfSteps) + " " + ActionStatusToString(getStatus());
        };
        SimulateStep * SimulateStep::clone() const {
            return new SimulateStep(*this);
        };

        AddOrder::AddOrder(int id): customerId(id){};
        void  AddOrder:: act(WareHouse &wareHouse) {
           bool found=false;
           for(Customer* customer : wareHouse.getCustomers()){
                if(customer->getId() == customerId ){
                    if(customer->canMakeOrder()==false){
                        error("Cannot place this order");
                        wareHouse.addAction(this);
                        found=true;
                        break; 
                    }
                    else{
                        Order* order = new Order(wareHouse.getOrderCounter(), customerId, wareHouse.getCustomer(customerId).getCustomerDistance());//***
                        wareHouse.getCustomer(order->getCustomerId()).deacreaseOrdersLeft();
                        wareHouse.addOrder(order);
                        wareHouse.getCustomer(order->getCustomerId()).addOrderId(order->getId());
                        wareHouse.addAction(this);
                        complete();
                        found=true;
                        break;
                    }
                }
           }            
              if (found==false){
                        error("Customer doesnt exist");
                        wareHouse.addAction(this);
              }
        };
        AddOrder* AddOrder::clone() const {
              return new AddOrder(*this);
        };
        string AddOrder:: toString() const {
            return "Order " + std::to_string(customerId) + " " + ActionStatusToString(getStatus());
        };
  
        AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders):
            customerName(customerName),
            customerType(customerType.compare("soldier") == 0 ? CustomerType::Soldier : CustomerType::Civilian),
            distance(distance),
            maxOrders(maxOrders)
        {};     
        void AddCustomer:: act(WareHouse &wareHouse) {
          Customer* customer=nullptr;  
          if(customerType == CustomerType:: Soldier)  
                 customer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName,  distance, maxOrders); //***
    
          else   customer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName,distance, maxOrders);//***
          
          wareHouse.addCustomer(customer);
          wareHouse.addAction(this);
          complete();  
        };
        AddCustomer* AddCustomer:: clone() const {
            return new AddCustomer(*this);
        };
        string  AddCustomer:: toString() const {
            return "customer " + 
            customerName + " " +
            CustomerTypeToString(customerType) + " " +
            std::to_string(distance) + " " +
            std::to_string(maxOrders) + " " +
            ActionStatusToString(getStatus());                                           
        };

        PrintOrderStatus::PrintOrderStatus(int id): orderId(id){};

        void PrintOrderStatus:: act(WareHouse &wareHouse) {
            if(orderId>wareHouse.getOrderCounter()-1 || orderId<0){
                wareHouse.addAction(this);
                error("Order doesnt  exist");
            }
            else{
                std::cout << "order ID: " + std::to_string(orderId) << std::endl;
                std::cout <<"status: " + OrderStatusToString(wareHouse.getOrder(orderId).getStatus())<<std::endl;
                std::cout << "customerID: " +to_string(wareHouse.getOrder(orderId).getCustomerId())<<std::endl;
                if(wareHouse.getOrder(orderId).getCollectorId()==NO_VOLUNTEER)
                    std::cout << "collector: NONE" << std::endl;
                else std::cout << "collector: " + to_string(wareHouse.getOrder(orderId).getCollectorId())<<std::endl;
                if(wareHouse.getOrder(orderId).getDriverId()==NO_VOLUNTEER)
                    std::cout << "driver:  NONE" << std::endl;
                else std::cout << "driver: " + to_string(wareHouse.getOrder(orderId).getDriverId())<<std::endl;
                wareHouse.addAction(this);
                complete();
            }
            
        };
        PrintOrderStatus* PrintOrderStatus::clone() const {
            return new PrintOrderStatus(*this);
        };
        string PrintOrderStatus:: toString() const {
            return "orderStatus " +to_string(orderId)+ "   " + ActionStatusToString(getStatus());
        };


        PrintCustomerStatus::PrintCustomerStatus(int customerId): customerId(customerId){};
        
        void PrintCustomerStatus:: act(WareHouse &wareHouse) {
            if(customerId>wareHouse.getCustomerCounter()-1 || customerId<0){
                error("Customer doesnt  exist") ;
            }
            else {
                std::cout << "CustomerID: " << customerId <<std::endl; 
                for(int orderId: wareHouse.getCustomer(customerId).getOrdersIds()){
                    std::cout << "OrderID: "<<orderId << " " << OrderStatusToString(wareHouse.getOrder(orderId).getStatus()) << " ";   
                }
                std::cout << "num orders left: " << std::to_string( wareHouse.getCustomer(customerId).getOrdersLeft() )<< std::endl;
                complete();
            }
            wareHouse.addAction(this);
            
            };
        
        PrintCustomerStatus* PrintCustomerStatus::clone() const {
            return new PrintCustomerStatus(*this);
        };
        string  PrintCustomerStatus::toString() const {
            return "customerStatus " +to_string(customerId)+ "  " + ActionStatusToString(getStatus());
        };
 


        PrintVolunteerStatus::PrintVolunteerStatus(int id):VolunteerId(id){};
        void  PrintVolunteerStatus::act(WareHouse &wareHouse) {
            bool found=false;
            for(Volunteer* volunteer : wareHouse.getVolunteers()){
                if(volunteer->getId() == VolunteerId){
                    std::cout << volunteer->toString() << std::endl;
                    found=true;
                    complete();
                    wareHouse.addAction(this);
                    break;   
                };
            };
            if (found==false){
                error("Volunteer doesnt exist");
                wareHouse.addAction(this); 
            }
        };    
        PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
            return new PrintVolunteerStatus(*this);
        };
        
        string PrintVolunteerStatus:: toString() const {
            return "VolunteerStatus " +to_string(VolunteerId)+ "   " + ActionStatusToString(getStatus());
        };

        PrintActionsLog::PrintActionsLog(){};
        void  PrintActionsLog::act(WareHouse &wareHouse) {
            for(BaseAction* action : wareHouse.getActions()){
                std::cout << action->toString() << std::endl;
            }
            wareHouse.addAction(this);
            complete();
        };
        PrintActionsLog* PrintActionsLog::clone() const {
            return new PrintActionsLog(*this);
        };
        string PrintActionsLog::toString() const {
            return "PrintActionsLog " + ActionStatusToString(getStatus());  
        };

        Close::Close(){};
        void  Close:: act(WareHouse &wareHouse){
            for(Order* order : wareHouse.getPendingOrders()){
                std::cout<< order->toString()+" "+ std::to_string (order->getCustomerId())+" "+ OrderStatusToString(order->getStatus())<<std::endl;
            }
            for(Order* order : wareHouse.getInProcessOrders()){
                std::cout<< order->toString()+" "+ std::to_string (order->getCustomerId())+" "+ OrderStatusToString(order->getStatus())<<std::endl;
            }
            for(Order* order : wareHouse.getCompletedOrders()){
                std::cout<< order->toString()+" " + std::to_string (order->getCustomerId())+" "+ OrderStatusToString(order->getStatus())<<std::endl;
            }
            wareHouse.addAction(this);
            wareHouse.close();       
        };
        Close* Close::clone() const {
            return new Close(*this);
        }
        string  Close:: toString() const {
            return "Close " + ActionStatusToString(getStatus());
        };
        BackupWareHouse::BackupWareHouse(){};
        void BackupWareHouse::act(WareHouse &wareHouse){
            if (backup == nullptr){
                backup = new WareHouse(wareHouse);
            }
            else{                
                *backup =  wareHouse;
            }
            
            complete();
        };
         
        BackupWareHouse *BackupWareHouse::clone() const {
            return new BackupWareHouse(*this);
        };
        string BackupWareHouse::toString() const {
            return "backup " + ActionStatusToString(getStatus());
        };


        RestoreWareHouse::RestoreWareHouse(){};
        void RestoreWareHouse::act(WareHouse &wareHouse){
             if(backup == nullptr){
                error("No backup available");
            }
            else {
                wareHouse = *backup;      
                complete();
            }
            
        };
        RestoreWareHouse *RestoreWareHouse::clone() const{
            return new RestoreWareHouse(*this);
        };
        string RestoreWareHouse::toString() const {
            return "Restore " + ActionStatusToString(getStatus());
        };
       