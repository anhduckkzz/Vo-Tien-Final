/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Sherlock;
class Watson;
class Criminal;
class Character;

class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum CharacterType{
    SHERLOCK,
    WATSON,
    CRIMINAL,
    ROBOT
};

enum ItemType { 
    MAGIC_BOOK,
    ENERGY_DRINK, 
    FIRST_AID, 
    EXCEMPTION_CARD, 
    PASSING_CARD 
};

enum ElementType { 
    PATH, 
    WALL, 
    FAKE_WALL 
};


enum RobotType { C = 0, S, W, SW };

class MapElement {
    friend class TestStudyInPink;
    protected:
        ElementType type;
    public:
        MapElement(ElementType in_type);
        virtual ~MapElement();
        virtual ElementType getType() const;
};

class Path : public MapElement {
    friend class TestStudyInPink;
    public:
        Path();
    };

class Wall : public MapElement {
    friend class TestStudyInPink;
    public: 
        Wall();
    };

class FakeWall : public MapElement {
    friend class TestStudyInPink;
    private:
        int req_exp;
    public:
        FakeWall(int in_req_exp);
        int getreq_exp() const;
}
;

class Map {
    friend class TestStudyInPink;
    private:
        MapElement*** map;
        int num_rows, num_cols;
    public:
        Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
        ~Map();
        bool isValid ( const Position & pos , MovingObject * mv_obj ) const ;
};

class Position {
    friend class TestStudyInPink;
    private:
        int r, c;
    public:
        static const Position npos; 

        Position(int r = 0, int c = 0);

        Position(const string & str_pos);

        int getRow() const;
        int getCol() const;
        void setRow(int r);
        void setCol(int c);

        string str() const;

        bool isEqual(int r, int c) const;
        bool isEqual(Position pos) const;
};

class MovingObject {
friend class TestStudyInPink;
    protected:
        int index;
        Position pos;
        Map* map;
        string name;
        CharacterType character_type;

    public:
        MovingObject(int index, const Position pos, Map * map, const string & name="");
        virtual ~MovingObject();
        virtual Position getNextPosition() = 0;
        virtual Position getCurrentPosition() const;
        virtual void setCurrentPosition(Position pos);
        int getIndex();
        virtual string getName() = 0;

        virtual void move() = 0;
        virtual string str() const = 0;

        virtual CharacterType getCharacterType() const = 0;
        virtual void setCharacterType(CharacterType character_type);
};

class Character : public MovingObject {
    friend class TestStudyInPink;
    protected:
        int hp;
        int exp;
        int num;
        Position next_pos;
        string moving_rule;   
    public:
        Character(int index, const Position &pos, Map * map, const string & name);
        virtual ~Character();
        virtual Position getNextPosition();
        Position getCurrentPosition() const;
        virtual string str() const = 0;
        virtual void move() = 0;
        CharacterType getCharacterType() const = 0;
        virtual string getName() = 0;
        
        int getRow();
        int getCol();
        virtual void setExp(int exp);
        virtual void setHp(int hp);
        virtual int getExp();
        virtual int getHp();
};

class Sherlock : public Character {
    friend class TestStudyInPink;
    private:
    SherlockBag* bag;
    public:
        Sherlock(int index, const string & moving_rule,const Position & pos, Map * map, int hp, int exp);
        ~Sherlock();
        void move() override;
        string str() const override;
        CharacterType getCharacterType() const override;
        bool meet(RobotC* robotc);
        bool meet(RobotS* robots);
        bool meet(RobotW* robotw);
        bool meet(RobotSW* robotsw);
        bool meet(Watson* watson);
        SherlockBag* getBag() const;
        string getName();

    };

    class Watson : public Character {
        friend class TestStudyInPink;
        public:     //illegal
        WatsonBag* bag;
    public:
        Watson(int index, const string & moving_rule, const Position & pos, Map * map, int hp, int exp);
        ~Watson(){};
        void move();
        string str() const;
        CharacterType getCharacterType() const;
        WatsonBag* getBag() const;

        bool meet(RobotC* robotc);
        bool meet(RobotS* robots);
        bool meet(RobotW* robotw);
        bool meet(RobotSW* robotsw);
        string getName();
};

