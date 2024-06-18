#include <iostream>
#include <map>
#include <vector>
#include<queue>
#include <cassert>

using namespace std;

class Vehicle;

class Trip {
public:
    Trip(Vehicle* vehicle, std::string pick_up_location, std::string drop_location, int departure_time)
        : vehicle(vehicle), pick_up_location(pick_up_location), drop_location(drop_location), departure_time(departure_time), booked_seats(0) {}

    // Getter functions
    Vehicle* getVehicle() const {
        return vehicle;
    }

    std::string getPickUpLocation() const {
        return pick_up_location;
    }

    std::string getDropLocation() const {
        return drop_location;
    }

    int getDepartureTime() const {
        return departure_time;
    }

    int getBookedSeats() const {
        return booked_seats;
    }

    // Setter functions
    void setVehicle(Vehicle* v) {
        vehicle = v;
    }

    void setPickUpLocation(std::string location) {
        pick_up_location = location;
    }

    void setDropLocation(std::string location) {
        drop_location = location;
    }

    void setDepartureTime(int time) {
        departure_time = time;
    }

    void setBookedSeats(int seats) {
        booked_seats = seats;
    }

private:
    Vehicle* vehicle;
    std::string pick_up_location;
    std::string drop_location;
    int departure_time;
    int booked_seats;
};

class BinaryTreeNode {
private:
    BinaryTreeNode* leftptr;
    BinaryTreeNode* rightptr;
    BinaryTreeNode* parentptr;
    int departuretime;
    Trip* tripnodeptr;
public:
    BinaryTreeNode(int departuretime = 0, Trip* tripenodeptr = nullptr, BinaryTreeNode* parentptr = nullptr)
        : leftptr(nullptr), rightptr(nullptr), parentptr(parentptr), departuretime(departuretime), tripnodeptr(tripenodeptr) {}

    // Getter functions
    BinaryTreeNode* getLeftPtr() const {
        return leftptr;
    }

    BinaryTreeNode* getRightPtr() const {
        return rightptr;
    }

    BinaryTreeNode* getParentPtr() const {
        return parentptr;
    }

    int getDepartureTime() const {
        return departuretime;
    }

    Trip* getTripNodePtr() const {
        return tripnodeptr;
    }

    // Setter functions
    void setLeftPtr(BinaryTreeNode* left) {
        leftptr = left;
    }

    void setRightPtr(BinaryTreeNode* right) {
        rightptr = right;
    }

    void setParentPtr(BinaryTreeNode* parent) {
        parentptr = parent;
    }

    void setDepartureTime(int time) {
        departuretime = time;
    }

    void setTripNodePtr(Trip* trip) {
        tripnodeptr = trip;
    }

    
};

class TransportService {
public:
// TransportService(std::string name) : name(name), BSThead(nullptr) {}
    TransportService(std::string name) : name(name), BSThead(nullptr) {}

    // Getter functions
    std::string getName() const {
        return name;
    }

    BinaryTreeNode* getBSTHead() const {
        return BSThead;
    }

    // Setter functions
    void setName(std::string service_name) {
        name = service_name;
    }

    void setBSTHead(BinaryTreeNode* node) {
        BSThead = node;
    }

    void addTrip(int key, Trip* trip) {
    // Create a new node for the trip
    BinaryTreeNode* newNode = new BinaryTreeNode;
    newNode->setDepartureTime(key);
    newNode->setTripNodePtr(trip);
    newNode->setLeftPtr(nullptr);
    newNode->setRightPtr(nullptr);

    // If the BST is empty, set the new node as the head
    if (getBSTHead() == nullptr) {
        setBSTHead(newNode);
    } 
    else {
        BinaryTreeNode* current = getBSTHead();
        BinaryTreeNode* parent = nullptr;
        while (current != nullptr) {
            parent = current;
            // Compare the new departure time with the current node's departure time
            if (key <= current->getDepartureTime()) {
                current = current->getLeftPtr();
                // If the left child is null, insert the new node here
                if (current == nullptr) {
                    parent->setLeftPtr(newNode);
                    newNode->setParentPtr(parent); // Set the parent node for the new node
                    return;
                }
            } 
            else {
                current = current->getRightPtr();
                // If the right child is null, insert the new node here
                if (current == nullptr) {
                    parent->setRightPtr(newNode);
                    newNode->setParentPtr(parent); // Set the parent node for the new node
                    return;
                }
            }
        }
    }
}

private:
    std::string name;
    BinaryTreeNode* BSThead;
};


class Vehicle {
private:
    std::string vehicle_number;
    int seating_capacity;
    std::vector<Trip*> trips;
public:
    Vehicle(std::string vehicle_number, int seating_capacity)
        : vehicle_number(vehicle_number), seating_capacity(seating_capacity) {}

    // Getter functions
    std::string getVehicleNumber() const {
        return vehicle_number;
    }

    int getSeatingCapacity() const {
        return seating_capacity;
    }

