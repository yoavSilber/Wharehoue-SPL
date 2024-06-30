#pragma once
#include <string>
#include <vector>
using namespace std;
#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <cstring>
#include <algorithm>
#include "Action.h"

class BaseAction;
class Volunteer;
// Warehouse responsible for Volunteers, Customers and Actions.

class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        const vector<BaseAction*> &getActions() const;
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer);//we added
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        void close();
        void open();
        vector<Order*> &getPendingOrders() ;//we added
        vector<Order*> &getInProcessOrders() ;//we added
        vector<Order*> &getCompletedOrders() ;//we added
        vector<Customer*> getCustomers() const;//we added
        vector<Volunteer*> getVolunteers() const;//we added
        int getCustomerCounter() const;//we added
        int getVolunteerCounter() const;//we added
        int getOrderCounter() const;//we added
        ~WareHouse();//destructor RULE OF 5
        WareHouse(const WareHouse &other);//copy constructor RULE OF 5
        WareHouse(WareHouse &&other);//move constructor RULE OF 5 steals the values from other
        WareHouse &operator=(const WareHouse &other);//copy assignment RULE OF 5  copy the value from other
        WareHouse &operator=(WareHouse &&other);//move assignment RULE OF 5 steals the values from other and delete other
        OrderStatus getOrderStatus(int orderId);//we added
    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique order IDs
};