class Criminal : public Character {
    private:
        Sherlock* sherlock;
        Watson * watson;
        int count;
        Position pre_pos = Position::npos;
    public:
        Criminal(int index, const Position & pos, Map * map, Sherlock * sherlock, Watson * watson);
        Position getNextPosition() override;
        void move() override;
        string str() const override;
        void setCount(int count);
        int getCount() const;
        Position getPreviousPosition();
        CharacterType getCharacterType() const;
        string getName();
};

class ArrayMovingObject {   
    private:
        MovingObject **arr_mv_objs;
        int capacity;
        int count;
    public:
        ArrayMovingObject(int capacity);
        ~ArrayMovingObject();
        bool isFull() const;
        bool checkMeet(int index) const;
        bool add(MovingObject * mv_obj);
        MovingObject * get(int index) const;
        int size() const; // return current number of elements in the array
        string str() const;
        
};

class Configuration {
    friend class StudyPinkProgram;
    friend class TestStudyInPink;
    private:
        int map_num_rows = 0, map_num_cols = 0;
        int max_num_moving_objects = 0;
        int num_walls = 0;
        Position * arr_walls = nullptr;
        int num_fake_walls = 0;
        Position * arr_fake_walls = nullptr;
        int num_steps = 0;
        string sherlock_moving_rule = "";
        Position sherlock_pos = Position::npos;
        int sherlock_hp = 0;
        int sherlock_exp = 0;
        string watson_moving_rule = "";
        Position watson_pos = Position::npos;
        int watson_hp = 0;
        int watson_exp = 0;
        Position criminal_pos = Position::npos;  

        
    public:
        Configuration(const string & filepath);
        ~Configuration();
        string str() const;
        void kill_pos(const string& input, Position& position);
        void kill_array_pos(const string& input, Position*& positions, bool iswall);
};  

// Robot, BaseItem, BaseBag,...
class Robot : public MovingObject{
    friend class TestStudyInPink;
    protected:
        RobotType robot_type;
        Criminal * criminal;
        BaseItem* item;

    public:
        Robot(int index, const Position & pos, Map * map, Criminal* criminal);
        static Robot* create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson);
        virtual Position getNextPosition() = 0;
        virtual void move();
        virtual string str() const = 0;
        virtual int getDistance() const = 0;
        virtual RobotType getRobotType() const = 0;
        virtual void setRobotType(RobotType robot_type);
        void setItem(ItemType itemtype, Criminal* criminal);
        int two_into_one(int p);
        virtual BaseItem* getItem();
        virtual string getName() = 0;
    };

class RobotC : public Robot {
    friend class TestStudyInPink;
    private:
    public:
        RobotC(int index , const Position & pos , Map * map , Criminal * criminal) ;
        Position getNextPosition();
        void move();
        int getDistance(Sherlock* sherlock);
        int getDistance(Watson* watson);
        int getDistance() const;
        string str() const;
        RobotType getRobotType() const override;
        CharacterType getCharacterType() const{return ROBOT;};

        string getName();
};

class RobotS : public Robot {
    friend class TestStudyInPink;
    private:
        Sherlock * sherlock;
    public:
        RobotS ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * Sherlock );
        Position getNextPosition();
        void move();
        int getDistance() const;
        string str() const;
        RobotType getRobotType() const;
        CharacterType getCharacterType() const{return ROBOT;};
        string getName();
        
};

class RobotW : public Robot {
    friend class TestStudyInPink;
    private:
        Watson * watson;
    public:
    RobotW ( int index , const Position & pos , Map * map , Criminal * criminal , Watson * watson );
    int getDistance() const;
    Position getNextPosition();
    string str() const;
    RobotType getRobotType() const;
    CharacterType getCharacterType() const{return ROBOT;};
    string getName();
   
};

class RobotSW : public Robot {
    friend class TestStudyInPink;
    private:
        Sherlock * sherlock;
        Watson * watson;
    public:
        RobotSW ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson* watson );
        int getDistance() const;
        Position getNextPosition();
        string str() const;
        RobotType getRobotType() const;
        CharacterType getCharacterType() const{return ROBOT;};
        string getName();
};

