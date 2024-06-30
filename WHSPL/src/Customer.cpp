
#include <string>
#include <vector>
#include "Customer.h"
using std::string;
using std::vector;


Customer::Customer(int id, const string &name, int locationDistance, int maxOrders)
    : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId(),ordersLeft(maxOrders) {};

const string& Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

int Customer::getCustomerDistance() const {
    return locationDistance;
}

int Customer::getMaxOrders() const {
    return maxOrders;
}

int Customer::getNumOrders() const {
    return  ordersId.size();
}

bool Customer::canMakeOrder() const {
    return ordersLeft != 0 ;
}

const vector<int>& Customer::getOrdersIds() const {
    return ordersId;
}

int Customer::addOrder(int orderId) {
    for(int num : ordersId) {
        if(num == orderId) {
            return -1;
        }
    }
    ordersId.push_back(orderId);
    return orderId;
}
void Customer:: deacreaseOrdersLeft()  {
     ordersLeft-- ;
};//we added
int Customer:: getOrdersLeft() const {
    return ordersLeft;
};//we added
void Customer::addOrderId(int orderId) {//we added
    ordersId.push_back(orderId);
}
// Customer::clone() is a pure virtual function and should not be defined in Customer.cpp

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {};

SoldierCustomer* SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
};
const string  SoldierCustomer::toString() const {
   return "customer "+ getName()+ " soldier " + std::to_string(getCustomerDistance())+" "+ std::to_string(getMaxOrders());  
};//we added

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

CivilianCustomer* CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}
const string  CivilianCustomer::toString() const {
     return "customer "+ getName()+ " civilian " + std::to_string(getCustomerDistance())+" "+ std::to_string(getMaxOrders());
};//we added