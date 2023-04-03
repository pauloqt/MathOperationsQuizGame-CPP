#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <windows.h>
#include<conio.h>
#include <thread>
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

//ALCANTARA, JOHN PAULO C.
//LEONEN, ABBEGAIL MILES
//BSCS-NS-1B

//This is a program that generates quizzes on fundamental operations
//It features a leaderboards where players can see their ranks based on overall score and score per operation
//Players can save their account and scores

using namespace std;

typedef struct node{    //Main Node- dito nasasalin ang data from retrieve
    string userId, pass;
    float add=0, sub=0, mul=0, div=0, total=0;
    struct node *next;
}NODE;

typedef struct addNode{ //Add Node- linkedlist ng Addition operation na sorted based on score
    string userId;
    float add=0;
    struct addNode *next;
}ADDNODE;

typedef struct subNode{//Sub Node- linkedlist ng Subtraction operation na sorted based on score
    string userId;
    float sub=0;
    struct subNode *next;
}SUBNODE;

typedef struct mulNode{ //Mul Node- linkedlist ng Multiplicaton operation na sorted based on score
    string userId;
    float mul=0;
    struct mulNode *next;
}MULNODE;

typedef struct divNode{ //Div Node- linkedlist ng Division operation na sorted based on score
    string userId;
    float div=0;
    struct divNode *next;
}DIVNODE;

class MyClass{
    private:
        NODE *head, *q, *p, *newNode; //declared node variables
        NODE user; //created a struct variable where initial infos from retrieve and log in will be stored.
        ADDNODE *headAdd, *qAdd, *pAdd, *newNodeAdd; //gumawa ng new pointers ng struct ADDNODE kung san i-store ang sorted score para sa add.
        SUBNODE *headSub, *qSub, *pSub, *newNodeSub;
        MULNODE *headMul, *qMul, *pMul, *newNodeMul;
        DIVNODE *headDiv, *qDiv, *pDiv, *newNodeDiv;
        int a, b, c, ans, score, i, lvl, range, choice;
        float calculateGrade(int x);
        void add();
        void addAdd(); void addSub(); void addMul(); void addDiv();
        void delAdd(); void delSub(); void delMul(); void delDiv(); void del();

    public:
        void logIn();
        void addition();
        void subtraction();
        void multiplication();
        void division();
        void leaderboards();
        void leaderboardsOp();
        void askLevel();
        void save();
        void retrieve();
        void init();
        void callOperations();
        int menu();

};

void gotoxy(int x,int y);
//void displayTime();

int main()
{
    srand(time(NULL)); //for randome number generator
    MyClass obj; //declaring an object of MyClass. ito ang gagamitin with (.) operator para ma-access ang functions/variables ng MyClass.
    obj.init(); //initialize head
    obj.retrieve(); //gumawa na ng node, nasalin na sa linkedlist mga existing account
    obj.logIn();
    system("cls");
    obj.callOperations();
    obj.leaderboards();
    system("cls");
    obj.leaderboardsOp();
    while(1){
        system("cls");
        switch(obj.menu()){
            case 1: system("cls"); obj.askLevel(); obj.addition(); break;
            case 2: system("cls"); obj.askLevel(); obj.subtraction(); break;
            case 3: system("cls"); obj.askLevel(); obj.multiplication(); break;
            case 4: system("cls"); obj.askLevel(); obj.division(); break;
            case 5: system("cls"); obj.leaderboards(); system("cls"); obj.leaderboardsOp(); break;
            case 6: system("cls"); obj.save(); exit(101);
        }
    }
}

