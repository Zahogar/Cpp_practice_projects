#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <utility>
#include <cmath>
#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
termios orig_termios;
#endif

void enable_raw_mode() {
#ifdef _WIN32
    return;
#else
    tcgetattr(STDIN_FILENO, &orig_termios);
    termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO); // Turn off canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
#endif
}

void disable_raw_mode() {
#ifdef _WIN32
    return;
#else
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
#endif
}

bool kbhit_alt() {
#ifdef _WIN32
    return _kbhit();
#else
    timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO+1, &fds, NULL, NULL, &tv) > 0;
#endif
}

char getch_alt() {
#ifdef _WIN32
    return _getch();
#else
    char buf = 0;
    termios old = {};
    if (tcgetattr(STDIN_FILENO, &old) < 0)
        perror("tcsetattr()");
    termios new_settings = old;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    read(STDIN_FILENO, &buf, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return buf;
#endif
}

// ANSI color codes
#define RESET               "\033[0m"
#define BLACK               "\033[30m"
#define RED                 "\033[31m"
#define GREEN               "\033[32m"
#define YELLOW              "\033[33m"
#define BLUE                "\033[34m"
#define MAGENTA             "\033[35m"
#define CYAN                "\033[36m"
#define WHITE               "\033[37m"

#define HIGHLIGHTED_RED     "\033[41m"
#define HIGHLIGHTED_GREEN   "\033[42m"
#define HIGHLIGHTED_YELLOW  "\033[43m"
#define HIGHLIGHTED_BLUE    "\033[44m"
#define HIGHLIGHTED_MAGENTA "\033[45m"
#define HIGHLIGHTED_CYAN    "\033[46m"
#define HIGHLIGHTED_WHITE   "\033[47m"

#define LIGHTER_BLACK       "\033[90m"
#define LIGHTER_RED         "\033[91m"
#define LIGHTER_GREEN       "\033[92m"
#define LIGHTER_YELLOW      "\033[93m"
#define LIGHTER_BLUE        "\033[94m"
#define LIGHTER_MAGENTA     "\033[95m"
#define LIGHTER_CYAN        "\033[96m"

// ANSI text style codes
#define BOLD                "\033[1m"
#define DARKER              "\033[2m"
#define ITALIC              "\033[3m"
#define UNDERLINE           "\033[4m"
#define BLINKING            "\033[5m"
#define HIGHLIGHTED         "\033[7m"
#define BLACKED             "\033[8m"
#define CROSSED             "\033[9m"
#define DOUBLE_UNDERLINE    "\033[21m"
#define UPPERLINE           "\033[53m"

std::string color_from_rarity(std::string rarity) {
    if (rarity == "Common") return "lighter green";
    if (rarity == "Rare") return "lighter blue";
    if (rarity == "Epic") return "magenta";
    if (rarity == "Legendary") return "yellow";
    else return "\n/// ERROR COLOR RARITY ///\n";
}

std::string color_parser(std::string color) {
    if (color[0] == 'h') {
        color = color.substr(12, color.length()-12);
        if (color == "red") return HIGHLIGHTED_RED;
        if (color == "green") return HIGHLIGHTED_GREEN;
        if (color == "yellow") return HIGHLIGHTED_YELLOW;
        if (color == "blue") return HIGHLIGHTED_BLUE;
        if (color == "magenta") return HIGHLIGHTED_MAGENTA;
        if (color == "cyan") return HIGHLIGHTED_CYAN;
        if (color == "white") return HIGHLIGHTED_WHITE;
        else return "\n/// COLOR ERROR ///\n";
    }
    else if (color[0] == 'l') {
        color = color.substr(8, color.length()-8);
        if (color == "black") return LIGHTER_BLACK;
        if (color == "red") return LIGHTER_RED;
        if (color == "green") return LIGHTER_GREEN;
        if (color == "yellow") return LIGHTER_YELLOW;
        if (color == "blue") return LIGHTER_BLUE;
        if (color == "magenta") return LIGHTER_MAGENTA;
        if (color == "cyan") return LIGHTER_CYAN;
        else return "\n/// COLOR ERROR ///\n";
    }
    else {
        if (color == "black") return BLACK;
        if (color == "red") return RED;
        if (color == "green") return GREEN;
        if (color == "yellow") return YELLOW;
        if (color == "blue") return BLUE;
        if (color == "magenta") return MAGENTA;
        if (color == "cyan") return CYAN;
        if (color == "white") return WHITE;
        else return "\n/// COLOR ERROR ///\n";
    }
}

std::string style_parser(std::string style) {
    std::string parsed_style = "";
    if (style.find("bold") != std::string::npos) parsed_style.append(BOLD);
    if (style.find("darker") != std::string::npos) parsed_style.append(DARKER);
    if (style.find("italic") != std::string::npos) parsed_style.append(ITALIC);
    if (style.find("underline") != std::string::npos) parsed_style.append(UNDERLINE);
    if (style.find("blinking") != std::string::npos) parsed_style.append(BLINKING);
    if (style.find("highlighted") != std::string::npos) parsed_style.append(HIGHLIGHTED);
    if (style.find("blacked") != std::string::npos) parsed_style.append(BLACKED);
    if (style.find("crossed") != std::string::npos) parsed_style.append(CROSSED);
    if (style.find("double underline") != std::string::npos) parsed_style.append(DOUBLE_UNDERLINE);
    if (style.find("upperline") != std::string::npos) parsed_style.append(UPPERLINE);
    
    if (parsed_style == "") return "\n/// STYLE ERROR ///\n";
    else return parsed_style;
}


void pr(const std::string& text, std::string color = "", std::string style = "", int speed=40) {
    bool skipDelay = false;
    enable_raw_mode();

    for (char c : text) {
        if (color != "") {
            if (style != "") std::cout << style_parser(style) << color_parser(color) << c << RESET << std::flush;
            else std::cout << color_parser(color) << c << RESET << std::flush;
        } else {
            std::cout << c << std::flush;
        }

        if (!skipDelay) {
            auto startTime = std::chrono::steady_clock::now();
            while (!skipDelay &&
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - startTime).count() < speed) {

                if (kbhit_alt()) {
                    char key = getch_alt();
                    if (key == ' ') {
                        skipDelay = true;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    disable_raw_mode();
}


float dodge_calculation(std::string name) {
    float result = 0;
    for (char c : name) {
        result += static_cast<float>(c);
    }
    while (result > 1) result /= 10.0f;
    return result;
}

float escape_calculation(std::string name) {
    float result = 0;
    for (char c : name) {
        result += log2f(static_cast<float>(c));
    }
    while (result > 1) result /= 10.0f;
    return static_cast<int>(result * 100) / 100.0f;
}

class Monster {
private:
    int _pv;
    int _attack;    // between 0 and 100
    int _defense;   // between 0 and 100

public:
    std::string name;
    
    Monster(std::string Name, int PV, int defense, int attack) {
        _pv = PV;
        _attack = attack;
        _defense = defense;
        name = Name;
    }

    int damage_taken(int player_attack) {
        return player_attack * (100 - _defense) / 100;
    }

    void take_damage(int damage) {
        _pv -= damage;
        return;
    }

    int get_attack() {
        return _attack;
    }
    
    int get_remaining_PV() {
        return _pv;
    }

    int get_defense() {
        return _defense;
    }

    void add_attack(int attack_gained) {
        _attack += attack_gained;
        return;
    }

    void add_defense(int defense_gained) {
        _defense += defense_gained;
        return;
    }
};

class Tools_armors_and_weapons {
public:
    std::string name;
    std::string modified_stat;
    int stat_modifier;

    Tools_armors_and_weapons(std::string nom, std::string stat, int n) {
        name = nom;
        modified_stat = stat;
        stat_modifier = n;
    }    
};

class Player {
private:
    Monster _slayer;
    std::vector<Tools_armors_and_weapons> _items;
    float _crit_chance;
    float _dodge_chance;
    float _escape_chance;

public:
    Tools_armors_and_weapons starter_gear1 = Tools_armors_and_weapons("Wooden Sword", "attack", 2);
    Tools_armors_and_weapons starter_gear2 = Tools_armors_and_weapons("Sausage necklace", "pv", 3);
    int max_hp;

    Player(std::string name) 
        : _slayer(name, 23, 10, 10)
    {
        max_hp = 23;
        _items.push_back(starter_gear1);
        _items.push_back(starter_gear2);
        _dodge_chance = dodge_calculation(name);
        _escape_chance = escape_calculation(name);
    }

    int damage_taken(int monster_attack) {return _slayer.damage_taken(monster_attack);}

    void take_damage(int damage) {_slayer.take_damage(damage);}

    int get_attack() {return _slayer.get_attack();}

    void add_hp(int hp_gained) {_slayer.take_damage(-hp_gained);}
    
    int get_remaining_PV() {return _slayer.get_remaining_PV();}

    float get_dodge() {return _dodge_chance;}

    float get_escape() {return _escape_chance;}

    std::string get_name() {return _slayer.name;}

    int get_defense() {return _slayer.get_defense();}

    void add_item(Tools_armors_and_weapons item) {_items.push_back(item);}

    void add_attack(int attack_gained) {_slayer.add_attack(attack_gained);}

    void add_defense(int defense_gained) {_slayer.add_defense(defense_gained);}

};

std::pair<std::string, int> Get_stats(std::string name) {

    //Commons
    if (name == "a Sharper Wooden Sword") return std::make_pair("attack", 5);
    if (name == "a Leather Jacket") return std::make_pair("defense", 2);
    if (name == "a Simple Cape") return std::make_pair("defense", 1);
    if (name == "a Lead Ring") return std::make_pair("hp", 2);
    if (name == "a Wheat Doll") return std::make_pair("hp", 1);
    if (name == "a Rope") return std::make_pair("attack", 2);
    if (name == "a Rosary") return std::make_pair("hp", 6);
    if (name == "a Cardboard Box with two holes") return std::make_pair("defense", 1);

    //Rares
    if (name == "an Iron Sword") return std::make_pair("attack", 10);
    if (name == "a Plate Armor") return std::make_pair("defense", 15);
    if (name == "a Chainmail Armor") return std::make_pair("defense", 18);
    if (name == "a Gold Ring") return std::make_pair("hp", 13);
    if (name == "a Voodoo Doll") return std::make_pair("attack", 13);
    if (name == "a Silver Pendant") return std::make_pair("hp", 14);
    if (name == "Copper Gauntlets") return std::make_pair("defense", 12);

    //Epics
    if (name == "a Netherrite Sword") return std::make_pair("attack", 21);
    if (name == "a Dragon Bone Armor") return std::make_pair("defense", 27);
    if (name == "Superman's Cape") return std::make_pair("defense", 22);
    if (name == "Magic Imbued Earrings") return std::make_pair("hp", 25);
    if (name == "Night Vision Goggles") return std::make_pair("hp", 20);
    if (name == "a Dynamite Stick") return std::make_pair("attack", 29);

    //Legendaries
    if (name == "Callandor") return std::make_pair("attack", 50);
    if (name == "the Doktorskaya Armor") return std::make_pair("defense", 50);
    if (name == "a True Sausage necklace") return std::make_pair("hp", 50);
    if (name == "the Hylian Shield") return std::make_pair("defense", 50);
    
    //should be happening
    else return std::make_pair("attack", rand()%101);
}

std::pair<Tools_armors_and_weapons, std::string> Get_item(int difficulty) {

    std::vector<std::string> Common = {"a Sharper Wooden Sword", "a Leather Jacket", "a Simple Cape", "a Lead Ring", "a Wheat Doll", "a Rope", "a Rosary", "a Cardboard Box with two holes"};
    std::vector<std::string> Rare = {"an Iron Sword", "a Plate Armor", "a Chainmail Armor", "a Gold Ring", "a Voodoo Doll", "a Silver Pendant", "Copper Gauntlets"};
    std::vector<std::string> Epic = {"a Netherrite Sword", "a Dragon Bone Armor", "Superman's Cape", "Magic Imbued Earrings", "Night Vision Goggles", "a Dynamite Stick", "Sa teel Helmet"};
    std::vector<std::string> Legendary = {"Callandor", "the Doktorskaya Armor", "a True Sausage necklace", "the Hylian Shield", "the Gambler's mask"};
    
    std::string name;
    std::string rarity_class;
    int rarity = rand() % 101;
    if (difficulty == 69) {
        if (rarity > 90) {name = Legendary[rand() % Legendary.size()]; rarity_class = "Legendary";}
        else if (rarity > 70) {name = Epic[rand() % Epic.size()]; rarity_class = "Epic";}
        else if (rarity > 40) {name = Rare[rand() % Rare.size()]; rarity_class = "Rare";}
        else {name = Common[rand() % Common.size()]; rarity_class = "Common";}
    }
    else {
        if (rarity > 90) {name = Legendary[rand() % Legendary.size()]; rarity_class = "Legendary";}
        else if (rarity > 70) {name = Epic[rand() % Epic.size()]; rarity_class = "Epic";}
        else if (rarity > 40) {name = Rare[rand() % Rare.size()]; rarity_class = "Rare";}
        else {name = Common[rand() % Common.size()]; rarity_class = "Common";}
    }
        
    std::pair item_stats = Get_stats(name);
    Tools_armors_and_weapons Item = Tools_armors_and_weapons(name, item_stats.first, item_stats.second);
    return std::make_pair(Item, rarity_class);
}

int battle(Player& hero, Monster& monster, bool is_boss=false) {
    
    std::string answer;
    char fled = 0;
    char is_stat = 0;

    while (monster.get_remaining_PV() > 0 && hero.get_remaining_PV() > 0) {

        pr("What do want to do: ");
        pr("attack", "red", "italic");
        pr(" or ");
        pr("flee", "cyan", "italic");
        pr(" or ");
        pr("stats\n", "magenta", "italic");
        std::cin >> answer;
        

        if (answer == "attack") {

            monster.take_damage(monster.damage_taken(hero.get_attack()));

            if (monster.get_remaining_PV() <= 0) break;
            else {
                pr(monster.name, is_boss ? "lighter yellow":"red", is_boss ? "bold blinking":"italic");
                pr(" is now at ");
                std::cout << monster.get_remaining_PV();
                pr(".\n");
                
            }
            is_stat = 0;
        }
        else if (answer == "flee") {
            if (rand()%101 / 100.0f < hero.get_escape()) {
                pr("You manage to escape without any injuries.\n\n");
                fled = 1;
                break;
            } else {
                pr("You failed to escape.\n");
            }
            is_stat = 0;
        }
        else if (answer == "stats") {
            pr("Player ");
            pr(hero.get_name(), "lighter magenta");
            pr(", health ");
            std::cout << hero.get_remaining_PV();
            pr(", attack ");
            std::cout << hero.get_attack();
            pr(", defense ");
            std::cout << hero.get_defense();
            pr(", dodge percentile ");
            std::cout << hero.get_dodge()*100;
            pr(", escape percentile ");
            std::cout << hero.get_escape()*100 << ".\n";

            pr(monster.name, is_boss ? "lighter yellow":"red", is_boss ? "bold blinking":"italic");
            pr(", health ");
            std::cout << monster.get_remaining_PV();
            pr(", attack ");
            std::cout << monster.get_attack();
            pr(", defense ");
            std::cout << monster.get_defense() << ".\n";

            is_stat = 1;
        }
        else {
            pr("You're confused on what to choose. ");
            pr(monster.name);
            pr(" doesn't wait for you.\n");
            is_stat = 0;
        }

        if (!is_stat) {
            if (rand()%101 > 100*hero.get_dodge()) {
                hero.take_damage(hero.damage_taken(monster.get_attack()));
            }
            else {
                pr("You've dodged the incoming attack !\n");
            }
            
            if (hero.get_remaining_PV() <= 0) break;
            else {
                pr("You're now at ");
                std::cout << hero.get_remaining_PV();
                pr(".\n");
                
            }
        }
    }

    if (!fled) {    
        if (hero.get_remaining_PV() <= 0) {
            pr("You died...\n");
            return 0;
        }
        else {
            pr("You managed to kill ");
            pr(monster.name, is_boss ? "lighter yellow":"red", is_boss ? "bold blinking":"italic");
            pr(" successfuly\n\n");
            
            return 1;
        }
    }
    else return 2;

    return -1;
}

void introduction() {

    pr("\n\n=============== LOADING GAME ===============");
    

    pr("\n\nYou find yourself inside a comfortable cart full of wheat.\n");
    pr("Suddenly, you hear a voice coming from the front of the cart.\n\n");
    
    
    pr("> Hey you, you're finally awake.\n", "green");
    pr("> I see a dust fog that's coming toward us. I don't pay you to sleep all day, go take a look!\n\n", "green");
    

    pr("While half asleep, you take your wooden sword and your necklace of sausages and\n");
    pr("jump off the cart, trying to identify the source of the dust.\n");
    

    return;
}

void end_of_game(int difficulty, int end_result) {
    if (end_result){
        pr("          ########################################\n", "", "", 10);
        pr("          #               Victory                #\n", "", "", 10);
        pr("          ########################################\n", "", "", 10);    
    }
    else {
        pr("          ########################################\n", "", "", 10);
        pr("          #        You just lost the game        #\n", "", "", 10);
        pr("          ########################################\n", "", "", 10);
    }
    return;
}

void try_your_luck(std::vector<Monster> monsters_to_defeat, Player& hero, int difficulty) {

    Monster m = monsters_to_defeat[0];
    monsters_to_defeat.erase(monsters_to_defeat.begin());
    
    pr("Your legs surprisingly goes numb and you get knocked out as the side of your head hit the ground.\n");
    pr("You're not dead, just transported to an other place.\n\n");

    pr("You're kinda lost, it is like a palace but the place is filled by the scent of perfectly cooked sausages.\n");
    pr("You inside the hall, and right before the gigantic door that leads to the throne room, there are 8 slot machines.\n");
    pr("A voice comes from the room.\n");

    pr("> You better get prepared before fighting against ", "green");
    pr(m.name, "lighter yellow", "bold blinking");
    pr(" !\n", "green");
    pr("> Each slot machine gives you a piece of equipment.\n\n", "green");

    pr("With no other choice, you pull the lever of all the 8 slot machines.\n");
    for (int i = 0; i < 8; i++) {
        auto [item, rarity] = Get_item(difficulty);
        pr(item.name, color_from_rarity(rarity), "bold");
        pr(" gives ");
        pr(item.modified_stat);
        pr(" +");
        std::cout << item.stat_modifier << " !\n\n";

        hero.add_item(item);
        
        //add stat to player
        if (item.modified_stat == "hp") {hero.add_hp(item.stat_modifier); hero.max_hp += item.stat_modifier;}
        else if (item.modified_stat == "attack") hero.add_attack(item.stat_modifier);
        else if (item.modified_stat == "defense") hero.add_defense(item.stat_modifier);
        else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;} 
    }
    pr("Now fully equiped, the doors open and you are facing ");
    pr(m.name, "lighter yellow", "bold blinking");
    pr(" .\n");

    int result = battle(hero, m, true);

    if (result == 0) {end_of_game(difficulty, 0); return;}
    if (result >= 1) {
        pr("HOOOOOOOOOOOOOOOOOOOOOOOOOOW ?!?!?!?!?!\n");
        pr("WHAT THE HEEEEEEEEEEEEEEEELLL !!!!!!!!!!!!\n");
        pr("Wait, let me process... [UIIA IAU UUIIA]\n\n");

        pr("Ok, it is unexpected I must say.\n");
        pr("I think I have no other choice.\n");

        pr("Congratulation ");
        pr(hero.get_name(), "lighter magenta");
        pr(", you are now the evil silly castle sausage pig's master and also the emperor of sausages.\n\n");
        end_of_game(difficulty, result);
    }
    if (result == -1) {
        std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
        return;
    }

    return;
}

void adventure(std::vector<Monster> monsters_to_defeat, Player& hero, int difficulty) {
    
    Monster m1 = monsters_to_defeat[0];
    monsters_to_defeat.erase(monsters_to_defeat.begin());  

    pr("\nAnyway, you finally identify the enemy.\n");
    pr("This is ");
    pr(m1.name, "red", "italic");
    pr(" !!\n");
    

    int result = battle(hero, m1);

    if (result == 0) {end_of_game(difficulty, 0); return;};
    if (result == 1) {
        pr("You notice something shiny from the corpse.\n"); 
        auto [item, rarity] = Get_item(difficulty);
        pr("It's ");
        pr(item.name, color_from_rarity(rarity), "bold");
        pr("!\n");
        pr(item.name, color_from_rarity(rarity), "bold");
        pr(" gives ");
        pr(item.modified_stat);
        pr(" +");
        std::cout << item.stat_modifier << " !\n\n";

        hero.add_item(item);
        
        //add stat to player
        if (item.modified_stat == "hp") {hero.add_hp(item.stat_modifier); hero.max_hp += item.stat_modifier;}
        else if (item.modified_stat == "attack") hero.add_attack(item.stat_modifier);
        else if (item.modified_stat == "defense") hero.add_defense(item.stat_modifier);
        else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;} 
    }
    if (result == -1) {
        std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
        return;
    }

    m1 = monsters_to_defeat[0];
    monsters_to_defeat.erase(monsters_to_defeat.begin());
    Monster m2 = monsters_to_defeat[0];
    monsters_to_defeat.erase(monsters_to_defeat.begin());

    pr("As you just defeated your first oponent, two others are approaching swiftly.\n");
    pr("It is ");
    pr(m1.name, "red", "italic");
    pr(" and ");
    pr(m2.name, "red", "italic");
    pr(" !!\n");   
    
    result = battle(hero, m1);

    if (result == 0) {end_of_game(difficulty, 0); return;}
    if (result == -1) {
        std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
        return;
    }
    else {
        result = battle(hero, m2);

        if (result == 0) {end_of_game(difficulty, 0); return;}
        if (result == -1) {
            std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
            return;
        }
        if (result == 1) {
            pr("As strangely as the first time, you get two other items.\n");
            auto [item1, rarity1] = Get_item(difficulty);
            auto [item2, rarity2] = Get_item(difficulty);
            pr("They are ");
            pr(item1.name, color_from_rarity(rarity1), "bold");
            pr(" and ");
            pr(item2.name, color_from_rarity(rarity2), "bold");
            pr("!\n");

            pr(item1.name, color_from_rarity(rarity1), "bold");
            pr(" gives ");
            pr(item1.modified_stat);
            pr(" +");
            std::cout << item1.stat_modifier << "!\n";

            
            hero.add_item(item1);
            hero.add_item(item2);

            //add stat to player
            if (item1.modified_stat == "hp") {hero.add_hp(item1.stat_modifier); hero.max_hp += item1.stat_modifier;}
            else if (item1.modified_stat == "attack") hero.add_attack(item1.stat_modifier);
            else if (item1.modified_stat == "defense") hero.add_defense(item1.stat_modifier);
            else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;} 
        
            pr(item2.name, color_from_rarity(rarity2), "bold");
            pr(" gives ");
            pr(item2.modified_stat);
            pr(" +");
            std::cout << item2.stat_modifier << " !\n\n";

            //add stat to player
            if (item2.modified_stat == "hp") {hero.add_hp(item2.stat_modifier); hero.max_hp+= item1.stat_modifier;}
            else if (item2.modified_stat == "attack") hero.add_attack(item2.stat_modifier);
            else if (item2.modified_stat == "defense") hero.add_defense(item2.stat_modifier);
            else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;} 
        }
    }

    pr("> Wow, you defeated them so easily !\n", "green");
    pr("> And my shipment is unscated ! This deserves a bonus !\n", "green");
    pr("> Here, drink this potion and rest in my cart.\n\n", "green");

    pr("You drink this disguting mixture because why not.\n");
    pr("After all, you are on the brink of death.\n");

    pr("You feel the smarmy warm liquid going down your throat as if you were drinking someone's diarrhea.\n");
    pr("As soon as you lay, you fell into a deep sleep.\n\n");

    hero.add_hp(hero.max_hp - hero.get_remaining_PV());

    pr("You feel fully rested inside your dream...\n");
    pr("The dream you are in feels familiar, you know this place.\n");
    pr("Wait, this YOUR home! But something feels different.\n\n");

    pr("By exploring an oddly familiar yet unknown world, you start to understand.\n");
    pr("You are in ");
    pr("Tel'aran'rhiod", "yellow", "italic");
    pr(", the legendary world of dream strangely linked to reality.\n");
    pr("Every wound you take here is real.\n");

    m1 = monsters_to_defeat[0];
    monsters_to_defeat.erase(monsters_to_defeat.begin());   
    pr("While walking on one path, you are attacked by ");
    pr(m1.name, "red", "italic");
    pr(".\n");

    result = battle(hero, m1);

    if (result == 0) {end_of_game(difficulty, 0); return;}
    if (result == 1) {
        auto [item, rarity] = Get_item(difficulty);
        pr("You find ");
        pr(item.name, color_from_rarity(rarity), "bold");
        pr(" on the corpse.\n");
        pr(item.name, color_from_rarity(rarity), "bold");
        pr(" gives ");
        pr(item.modified_stat);
        pr(" +");
        std::cout << item.stat_modifier << " !\n\n";

        hero.add_item(item);

        //add stat to player
        if (item.modified_stat == "hp") {hero.add_hp(item.stat_modifier); hero.max_hp+= item.stat_modifier;}
        else if (item.modified_stat == "attack") hero.add_attack(item.stat_modifier);
        else if (item.modified_stat == "defense") hero.add_defense(item.stat_modifier);
        else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;}
    }
    if (result == -1) {
        std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
        return;
    }

    m1 = monsters_to_defeat[0];
    monsters_to_defeat.erase(monsters_to_defeat.begin());

    pr("You're kind of afraid to continue, your walk but do it anyway because you don't know how to get out of ");
    pr("Tel'aran'rhiod", "yellow", "italic");
    pr(".\n");
    pr("As you continue you walk, you encounter ");

    if (difficulty == 0) {
        
        pr(m1.name, "lighter yellow", "bold blinking");
        pr(", the legendary road thief!!\n\n");

        pr("> Alright ", "green");
        pr(hero.get_name(), "lighter magenta");
        pr(", let's fight !\n\n", "green");

        pr("This is going to be a tough battle.\n\n");


        result = battle(hero, m1, true);

        if (result == 0) {end_of_game(difficulty, 0); return;}
        if (result >= 1) {
            pr("You are exhausted from this fight and go straight to your bed...\n");
            end_of_game(difficulty, result);
        }
        if (result == -1) {
            std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
            return;
        }

    } else {

        pr(m1.name, "red", "italic");
        std::cout << "!\n";

        result = battle(hero, m1);

        if (result == 0) {end_of_game(difficulty, 0); return;}
        if (result == 1) {
            auto [item, rarity] = Get_item(difficulty);
            pr("You find ");
            pr(item.name, color_from_rarity(rarity), "bold");
            pr(" on the corpse.\n");
            pr(item.name, color_from_rarity(rarity), "bold");
            pr(" gives ");
            pr(item.modified_stat);
            pr(" +");
            std::cout << item.stat_modifier << " !\n\n";

            hero.add_item(item);

            //add stat to player
            if (item.modified_stat == "hp") {hero.add_hp(item.stat_modifier); hero.max_hp+= item.stat_modifier;}
            else if (item.modified_stat == "attack") hero.add_attack(item.stat_modifier);
            else if (item.modified_stat == "defense") hero.add_defense(item.stat_modifier);
            else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;}
        }
        if (result == -1) {
            std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
            return;
        }
        
        pr("You come back to your home.\n");
        pr("After pondering for a while, you get into your bed...\n");

        hero.add_hp(hero.max_hp - hero.get_remaining_PV());

        pr("You wake up with a headache, but without a trace of any wound, chained inside a gloomy cell.\n");
        pr("Altough the effects of the potion are horrible, you find no trace of wound from ");
        pr("Tel'aran'rhiod", "yellow", "italic");
        std::cout << ".\n";
        pr("You hear a voice... It's the merchant's.\n\n");

        pr("> Ah, you'll be a formidable entertainment !\n\n", "green");
        
        pr("You realize that this jerk sold you as a gladiator in an illegal arena.\n");
        pr("Now you have to fight for your liberty as for your life.\n\n");

        pr("The rules of the arena are simple: you can only get out if the crowd wants it, and if you defeat an ennemy, you get its equipment.\n\n");
        
        pr("Before entering the scene, you get back your equipment, yes even the one you got while dreaming.\n");
        pr("Brace yourself, your fights will be harder as you kill more opponents.\n");

        m1 = monsters_to_defeat[0];
        monsters_to_defeat.erase(monsters_to_defeat.begin());
        pr("For the first match, it's only one monster: ");
        pr(m1.name, "red", "italic");
        std::cout << "!\n";

        result = battle(hero, m1);

        if (result == 0) {end_of_game(difficulty, 0); return;}
        if (result == 1) {
            auto [item, rarity] = Get_item(difficulty);
            pr("Following the rules, you take ");
            pr(item.name, color_from_rarity(rarity), "bold");
            pr("!\n");
            pr(item.name, color_from_rarity(rarity), "bold");
            pr(" gives ");
            pr(item.modified_stat);
            pr(" +");
            std::cout << item.stat_modifier << " !\n\n";

            hero.add_item(item);

            //add stat to player
            if (item.modified_stat == "hp") {hero.add_hp(item.stat_modifier); hero.max_hp+= item.stat_modifier;}
            else if (item.modified_stat == "attack") hero.add_attack(item.stat_modifier);
            else if (item.modified_stat == "defense") hero.add_defense(item.stat_modifier);
            else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;}
        }
        if (result == -1) {
            std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
            return;
        }

        m1 = monsters_to_defeat[0];
        monsters_to_defeat.erase(monsters_to_defeat.begin());
        m2 = monsters_to_defeat[0];
        monsters_to_defeat.erase(monsters_to_defeat.begin());

        pr("Now, two at once !\n");
        pr(m1.name, "red", "italic");
        pr(" and ");
        pr(m2.name, "red", "italic");
        pr(" !!\n");   
        
        result = battle(hero, m1);

        if (result == 0) {end_of_game(difficulty, 0); return;}
        if (result == -1) {
            std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
            return;
        }
        else {
            result = battle(hero, m2);

            if (result == 0) {end_of_game(difficulty, 0); return;}
            if (result == -1) {
                std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
                return;
            }
            if (result == 1) {
                pr("From the lacerated corpses of the monster you oblirated, you choose ");
                auto [item1, rarity1] = Get_item(difficulty);
                auto [item2, rarity2] = Get_item(difficulty);
                pr(item1.name, color_from_rarity(rarity1), "bold");
                pr(" and ");
                pr(item2.name, color_from_rarity(rarity2), "bold");
                pr("!\n");

                pr(item1.name, color_from_rarity(rarity1), "bold");
                pr(" gives ");
                pr(item1.modified_stat);
                pr(" +");
                std::cout << item1.stat_modifier << "!\n";

                hero.add_item(item1);
                hero.add_item(item2);

                //add stat to player
                if (item1.modified_stat == "hp") {hero.add_hp(item1.stat_modifier); hero.max_hp += item1.stat_modifier;}
                else if (item1.modified_stat == "attack") hero.add_attack(item1.stat_modifier);
                else if (item1.modified_stat == "defense") hero.add_defense(item1.stat_modifier);
                else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;} 
            
                pr(item2.name, color_from_rarity(rarity2), "bold");
                pr(" gives ");
                pr(item2.modified_stat);
                pr(" +");
                std::cout << item2.stat_modifier << " !\n\n";

                //add stat to player
                if (item2.modified_stat == "hp") {hero.add_hp(item2.stat_modifier); hero.max_hp+= item1.stat_modifier;}
                else if (item2.modified_stat == "attack") hero.add_attack(item2.stat_modifier);
                else if (item2.modified_stat == "defense") hero.add_defense(item2.stat_modifier);
                else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;} 
            }
        }

        if (difficulty == 1) {

            m1 = monsters_to_defeat[0];
            monsters_to_defeat.erase(monsters_to_defeat.begin());
            
            pr("The voice of the announcer resound through the overly loud speakers.\n");
            pr("> Now, lady and gentleman, it is time for the greatest fight of all time !\n", "green");
            pr("> The rookie: ", "green");
            pr(hero.get_name(), "lighter magenta");
            pr(" against the legendary: ", "green");
            pr(m1.name, "lighter yellow", "bold blinking");
            pr(" !\n\n", "green");

            hero.add_hp((hero.max_hp - hero.get_remaining_PV())/2);

            pr("The theme song of Alcathor [Baby Shark] is blowing out of the speakers and the crowd is going crazy.\n");
            pr("The song won't stop, you have to beat him under these conditions.\n");

            result = battle(hero, m1, true);

            if (result == 0) {end_of_game(difficulty, 0); return;}
            if (result >= 1) {
                pr("You're crown as the best fighter.\n");
                pr("Unfortunatly, you'll have to fight until you die (even of old age)\nbecause the public is thrilled at every single one of your fights.\n");
                end_of_game(difficulty, result);
            }
            if (result == -1) {
                std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
                return;
            }


        }

        else {

            // Next part of the adventure
            pr("After, you've defeated mediocrily all your opponents, the public is bored and free you.\n");
            pr("You come out of a manhole in the middle of a small town.\n");
            pr("You so dirty that everyone that sees you is taking you for a beggar.\n\n");

            pr("One soul takes pitty of you and give you ");
            auto [item, rarity] = Get_item(difficulty);
            pr(item.name, color_from_rarity(rarity), "bold");
            pr(" !\n");

            pr(item.name, color_from_rarity(rarity), "bold");
            pr(" gives ");
            pr(item.modified_stat);
            pr(" +");
            std::cout << item.stat_modifier << " !\n\n";

            hero.add_item(item);
            
            //add stat to player
            if (item.modified_stat == "hp") {hero.add_hp(item.stat_modifier); hero.max_hp += item.stat_modifier;}
            else if (item.modified_stat == "attack") hero.add_attack(item.stat_modifier);
            else if (item.modified_stat == "defense") hero.add_defense(item.stat_modifier);
            else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;}

            pr("You remember who put you through all this trouble: it's the merchand !\n");
            pr("After all, he didn't even pay you for your services on the road !!\n");
            pr("There you choose to go talk to every trader to see if they know him.\n\n");

            pr("You get all kind of answers:\n");
            pr("> No, never seen. ; > Yes, yes, yes, eeeeh, finally no. ; > Get out of my face disgusting beggar !!\n", "green");
            pr("No info at all. There is still your last chance: Clement the bartender.\n");
            pr("You enter the western-like bar and go straight to the counter while dodging every disgusted look of the other clients.\n\n");

            pr("> Hey you, I've never seen you before. What makes you come here ?\n\n", "green");

            pr("It's Clement. You briefly explain your situation and ask for some info about the mysterious trader.\n");
            pr("> Hmmm... I may or may not know him, if you know what I mean.\n\n", "green");

            pr("Of course, you have to pay him to get them. You hand him your gains from the arena.\n\n");

            pr("> I haven't seen him personnaly but I know someone who, for sure, have a way to get to him.\n", "green");
            pr("> Follow me in the back.\n\n", "green");

            pr("The back is moody. You follow Clement into his cellar where he meets with his network of spies.\n");
            pr("Here you meet Kagax, a singular person who seems a little be deranged (this is an understatement).\n\n");
            
            pr("> Well, you want to know more about Mr.", "green");
            pr("Icarag the fallen", "black");
            pr(" ??\n", "green");
            pr("> Here's what I can tell you, he lives inside a manor in the south of the town.\n", "green");
            pr("> I don't why he likes to disguise as a trader when he clearly as enough money to buy the whole land.\n", "green");
            pr("> If you want to attack him, may the Vis Arcana be with you !\n\n", "green");

            pr("You note all those information in a part of your brain, get out of the bar and go to this manor.\n");
            
            pr("The estate is far too guarded to try entering under daylight.\n");
            pr("You also notice a sewer entrance from an nearby alley.\n");
            
            std::string answer;
            pr("What do you want to do ? wait or try the sewers\n");
            std::cin >> answer;

            while (answer != "wait" && answer.find("sewer") == std::string::npos) {
                pr("You ponder for a while...\n");
                pr("What do you want to do ? wait or try the sewers\n");
                std::cin >> answer;
            }

            if (answer == "wait") {
                pr("Luck is with you because this night is a New Moon.\n");
                pr("You look around the house to see if there's any easier way to enter than knocking.\n");
                pr("The backdoor seems perfect for this job.\n");
                pr("What do you want to do ? knock or pick\n");
                std::cin >> answer;
                while (answer != "knock" && answer != "pick") {
                    pr("You wait a little...\n");
                    pr("What do you want to do ? knock or pick\n");
                    std::cin >> answer;
                }
                
                if (answer == "knock") {
                    monsters_to_defeat.erase(monsters_to_defeat.begin());
                    pr("The door is unlocked...\n");
                    pr("The place is marvelous, everything seems to cost more that any other house of the city.\n");
                }
                else {
                    pr("You try to pick the door...\n");
                    pr("Nothing happened, the lock seems too hard.\n");
                    pr("Let's try once again...\n\n");
                    
                    m1 = monsters_to_defeat[0];
                    m1.name = "a Guard";
                    monsters_to_defeat.erase(monsters_to_defeat.begin());

                    pr("The door opens, not because you succeeded, but because ");
                    pr(m1.name, "red", "italic");
                    pr(" opened it.\n");
                    pr("You have no choice but to fight.\n");
                    
                    result = battle(hero, m1);

                    if (result == 0) {end_of_game(difficulty, 0); return;};
                    if (result == 1) {
                        auto [item, rarity] = Get_item(difficulty);
                        pr("You collect ");
                        pr(item.name, color_from_rarity(rarity), "bold");
                        pr(" from the corpse.\n");
                        pr(item.name, color_from_rarity(rarity), "bold");
                        pr(" gives ");
                        pr(item.modified_stat);
                        pr(" +");
                        std::cout << item.stat_modifier << " !\n\n";

                        hero.add_item(item);
                        
                        //add stat to player
                        if (item.modified_stat == "hp") {hero.add_hp(item.stat_modifier); hero.max_hp += item.stat_modifier;}
                        else if (item.modified_stat == "attack") hero.add_attack(item.stat_modifier);
                        else if (item.modified_stat == "defense") hero.add_defense(item.stat_modifier);
                        else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;} 
                    }
                    if (result == -1) {
                        std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
                        return;
                    }
                    
                    pr("The room you're in is a kind of food safe.\n");
                    pr("Why not eat some while you're at it ?!\n");
                    hero.add_hp(hero.max_hp - hero.get_remaining_PV());
                }
                
                
                pr("Suddenly, you hear a scream from the basement of the manor.\n");
                pr("You find the stairs and descend.\n");
            }
            else {
                pr("Making sure nobody saw you, you enter the sewers.\n");
                pr("The place stinks as it should but you see a sewer tunnel that comes from the manor.\n");
                pr("Slowly you make your way to the manor.\n\n");
                
                m1 = monsters_to_defeat[0];
                monsters_to_defeat.erase(monsters_to_defeat.begin());
                
                pr("However, a mass that was laid down suddenly rise up, it's ");
                pr(m1.name, "red", "italic");
                pr(" .\n");

                result = battle(hero, m1);

                if (result == 0) {end_of_game(difficulty, 0); return;};
                if (result == 1) {
                    auto [item, rarity] = Get_item(difficulty);
                    pr("You collect ");
                    pr(item.name, color_from_rarity(rarity), "bold");
                    pr(" from the corpse.\n");
                    pr(item.name, color_from_rarity(rarity), "bold");
                    pr(" gives ");
                    pr(item.modified_stat);
                    pr(" +");
                    std::cout << item.stat_modifier << " !\n\n";

                    hero.add_item(item);
                    
                    //add stat to player
                    if (item.modified_stat == "hp") {hero.add_hp(item.stat_modifier); hero.max_hp += item.stat_modifier;}
                    else if (item.modified_stat == "attack") hero.add_attack(item.stat_modifier);
                    else if (item.modified_stat == "defense") hero.add_defense(item.stat_modifier);
                    else {std::cout << "\n\n /// ERROR STATS //// \n\n"; return;} 
                }
                if (result == -1) {
                    std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
                    return;
                }

                hero.add_hp(hero.max_hp - hero.get_remaining_PV());
                pr("You also drink a potion that looks like a regeneration potion.\n");
                pr("You see a door that connects to the basement of the manor.\n");
            }

            pr("You find yourself inside a kind of prison cell with multiple iron doors.\n");
            pr("You try to open them one by one but none, except for one, are unlocked.\n\n");
            
            pr("Feeling near the end, you pass the doorway.\n");
            pr("You're in a gigantic room that is weirdly similar to the arena.\n\n");
            
            pr("> I knew you would come sooner or later...\n", "green");
            pr("> You were not entertaining at all in the arena, some of my friends won't listen to me anymore.\n", "green");
            pr("> I was expecting better...\n", "green");
            pr("> Die you hobo bastard !!!\n\n", "green");

            
            m1 = monsters_to_defeat[0];
            monsters_to_defeat.erase(monsters_to_defeat.begin());
            m2 = monsters_to_defeat[0];
            monsters_to_defeat.erase(monsters_to_defeat.begin());
            Monster m3 = monsters_to_defeat[0];
            monsters_to_defeat.erase(monsters_to_defeat.begin());
            
            
            pr("A portcullis goes up and ");
            pr(m1.name, "red", "italic");
            pr(", ");
            pr(m2.name, "red", "italic");
            pr(", ");
            pr(m3.name, "red", "italic");
            pr(" are running out of the previouly blocked entrance.\n");
            
            result = battle(hero, m1);

            if (result == 0) {end_of_game(difficulty, 0); return;}
            if (result == -1) {
                std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
                return;
            }
            else {
                result = battle(hero, m2);

                if (result == 0) {end_of_game(difficulty, 0); return;}
                if (result == -1) {
                    std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
                    return;
                }
                else{
                    result = battle(hero, m3);

                    if (result == 0) {end_of_game(difficulty, 0); return;}
                    if (result == -1) {
                        std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
                        return;
                    }
                
                }
            }
            
            m1 = monsters_to_defeat[0];
            monsters_to_defeat.erase(monsters_to_defeat.begin());

            pr("> How did you survive ? (Nice fight by the way).\n", "green");
            pr("> Now I, ", "green");
            pr(m1.name, "lighter yellow", "bold blinking");
            pr(", have to kill you myself.\n\n", "green");

            pr("You quickly find on the ground and drink a instant health 2 potion.\n");
            pr(m1.name, "lighter yellow", "bold blinking");
            pr(" spares you no time, you have to fight.\n");
            
            result = battle(hero, m1, true);

            if (result == 0) {end_of_game(difficulty, 0); return;}
            if (result == -1) {
                std::cout << HIGHLIGHTED_RED << "\n\nError while battling\n" << RESET;
                return;
            }

            pr("You've defeated anybody who stood on your path...\n");
            pr("You will very likely die a hobo.\n\n\n");
            
            pr("Why not trying a more unusual difficulty next time ?\n", "cyan");
            

        }
    }
    

    return;   
}

