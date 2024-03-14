#ifndef USER_H
#define USER_H

#include <string>
// #include <iostream>

typedef enum access {
    PASSENGER = 0,
    ADMIN = 1,
    STAFF = 2,
    CUSTOMS = 3,
    BORDER = 4
} access;

typedef class User {
    private:
        std::string login;
        std::string password;
        std::string file_log;

    public:
        /*
         * getters
         */
        std::string getRole() const;
        std::string getLogin() const;
        std::string getPassword() const;

        /*
         * setters
         */
        void setRole(const std::string);
        void setLogin(const std::string);
        void getPassword(const std::string);

        /*
         * constructor
         */
        // User();
        User(const std::string login, const std::string pass, const std::string role);
        ~User();

        /*
         * Service Functions
         */
        void registration();
} User;


#endif // USER_H