void MyClass:: logIn(){
    //Ask users about account info, and checks if account is new or existing.
    //Kapag di pa existing, automatic na mag-create ng account
    //Kapag existing na, iko-compare ang password until mag-match.

    q=p=head;
    user.add=0; user.sub=0; user.mul=0; user.div=0; user.total=0; //initializes the value of user.operation para kapag isasalin na sa linked list, 0 value.

    gotoxy(40,4); cout <<"+++++++++++++++++++++++++++++";
    gotoxy(50,5); cout <<"LOG IN";
    gotoxy(40,6); cout <<"+++++++++++++++++++++++++++++";
    gotoxy(40,8); cout <<"Enter user ID: ";
    getline(cin, user.userId);
    gotoxy(40,9); cout <<"Enter password: ";
    getline(cin, user.pass);

    while(p!=NULL && p->userId != user.userId){ //hinhanap account kung existing na
        q=p;
        p=p->next;
    }

    if(p==NULL){ //di pa existing
        gotoxy(40,11); cout <<"Account Created successfully.\n"; // di pa existing, print account created
        add();
    }

    else{ //existing na, tatanungin password.
        while(user.pass != p->pass){
            system("cls");
            gotoxy(40,7); cout<< "Incorrect Password";
            gotoxy(40,8); cout <<"LOG IN";
            gotoxy(40,9); cout <<"Enter user ID: ";
            gotoxy(60,9); cout << p->userId;
            gotoxy(40,10); cout <<"Enter password: ";
            gotoxy(60,10);getline(cin, user.pass);
        }
        gotoxy(40,11); cout <<"Log In successfully.\n";
        user.add= p->add; user.sub= p->sub; user.mul= p->mul; user.div= p->div; //sets scores ng account logged in
        delAdd(); delSub(); delMul(); delDiv(); //naglagay ng delete ng node operations dito para yung mga exisiting ay hindi madoble dahil tinawag ang callOperations after log in.
    }
    gotoxy(40,12); system("pause");
}

void MyClass:: add(){
    //Nilalagay sa main linked list ang account info from retrieve at log in.
    //Sino-sort ang overall scores at nilalagay sa main linkedlist.

    p=q=head;
    newNode= new NODE; //"new" operator allocates memoryspace to new node.
    newNode->userId= user.userId;//-> is used for pointer to point structure members
    newNode->pass= user.pass;
    newNode->add= user.add; newNode->sub= user.sub; newNode->mul= user.mul; newNode->div= user.div;
    newNode->total= user.total;

    while(p!=NULL && newNode->total < p->total){ //q and p ay gagapang until makarating sa dulo or mahanap saan largest si n->name
        q=p;
        p=p->next;
    } //head-> q-> newNode-> p

    if(p==head){    //kapag wala pang laman
        head=newNode; //first node or head will have the newNode.
    }

    else{   //kapag may laman na.
        q->next=newNode; //either sa dulo or isisingit si newNode
    }

    newNode->next=p; //dapat lagyan yung kasunod. Kapag dulo null, kapag hindi, anuman value ni p.
    //head-> q-> newNode-> p

}

void MyClass:: callOperations(){
    //Tinatawag lang ang mga functions na magsasalin ng info to operation nodes/linkedlist

    addAdd();
    addSub();
    addMul();
    addDiv();
}


void MyClass:: addition(){
    //Generates random number from specific range based on level
    //Shows Score and grade
    //Calls add() and del() function for addition.

    //std::thread first(displayTime);
    system("cls");
    gotoxy(46,4); cout << "+++++++++++++++++++++++++++++++++++";
    gotoxy(60,5); cout <<"ADDITION";
    gotoxy(46,6); cout << "+++++++++++++++++++++++++++++++++++";
    score= 0;
    for(i=1; i<=10; i++){
        a= rand() %range +1;
        b= rand() %range +1;
        gotoxy(45,8+i); cout <<i <<".) " <<a <<" + " <<b;
        gotoxy(55,8+i);cout<<"   = ";
        gotoxy(60,8+i); cin >>ans;
        if(ans==(a+b)){gotoxy(70,8+i); cout <<"Correct!"; score++;}
        else{gotoxy(70,8+i);cout <<"Incorrect! Answer: " <<a+b <<"\n";}
    }

    gotoxy(45,20); cout <<"Your score: " <<score;
    user.add= calculateGrade(score); //assign yung grase sa user.add
    gotoxy(45,21); cout <<"Your Grade: " <<user.add <<"\n";
    user.add+= newNodeAdd->add; //previous score sa add + new score
    delAdd(); //dedelete yung previous account
    addAdd(); //ilalagay na sa addlinkedlist yung new version ng account.
    gotoxy(45, 23); system("pause");
}