    vector<Trip*> getTrips(){
        return trips;
    }

    // Setter functions
    void setVehicleNumber(std::string number) {
        vehicle_number = number;
    }

    void setSeatingCapacity(int capacity) {
        seating_capacity = capacity;
    }

    void addTrip(Trip* trip) {
        trips.push_back(trip);
    }
};

class Location {
private:
    std::string name;
    std::vector<TransportService*>serviceptrs;
    std::vector<Trip*> trips;
public:
    Location(std::string name) : name(name) {}

    // Getter functions
    std::string getName() const {
        return name;
    }

    vector<Trip*> getTrips(){
        return trips;
    }

    TransportService* getServicePtr(std::string droplocation) const {
        int i;
        for(i=0;i<serviceptrs.size();i++){
            if(serviceptrs[i]->getName()==droplocation){
                return serviceptrs[i];
            }
        }
        // return nullptr;
    }

    // Setter functions
    void setName(std::string location_name) {
        name = location_name;
    }

    void setServicePtr(std::string droplocation) {
    TransportService* result = new TransportService(droplocation);
        serviceptrs.push_back(result);
    // return result; // Return the result whether it's found or created
}
    void addTrip(Trip* trip) {
        trips.push_back(trip);
    }
};

class BinaryTree {
protected:
    BinaryTreeNode* root;
public:
    BinaryTree() : root(nullptr) {}

    int getHeight() const {
    return getHeightRecursive(root);
    }
    int getHeightRecursive(BinaryTreeNode* node) const {
        if (node == nullptr) {
            return -1; // Height of an empty tree is -1.
        }
        int leftHeight = getHeightRecursive(node->getLeftPtr());
        int rightHeight = getHeightRecursive(node->getRightPtr());
        // Return the maximum height of the left and right subtrees, plus 1 for the current node.
        return std::max(leftHeight, rightHeight) + 1;
    }

    int getNumberOfNodes() const {
        // return the no of nodes in the tree
        int no=0;
        queue<BinaryTreeNode*> nodes;
        BinaryTreeNode* temp = root;
        nodes.push(temp);
        while(!nodes.empty()){
            BinaryTreeNode* current = nodes.front();
            nodes.pop();
            no++;
            if(current->getLeftPtr()!=nullptr)
            nodes.push(current->getLeftPtr());
            if(current->getRightPtr()!=nullptr)
            nodes.push(current->getRightPtr());
        }
        return no;
    }
};

class BinarySearchTree : public BinaryTree {
public:
    BinarySearchTree() {}

    BinaryTreeNode* getElementWithMinimumKey() const {
        BinaryTreeNode* temp = root;
        while(temp->getLeftPtr()!=nullptr){
            temp = temp->getLeftPtr();
        }
        return temp;
    }

    BinaryTreeNode* getElementWithMaximumKey() const {
        BinaryTreeNode* temp = root;
        while(temp->getRightPtr()!=nullptr){
            temp = temp->getRightPtr();
        }
        return temp;
    }

    BinaryTreeNode* searchNodeWithKey(int key) const {
        // Implement this function to search for a node with the given key or the next larger key
        BinaryTreeNode* temp = root;
        BinaryTreeNode* result = nullptr;
        while(temp!=nullptr){
            if(key==temp->getDepartureTime()){
                return temp;
            }
            else if(key < temp->getDepartureTime()) {
                result = temp;
                temp = temp->getLeftPtr();
            }
            else {
                result = temp;
                temp = temp->getRightPtr();
            }
        }
        return result;
    }

    vector<BinaryTreeNode*> inorder(BinaryTreeNode* node) const {
        vector<BinaryTreeNode*> result;
        if (node == nullptr) {
            return result;
        }
        vector<BinaryTreeNode*> leftSubtree = inorder(node->getLeftPtr());
        result.insert(result.end(), leftSubtree.begin(), leftSubtree.end());
        result.push_back(node);
        vector<BinaryTreeNode*> rightSubtree = inorder(node->getRightPtr());
        result.insert(result.end(), rightSubtree.begin(), rightSubtree.end());
        return result;
    }

    BinaryTreeNode* getSuccessorNode(BinaryTreeNode* node) const {
        // Implement this function to return the successor node of the given node
        vector<BinaryTreeNode*> result = inorder(root);
        BinaryTreeNode* successor;
        for(int i=0;i<result.size();i++){
            if(result[i]==node){
                successor = result[i+1];
                break;
            }
        }
        return successor;
    }

    BinaryTreeNode* getSuccessorNodeByKey(int key) const {
        // Implement this function to return the successor node by key
        BinaryTreeNode* node = searchNodeWithKey(key);
        BinaryTreeNode* s = getSuccessorNode(node);
        return s;
    }

