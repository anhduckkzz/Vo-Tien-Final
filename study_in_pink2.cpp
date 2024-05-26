// TODO:
// Chỉnh lại di chuyển của RobotC (gợi ý: thực hiện hàm getPrevPosition cho Criminal)
// Mô tả về các meet thay đổi (đã được confirm 90%)
// File study_in_pink2.h những phần trước "addition" là gốc của đề (không thay đổi)
// Chỉnh tên thuộc tính hp và exp của Sherlock và Watson
// Position có nạp chồng 2 hàm isEqual
// isStop kiểm tra vị trí Sherlock, Watson với Criminal và hp của Sherlock, Watson
// Hàm run chú ý chỉnh từ hàm run() gốc (vị trí gọi printResult và printStep)
// Hàm move của Sherlock và Watson khi exp == 0 sẽ không thực hiện gì
// NOTE:
// chú ý các phần addition nên sửa đổi để tránh đạo code
// nộp Bkel cần xóa đổi lại 2 hàm printResult và printStep gốc, xóa thuộc tính outputFile
// TODO:
// Chỉnh lại di chuyển của RobotC (gợi ý: thực hiện hàm getPrevPosition cho Criminal)
// Mô tả về các meet thay đổi (đã được confirm 90%)
// File study_in_pink2.h những phần trước "addition" là gốc của đề (không thay đổi)
// Chỉnh tên thuộc tính hp và exp của Sherlock và Watson
// Position có nạp chồng 2 hàm isEqual
// isStop kiểm tra vị trí Sherlock, Watson với Criminal và hp của Sherlock, Watson
// Hàm run chú ý chỉnh từ hàm run() gốc (vị trí gọi printResult và printStep)
// Hàm move của Sherlock và Watson khi exp == 0 sẽ không thực hiện gì
// NOTE:
// chú ý các phần addition nên sửa đổi để tránh đạo code
// nộp Bkel cần xóa đổi lại 2 hàm printResult và printStep gốc, xóa thuộc tính outputFile
#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

int distance(Position char1, Position char2){
    return( abs(char1.getRow() - char2.getRow()) + abs(char1.getCol() - char2.getCol()) );
}
// TASK 3.1 - MapElement
// Requirement 1
MapElement::MapElement(ElementType in_type){
    this->type = in_type;
}
MapElement::~MapElement(){};
ElementType MapElement::getType() const {
    return type;
}

//Requirement 2
Path::Path() : MapElement(type) {
    type = PATH;
}
Wall::Wall() : MapElement(type) {
    type = WALL;
}
FakeWall::FakeWall(int in_req_exp) : MapElement(type){
    this->type = FAKE_WALL;
    this->req_exp = in_req_exp;
}
int FakeWall::getreq_exp() const{
    return req_exp;
}

//TASK 3.2 - Map