void MyClass:: addAdd(){
    //kukuhanin ang account info at score sa main then isasalin sa nodes ng addition
    //Nagso-sort based on addition scores.

    pAdd=qAdd=headAdd; //set lahat sa unang node
    newNodeAdd= new ADDNODE;// allocate ng space kay newNodeAdd
    newNodeAdd->userId= user.userId; //salin data ng account sa addlinkedlist.
    newNodeAdd->add= user.add; //salin ang addscore sa addlinkedlist
    newNode->add=newNodeAdd->add; //set sa same value ang mainlinkedlist at addlinkedlist

    while(pAdd!=NULL && newNodeAdd->add <= pAdd->add){ //q and p ay gagapang until makarating sa dulo or mahanap saan lowest si score sa add
        qAdd=pAdd;
        pAdd=pAdd->next;}

    if(pAdd==headAdd){    //kapag wala pang laman
        headAdd=newNodeAdd;} //first node or head will have the newNode.

    else{   //kapag may laman na.
        qAdd->next=newNodeAdd;} //either sa dulo or isisingit si newNode

    newNodeAdd->next=pAdd; //dapat lagyan yung kasunod. Kapag dulo null, kapag hindi, anuman value ni p.
    //head-> q-> newNode-> p
}

void MyClass:: subtraction(){
    //Generates random number from specific range based on level
    //Shows Score and grade
    //the second number is always less than or equal to the first number, para hindi mag-negative answer.
    //Calls add() and del() function for subtraction.

    system("cls");
    gotoxy(46,4); cout << "+++++++++++++++++++++++++++++++++++";
    gotoxy(57,5); cout <<"SUBTRACTION";
    gotoxy(46,6); cout << "+++++++++++++++++++++++++++++++++++";
    score= 0;
    for(i=1; i<=10; i++){
        a= rand() %range +1;
        b= rand() %a +1;
        gotoxy(45,8+i); cout <<i <<".) " <<a <<" - " <<b;
        gotoxy(55,8+i); cout<<"   = ";
        gotoxy(60,8+i); cin >>ans;
        if(ans==(a-b)){gotoxy(70,8+i);cout <<"Correct!"; score++;}
        else{gotoxy(70,8+i);cout <<"Incorrect! Answer: " <<a-b <<"\n";}
    }
    gotoxy(45,20); cout <<"Your score: " <<score;
    user.sub= calculateGrade(score); //assign yung grase sa user.add
    gotoxy(45,21); cout <<"Your Grade: " <<user.sub <<"\n";
    user.sub+= newNodeSub->sub; //previous score sa add + new score
    delSub(); //dedele yung previous account
    addSub(); //ilalagay na sa addlinkedlist yung new version ng account.
    gotoxy(45, 23); system("pause");
}

void MyClass:: addSub(){
    //kukuhanin ang account info at score sa main then isasalin sa nodes ng subtration
    //Nagso-sort based on subtraction scores.

    pSub=qSub=headSub; //set lahat sa unang node
    newNodeSub= new SUBNODE;// allocate ng space kay newNodeSub
    newNodeSub->userId= user.userId; //salin data ng account sa sublinkedlist.
    newNodeSub->sub= user.sub; //salin ang addscore sa sublinkedlist
    newNode->sub=newNodeSub->sub; //set sa same value ang mainlinkedlist at sublinkedlist

    while(pSub!=NULL && newNodeSub->sub <= pSub->sub){ //q and p ay gagapang until makarating sa dulo or mahanap saan lowest si score sa sub
        qSub=pSub;
        pSub=pSub->next;}

    if(pSub==headSub){    //kapag wala pang laman
        headSub=newNodeSub;} //first node or head will have the newNode.

    else{   //kapag may laman na.
        qSub->next=newNodeSub;} //either sa dulo or isisingit si newNode

    newNodeSub->next=pSub; //dapat lagyan yung kasunod. Kapag dulo null, kapag hindi, anuman value ni p.
    //head-> q-> newNode-> p
}

void MyClass:: multiplication(){
    //Generates random number from specific range based on level
    //Shows Score and grade
    //Calls add() and del() function for subtraction.

    system("cls");
    gotoxy(46,4); cout << "+++++++++++++++++++++++++++++++++++";
    gotoxy(57,5); cout <<"MULTIPLICATION";
    gotoxy(46,6); cout << "+++++++++++++++++++++++++++++++++++";
    score= 0;
    for(i=1; i<=10; i++){
        a= rand() %range +1;
        b= rand() %range +1;
        gotoxy(45,8+i); cout <<i <<".) " <<a <<" x " <<b;
        gotoxy(55,8+i); cout<<"   = ";
        gotoxy(60,8+i); cin >>ans;
        if(ans==(a*b)){gotoxy(70,8+i); cout <<"Correct!"; score++;}
        else{gotoxy(70,8+i); cout <<"Incorrect! Answer: " <<a*b <<"\n";}
    }
    gotoxy(45,20); cout <<"Your score: " <<score;
    user.mul= calculateGrade(score); //assign yung grase sa user.add
    gotoxy(45,21); cout <<"Your Grade: " <<user.mul <<"\n";
    user.mul+= newNodeMul->mul; //previous score sa add + new score
    delMul(); //dedele yung previous account
    addMul(); //ilalagay na sa addlinkedlist yung new version ng account.
    gotoxy(45, 23); system("pause");
}

