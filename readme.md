
# Online Banking Management System

### Project goal :
To develop a Banking management system that is user friendly and multi-functional. 


### How to run the compile and run project ?

__Server Side:__

```bash
cc server.c -o server
./server
```

__Client Side:__

```bash
cc client.c -o client
./client
```
### Functionalities

1. All account details and customer information is stored in files. 
2. Login system for account holders and password protected administrative access from Admin. Thus, preventing the system from unauthorised access. 
3. Three types of login :
    - Normal user
    - Joint account user
    - Administrator (Admin)
4. On successful login of customer using UserName and Password, a menu is displayed as :
    - Deposit
    - Withdraw amount
    - Check Balance
    - Change Password
    - View Account Details
    - Exit
5. On successful login of admin, the following menu is displayed:
    - Add an account
    - Delete an account
    - Modify an account
    - Search for a specific account
    - Print all accounts Details
    - Exit
6. In case of joint account login, __file locking__ is used for reading/writing account details when required.
7. Server maintains and connects to the database(files) and it reacts to the clients requests concurrently. This connection of client and server is done using __socket programming__. 