Map::Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls){
    //pass parameter
    this->num_cols = num_cols;
    this->num_rows = num_rows;

    // Allocate memory for the map
    map = new MapElement**[num_rows];
    for (int i = 0; i < num_rows; i++) {
        map[i] = new MapElement*[num_cols];
        for (int j = 0; j < num_cols; j++) {
            map[i][j] = new Path(); // Initialize the map with Path - default
        }
    }

    for(int i = 0;i < num_rows;i++){
        for(int j = 0;j < num_cols;j++){
            //Fake Walls
            for (int k = 0; k < num_fake_walls; k++) {
                
                int fake_wall_col = array_fake_walls[k].getCol();
                int fake_wall_row = array_fake_walls[k].getRow();
                
                if(map[i][j] == map[fake_wall_row][fake_wall_col]){
                    delete map[fake_wall_row][fake_wall_col];
                    map[fake_wall_row][fake_wall_col] = new FakeWall((i * 257 + j * 139 + 89) % 900 + 1); // Replace with FakeWall
                }else{
                    //in case if the coordinate is out of bound
                    continue;
                }
            }
            
            //Walls
            for (int k = 0; k < num_walls; k++) {
                
                int wall_row = array_walls[k].getRow();
                int wall_col = array_walls[k].getCol();

                if(map[i][j] == map[wall_row][wall_col]){
                    delete map[wall_row][wall_col];
                    map[wall_row][wall_col] = new Wall(); // Replace with Wall
                }else{
                    //in case if the coordinate is out of bound
                    continue;
                }
            }
        }
    }
}
Map::~Map(){
    for(int i = 0;i < num_rows;i++){
        for(int j = 0;j < num_cols;j++){
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const {
    int r = pos.getRow();
    int c = pos.getCol();

    MapElement *element = map[r][c];

    if(r < 0 || r >= num_rows || c < 0 || c >= num_cols){
        return false; // Out of bounds
    }
   
    if(element->getType() == WALL){
        return false; // Wall
    }else if (element->getType() == FAKE_WALL){
        if(mv_obj->getCharacterType() == WATSON){
            Watson * watson = dynamic_cast<Watson*>(mv_obj);

            if(watson->getExp() <= dynamic_cast<FakeWall*>(element)->getreq_exp()){
                return false;
            }
            
        }
    }
    return true; // Default case
}

//Task 3.3 - Position

const Position Position::npos=Position(-1,-1);

Position::Position(int r, int c){
    this->r = r;
    this->c = c;
}
Position::Position(const string & str_pos){
    //extract from (x,y) into x and y
    if(str_pos.find(',') != string::npos){
        this->r = stoi(str_pos.substr(1,str_pos.find(',') - 1));
        this->c = stoi(str_pos.substr(str_pos.find(',') + 1,str_pos.find(')')  - str_pos.find(',') - 1));
    }
}

bool Position::isEqual(int r, int c) const{
    if(this->r == r && this->c == c){
        return true;
    }
    //else
    return false;
}

void Position::setRow(int r){
    this->r = r;
}
int Position::getRow() const{
    return r;
}
void Position::setCol(int c){
    this->c = c;
}
int Position::getCol() const{
    return c;
}
string Position::str() const{
    return "(" + to_string(r) + "," + to_string(c) + ")";
};

bool Position::isEqual(Position pos) const{
    if(r == pos.getRow() && c == pos.getCol()) return true;
    else return false;
};

//Task 3.4 - MovingObject

MovingObject::MovingObject(int index,const Position pos, Map * map,const string & name){
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = "";
} 
MovingObject::~MovingObject(){};

Position MovingObject::getCurrentPosition() const{
    return pos; 
}

void MovingObject::setCurrentPosition(Position pos){
    this->pos = pos;
}

void MovingObject::setCharacterType(CharacterType character_type){
    this->character_type = character_type;
}
//Task Character

Character::Character(int index, const Position &pos, Map *map, const string &name) : MovingObject(index, pos, map, name) {};

Character::~Character() {};

Position Character::getNextPosition() {
    Position next_pos = pos;
    if (num == moving_rule.length())
        num = 0;
    if (num < moving_rule.length())
    {
        switch (moving_rule[num])
        {
        case 'U':
            next_pos.setRow(pos.getRow() - 1);
            break;
        case 'D':
            next_pos.setRow(next_pos.getRow() + 1);
            break;
        case 'R':
            next_pos.setCol(next_pos.getCol() + 1);
            break;
        case 'L':
            next_pos.setCol(next_pos.getCol() - 1);
            break;
        default:
            break;
        }
        if (map->isValid(next_pos, this))
            return next_pos;
        else
        {
            next_pos = Position::npos;
        }
    }
num++;
    return Position::npos;
}
Position Character::getCurrentPosition() const{
    return pos;
}


int Character::getCol(){
    return pos.getCol();
}
int Character::getRow(){
    return pos.getRow();
}
string Character::str() const{
    return 0;
};
void Character::move(){
    next_pos = getNextPosition();
    if (!next_pos.isEqual(Position::npos))
    {
        pos = next_pos;
    }
};

int Character::getExp(){
    return exp;
}

int Character::getHp(){
    return hp;
}


//Task 3.5 - Sherlock

Sherlock::Sherlock(int index, const string &moving_rule,const  Position &pos, Map *map, int hp, int exp) : Character(index, pos, map, "Sherlock"){
    this->moving_rule = moving_rule;
    this->index = 1;
    this->exp = exp;
    bag = new SherlockBag(this);
    this->hp = hp;
    if (hp < 0) {
        hp = 0;
    } else if (hp > 900) {
        hp = 900;
    }

    if (exp < 0) {
        exp = 0;
    } else if (exp > 900) {
        exp = 900;
    }
}
void Sherlock::move() {
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos))return;
        pos = next_pos;
}

CharacterType Sherlock::getCharacterType() const{
    return SHERLOCK;
}
string Sherlock::str() const {
    return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule +"]";
}

Sherlock::~Sherlock() {}

bool Sherlock::meet(RobotS* robots){
    //check if sherlock meet robotS
    if(robots == nullptr){
        return false;
    }

    //get out of from bag the excemption card
    BaseItem* card = bag->get(EXCEMPTION_CARD);   
    //if there is an excemption card in the bag 
    if(card != nullptr){
        //if item can be used
        if(card->canUse(this, robots)){
            //use the item
            card->use(this, robots);
            //combat win
            if(this->getExp() > 400){
                bag->insert(robots->getItem());
                delete robots;
            //combat lose,exp remains the same
            }else{
                this->setExp(this->getExp());
            }

        //if the card cannot be used,combat
        }else{
            //sherlock win if exp > 400
            if(this->getExp() > 400){
                bag->insert(robots->getItem());
                delete robots;
            }else{
                //sherlock defeat and lose 10% exp,no receive
                this->setExp(this->getExp() * 0.9);
                this->setExp(this->getExp());
            }
        }
    //if there is no excemption card in the bag
    }else{
        //win if exp > 400
        if(this->getExp() > 400){
            BaseItem* item = robots->getItem();
            bag->insert(item);
            delete robots;
        //defeat and lose 10% exp,no receive
        }else{
            this->setExp(this->getExp() * 0.9);
            //how to use aftercombat-item
        }
    }

    BaseItem* after_combat_item = this->bag->get();
    if(after_combat_item != nullptr){
        after_combat_item->use(this,nullptr);
        //already delete in linked list, no need to delete
    }

    //check exp and hp
    if(hp < 0){
        hp = 0;
        }else if(hp > 900){
            hp = 900;
        }

        if(exp < 0){
            exp = 0;
        }else if(exp > 900){
            exp = 900;
        }

    return true;
}

bool Sherlock::meet(RobotW* robotw){
    //auto win and auto receive item,no aftercombat-item
    if(robotw == nullptr){
        return false;
    }
    bag->insert(robotw->getItem());
    delete robotw;
    return true;
}

bool Sherlock::meet(RobotSW* robotsw){
    if(robotsw == nullptr){
        return false;
    }
        //check excemption card
        BaseItem *card = bag->get(EXCEMPTION_CARD);
        if (card != nullptr){
            // check canuse
            if (card->canUse(this, robotsw)){
                //use card
                card->use(this,robotsw);
                if(this->exp > 300 && this->hp > 335){
                    bag->insert(robotsw->getItem());
                    delete robotsw;
                }else{
                    //defeat,no receive,use card
                    this->setExp(this->getExp());
                    this->setHp(this->getHp());
                }
            //card cannot be used
            }else{
                //win
                if(this->exp > 300 && this->hp > 335){
                    BaseItem *item = robotsw->getItem(); 
                    bag->insert(item);
                    delete robotsw;
                }else{
                    //defeat,lose 15% exp and hp,no receive
                    this->setExp(this->getExp() * 0.85);
                    this->setHp(this->getHp() * 0.85);

                    if(hp < 0){
                    hp = 0;
                    }else if(hp > 900){
                        hp = 900;
                    }

                    if(exp < 0){
                        exp = 0;
                    }else if(exp > 900){
                        exp = 900;
                    }
                }
            }
        // no exist card    
        }else {
            //win
                if (this->exp > 300 || this->exp > 335){
                    bag->insert(robotsw->getItem());
                    delete robotsw;
                }else{
                    this->setExp(this->getExp() * 0.85);
                    this->setExp(this->getHp() * 0.85);

                    if(hp < 0){
                    hp = 0;
                    }else if(hp > 900){
                        hp = 900;
                    }

                    if(exp < 0){
                        exp = 0;
                    }else if(exp > 900){
                        exp = 900;
                    }
                }
            }
            
    BaseItem* after_combat_item = this->bag->get();
    if(after_combat_item != nullptr){
        after_combat_item->use(this,nullptr);
        //already delete in linked list, no need to delete
    }
    return true;
}


bool Sherlock::meet(RobotC* robotc){
    if(robotc == nullptr){
        return false;
    }

    
    if(this->getExp() > 500){
        bag->insert(robotc->getItem());
        delete robotc;
    }else{
        Position win_pos = robotc->getNextPosition();
        this->pos = win_pos;
    }
    BaseItem* after_combat_item = this->bag->get();
    if(after_combat_item != nullptr){
        after_combat_item->use(this,nullptr);
        //already delete in linked list, no need to delete
    }
    return true;

}
bool Sherlock::meet(Watson* watson){
    if(watson == nullptr){
        return false;
    }

    BaseItem* passingcard = this->bag->get(PASSING_CARD);
    BaseItem* excemptioncard = watson->bag->get(EXCEMPTION_CARD);

    if(passingcard != nullptr && excemptioncard != nullptr){
        while ((passingcard = bag->get(PASSING_CARD)) != nullptr) {
            watson->bag->insert(passingcard);
        }

        while ((excemptioncard = watson->bag->get(EXCEMPTION_CARD)) != nullptr) {
            bag->insert(excemptioncard);
        }
    }
    BaseItem* after_combat_item = this->bag->get();
    if(after_combat_item != nullptr){
        after_combat_item->use(this,nullptr);
        //already delete in linked list, no need to delete
    }
    return true;
}

SherlockBag* Sherlock::getBag() const{
    return bag;

}

string Sherlock::getName(){
    return "Sherlock";
}
//Task 3.6 - Watson

Watson::Watson(int index, const string &moving_rule, const Position &pos, Map *map, int hp, int exp) : Character(index, pos, map, "Watson"){
    this->moving_rule = moving_rule;
    this->exp = exp;
    bag = new WatsonBag(this);
    this->hp = hp; 
    if (hp < 0) {
        hp = 0;
    } else if (hp > 500) {
        hp = 500;
    }

    if (exp < 0) {
        exp = 0;
    } else if (exp > 900) {
        exp = 900;
    }   
    
}

void Watson::move(){
    Position next_pos = this->getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
    else if(this->getExp() == 0 || this->getHp() == 0) return;
        pos = next_pos;
    
}

CharacterType Watson::getCharacterType() const{
    return WATSON;
}

string Watson::str() const {
    return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

WatsonBag* Watson::getBag() const{
    return bag;
}

bool Watson::meet(RobotS* robots){
    if(robots == nullptr){
        return false;
    }
    BaseItem* card = bag->get(PASSING_CARD);
    //there is a card
    if(card != nullptr){
        PassingCard* passingcard = new PassingCard(pos.getCol(),pos.getRow());
        if(passingcard->getChallenge() == "ROBOTS" || passingcard->getChallenge() == "ALL"){
            passingcard->use(this,robots);
            delete robots;
        }else{
            this->setExp(this->getExp() - 50);
        }
    //no card
    }else{
        return true;    
    }
    BaseItem* after_combat_item = this->bag->get();
    if(after_combat_item != nullptr){
        after_combat_item->use(this,nullptr);
        //already delete in linked list, no need to delete
    }
    return true;
}

bool Watson::meet(RobotW* robotw){
    if(robotw == nullptr){
        return false;
    }else{
        BaseItem* card = bag->get(PASSING_CARD);
        if(card != nullptr){
            //check card and use card
            PassingCard* passingcard = new PassingCard(pos.getCol(),pos.getRow());
            if(passingcard->getChallenge() == "ROBOTW" || passingcard->getChallenge() == "ALL"){
                passingcard->use(this,robotw);
                delete robotw;
            }else{
                this->setExp(this->getExp() - 50);
            }
        }else{
            //win the combat,receive item
            if(this->getHp() > 350){
                bag->insert(robotw->getItem());
                delete robotw;
            }else{
                //lose
                this->setHp(this->getHp() * 0.85);
                this->setExp(this->getExp() * 0.85);

                if(hp < 0){
                hp = 0;
                }else if(hp > 900){
                    hp = 900;
                }

                if(exp < 0){
                    exp = 0;
                }else if(exp > 900){
                    exp = 900;
                }
            }
        }
    }
    BaseItem* after_combat_item = this->bag->get();
    if(after_combat_item != nullptr){
        after_combat_item->use(this,nullptr);
        //already delete in linked list, no need to delete
    }
    return true;
}

bool Watson::meet(RobotSW* robotsw){
    if(robotsw == nullptr){
        return false;
    }
    BaseItem* card = bag->get(PASSING_CARD);
    if(card != nullptr){
        PassingCard* passingcard = new PassingCard(pos.getCol(),pos.getRow());
        if(passingcard->getChallenge() == "ROBOTSW" || passingcard->getChallenge() == "ALL"){
            if(passingcard->canUse(this,robotsw)){
                passingcard->use(this,robotsw);
                delete robotsw;
            }else{
                this->setExp(this->getExp() - 50);
            }
        }
        //no use card
    }else{
        if(this->getExp() > 600 && this->getHp() > 165){
            //win
            this->bag->insert(robotsw->getItem());
            delete robotsw;
        }else{
            this->setExp(this->getExp() * 0.85);
            this->setHp(this->getHp() * 0.85);

            if(hp < 0){
            hp = 0;
            }else if(hp > 900){
                hp = 900;
            }

            if(exp < 0){
                exp = 0;
            }else if(exp > 900){
                exp = 900;
            }
        }
    }
    BaseItem* after_combat_item = this->bag->get();
    if(after_combat_item != nullptr){
        after_combat_item->use(this,nullptr);
        //already delete in linked list, no need to delete
    }
    return true;
}

bool Watson::meet(RobotC* robotc)
{
    if(robotc == nullptr){
        return false;
    }
    else{
        BaseItem* card = bag->get(PASSING_CARD);
        if(card != nullptr){
            PassingCard* passingcard = new PassingCard(pos.getCol(),pos.getRow());
            if(passingcard->getChallenge() == "ROBOTC" || passingcard->getChallenge() == "ALL"){
                passingcard->use(this,robotc);
                delete robotc;
            }else{
                this->setExp(this->getExp() - 50);

                if(exp < 0){
                    exp = 0;
                }else if(exp > 900){
                    exp = 900;
                }
            }
        }else{
            this->bag->insert(robotc->getItem());
            delete robotc;
        }
    }
    BaseItem* after_combat_item = this->bag->get();
    if(after_combat_item != nullptr){
        after_combat_item->use(this,nullptr);
        //already delete in linked list, no need to delete
    }
    return true;
}

string Watson::getName(){
    return "Watson";
}
    //Task 3.7 - Criminal

Criminal::Criminal(int index, const Position &pos, Map *map, Sherlock * sherlock, Watson * watson) : Character(index, pos, map, "Criminal") {
    this->sherlock = sherlock;
    this->watson = watson;
    this->index = 0;
    count = 0;
}
void Criminal::move(){
    Position next_pos = this->getNextPosition();
    if (next_pos.isEqual(Position::npos)) {
        count++;
        pre_pos = pos;
    return;
    }
    pre_pos = pos;
    pos = next_pos;
}

Position Criminal::getPreviousPosition(){
    return pre_pos;
}

void Criminal::setCount(int count){
    this->count = count;
}
int Criminal::getCount() const{
    return count;
}
string Criminal::str() const {
    return "Criminal[index=" + to_string(0) + ";pos=" + pos.str() + "]";
}
Position Criminal::getNextPosition(){

    int crim_col = pos.getCol();
    int crim_row = pos.getRow();

    Position arr[4];  // arr of Position of location of Criminal
    arr[0] = Position(crim_row - 1, crim_col);
    arr[1] = Position(crim_row,crim_col - 1);
    arr[2] = Position(crim_row + 1,crim_col);
    arr[3] = Position(crim_row,crim_col + 1);
    

    int arr_dis[4]; // arr of distance
    arr_dis[0] = distance(arr[0],sherlock->getCurrentPosition()) + distance(arr[0],watson->getCurrentPosition());
    arr_dis[1] = distance(arr[1],sherlock->getCurrentPosition()) + distance(arr[1],watson->getCurrentPosition());
    arr_dis[2] = distance(arr[2],sherlock->getCurrentPosition()) + distance(arr[2],watson->getCurrentPosition());
    arr_dis[3] = distance(arr[3],sherlock->getCurrentPosition()) + distance(arr[3],watson->getCurrentPosition());

    for (int i = 0; i < 4; i++) {
        if(!map->isValid(arr[i],this)){
            //calculate all 4 distance
            arr_dis[i] = INT_MIN;
        }
    }

    //filter
    int max = arr_dis[0];
    int max_index = 0;
    for(int i = 1;i < 4;i++){
        if(arr_dis[i] > max){
            max = arr_dis[i];
            max_index = i;
        }
    }

    if (max == INT_MIN) {
        next_pos = Position::npos;
    }else{
        next_pos = arr[max_index];
    }
    return next_pos;
}

CharacterType Criminal::getCharacterType() const{
    return CRIMINAL;
}

string Criminal::getName(){
    return "Criminal";
}

//Task 3.8 - ArrayMovingObject

ArrayMovingObject::ArrayMovingObject(int capacity){
    this->capacity = capacity;
    this->count = 0;
    arr_mv_objs = new MovingObject *[capacity];
}
ArrayMovingObject::~ArrayMovingObject(){
    delete arr_mv_objs;
};
bool ArrayMovingObject::isFull() const {
    if (count == capacity) {
        return true;
    }
    return false;
}
int MovingObject::getIndex(){
    return index;
}

bool ArrayMovingObject::add(MovingObject *mv_obj) {
    if (this->isFull()) {
        return false;
    }
    arr_mv_objs[count] = mv_obj;
    count++;
    return true; 
}
MovingObject * ArrayMovingObject::get(int index) const {
    if (index < 0 || index > count) {
        return nullptr;
    }
    return arr_mv_objs[index];
}
int ArrayMovingObject::size() const {
    return count;
}   
string ArrayMovingObject::str() const {
    string res = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity) + ";]";
    for (int i = 0; i < count; i++) {
        res+= arr_mv_objs[i]->str();
        if(i != count - 1){
            res += ";";
        }
    }
    res += "]";
    return res;
}

bool ArrayMovingObject::checkMeet(int index) const{
    MovingObject* mv1 = arr_mv_objs[index];
    for(int i = 0; i < count;i++){
        MovingObject* mv2 = arr_mv_objs[i];

        if(mv1->getCurrentPosition().isEqual(mv2->getCurrentPosition())){
            //SHERLOCK
            if(mv1->getCharacterType() == SHERLOCK){
                Sherlock * sherlock = dynamic_cast<Sherlock*>(arr_mv_objs[i]);
                if(mv2->getCharacterType() == SHERLOCK){
                    continue;
                }else if(mv2->getCharacterType() == WATSON){
                    Watson * watson = dynamic_cast<Watson*>(mv2);
                    sherlock->meet(watson);
                }else if(mv2->getCharacterType() == CRIMINAL){
                    // no method
                    
                    return true;
                } else if(mv2->getCharacterType() == ROBOT){
                    if(dynamic_cast<Robot*>(mv2)->getRobotType() == C){
                        RobotC * robotc = dynamic_cast<RobotC*>(mv2);
                        sherlock->meet(robotc);
                        return true;
                    }else if(dynamic_cast<RobotS*>(mv2)->getRobotType() == S){
                        RobotS * robots = dynamic_cast<RobotS*>(mv2);
                        sherlock->meet(robots);
                    }else if(dynamic_cast<RobotW*>(mv2)->getRobotType() == W){
                        RobotW * robotw = dynamic_cast<RobotW*>(mv2);
                        sherlock->meet(robotw);
                    }else if(dynamic_cast<RobotSW*>(mv2)->getRobotType() == SW){
                        RobotSW * robotsw = dynamic_cast<RobotSW*>(mv2);
                        sherlock->meet(robotsw);
                    }
                }
                //WATSON
            }else if(mv1->getCharacterType() == WATSON){
                Watson * watson = dynamic_cast<Watson*>(mv1);  
                if(mv2->getCharacterType() == WATSON){
                    continue;
                }else if(mv2->getCharacterType() == SHERLOCK){
                    Sherlock * sherlock = dynamic_cast<Sherlock*>(mv2);
                    sherlock->meet(watson);
                }else if(mv2->getCharacterType() == CRIMINAL){
                    Criminal * criminal = dynamic_cast<Criminal*>(mv2);
                }else if(mv2->getCharacterType() == ROBOT){
                    if(dynamic_cast<Robot*>(mv2)->getRobotType() == C){
                        RobotC * robotc = dynamic_cast<RobotC*>(mv2);
                        watson->meet(robotc);
                        return true;
                    }else if(dynamic_cast<RobotS*>(mv2)->getRobotType() == S){
                        RobotS * robots = dynamic_cast<RobotS*>(mv2);
                        watson->meet(robots);
                    }else if(dynamic_cast<RobotW*>(mv2)->getRobotType() == W){
                        RobotW * robotw = dynamic_cast<RobotW*>(mv2);
                        watson->meet(robotw);
                    }else if(dynamic_cast<RobotSW*>(mv2)->getRobotType() == SW){
                        RobotSW * robotsw = dynamic_cast<RobotSW*>(mv2);
                        watson->meet(robotsw);
                    }
                }
            //CRIMINAL
            }else if(mv1->getCharacterType() == CRIMINAL){
                Criminal * criminal = dynamic_cast<Criminal*>(arr_mv_objs[i]);
                if(mv2->getCharacterType() == CRIMINAL){
                    continue;
                }else if(mv2->getCharacterType() == SHERLOCK){
                    Sherlock * sherlock = dynamic_cast<Sherlock*>(mv2);
                    sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition());
                    return true;
                }else if(mv2->getCharacterType() == WATSON){
                    Watson * watson = dynamic_cast<Watson*>(mv2);
                    return true;
                }else if(mv2->getCharacterType() == ROBOT){
                    if(dynamic_cast<Robot*>(mv2)->getRobotType() == C){
                        RobotC * robotc = dynamic_cast<RobotC*>(mv2);
                    }else if(dynamic_cast<RobotS*>(mv2)->getRobotType() == S){
                        RobotS * robots = dynamic_cast<RobotS*>(mv2);
                    }else if(dynamic_cast<RobotW*>(mv2)->getRobotType() == W){
                        RobotW * robotw = dynamic_cast<RobotW*>(mv2);
                    }else if(dynamic_cast<RobotSW*>(mv2)->getRobotType() == SW){
                        RobotSW * robotsw = dynamic_cast<RobotSW*>(mv2);
                    }
                }
                //ROBOT
            }else if(mv1->getCharacterType() == ROBOT){
                if(dynamic_cast<RobotC*>(mv2)->getRobotType() == C){
                    RobotC * robotc = dynamic_cast<RobotC*>(arr_mv_objs[i]);
                    if(mv2->getCharacterType() == SHERLOCK){
                        Sherlock * sherlock = dynamic_cast<Sherlock*>(mv2);
                        sherlock->meet(robotc);
                        return true;
                    }else if(mv2->getCharacterType() == WATSON){
                        Watson * watson = dynamic_cast<Watson*>(mv2);
                        watson->meet(robotc);
                    }else if(mv2->getCharacterType() == CRIMINAL){
                        Criminal * criminal = dynamic_cast<Criminal*>(mv2);
                        continue;
                    }
                }else if(dynamic_cast<RobotS*>(mv1)->getRobotType() == S){
                    RobotS * robots = dynamic_cast<RobotS*>(arr_mv_objs[i]);
                    if(mv2->getCharacterType() == SHERLOCK){
                        Sherlock * sherlock = dynamic_cast<Sherlock*>(mv2);
                        sherlock->meet(robots);
                        return true;
                    }else if(mv2->getCharacterType() == WATSON){
                        Watson * watson = dynamic_cast<Watson*>(mv2);
                        watson->meet(robots);
                    }else if(mv2->getCharacterType() == CRIMINAL){
                        Criminal * criminal = dynamic_cast<Criminal*>(mv2);
                    }

                }else if(dynamic_cast<RobotW*>(mv1)->getRobotType() == W){
                    RobotW * robotw = dynamic_cast<RobotW*>(arr_mv_objs[i]);
                    if(mv2->getCharacterType() == SHERLOCK){
                        Sherlock * sherlock = dynamic_cast<Sherlock*>(mv2);
                        sherlock->meet(robotw);
                        return true;
                    }else if(mv2->getCharacterType() == WATSON){
                        Watson * watson = dynamic_cast<Watson*>(mv2);
                        watson->meet(robotw);
                    }else if(mv2->getCharacterType() == CRIMINAL){
                        Criminal * criminal = dynamic_cast<Criminal*>(mv2);
                    }

                }else if(dynamic_cast<RobotSW*>(mv1)->getRobotType() == SW){
                    RobotSW * robotsw = dynamic_cast<RobotSW*>(arr_mv_objs[i]);
                    if(mv2->getCharacterType() == SHERLOCK){
                        Sherlock * sherlock = dynamic_cast<Sherlock*>(mv2);
                        sherlock->meet(robotsw);
                        return true;
                    }else if(mv2->getCharacterType() == WATSON){
                        Watson * watson = dynamic_cast<Watson*>(mv2);
                        watson->meet(robotsw);
                    }else if(mv2->getCharacterType() == CRIMINAL){
                        Criminal * criminal = dynamic_cast<Criminal*>(mv2);
                    }
                }
            }
        }
    }
    return false;
}


