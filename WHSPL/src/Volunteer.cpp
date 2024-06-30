#include <string>
#include <vector>
#include "Order.h"
#include "Volunteer.h"
using std::string;
using std::vector;
#define NO_ORDER -1

        Volunteer::Volunteer(int id, const string &name):
         type(Volunteertype::volunteer),completedOrderId(NO_ORDER),activeOrderId(NO_ORDER),id(id),name(name){};

        int Volunteer:: getId() const{
            return id;
        };
        const string& Volunteer:: getName() const{
            return name;
        };

        int  Volunteer:: getActiveOrderId() const{
            return activeOrderId;
        };
        int Volunteer:: getCompletedOrderId() const{
            return completedOrderId;
        };
        bool Volunteer:: isBusy() const{; // Signal whether the volunteer is currently processing an order    
            if (activeOrderId!=NO_ORDER){
                return true;
            }
            else return false;
        };
        void Volunteer::resetCompletedOrderId() {
             completedOrderId = NO_ORDER;
        };

        CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown):
            Volunteer(id,name),coolDown(coolDown),timeLeft(coolDown){type = Volunteertype::collector;};
        
        CollectorVolunteer* CollectorVolunteer:: clone() const {
            return new CollectorVolunteer(*this);
        };
        
        void CollectorVolunteer:: step() {
            if(decreaseCoolDown()){
                
                activeOrderId=NO_ORDER;
            }
           
        };
        int CollectorVolunteer:: getCoolDown() const{
            return coolDown;
        };
        int CollectorVolunteer::getTimeLeft() const {
            return timeLeft;
        }

        void CollectorVolunteer::setTimeLeft(int time) {
            timeLeft = time;
        }

        bool CollectorVolunteer::decreaseCoolDown() {
            timeLeft--;
            if (timeLeft == 0){
                completedOrderId=activeOrderId;
                activeOrderId=NO_ORDER;
                return true;
            }
            else
                return false;
        }

        bool CollectorVolunteer::hasOrdersLeft() const {
            return true;
        }
        bool CollectorVolunteer:: canTakeOrder(const Order &order) const {
            return !isBusy();
        };
        void  CollectorVolunteer:: acceptOrder(const Order &order) {
        // Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
            activeOrderId=order.getId();
            timeLeft=coolDown;
        };
        string  CollectorVolunteer:: toString() const {
            if (isBusy()){
                return "Volunteer ID " + std::to_string(getId()) + " "+ "is busy: TRUE" +" OrderID: "+std::to_string(getActiveOrderId())+ " time left: "+std::to_string(getTimeLeft())+ " OrdersLeft : NO LIMIT";
            }
            return "Volunteer ID " + std::to_string(getId()) + " "+ "is busy: FALSE" +" OrderID: NONE"+ " time left: "+std::to_string(getTimeLeft())+ " OrdersLeft : NO LIMIT";
        };
   
        LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders):
            CollectorVolunteer(id,name,coolDown),maxOrders(maxOrders),ordersLeft(maxOrders){type = Volunteertype::limited_collector;};

        LimitedCollectorVolunteer* LimitedCollectorVolunteer:: clone() const {
            return new LimitedCollectorVolunteer(*this);
        };
        bool LimitedCollectorVolunteer::  hasOrdersLeft() const {
            if (ordersLeft==0){   
                return false;
            }
            else return true;
        };
        bool LimitedCollectorVolunteer:: canTakeOrder(const Order &order) const {    
            if (getTimeLeft()==getCoolDown() && ordersLeft>0 )
                return true;
            else return false;      
        };

        void  LimitedCollectorVolunteer:: acceptOrder(const Order &order) {
 // Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
            activeOrderId=order.getId();
            setTimeLeft(getCoolDown());
            ordersLeft--;
            
        };
        int LimitedCollectorVolunteer::  getMaxOrders() const{
            return maxOrders;
        };
        int LimitedCollectorVolunteer:: getNumOrdersLeft() const{
            return ordersLeft;
        };
        string LimitedCollectorVolunteer:: toString() const {
           if (isBusy()){
                return "Volunteer ID " + std::to_string(getId()) + " "+ "is busy: TRUE" +" OrderID: "+std::to_string(getActiveOrderId())+ " time left: "+std::to_string(getTimeLeft())+" OrdersLeft:" + std::to_string(getNumOrdersLeft());
            }
           return "Volunteer ID " + std::to_string(getId()) + " "+ "is busy: FALSE"+" OrderID: NONE"+ " time left: "+std::to_string(getTimeLeft())+" OrdersLeft:" + std::to_string(getNumOrdersLeft());
        };
    
        DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep):
             Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep),distanceLeft(maxDistance){type = Volunteertype::driver;};
        DriverVolunteer* DriverVolunteer::clone() const {
            return new DriverVolunteer(*this);
        }
        int  DriverVolunteer:: getDistanceLeft() const{
            return distanceLeft;
        };
        int DriverVolunteer:: getMaxDistance() const{
            return maxDistance;
        };
        int DriverVolunteer:: getDistancePerStep() const{
           return distancePerStep;
        };  
        bool DriverVolunteer:: decreaseDistanceLeft(){
            distanceLeft = distanceLeft - distancePerStep;
            if(distanceLeft <=0){
                return true;
            }
            return false;
        }; //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
        bool DriverVolunteer::  hasOrdersLeft() const {
            return true;
        };
        bool DriverVolunteer::  canTakeOrder(const Order & order) const {
            if(order.getDistance() <= maxDistance && !isBusy()){
                return true;
            }
            return false;
        }; // Signal if the volunteer is not busy and the order is within the maxDistance
        void  DriverVolunteer:: acceptOrder(const Order &order) {  
            distanceLeft = order.getDistance();
            activeOrderId = order.getId();
        }; // Reset distanceLeft to maxDistance
        void DriverVolunteer::  step() {
            if (isBusy()){
                distanceLeft = distanceLeft-distancePerStep;
                if(distanceLeft <=0){
                    distanceLeft=0;
                    completedOrderId=activeOrderId;
                    activeOrderId=NO_ORDER;
                }
            }
        }; // Decrease distanceLeft by distancePerStep
        string DriverVolunteer::  toString() const {
            if(isBusy()){
                return "Volunteer ID " + std::to_string(getId()) + " "+ "is busy: TRUE" +" OrderID: "+std::to_string(getActiveOrderId())+ " distance left: "+std::to_string(getDistanceLeft())+ " OrdersLeft : NO LIMIT";
            };
            return "Volunteer ID " + std::to_string(getId()) + " "+ "is busy: FALSE" +" OrderID: NONE"+ " distance left: "+std::to_string(getDistanceLeft())+ " OrdersLeft : NO LIMIT";
        };
        LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
            DriverVolunteer(id,name,maxDistance,distancePerStep),maxOrders(maxOrders),ordersLeft(maxOrders){type = Volunteertype::limited_driver;};

        LimitedDriverVolunteer* LimitedDriverVolunteer:: clone() const {
            return new LimitedDriverVolunteer(*this);
        };
        int LimitedDriverVolunteer:: getMaxOrders() const{
            return maxOrders;
        };
        
        int  LimitedDriverVolunteer::getNumOrdersLeft() const{
            return ordersLeft;
        };
        bool LimitedDriverVolunteer:: hasOrdersLeft() const {
            if(ordersLeft==0)
                return false;
            return true;
        };
        bool LimitedDriverVolunteer:: canTakeOrder(const Order &order) const { // Signal if the volunteer is not busy, the order is within the maxDistance and have orders left
            if (hasOrdersLeft()&&
                order.getDistance()<=getMaxDistance()&&
                !isBusy()){
                    return true;
                }
            else return false; 
        };
        void LimitedDriverVolunteer::  acceptOrder(const Order &order) { // Reset distanceLeft to maxDistance and decrease ordersLeft
            DriverVolunteer:: acceptOrder(order);
            ordersLeft--;
        };
        string LimitedDriverVolunteer:: toString() const {  
            if(isBusy()){ 
           return "Volunteer ID " + std::to_string(getId()) + " "+ "is busy: TRUE" +" OrderID: "+std::to_string(getActiveOrderId())+ " distance left: "+std::to_string(getDistanceLeft())+" OrdersLeft: " + std::to_string(getNumOrdersLeft());
        };
         return "Volunteer ID " + std::to_string(getId()) + " "+ "is busy: FALSE" +" OrderID: NONE"
         + " distance left: "+std::to_string(getDistanceLeft())+" OrdersLeft: " + std::to_string(getNumOrdersLeft());
        };
        