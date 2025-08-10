#include <iostream>
#include <string>


class Bank_account {

private:
    int _money;

public:
    Bank_account() {
        _money = 0;
        std::cout << "Account initialized !\n";
    }
    
    void deposit(int amount) {
        _money += amount;
        std::cout << "Added " << amount << " to your account\n";
    }

    void withdraw(int amount) {
        _money -= amount;
        std::cout << "Withdrawn " << amount << " from your account\n";
        if (amount > _money) std::cout << "Be careful, your balance is negative\n"; 
    }

    void check_balance() {
        std::cout << "You have " << _money << " on your account\n";
    }

};



int main(int argc, char const *argv[]) {

    char do_something = 1;
    char answer;
    std::string str_answer;
    int amount;

    std::cout << "Would you want to create an account ? (y/n)\n";
    std::cin >> answer;

    if (answer == 'y') {

        Bank_account my_account;
     
        while (do_something) {
            
            std::cout << "Do want to do something ? (y/n)\n";
            std::cin >> answer;
            if (answer == 'y') {
                do_something = 1;
                std::cout << "What do you want to do ? Deposit, withdraw or balance ?\n";
                std::cin >> str_answer;

                if (str_answer == "deposit") {
                    std::cout << "How much ?\n";
                    std::cin >> amount;

                    my_account.deposit(amount);
                }
                if (str_answer == "withdraw") {
                    std::cout << "How much ?\n";
                    std::cin >> amount;

                    my_account.withdraw(amount);
                }
                if (str_answer == "balance") {
                    my_account.check_balance();
                }          
            } else break;
        }
    }
    std::cout << "Well goodbye then\n";

    return 0;
}