//Task 3.9 - Configuration

Configuration::Configuration(const string &filepath){
    ifstream file(filepath);
    string line;
    //initialize the array of walls and fake walls 
    arr_fake_walls = NULL;
    arr_walls = NULL;

    if (file.is_open()) {

        while (getline(file, line)) {
            stringstream target(line);
            string key, value;  
            getline(target, key, '=');
            getline(target, value);

            if (key == "MAP_NUM_ROWS") {
                map_num_rows = stoi(value);
            }else if(key == "MAP_NUM_COLS"){
                map_num_cols = stoi(value);
            }else if (key == "MAX_NUM_MOVING_OBJECTS"){
                max_num_moving_objects = stoi(value);
            }else if (key == "ARRAY_WALLS"){    
                kill_array_pos(value, arr_walls, true);
            }else if (key == "ARRAY_FAKE_WALLS"){
                kill_array_pos(value, arr_fake_walls, false);
            }else if (key == "SHERLOCK_MOVING_RULE"){
                sherlock_moving_rule = value;
            }else if (key == "SHERLOCK_INIT_POS"){
                kill_pos(value, sherlock_pos);
            }else if (key == "SHERLOCK_INIT_HP"){
                sherlock_hp = stoi(value);
            }else if (key == "SHERLOCK_INIT_EXP"){
                sherlock_exp = stoi(value);
            }else if (key == "WATSON_MOVING_RULE"){
                watson_moving_rule = value;
            }else if (key == "WATSON_INIT_POS"){
                kill_pos(value, watson_pos);
            }else if (key == "WATSON_INIT_HP"){
                watson_hp = stoi(value);
            }else if (key == "WATSON_INIT_EXP"){
                watson_exp = stoi(value);
            }else if (key == "CRIMINAL_INIT_POS"){
                kill_pos(value, criminal_pos);
            }else if (key == "NUM_STEPS"){
                num_steps = stoi(value);
            }
        }
        file.close();
    }
}
Configuration::~Configuration(){
    if(arr_fake_walls != nullptr){  
        delete [] arr_walls;
    }

    if(arr_fake_walls != nullptr){
        delete [] arr_fake_walls;
    }
}
string Configuration::str() const {
    stringstream print;
    
    print << "Configuration[" << endl;
    print << "MAP_NUM_ROWS=" << to_string(map_num_rows) << endl;
    print << "MAP_NUM_COLS=" << to_string(map_num_cols) << endl;
    print << "MAX_NUM_MOVING_OBJECTS=" << to_string(max_num_moving_objects) << endl;
    print << "NUM_WALLS=" << to_string(num_walls) << endl;
    print << "ARRAY_WALLS=[";
    for (int i = 0; i < num_walls; ++i) {
        print << "(" << to_string(arr_walls[i].getRow()) << "," << to_string(arr_walls[i].getCol()) << ")";
        if (i < num_walls - 1) print << ";";
    }
    print << "]" << endl;
    print << "NUM_FAKE_WALLS=" << to_string(num_fake_walls) << endl;
    print << "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < num_fake_walls; ++i) {
        print << "(" << to_string(arr_fake_walls[i].getRow()) << "," << to_string(arr_fake_walls[i].getCol()) << ")";
        if (i < num_fake_walls - 1) print << ";";
    }
    print << "]" << endl;  
    print << "SHERLOCK_MOVING_RULE=" << sherlock_moving_rule << endl;
    print << "SHERLOCK_INIT_POS=" << "(" << to_string(sherlock_pos.getRow()) << "," << to_string(sherlock_pos.getCol()) << ")" << endl;
    print << "SHERLOCK_INIT_HP=" << to_string(sherlock_hp) << endl;
    print << "SHERLOCK_INIT_EXP=" << to_string(sherlock_exp) << endl;
    print << "WATSON_MOVING_RULE=" << watson_moving_rule << endl;
    print << "WATSON_INIT_POS=" << "(" << to_string(watson_pos.getRow()) << "," << to_string(watson_pos.getCol()) << ")" << endl;
    print << "WATSON_INIT_HP=" << to_string(watson_hp) << endl;
    print << "WATSON_INIT_EXP=" << to_string(watson_exp) << endl;
    print << "CRIMINAL_INIT_POS=" << "(" << to_string(criminal_pos.getRow()) << ","  << to_string(criminal_pos.getCol()) << ")" << endl;
    print << "NUM_STEPS=" << to_string(num_steps) << endl;
    print << "]";
    return print.str();
}
void Configuration::kill_array_pos(const string& input, Position*& arr_pos,bool iswall){
    int count_semicolons = 0; 
    for (int i = 0; i < input.length(); i++){
        //loop through the string and count the number of semicolons
        if (input[i] == ','){
            count_semicolons++;
        }
    }
    int num_pos = count_semicolons;

    if(iswall){
        num_walls = num_pos;
    }else{
        num_fake_walls = num_pos;
    }

    if(count_semicolons == 0){
        return;
    }
    arr_pos = new Position[num_pos]; //new array of positions with given slots
    int start = input.find("[") + 1;
    int end = input.find("]");
    string substr = input.substr(start, end - start);
    int pos = 0;

    for (int i =  0; i < num_pos - 1; i++){
        pos = substr.find(";");
        string pair = substr.substr(0, pos);
        int comma_pos = pair.find(",");
        int row = stoi(pair.substr(1, comma_pos - 1));
        int col = stoi(pair.substr(comma_pos + 1, pair.size() - comma_pos - 2));
        arr_pos[i] = Position(row, col);
        substr.erase(0, pos + 1);
    }
    //the last pair
    int comma_pos = substr.find(",");
    int row = stoi(substr.substr(1, comma_pos - 1));
    int col = stoi(substr.substr(comma_pos + 1, substr.size() - comma_pos - 2));    
    arr_pos[num_pos - 1] = Position(row,col);

    //delete[] arr_pos;
    return;
}