void MyClass:: addMul(){ //hahanapin sorted position, and ia-add sa linkedlist ADD.
    //kukuhanin ang account info at score sa main then isasalin sa nodes ng subtration
    //Nagso-sort based on multiplication scores.

    pMul=qMul=headMul; //set lahat sa unang node
    newNodeMul= new MULNODE;// allocate ng space kay newNodeMul
    newNodeMul->userId= user.userId; //salin data ng account sa mullinkedlist.
    newNodeMul->mul= user.mul; //salin ang mulscore sa mullinkedlist
    newNode->mul=newNodeMul->mul; //set sa same value ang mainlinkedlist at mullinkedlist

    while(pMul!=NULL && newNodeMul->mul <= pMul->mul){ //q and p ay gagapang until makarating sa dulo or mahanap saan lowest si score sa add
        qMul=pMul;
        pMul=pMul->next;}

    if(pMul==headMul){    //kapag wala pang laman
        headMul=newNodeMul;} //first node or head will have the newNode.

    else{   //kapag may laman na.
        qMul->next=newNodeMul;} //either sa dulo or isisingit si newNode

    newNodeMul->next=pMul; //dapat lagyan yung kasunod. Kapag dulo null, kapag hindi, anuman value ni p.
    //head-> q-> newNode-> p
}

void MyClass:: division(){
    //Generates random number from specific range based on level
    //Shows Score and grade
    //the second number is always a multiple of the first number para divisible sila
    //Calls add() and del() function for division.

    system("cls");
    gotoxy(46,4); cout << "+++++++++++++++++++++++++++++++++++";
    gotoxy(60,5); cout <<"DIVISION";
    gotoxy(46,6); cout << "+++++++++++++++++++++++++++++++++++";
    score= 0;
    for(i=1; i<=10; i++){
        a= rand() %range +1;
        c= rand() %range +1;
        while(a*c> range*2){
            c--;
        }
        b=a*c;
        gotoxy(45,8+i); cout <<i <<".) " <<b <<" / " <<a;
        gotoxy(55,8+i); cout<<"   = ";
        gotoxy(60,8+i); cin >>ans;
        if(ans==(b/a)){gotoxy(70,8+i); cout <<"Correct!"; score++;}
        else{gotoxy(70,8+i); cout <<"Incorrect! Answer: " <<b/a <<"\n";}
    }
    gotoxy(45,20); cout <<"Your score: " <<score;
    user.div= calculateGrade(score); //assign yung grase sa user.add
    gotoxy(45,21); cout <<"Your Grade: " <<user.div <<"\n";
    user.div+= newNodeDiv->div; //previous score sa add + new score
    delDiv(); //dedele yung previous account
    addDiv(); //ilalagay na sa addlinkedlist yung new version ng account.
    gotoxy(45, 23); system("pause");
}

void MyClass:: addDiv(){
    //kukuhanin ang account info at score sa main then isasalin sa nodes ng division
    //Nagso-sort based on division scores.

    pDiv=qDiv=headDiv; //set lahat sa unang node
    newNodeDiv= new DIVNODE;// allocate ng space kay newNodeDiv
    newNodeDiv->userId= user.userId; //salin data ng account sa divlinkedlist.
    newNodeDiv->div= user.div; //salin ang divscore sa addlinkedlist
    newNode->div=newNodeDiv->div; //set sa same value ang mainlinkedlist at divlinkedlist

    while(pDiv!=NULL && newNodeDiv->div <= pDiv->div){ //q and p ay gagapang until makarating sa dulo or mahanap saan lowest si score sa add
        qDiv=pDiv;
        pDiv=pDiv->next;}

    if(pDiv==headDiv){    //kapag wala pang laman
        headDiv=newNodeDiv;} //first node or head will have the newNode.

    else{   //kapag may laman na.
        qDiv->next=newNodeDiv;} //either sa dulo or isisingit si newNode

    newNodeDiv->next=pDiv; //dapat lagyan yung kasunod. Kapag dulo null, kapag hindi, anuman value ni p.
    //head-> q-> newNode-> p
}