int main(int argc, char const *argv[]) {
    
    std::string answer;
    srand(time(NULL));
    std::vector<std::string> monster_names {"a Rat", "a Slime", "an Ogre", "a Troll", "a Goblin", "an Ork", "a Wizard", "a Bandit", "a Snake"};
    
    std::cout << "Wanna play a game ? (y/n)\n";
    std::cin >> answer;

    if (answer == "y") {
        std::cout << "Choose your difficulty : easy, normal or hard\n";
        std::cin >> answer;

        // initialize monsters w/ difficulty
        std::vector<Monster> monsters_to_defeat = {};

        int difficulty;
        if (answer == "easy") difficulty = 0;
        else if (answer == "normal") difficulty = 1;
        else if (answer == "hard") difficulty = 2;
        else if (answer == "saucisse") difficulty = 69;
        else difficulty = -1;

        switch (difficulty) {
        case 0: {// easy
            for (int i = 0; i < 4; i++) {
                Monster monster_to_add = Monster(monster_names[rand()%monster_names.size()], 1+rand()%10+i*3, 10+i*3, 10+i*3);
                monsters_to_defeat.insert(monsters_to_defeat.end(), monster_to_add);
            }
            Monster boss = Monster("Gen'duel the frightening", 30, 30, 30);
            monsters_to_defeat.insert(monsters_to_defeat.end(), boss);
            break;
        }
        case 1: {// normal
            for (int i = 0; i < 8; i++) {
                Monster monster_to_add = Monster(monster_names[rand()%monster_names.size()], 1+rand()%10+i*3, 10+i*3, 10+i*3);
                monsters_to_defeat.insert(monsters_to_defeat.end(), monster_to_add);
            }
            Monster boss = Monster("Alcathor the great", 40, 40, 40);
            monsters_to_defeat.insert(monsters_to_defeat.end(), boss);
            break;
        }
        case 2: {// hard
            for (int i = 0; i < 12; i++) {
                Monster monster_to_add = Monster(monster_names[rand()%monster_names.size()], 1+rand()%10+i*3, 10+i*3, 10+i*3);
                monsters_to_defeat.insert(monsters_to_defeat.end(), monster_to_add);
            }
            Monster boss = Monster("Icarag the fallen", 60, 60, 60);
            monsters_to_defeat.insert(monsters_to_defeat.end(), boss);
            break;
        }
        case 69: {// impossible
            Monster boss = Monster("Zahogar the almighty", 1000, 99, 100);
            monsters_to_defeat.insert(monsters_to_defeat.end(), boss);
            break;
        }
        default:
            std::cout << "You think you're funny huh ?!\n";
            break;
        }
        
        if (difficulty == -1) return 0;
        introduction();

        pr("As your feet touch the ground, you feel so dizzy that you cannot even remember your own name.\n");

        pr("It's on the tip of your tongue. It's ... ");
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
        std::getline(std::cin, answer);
        Player hero = Player(answer);

        if (difficulty == 69) try_your_luck(monsters_to_defeat, hero, difficulty);
        else {
            adventure(monsters_to_defeat, hero, difficulty);
        }
    }
    else std::cout << "Goodbye then...\n";

    return 0;
}