void Configuration::kill_pos(const string& input, Position& position){
    int start = input.find("(") + 1;
    int comma_pos = input.find(",");
    position.setRow(stoi(input.substr(start, comma_pos - start)));
    int end = input.find(")");
    position.setCol(stoi(input.substr(comma_pos + 1, end - comma_pos - 1)));
}

//Task 3.10 - Robot
Robot::Robot(int index, const Position & pos, Map * map, Criminal* criminal) : MovingObject(index, pos, map,"Robot"){
    this->criminal = criminal;
    int p = pos.getCol() * pos.getRow();
    int s = two_into_one(p);

    if(s == 1 || s == 0){
        setItem(MAGIC_BOOK, criminal);
    }else if(s == 2 || s == 3){
        setItem(ENERGY_DRINK,criminal);
    }else if(s == 4 || s == 5){
        setItem(EXCEMPTION_CARD,criminal);
    }else if(s == 6 || s == 7){
        setItem(FIRST_AID,criminal);
    }else if(s == 8 || s == 9){
        setItem(PASSING_CARD,criminal);
    }
}

Position getNextPosition(){
return 0;
}
void Robot::move(){
}
string Robot::str() const{
    return 0;
}
int Robot::getDistance()const{
    return 0;
}


Robot* Robot::create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson){
    
    int disCS = distance(criminal->getPreviousPosition(),sherlock->getCurrentPosition());
    int disCW = distance(criminal->getPreviousPosition(),watson->getCurrentPosition());

    Position robot_pos = criminal->getPreviousPosition();
    if(criminal->getCount() % 3 == 0 && criminal->getCount() != 0){
        if (criminal->getCount() == 3){
        return new RobotC(index, robot_pos, map, criminal);
    }else if(disCS > disCW){    
        RobotS* robots =  new RobotS(index, robot_pos, map, criminal, sherlock);
        return robots;
    }
    else if(disCS < disCW){
        RobotW* robotw =  new RobotW(index, robot_pos, map, criminal, watson);
        return robotw;
    }
    else if(disCS == disCW){
        RobotSW* robotsw = new RobotSW(index, robot_pos, map, criminal, sherlock, watson);
        return robotsw;
    }
}
return nullptr;
}