class BaseItem{
    protected:
    ItemType item;
    public:
        virtual bool canUse ( Character * obj , Robot * robot ) = 0;
        virtual void use ( Character * obj , Robot * robot ) = 0;
        virtual ItemType getItemType() const = 0;
        virtual void setItemType(ItemType item);
        virtual string getName() = 0;
};

class MagicBook : public BaseItem {
    public:
        bool canUse ( Character * obj , Robot * robot );
        void use ( Character * obj , Robot * robot );
        string getName();
        ItemType getItemType() const;
};

class EnergyDrink:public BaseItem {
    public:
        bool canUse ( Character * obj , Robot * robot ) override;
        void use ( Character * obj , Robot * robot ) override;
        ItemType getItemType() const;
        string getName();
};

class FirstAid : public BaseItem{
    public:
        bool canUse ( Character * obj , Robot * robot )override;
        void use ( Character * obj , Robot * robot ) override;
        ItemType getItemType() const;
        string getName();

};

class ExcemptionCard: public BaseItem{
    public:
        bool canUse ( Character * obj , Robot * robot ) override;
        void use ( Character * obj , Robot * robot ) override;
        ItemType getItemType() const;
        string getName();

};

class PassingCard: public BaseItem{
    private:
        string challenge;
    public:
        PassingCard(int a,int b);
        PassingCard(string challenge);
        bool canUse ( Character * obj , Robot * robot ) override;
        void use ( Character * obj , Robot * robot ) override;
        ItemType getItemType() const;
        string getName();
        string getChallenge() const {
            return challenge;
        };
        
};

//BaseBag
class BaseBag{
    public:
        class Node{
            public:
                BaseItem* item;
                Node* next;
            public:
                Node(BaseItem* item,Node* next = nullptr);
                ~Node(){};
        };
    protected:
        Node* head;
        Character* obj;
    public:
        virtual bool insert(BaseItem* item);
        virtual BaseItem* get() = 0;
        virtual BaseItem* get(ItemType itemtype) = 0;
        virtual string str() const;
        BaseBag(int capacity);
        virtual ~BaseBag();
        bool checkItem(ItemType itemtype);
        bool isFull() const;
        virtual int getCapacity() = 0;
        virtual void setCapacity(int capacity);
    protected:
        int size;
        int capacity;
};

class SherlockBag : public BaseBag{
    friend class TestStudyInPink;
    private:
        Sherlock* sherlock;
    public:
        SherlockBag(Sherlock* sherlock);
        int getCapacity();
        void setCapacity(int capacity);
        BaseItem* get();
        virtual BaseItem* get(ItemType itemtype);
};

class WatsonBag : public BaseBag{
    friend class TestStudyInPink;
    private:
        Watson* watson;
    public:
        WatsonBag(Watson* watson);
        virtual BaseItem* get();
        int getCapacity();
        void setCapacity(int capacity);
        BaseItem* get(ItemType itemtype);
        
};
class StudyPinkProgram {
    friend class TestStudyInPink;
    private:
        // Sample attributes
        Configuration * config;
        Sherlock * sherlock;
        Watson * watson;
        Criminal * criminal;
        Map * map;
        ArrayMovingObject * arr_mv_objs;
    public:
        StudyPinkProgram(const string & config_file_path);

        bool isStop() const;

        void printResult() const {
            // if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            //     cout << "Sherlock caught the criminal" << endl;
            // }
            // else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            //     cout << "Watson caught the criminal" << endl;
            // }
            // else {
            //     cout << "The criminal escaped" << endl;
            // }
        }

        void printStep(int si) const {
            // cout << "Step: " << setw(4) << setfill('0') << si
            //     << "--"
            //     << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
        }

        void printInfo(int si, int i, ofstream &OUTPUT);
        void run(bool verbose,ofstream &OUTPUT);
            // Note: This is a sample code. You can change the implementation as you like.
            // TODO
        ~StudyPinkProgram();
};


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
