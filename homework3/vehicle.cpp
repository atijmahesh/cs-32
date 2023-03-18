class Vehicle {
    private:
        string name;
    public:
    Vehicle(string id) {
        name = id;
    }
    string id() const {
        return name;
    }
    virtual string description() const = 0;
    
    virtual bool canHover() const {
        return true;
    }
    virtual ~Vehicle() {}
};
 
class Drone:public Vehicle {
public:
    Drone(string id):Vehicle(id) {}
    virtual string description() const {
        return "a drone";
    }
    virtual ~Drone() {
        cout << "Destroying " << id() << ", a drone" << endl;
    }
};

class Balloon:public Vehicle {
private:
    bool isLarge;
public:
    Balloon(string id, int d):Vehicle(id) {
        if(d < 8) isLarge = false;
        else isLarge = true;
    }
    virtual string description() const {
        if(isLarge)
            return "a large balloon";
        return "a small balloon";
    }
    virtual ~Balloon() {
        cout << "Destroying the balloon " << id() << endl;
    }
};

class Satellite:public Vehicle {
public:
    Satellite(string id):Vehicle(id) {}
    virtual string description() const {
        return "a satellite";
    }
    virtual bool canHover() const {
        return false;
    }
    virtual ~Satellite() {
        cout << "Destroying the satellite " << id() << endl;
    }
};