void Robot::setRobotType(RobotType robot_type){
    this->robot_type = robot_type;
}

void Robot::setItem(ItemType itemtype, Criminal* criminal)
{
    switch (itemtype)
    {
    case MAGIC_BOOK:
        item = new MagicBook();
        break;
    case ENERGY_DRINK:
        item = new EnergyDrink();
        break;
    case FIRST_AID:
        item = new FirstAid();
        break;
    case EXCEMPTION_CARD:
        item = new ExcemptionCard();
        break;
    case PASSING_CARD:
        int t = (criminal->getRow()*11 + criminal->getCol()) % 4;
        if(t == 0){
            item = new PassingCard("RobotS");
        }else if(t == 1){
            item = new PassingCard("RobotC");
        }else if(t == 2){
            item = new PassingCard("RobotSW");
        }else if(t == 3){
            item = new PassingCard("all");
        }
        break;
    }
};

int Robot::two_into_one(int p){
    int result;
    string temp = to_string(p);
    if(temp.length() == 1){
        result = p;
    }else{
        result = (p % 10) + (p / 10);
        if(result >= 10){
            result = two_into_one(result);
        }
    }
    return result;
}

BaseItem* Robot::getItem(){
    if (item->getItemType() == MAGIC_BOOK)
        return new MagicBook();
    else if (item->getItemType() == ENERGY_DRINK)
        return new EnergyDrink();
    else if (item->getItemType() == FIRST_AID)
        return new FirstAid();
    else if (item->getItemType() == EXCEMPTION_CARD)
        return new ExcemptionCard();
    else if (item->getItemType() == PASSING_CARD) {
        PassingCard* card = dynamic_cast<PassingCard*>(item);
        return new PassingCard(pos.getCol(), pos.getRow());
    }
        
    else
        return nullptr;
}
//RobotC