    BinaryTreeNode* getPredecessorNode(BinaryTreeNode* node) const {
        // Implement this function to return the predecessor node of the given node
        vector<BinaryTreeNode*> result = inorder(root);
        BinaryTreeNode* pre;
        for(int i=0;i<result.size();i++){
            if(result[i]==node){
                pre = result[i-1];
                break;
            }
        }
        return pre;
    }

    BinaryTreeNode* getPredecessorNodeByKey(int key) const {
        // Implement this function to return the predecessor node by key
        BinaryTreeNode* node = searchNodeWithKey(key);
        BinaryTreeNode* p = getPredecessorNode(node);
        return p;
    }
};

class TravelDesk {
private:
    std::vector<Vehicle*> vehicles;
    std::vector<Location*> locations;
public:
    void add_trip(std::string vehicle_number, int seating_capacity, std::string pick_up_location, std::string drop_location, int departure_time) {
    // Check if the vehicle already exists, if not, create a new one with the seating capacity
    // cout<<"in it";
    Vehicle* vehicle = nullptr;
    int i, flag = 0, j, jf = 0;
    for (i = 0; i < vehicles.size(); i++) {
        if (vehicles[i]->getVehicleNumber() == vehicle_number) {
            flag = 1;
            vehicle = vehicles[i];
            break;
        }
    }
    if (flag == 0) {
        vehicle = new Vehicle(vehicle_number, seating_capacity);
        vehicles.push_back(vehicle);
    }

    // cout<<vehicle->getVehicleNumber()<<" ";

    // Create a new trip and add it to the appropriate objects
    Trip* newtrip = new Trip(vehicle,pick_up_location,drop_location,departure_time);
    vehicle->addTrip(newtrip);
    // cout<<newtrip->getPickUpLocation()<<" ";

    // Find or create the Location object for the pick-up location
    Location* location = nullptr;
    for (j = 0; j < locations.size(); j++) {
        if (pick_up_location == locations[j]->getName()) {
            jf = 1;
            location = locations[j];
            location->addTrip(newtrip);
            break;
        }
    }
    TransportService* serviceobj = nullptr;
    if (jf == 0) {
        // Create a new TransportService if it doesn't exist
        location = new Location(pick_up_location);
        location->addTrip(newtrip);
        serviceobj = new TransportService(drop_location); // Constructor call here
        locations.push_back(location);
        location->setServicePtr(drop_location);
    } else {
        // Retrieve the existing TransportService associated with the drop_location
        serviceobj = location->getServicePtr(drop_location);
    }
    // cout<<serviceobj->getName()<<" ";

    // Add the trip directly to the TransportService's BST
    serviceobj->addTrip(departure_time, newtrip);
    // cout<<"Added!"<<endl;
}

    std::vector<Trip*> show_trips(std::string pick_up_location, int after_time, int before_time) {
    int i; vector<Trip*> result;
    Location* loc = nullptr;
    for(i=0;i<locations.size();i++){
        if(locations[i]->getName()==pick_up_location){
            loc = locations[i];
            break;
        }
    }
    for(i=0;i<loc->getTrips().size();i++){
        if(loc->getTrips()[i]->getDepartureTime()>=after_time && loc->getTrips()[i]->getDepartureTime()<before_time)     
        result.push_back(loc->getTrips()[i]);
    }
    return result;
    }

    Trip* book_trip(std::string pick_up_location, std::string drop_location, std::string vehicle_number, int departure_time) {
    // Find the Location object for the pick-up location
    Vehicle* vehicle = nullptr;
    for (Vehicle* vec : vehicles) {
        if (vec->getVehicleNumber() == vehicle_number) {  // Use vec here, not vehicle
            vehicle = vec;
            break;
        }
    }
    if (vehicle == nullptr) {
        // Handle the case where the vehicle is not found
        return nullptr;
    }
    Trip* result = nullptr; // Initialize result to nullptr
    for (int i = 0; i < vehicle->getTrips().size(); i++) {
        if (vehicle->getTrips()[i]->getPickUpLocation() == pick_up_location && vehicle->getTrips()[i]->getDepartureTime() == departure_time)
            result = vehicle->getTrips()[i];
    }
    if (result != nullptr) {
        result->setBookedSeats(result->getBookedSeats() + 1);
    }
    return result;
}


    Location* getLocation(std::string location){
        int i;
        for(i=0;i<locations.size();i++){
            if(locations[i]->getName()==location){
                return locations[i];
            }
        }
        return nullptr;
    }
};

// int main(){
//     TravelDesk travelDesk;

//     // Add trips
//     for (int i = 0; i < 10; ++i) {
//         std::string vehicleNumber = "A" + std::to_string(i);
//         travelDesk.add_trip(vehicleNumber, 4, "LocationA", "LocationB", 1000 + i * 100);
//     }

//     // Add trips for LocationX
//     for (int i = 0; i < 10; ++i) {
//         std::string vehicleNumber = "X" + std::to_string(i);
//         travelDesk.add_trip(vehicleNumber, 3, "LocationX", "LocationY", 1500 + i * 100);
//     }
// }