void MyClass:: leaderboards(){
int i=1;
    //displays the overall leaderboard
    //calculates the overall score
    //deletes the previous node in main, and add a new one para ma-sort and save ang new score.

    user.total= user.add + user.sub + user.mul + user.div;
    del();
    add();

    p=head;
    while(p!=NULL){
        //cout <<i++ <<".)" <<p->userId <<"\t" <<p->add <<"\t" <<p->sub <<"\t" <<p->mul <<"\t" <<p->div <<"\t" <<p->total <<endl;
        gotoxy(60,3); printf(BCYN R"EOF(
                                         _       _   _  _   _   _        _   _   __
                                     |  |_  /\  | \ |_ |_) |_) / \  /\  |_) | \ (_
                                     |_ |_ /--\ |_/ |_ | \ |_) \_/ /--\ | \ |_/ __) )EOF" BWHT);

        gotoxy(40,8); cout<<"+++++++++++++++++++++++++++++++++++++++++";
        gotoxy(40,9); cout<<"|"; gotoxy(80,9); cout<<"|";
        gotoxy(40,10); cout<<"|"; gotoxy(80,9); cout<<"|";
        gotoxy(40,9+i); cout<<"|"; gotoxy(80,9+i); cout<<"|";
        gotoxy(43,9+i); cout<<i++ <<".)"<<p->userId;
        gotoxy(70,8+i); cout<<p->total;
        p=p->next;
    }
    gotoxy(40,9+i+1); cout<<"++++++++++++++++++++++++++++++++++++++++\n";
     gotoxy(40,10+i+1);system("pause");

}

void MyClass:: leaderboardsOp(){
    //displays the leaderboard of each operation

ADDNODE *pAdd;
SUBNODE *pSub;
MULNODE *pMul;
DIVNODE *pDiv;
int i=1;

    gotoxy(60,1); printf(BCYN R"EOF(
                                         _       _   _  _   _   _        _   _   __
                                     |  |_  /\  | \ |_ |_) |_) / \  /\  |_) | \ (_
                                     |_ |_ /--\ |_/ |_ | \ |_) \_/ /--\ | \ |_/ __) )EOF" BWHT);
    gotoxy(1,5); cout<<"____________________________________________________________________________________________________";
    gotoxy(8,6); cout <<"ADDITION";
    pAdd=headAdd;
    /*for(i=1; i!=4; i++){
        gotoxy(5, 5+i); cout <<"|"; gotoxy(35, 3+i); cout <<"|";
    }*/

    gotoxy(1,6); cout <<"|"; gotoxy(25,6); cout <<"|";
    gotoxy(1,7); cout <<"|"; gotoxy(25,7); cout <<"|";
    gotoxy(1,8); cout <<"|"; gotoxy(25,8); cout <<"|";
    gotoxy(1,9); cout <<"|"; gotoxy(25,9); cout <<"|";
    gotoxy(1,10); cout <<"|"; gotoxy(25,10); cout <<"|";

    while(pAdd!=NULL){
        gotoxy(3,8+i); cout <<i++ <<".)" <<pAdd->userId;
        gotoxy(20,7+i); cout <<pAdd->add;
        gotoxy(1,8+i); cout <<"|"; gotoxy(25,8+i); cout <<"|";
        pAdd=pAdd->next;
    }

    gotoxy(1,7); cout<<"___________________________________________________________________________________________________";
    gotoxy(33,6); cout <<"SUBTRACTION";
    i=1;
    pSub=headSub;
    gotoxy(50,6); cout <<"|";
    gotoxy(50,7); cout <<"|";
    gotoxy(50,8); cout <<"|";
    gotoxy(50,9); cout <<"|";
    gotoxy(50,10); cout <<"|";
    while(pSub!=NULL){
        gotoxy(28,8+i); cout <<i++ <<".)" <<pSub->userId;
        gotoxy(45,7+i); cout <<pSub->sub;
        gotoxy(50,8+i); cout <<"|";
        pSub=pSub->next;
    }

    gotoxy(56,6); cout <<"MULTIPLICATION";
    i=1;
    pMul=headMul;
    gotoxy(1,7); cout <<"|"; gotoxy(25,7); cout <<"|";
    gotoxy(75,6); cout <<"|";
    gotoxy(75,7); cout <<"|";
    gotoxy(75,8); cout <<"|";
    gotoxy(75,9); cout <<"|";
    gotoxy(75,10); cout <<"|";
    while(pMul!=NULL){
        gotoxy(53,8+i); cout <<i++ <<".)" <<pMul->userId;
        gotoxy(70,7+i); cout <<pMul->mul;
        gotoxy(75,8+i); cout <<"|";

        pMul=pMul->next;
    }

    gotoxy(83,6); cout <<"DIVISION";
    i=1;
    pDiv=headDiv;
    gotoxy(100,6); cout <<"|";
    gotoxy(100,7); cout <<"|";
    gotoxy(100,8); cout <<"|";
    gotoxy(100,9); cout <<"|";
    gotoxy(100,10); cout <<"|";
    while(pDiv!=NULL){
        gotoxy(78,8+i); cout <<i++ <<".)" <<pDiv->userId;
        gotoxy(95,7+i); cout <<pDiv->div;
        gotoxy(100,8+i); cout <<"|";
        pDiv=pDiv->next;
    }
    gotoxy(1,8+i+1); cout<<"____________________________________________________________________________________________________";
    gotoxy(10,20); system("pause");
}