RobotC::RobotC ( int index , const Position & pos , Map * map , Criminal * criminal ): Robot(index,pos,map,criminal){};

Position RobotC::getNextPosition(){
    Position next_pos = criminal->getCurrentPosition();
    if (map->isValid(next_pos,this)){
        return next_pos;
    }else{
        return Position::npos;
    
    }
}
void RobotC::move(){
    Position next_pos = getCurrentPosition();
    if (next_pos.isEqual(Position::npos));
        pos = next_pos;
}
int RobotC::getDistance(Sherlock* sherlock){
    int distance = abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
    return distance;
}
int RobotC::getDistance(Watson* watson){
    int distance = abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
    return distance;
}
//avoid abstract class
int RobotC::getDistance() const{
    return 0;
};

string RobotC::str() const{
    return "Robot[pos=" + pos.str() + ";type=" + "C" + ";dist=" + "" + "]";
};

RobotType RobotC::getRobotType() const{
    return RobotType::C;
}

string RobotC::getName(){
    return "RobotC";
}
//RobotS

RobotS::RobotS ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * Sherlock ): Robot(index,pos,map,criminal){
    this->sherlock = sherlock;
}

Position RobotS::getNextPosition() {
    Position next_pos = pos;
    int min_distance = 100;
    Position arr[4];
    arr[0] = Position(pos.getRow(), pos.getCol()+1);  // Up
    arr[1] = Position(pos.getRow()+1, pos.getCol());  // Right
    arr[2] = Position(pos.getRow(), pos.getCol()-1);  // Down
    arr[3] = Position(pos.getRow()-1, pos.getCol());  // Left
    for(int i = 0; i < 4; i++){
        if(map->isValid(arr[i], this)){
            int distance = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol());
            if(distance < min_distance){
                min_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == min_distance){
                continue;
            }
        }
        else continue;
    }
    return next_pos;
}
int RobotS::getDistance() const{
    return distance(this->getCurrentPosition(),sherlock->getCurrentPosition());
}

void RobotS::move(){

    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos));

    pos = next_pos;
}
string RobotS::str() const{
    return "Robot[pos="+pos.str()+"type=S;dist="+to_string(getDistance())+"]";
}

RobotType RobotS::getRobotType() const{
    return S;
}

string RobotS::getName(){
    return "RobotS";
}
//RobotW
Position RobotW::getNextPosition() {
    Position next_pos = pos;
    int min_distance = 100;
    Position arr[4];
    arr[0] = Position(pos.getRow(), pos.getCol()+1);
    arr[1] = Position(pos.getRow()+1, pos.getCol());
    arr[2] = Position(pos.getRow(), pos.getCol()-1);
    arr[3] = Position(pos.getRow()-1, pos.getCol());
    for (int i = 0; i < 4; i++){
        if (map->isValid(arr[i], this)){
            int distance = abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
            if (distance < min_distance){
                min_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == min_distance){
                continue;
            }
        }
        else continue;
    }
    return next_pos;
};
RobotW::RobotW ( int index , const Position & pos , Map * map , Criminal * criminal , Watson * watson ): Robot(index,pos,map,criminal){
    this->watson = watson;
}
int RobotW::getDistance() const{
    int distance = abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
    return distance;
}
string RobotW::str() const{
    return "Robot[pos="+pos.str()+";type="+"SW"+";dist="+to_string(getDistance())+"]";
};

RobotType RobotW::getRobotType() const{
    return W;
}

string RobotW::getName(){
    return "RobotW";
}

//RobotSW

RobotSW::RobotSW ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson* watson ) : Robot(index,pos,map,criminal){
    this->sherlock = sherlock;
    this->watson = watson;
}
Position RobotSW::getNextPosition() {
    Position next_pos = pos;
    int min_distance = 100;
    Position arr[8];
    arr[0] = Position(pos.getRow(), pos.getCol()+2);
    arr[1] = Position(pos.getRow()-1,pos.getCol()+1);
    arr[2] = Position(pos.getRow()+2, pos.getCol());
    arr[3] = Position(pos.getRow()+1, pos.getCol()+1);
    arr[4] = Position(pos.getRow(), pos.getCol()-2);
    arr[5] = Position(pos.getRow()+1, pos.getCol()-1);
    arr[6] = Position(pos.getRow()-2, pos.getCol());
    arr[7] = Position(pos.getRow()-1, pos.getCol()-1);
    for (int i = 0; i < 8; i++){
        if (map->isValid(arr[i], this)){
            int distance = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol()) + abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
            if (distance < min_distance){
                min_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == min_distance){
                continue;
            }   
        }
        else continue;
    }
    return next_pos;
};
int RobotSW::getDistance() const{
    int sherlock_dis,watson_dis;
    sherlock_dis = abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
    watson_dis = abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());

    return sherlock_dis + watson_dis;
}
string RobotSW::str() const{
    return "Robot[pos="+pos.str()+"type="+"SW"+"dist="+to_string(getDistance())+"]";
}

RobotType RobotSW::getRobotType() const{
    return SW;
}

string RobotSW::getName(){
    return "RobotSW";
}
//Task 3.11 - BaseItem
bool BaseItem::canUse(Character * obj, Robot * robot){
    return 0;
}

void Character::setExp(int exp){
    this->exp = exp;
}

void Character::setHp(int hp){
    this->hp = hp;
}
//BaseItem

void BaseItem::setItemType(ItemType item){
    this->item = item;
}
// Class for each item
// MagicBook
ItemType MagicBook::getItemType() const{ 
    return MAGIC_BOOK;
}

bool MagicBook::canUse(Character * obj, Robot * robot){
    if(obj->getExp() < 350 && robot == nullptr){
        return true;
    }
    return false;
}
void MagicBook::use(Character * obj, Robot * robot){
    if(canUse(obj,robot)){
        obj->setExp(ceil(obj->getExp()*125/100));
    }
}

