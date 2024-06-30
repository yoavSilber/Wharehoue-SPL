#include "Order.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

        Order::Order(int id, int customerId, int distance):
        id(id),customerId(customerId),distance(distance),status(OrderStatus::PENDING),collectorId(NO_VOLUNTEER),driverId(NO_VOLUNTEER)
        {};
        
        int Order:: getId() const{
            return id;
        };
        int Order:: getCustomerId() const{
            return customerId;
        };
        void  Order::setStatus(OrderStatus status){
            this->status=status;
        };
        void  Order::setCollectorId(int collectorId){
            this->collectorId=collectorId;
        };
        void  Order::setDriverId(int driverId){;
            this->driverId=driverId;
        };
        int  Order::getCollectorId() const{
            return collectorId;
        };
        int  Order::getDriverId() const{
            return driverId;
        };
        int Order:: getDistance() const{//we added
            return distance;
        };
        OrderStatus  Order:: getStatus() const{
            return status;
        };
        
        const string  Order::toString() const{
            return "Order "+ std::to_string(id); 
        };
        Order*  Order:: Clone() const {//we added
            return new Order(*this);
        }; 
