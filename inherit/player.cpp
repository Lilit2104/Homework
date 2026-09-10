#include <iostream>
#include <string>



class Player{
    public:
        std::string player_name;
        int score;
    public:
        Player(std::string player_name, int score)
            : player_name(player_name), score(score) {}

        void addScore(int n)
        {
            score += n;
        }

        void PrintInfo()
        {
            std::cout << "Player name:  " << player_name << "Score:  " << score << std::endl;
        }
};

class Footbollplayer : public Player{
    public:
        int goal_count;

    public:
        Footbollplayer(std::string player_name, int score, int goal_count)
            : Player(player_name, score), goal_count(goal_count) {}

        void scoreGoal()
        {
            goal_count += 1;
        }

        void scoreGoal(int n)
        {
            goal_count += n;
        }
};

class Basketballplayer : public Player{
    public:
        int threePointCount;
    public:
        Basketballplayer(std::string player_name, int score, int threePointCount) 
            : Player(player_name, score), threePointCount(threePointCount) {}

        void threePointer() 
        {
            threePointCount += 1;
            std::cout << "YEEYY it is a three-pointer!" << std::endl;
        }
};

class Gamer: public Player{
    public:
        int level;
    public:
        Gamer(std::string player_name, int score, int level)
            : Player(player_name, score), level(level) {}
        void levelUp() 
        {
            level += 1;
        }

        void levelUp(int n)
        {
            level += n;
        } 
};

int main() {
    
    Footbollplayer pedri("Pedri", 0, 0);
    pedri.PrintInfo();
    pedri.addScore(10);
    pedri.scoreGoal();      
    pedri.scoreGoal(3);     
    std::cout << "Goals: " << pedri.goal_count << std::endl;
    pedri.PrintInfo();
   

    Basketballplayer jordan("Jordan", 0, 0);
    jordan.PrintInfo();
    jordan.addScore(20);
    jordan.threePointer();
    jordan.threePointer();
    std::cout << "Three pointers: " << jordan.threePointCount << std::endl;
    jordan.PrintInfo();


   
    Gamer ninja("Ninja", 0, 1);
    ninja.PrintInfo();
    ninja.addScore(50);
    ninja.levelUp();      
    ninja.levelUp(3);     
    std::cout << "Level: " << ninja.level << std::endl;
    ninja.PrintInfo();

    return 0;
}