void MyClass:: delAdd(){
    //deletes the previous account of the account logged in in add linkedlist.
ADDNODE *qAdd, *pAdd;

    qAdd=pAdd=headAdd; //set sa una
    while(pAdd!=NULL && pAdd->userId != user.userId){//hahanapin ang previous account ni user
        qAdd=pAdd;
        pAdd=pAdd->next;}

    if(pAdd==headAdd){ //if head hinahanap
        headAdd=pAdd->next;} //assigns the next head.

    else{
        qAdd->next= pAdd->next; }

    free(pAdd); //deallocates memoryspace of p.
}

void MyClass:: delSub(){
    //deletes the previous account of the account logged in in sub linkedlist.
SUBNODE *qSub, *pSub;

    qSub=pSub=headSub; //set sa una
    while(pSub!=NULL && pSub->userId != user.userId){//hahanapin ang previous account ni user
        qSub=pSub;
        pSub=pSub->next;}

    if(pSub==headSub){ //if head hinahanap
        headSub=pSub->next;} //assigns the next head.

    else{
        qSub->next= pSub->next; }

    free(pSub); //deallocates memoryspace of p.
}

void MyClass:: delMul(){
    //deletes the previous account of the account logged in in mul linkedlist
MULNODE *qMul, *pMul;

    qMul=pMul=headMul; //set sa una
    while(pMul!=NULL && pMul->userId != user.userId){//hahanapin ang previous account ni user
        qMul=pMul;
        pMul=pMul->next;}

    if(pMul==headMul){ //if head hinahanap
        headMul=pMul->next;} //assigns the next head.

    else{
        qMul->next= pMul->next; }

    free(pMul); //deallocates memoryspace of p.
}

void MyClass:: delDiv(){
    //deletes the previous account of the account logged in div linkedlist
DIVNODE *qDiv, *pDiv;

    qDiv=pDiv=headDiv; //set sa una
    while(pDiv!=NULL && pDiv->userId != user.userId){//hahanapin ang previous account ni user
        qDiv=pDiv;
        pDiv=pDiv->next;}

    if(pDiv==headDiv){ //if head hinahanap
        headDiv=pDiv->next;} //assigns the next head.

    else{
        qDiv->next= pDiv->next; }

    free(pDiv); //deallocates memoryspace of p.
}

void MyClass:: del(){
    //deletes the previous account of the account logged in in main linkedlist.
NODE *q, *p;

    q=p=head; //set sa una
    while(p!=NULL && p->userId != user.userId){//hahanapin ang previous account ni user
        q=p;
        p=p->next;}

    if(p==head){ //if head hinahanap
        head=p->next;} //assigns the next head.

    else{
        q->next= p->next; }

    free(p); //deallocates memoryspace of p.
}

