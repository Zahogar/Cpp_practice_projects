#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <pair>
#include <cassert>

class Book{
public:

    std::string Title;
    std::string Author;
    int Release_year;
    std::string Publisher;
    std::string Language;
    int Number_of_pages;

    Book() = default;

    Book(std::string title, std::string author, int release_year, std::string publisher, std::string language, int number_of_pages)
        : Title(title), Author(author), Release_year(release_year), Publisher(publisher), Language(language), Number_of_pages(number_of_pages) {}

    bool operator==(const Book& other) const {
        return Title == other.Title &&
               Author == other.Author &&
               Release_year == other.Release_year &&
               Publisher == other.Publisher &&
               Language == other.Language &&
               Number_of_pages == other.Number_of_pages;
    }

    bool operator<(const Book& other) const {
        return Title < other.Title;
    }

};

class Library {
public:
    std::string Name;

    Library(std::string name) : Name(name) {
        _bookshelves = std::vector<Book>();
    }

    int getNumberOfBooks() {
        return _bookshelves.size();
    }

    void addBook(Book book) {
        _bookshelves.push_back(book);
    }

    void editBook(std::string title, std::string author) {

        int idx = _find_book_from_auth_x_title(title, author);
        
        assert(it != _bookshelves.end());

        int idx_bookshelves = std::distance(_bookshelves.begin(), it);
        
        std::cout << "What do you want to change ?\n";
        std::cout << "1 : Title\n2 : Author\n3 : Year of release\n4 : Publisher\n5 : Language\n6 : Number of pages\n7 : Nothing\n"; 
        
        int int_answer;
        std::string str_answer;
        std::cin >> int_answer;

        switch (int_answer) {
        case 1:
            std::cout << "New title: ";
            std::cin >> str_answer;
            std::cout << std::endl;
            book.Title = str_answer;
            break;
        case 2:
            std::cout << "New author: ";
            std::cin >> str_answer;
            std::cout << std::endl;
            book.Author = str_answer;
            break;
        case 3:
            std::cout << "New release year: ";
            std::cin >> int_answer;
            std::cout << std::endl;
            book.Release_year = int_answer;
            break;
        case 4:
            std::cout << "New publisher: ";
            std::cin >> str_answer;
            std::cout << std::endl;
            book.Publisher = str_answer;
            break;
        case 5:
            std::cout << "New language: ";
            std::cin >> str_answer;
            std::cout << std::endl;
            book.Language = str_answer;
            break;
        case 6:
            std::cout << "New number of pages: ";
            std::cin >> int_answer;
            std::cout << std::endl;
            book.Number_of_pages = int_answer;
            break;
        case 7:
            std::cout << "Exited the book editor.\n";
            return;
        default:
            std::cout << "Couldn't match " << int_answer << " with any input.\n";
            return;
        }

        _bookshelves[idx_bookshelves] = book;
        
        /*from_byTitle[idx_from_byTitle] = book;
        _byTitle[title] = from_byTitle;

        from_byAuthor[idx_from_byAuthor] = book;
        _byAuthor[author] = from_byTitle;
        */

        return;

    }

    void deleteBook(std::string title, std::string author) {

    }

private:
    std::vector<Book> _bookshelves;
    std::unordered_map<std::string, std::vector<Book>> _byTitle;
    std::unordered_map<std::string, std::vector<Book>> _byAuthor;

    int _find_book_from_auth_x_title(std::string title, std::string author) {
        
        for (int i = 0; i < _bookshelves.size(); i++) {

            std::vector<Book> potential_results;
            if (_bookshelves[i].Title == title && _bookshelves[i].Author == author) potential_results.push_back(_bookshelves[i]);
            if (potential_results.size())
        }
    }
};



int main() {
    std::vector<std::string> book_titles = {
        "The Eye of the World", 
        "The Great Hunt", 
        "The Dragon Reborn",
        "The Shadow Rising",
        "The Fires of Heaven",
        "Lord of Chaos",
        "A Crown of Swords",
        "The Path of Daggers",
        "Winter's Heart",
        "Crossroads of Twilight",
        "New Spring",
        "Knife of Dreams",
        "The Gathering Storm",
        "Towers of Midnight",
        "A Memory of Light"
    };
    std::string author = "Robert Jordan";
    std::string publisher = "Bragelonne";
    std::string language = "English";
    std::vector<int> release_years = {
        1990, 1990, 1991, 1992, 1993, 1994, 1996, 1998,
        2000, 2003, 2004, 2005, 2009, 2010, 2013
    };
    std::vector<int> number_of_pages = {
        814, 624, 624, 704, 832, 784, 672, 784,
        672, 672, 672, 784, 784, 784, 832, 832
    };

    Library my_library = Library("My Library");
    for (int i = 0; i < book_titles.size(); i++) {
        my_library.addBook(Book(book_titles[i], author, release_years[i], publisher, language, number_of_pages[i]));
    }

    std::cout << my_library.getNumberOfBooks() <<" books were added to the library." << std::endl;
    std::cout << "Together, they account for " << std::accumulate(number_of_pages.begin(), number_of_pages.end(), 0) << " pages." << std::endl;
    std::cout << "And were written by " << author << ", in the span of " << (release_years.back() - release_years.front()) << " years." << std::endl;
    std::cout << "\n";

    std::string title_to_edit = "The Eye of the World";
    my_library.editBook(title_to_edit, author);
    std::cout << "After editing, the library now contains " << my_library.getNumberOfBooks() << " books." << std::endl;
    //my_library.deleteBook(title_to_edit, author);
    std::cout << "After deleting 'The Eye of the World', the library now contains " << my_library.getNumberOfBooks() << " books." << std::endl;
    return 0;
}