string MagicBook::getName(){
    return "MagicBook";
}
//EnergyDrink
ItemType EnergyDrink::getItemType() const{
    return ENERGY_DRINK;
}
bool EnergyDrink::canUse(Character * obj, Robot * robot){
    if(obj->getHp() < 100 && robot == nullptr){
        return true;
    }
    return false;
}

void EnergyDrink::use(Character * obj, Robot * robot){
    if(canUse(obj,robot)){
        obj->setHp(ceil(obj->getHp() * 120/100));
    }
}

string EnergyDrink::getName(){
    return "EnergyDrink";
}
//FirstAid
ItemType FirstAid::getItemType() const{
    return FIRST_AID;
}
bool FirstAid::canUse(Character * obj, Robot * robot){
    if((obj->getHp() <= 100 || obj->getExp() <= 350)&& robot == nullptr){
        return true;
    }
    return false;
}

void FirstAid::use(Character * obj, Robot * robot){
    if(canUse(obj,robot)){
        obj->setHp(obj->getHp() * 150/100);
    }
}

string FirstAid::getName(){
    return "FirstAid";
}
//ExcemptionCard
ItemType ExcemptionCard::getItemType() const{
    return EXCEMPTION_CARD;
}
bool ExcemptionCard::canUse(Character * obj, Robot * robot){
    if(obj->getExp() % 2 == 1 && obj->getCharacterType() == SHERLOCK){
        return true;
    }
    return false;
}

void ExcemptionCard::use(Character * obj, Robot * robot){
    int temp_hp = obj->getHp();
    int temp_exp = obj->getExp();
    if(canUse(obj,robot)){
        obj->setExp(obj->getExp());
        obj->setHp(obj->getHp());
    }
}

string ExcemptionCard::getName(){
    return "ExcemptionCard";
}
//PassingCard
PassingCard::PassingCard(int a,int b){
    int t = (a * 11 + b) % 4;
    if(t == 0){
        challenge == "ROBOTS";
    }
    else if(t==1){
        challenge == "ROBOTC";
    }
    else if(t==2){
        challenge == "ROBOTSW";
    }
    else if(t==3){
        challenge == "ALL";
    }
}
PassingCard::PassingCard(string challenge){
    this->challenge = challenge;
}
ItemType PassingCard::getItemType() const{
    return PASSING_CARD;
}

bool PassingCard::canUse(Character * obj, Robot * robot){
    if(obj->getExp() % 2 == 0 && obj->getCharacterType() == WATSON){
        return true;
    }
    return false;
}
void PassingCard::use(Character * obj, Robot * robot){
    if(canUse(obj, robot)){
        if(challenge == "ROBOTS"){
            if(robot->getRobotType() == S){
                obj->setHp(obj->getHp());
                obj->setExp(obj->getExp());
            }
        }
        else if(challenge == "ROBOTC"){
            if(robot->getRobotType() == C){
                obj->setExp(obj->getExp());
                obj->setHp(obj->getHp());
            }
        }
        else if(challenge == "ROBOTSW"){
            if(robot->getRobotType() == SW){
                obj->setExp(obj->getExp());
                obj->setHp(obj->getHp());
            }
        }
        else if(challenge == "ALL"){
            obj->setExp(obj->getExp());
            obj->setHp(obj->getHp());
        }
        else 
            obj->setHp(obj->getHp() - 50);
    }
}

string PassingCard::getName(){
    return "PassingCard";
}
//Task 3.12 - BaseBag
BaseBag::Node::Node(BaseItem* item, Node* next): item(item){};

BaseBag::BaseBag(int capacity){
    this->capacity = capacity;
    size = 0;
    head = nullptr;
}

BaseBag::~BaseBag() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current->item; // Assuming ownership of items
        delete current;
        current = nextNode;
    }
}

bool BaseBag::insert(BaseItem* item){
    if(item == nullptr){
        return false;
    }
    Node* temp = new Node(item);
    if(size == 0){
        head = temp;
    }
    else{
        if(size < capacity){
            temp->next = head;
            head = temp;
        }
        else
        { 
            return false;
        }
    }
    size++;
    delete temp;
    return true;
}