void MyClass:: askLevel(){
    //ask level: Easy, average, hard
    //sets the range of the numbers na mage-generate based on level chosen.

    gotoxy(46,6); cout << "+++++++++++++++++++++++++++++++++++";
    gotoxy(60,7); cout <<"LEVELS";
    gotoxy(46,8); cout << "+++++++++++++++++++++++++++++++++++";
    gotoxy(45,10); cout <<"(1) Easy\n";
    gotoxy(45,11); cout <<"(2) Average\n";
    gotoxy(45,12); cout <<"(3) Hard\n";
    gotoxy(45,13); cout <<"Choose level of difficulty (1-3) : ";
    cin >>lvl;
    while(lvl>3){system("cls"); askLevel();}
    if(lvl==1){range=10;}
    else if(lvl==2){range=20;}
    else if(lvl==3){range=30;}
    else{ gotoxy(45,12); cout <<"Invalid level";}

}

float MyClass:: calculateGrade(int x){
    //Calculates the grade of each plays corresponding to the level of difficulty.
float gradeOp;
    if(lvl==1){gradeOp=(float) x/10*50;}
    if(lvl==2){gradeOp=(float) x/10*75;}
    if(lvl==3){gradeOp=(float) x/10*100;}
    return gradeOp;
}


void MyClass:: save(){
    //overwrites all the data to a text file.

fstream fp;
fp.open("playerslog.txt", ios::out);
    p=head;

    while(p!=NULL){
        fp<< p->userId <<endl <<p->pass <<" " <<p->add <<" " <<p->sub <<" " <<p->mul <<" " <<p->div <<" " <<p->total <<endl;
        p=p->next;
    }

    cout <<"\nRecords saved succefully.\n";
    system("pause");
    fp.close(); //closes the file
}

void MyClass:: retrieve(){
    //reads all the data from a text file
fstream fp;
fp.open("playerslog.txt", ios:: in);

    if(!fp){
        cout <<"File error!";
        system("pause");}

    else{
        while(true){
            getline(fp, user.userId);
            fp >>user.pass >>user.add >>user.sub >>user.mul >>user.div >>user.total;
            fp.ignore();
            if(!fp.eof()){
                add();
                callOperations();}
            else{break;}
        }
    }

    cout <<"\nRecords retrieved succefully.\n";
    system("pause");
    fp.close(); //closes the file
}


void MyClass:: init(){
    //initialize all the head nodes equal to NULL
    head=NULL;
    headAdd=NULL;
    headSub=NULL;
    headMul=NULL;
    headDiv=NULL;
}

int MyClass:: menu(){
    //displays different options
    //serves as the home tab

    gotoxy(50,1); printf(BCYN R"EOF(
                   ____  _    _ _____ ______   _____ ______ _   _ ______ _____         _______ ____  _____
                  / __ \| |  | |_   _|___  /  / ____|  ____| \ | |  ____|  __ \     /\|__   __/ __ \|  __ \
                 | |  | | |  | | | |    / /  | |  __| |__  |  \| | |__  | |__) |   /  \  | | | |  | | |__) |
                 | |  | | |  | | | |   / /   | | |_ |  __| | . ` |  __| |  _  /   / /\ \ | | | |  | |  _  /
                 | |__| | |__| |_| |_ / /__  | |__| | |____| |\  | |____| | \ \  / ____ \| | | |__| | | \ \
                  \___\_\\____/|_____/_____|  \_____|______|_| \_|______|_|  \_\/_/    \_\_|  \____/|_|  \_\)EOF"BWHT);

    gotoxy(25,10); cout<<"Hi, "<<user.userId<<"!\n";
    gotoxy(53,12); cout << "++++++++++++++++++++";
    gotoxy(60,13); cout << "MENU";
    gotoxy(53,14); cout << "++++++++++++++++++++";
    gotoxy(53,16); cout << "(1) Addition";
    gotoxy(53,17); cout << "(2) Subtraction";
    gotoxy(53,18); cout << "(3) Multiplication";
    gotoxy(53,19); cout << "(4) Division";
    gotoxy(53,20); cout << "(5) Leaderboards";
    gotoxy(53,21); cout << "(6) Save and Exit";
    gotoxy(53,22); cout << "Select 1-6 only: ";
    cin >> choice;
    return choice;
}


void gotoxy(int x,int y){
    COORD coord = {0,0};
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

/*void displayTime(){

	int min=0,sec=0;
	while(min<=1){
		Sleep(1000);
		Beep(900,100);
		//system("cls");
		gotoxy(1,4); cout<<min<<" minutes : "<<sec<<" seconds ";
		gotoxy(46,20);
		sec++;
		if(sec==60){
			sec=0;
			min++;}
	}
	return;
}*/
