#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

#define NO_VOLUNTEER -1
class Volunteer;

enum class OrderStatus {
    PENDING,
    COLLECTING,
    DELIVERING,
    COMPLETED,
};


class Order {

    public:
        Order(int id, int customerId, int distance);
        int getId() const;
        int getCustomerId() const;
        void setStatus(OrderStatus status);
        void setCollectorId(int collectorId);
        void setDriverId(int driverId);
        int getCollectorId() const;
        int getDriverId() const;
        int getDistance() const ;//WE added
        OrderStatus getStatus() const;
        const string toString() const;
        virtual ~Order(){};
        Order* Clone() const ; //we added
    private:
        const int id;
        const int customerId;
        const int distance;
        OrderStatus status;
        int collectorId; //Initialized to NO_VOLUNTEER if no collector has been assigned yet
        int driverId; //Initialized to NO_VOLUNTEER if no driver has been assigned yet
};