BaseItem* BaseBag::get(){
    Node* current = head;
    Node* prev = nullptr;
    while (current != nullptr) {
        if ((current->item->getItemType() == MAGIC_BOOK || current->item->getItemType() == ENERGY_DRINK || current->item->getItemType() == FIRST_AID)){
            if (prev != nullptr) {
                prev->next = head;
                Node* temp = head->next;
                head->next = current->next;
                current->next = temp;
                head = temp;
                delete temp;
            } else {
                head = current->next;
            }
            BaseItem* item = current->item;
            delete current;
            delete prev;
            size--;
            return item;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}

BaseItem* BaseBag::get(ItemType itemtype) {
    Node* current = head;
    Node* prev = nullptr;
    while (current != nullptr) {
        if (current->item->getItemType() == itemtype) {
            if (prev != nullptr) {
                prev->next = head;
                Node* temp = head->next;
                head->next = current->next;
                current->next = temp;
                head = temp;
                delete temp;
            } else {
                head = current->next;
            }
            BaseItem* item = current->item;
            delete current;
            delete prev;
            size--;
            return item;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}

BaseItem* BaseBag(ItemType itemtype){
        return nullptr;
}

string BaseBag::str() const {
    stringstream ss;
    ss << "Bag[count=" << to_string(size) << ";";
    Node* current = head;
    for(int i = 0; i < size;i++){
        ss << (current->item->getName());
        current = current->next;
        if (i != size - 1) {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}

bool BaseBag::checkItem(ItemType itemtype) {
    Node* current = head;
    while (current != nullptr) {
        if (current->item->getItemType() == itemtype) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool BaseBag::isFull() const {
    return size >= capacity;
}

void BaseBag::setCapacity(int capacity) {
    this->capacity = capacity;
}
//SherlockBag
SherlockBag::SherlockBag(Sherlock* sherlock) : BaseBag(13), sherlock(sherlock) {};

int SherlockBag::getCapacity(){ 
    return capacity;
}

void SherlockBag::setCapacity(int capacity){
    this->capacity = capacity;
}

BaseItem* SherlockBag::get(){
    Node *current = head;
    while (current != nullptr)
    {
        if (current->item != nullptr)
        {
            if(current->item->getItemType() == PASSING_CARD || current->item->getItemType() == ENERGY_DRINK || current->item->getItemType() == FIRST_AID){
                if (current->item->canUse(sherlock, nullptr))
                {
                    BaseItem *tempitem = current->item;
                    current->item = head->item;
                    head->item = tempitem;
                    Node *temp = head;
                    head = head->next;
                    delete temp;
                    return tempitem;
                }
            }
        }
        current = current->next;
    }
    return nullptr;
}


BaseItem* SherlockBag::get(ItemType itemtype){
     Node* current = head;
    Node* prev = nullptr;
    while (current != nullptr) {
        if (current->item->getItemType() == itemtype) {
            if (prev != nullptr) {
                prev->next = head;
                Node* temp = head->next;
                head->next = current->next;
                current->next = temp;
                head = temp;
                delete temp;
            } else {
                head = current->next;
            }
            BaseItem* item = current->item;
            delete current;
            delete prev;
            size--;
            return item;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}
//WatsonBag
WatsonBag::WatsonBag(Watson* watson) : BaseBag(15), watson(watson) {};
int WatsonBag::getCapacity(){
    return capacity;
}

void WatsonBag::setCapacity(int capacity){
    this->capacity = capacity;
}

BaseItem* WatsonBag::get(){
    Node *current = head;
    while (current != nullptr)
    {
        if (current->item != nullptr)
        {
            if (current->item->canUse(watson, nullptr))
            {
                BaseItem *tempitem = current->item;
                current->item = head->item;
                head->item = tempitem;
                Node *temp = head;
                head = head->next;
                delete temp;
                return tempitem;
            }
        }
        current = current->next;
    }
    return nullptr;
}

BaseItem* WatsonBag::get(ItemType itemtype){
    Node* current = head;
    Node* prev = nullptr;
    while (current != nullptr) {
        if (current->item->getItemType() == itemtype) {
            if (prev != nullptr) {
                prev->next = head;
                Node* temp = head->next;
                head->next = current->next;
                current->next = temp;
                head = temp;
                delete temp;
            } else {
                head = current->next;
            }
            BaseItem* item = current->item;
            delete current;
            delete prev;
            size--;
            return item;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}
//StudyinPinkProgram
StudyPinkProgram::StudyPinkProgram(const string & config_file_path){
    this->config = new Configuration(config_file_path);
    this->map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    this->sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_pos, map, config->sherlock_hp, config->sherlock_exp);
    this->watson = new Watson(2, config->watson_moving_rule, config->watson_pos, map, config->watson_hp, config->watson_exp);
    this->criminal = new Criminal(0, config->criminal_pos, map, sherlock, watson);
    this->arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    
    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
}

StudyPinkProgram::~StudyPinkProgram(){
    delete config;
    delete map;
    delete arr_mv_objs;
}
bool StudyPinkProgram::isStop() const{
    if(sherlock->getHp() <= 0 || watson->getHp() <= 0 || criminal->getCurrentPosition().isEqual(sherlock->getCurrentPosition()) || criminal->getCurrentPosition().isEqual(watson->getCurrentPosition())){
        return true;
    }
    return false;
}

void StudyPinkProgram::printInfo(int si, int i, ofstream &OUTPUT) 
    {
        OUTPUT << endl
               << "*************AFTER MOVE*************" << endl;
        OUTPUT
            << "ROUND : " << si << " - TURN : " << i << endl;
        stringstream ss(arr_mv_objs->str());
        string lineArr = "";
        getline(ss, lineArr, 'C');
        OUTPUT << lineArr <<  endl; //"]" << endl;
        getline(ss, lineArr, ']');
        OUTPUT << "\tC" << lineArr << "]" << endl;
        while (getline(ss, lineArr, ']'))
        {
            if (lineArr.length() > 0)
                OUTPUT << "\t" << lineArr.substr(1) << "]" << endl;
        }
        OUTPUT << "Sherlock HP_" << sherlock->getHp() << " EXP_" << sherlock->getExp() << endl
               << "Watson HP_" << watson->getHp() << " EXP_" << watson->getExp() << endl
               << "SherlockBag : " << sherlock->getBag()->str() << endl
               << "WatsonBag : " << watson->getBag()->str() << endl;
    }


// void StudyPinkProgram::run(bool verbose, ofstream &OUTPUT)
// {
//     if (isStop())
//     {
//         cout << "stopped" << endl;
//         printStep(0);
//         printInfo(0, 0, OUTPUT);
//         return;
//     }
//     else
//     {
//         cout << "not stopped" << endl;
//     }
//     for (int istep = 0; istep < config->num_steps; ++istep)
//     {
//         cout << "ROUND----------------------" << istep << endl;
//         for (int i = 0; i < arr_mv_objs->size(); ++i)
//         {
//             cout << "TURN------------- " << i << endl;
//             arr_mv_objs->get(i)->move();
//             cout << "name " << arr_mv_objs->get(i)->getName() << endl;
//             arr_mv_objs->checkMeet(i);
//             if (isStop())
//             {
//                 cout << "stopped in the inner loop" << endl;
//                 printStep(istep);
//                 printInfo(istep, i, OUTPUT);
//                 return;
//             }
//             else
//             {
//                 cout << "arr_mv_objs->get(i)->getObjectType() : " << arr_mv_objs->get(i)->getCharacterType() << endl;
//                 cout << "not stopped in the inner loop" << endl;
//                 MovingObject *robot;
//                 if (arr_mv_objs->get(i)->getCharacterType() == CharacterType::CRIMINAL)
//                 {
//                         robot = Robot::create(arr_mv_objs->size(), map, criminal, sherlock, watson);
//                         cout << "name of this fking robot : " << endl;
//                         cout << robot->getName() << endl;
//                         cout << robot->getName() << endl;
//                         if (robot != NULL)
//                         {
//                             // cout << "name::::::: " << robot->getName() << endl;
//                             cout << "hehe " << endl;
//                             cout << "criminal->getRobotCcount() : " << criminal->getCount() << endl;
//                             // cout << "robot->getName() : " << robot->getName() << endl;
//                             // cout << robot->getType() << endl;
//                             if (robot->getName() == "C" && criminal->getCount() > 1)
//                             {
//                                 cout << "cant spwawn more than 1 robotC" << endl;
//                                 arr_mv_objs->add(NULL);
//                             }
//                             else
//                             {
//                                 cout << "spawning robotCCCCCCCCCCCCC " << endl;
//                                 arr_mv_objs->add(robot);
//                             }
//                         }
//                         else
//                         {
//                             cout << "Robot not created" << endl;
//                         }
                    
//                 }
//             }
//             printInfo(istep, i, OUTPUT);
//         }
//     }
// }

// void StudyPinkProgram::run(bool verbose, ofstream &OUTPUT){
//     if (isStop()) {
//         return;
//     }

//     for (int istep = 0; istep < config->num_steps; ++istep) {
//         int roundsize=arr_mv_objs->size();
//         for (int i = 0; i < roundsize; ++i) {
//             arr_mv_objs->get(i)->move();
//             if (isStop()) {
//                 printInfo(istep, i, OUTPUT);
//                 return;
//             }
//             arr_mv_objs->checkMeet(i);
//             //end game
//             //create robot
//             Robot* new_robot = Robot::create(arr_mv_objs->size(), map, criminal, sherlock, watson);
//             if (new_robot != nullptr) {
//                 arr_mv_objs->add(new_robot);
//             }

//             if (isStop()) {
//                 printInfo(istep, i, OUTPUT);
//                 return;
//             }

//             if (verbose) {
//                 printInfo(istep, i, OUTPUT);
//             }
//         }
//     }
// }

void StudyPinkProgram::run(bool verbose, ofstream &OUTPUT){
        for (int istep = 0; istep < config->num_steps; ++istep) {
            int roundsize=arr_mv_objs->size();
            if(isStop()){
                return;
            }
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
                // move each moving object
                arr_mv_objs->get(i)->move();
                if (isStop()) {
                    printInfo(istep, i, OUTPUT);
                    return;
                }
                //check tao robot
                if(arr_mv_objs->get(i)->getName()=="Criminal"){
                    Robot* robot=Robot::create(arr_mv_objs->size(),map,criminal,sherlock,watson);
                    if(robot!=NULL){
                        arr_mv_objs->add(robot);
                        // roundsize=arr_mv_objs->size();
                    }
                    else{
                        delete robot;
                    }
                }
                // check meet
                bool a= arr_mv_objs->checkMeet(i);
                if (a) {
                    printInfo(istep, i, OUTPUT);
                    return;
                }
                if (isStop()) {
                    printInfo(istep, i, OUTPUT);
                    return;
                }
                // if (verbose) {
                //     printInfo(istep, i, OUTPUT);
                // }
                printInfo(istep, i, OUTPUT);
            }
        }
    };



////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
///////////////////////////////////////////////