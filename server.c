#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

void login(int);
void adminMenu(int);
void userMenu(int, long long int);
char* getPassword(char[]);

static long long int current_acc_no = 10000000;
struct customer{
  char userName[1024];
  char password[1024];
  long long int account_number;
  int type;
};

struct account{
  long long int account_number;
  double balance;
};
struct transaction{
  long long int account_number;
  char data[200];
  double amount;
  double bal_remaining;
};
int isDBEmpty(){
  int fd1 = open("customers.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
  struct customer db;
  lseek(fd1,0, SEEK_SET);
  int ret = read(fd1, (char*)&db, sizeof(struct customer));
  close(fd1);
  if(ret) return 0;
  else return 1;
}
int isAccountExists(char uname[]){
  if(isDBEmpty()) return 0;
  struct customer db;
	int fd1 = open("customers.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
	while(read(fd1, (char *)&db, sizeof(struct customer))){
		if(strcmp(db.userName, uname)==0){
				close(fd1);
				return 1;
			}
	}
	close(fd1);
	return 0;
}
// long long int lastAccountNumber(){
//   struct customer db;
// 	int fd1 = open("customers.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
//   long long int laccn;
//   lseek(fd1, -1*(sizeof(struct customer)), SEEK_END);
// 	while(read(fd1, (char *)&db, sizeof(struct customer))){
//     laccn = db.account_number;
// 	}
//   close(fd1);
// 	return laccn;
// }
void addAccount(int sd){
  char uname[1024], password[1024], msg[1024];
  int flg = 0;
  long long int acc_no;      // = (rand()%10000000);
	int fd1 = open("customers.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
  int fd2 = open("accounts.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
  struct customer db;
  struct account acc_db;
  while(!flg){
    read(sd, uname, sizeof(uname));
    if(isDBEmpty()) flg = 1;
    else if(!isAccountExists(uname)) flg = 1;
    write(sd, &flg, sizeof(flg));
  }
  read(sd, password, sizeof(password));



  strcpy(db.userName, uname);
	strcpy(db.password, password);
  db.account_number = (current_acc_no++);
	db.type = 1;

  acc_db.account_number = db.account_number;
  acc_db.balance = 0.0;

  lseek(fd1, 0, SEEK_END);
  lseek(fd2, 0, SEEK_END);
  write(fd1, (char*)&db, sizeof(struct customer));
  write(fd2, (char*)&acc_db, sizeof(struct account));

  write(sd, "Account added successfully !!!\n", sizeof("Account added successfully !!!\n"));
  close(fd1);
  close(fd2);
  adminMenu(sd);
}
void deleteAccount(int sd){
  char uname[1024];
  printf("Deleting account......\n");
  read(sd, uname, sizeof(uname));
  printf("\nEntered uname: %s\n", uname);
  long long int acc_no;
  if(!isAccountExists(uname)){
    write(sd, "Account does not exist !!!\n", sizeof("Account does not exist !!!\n"));
    adminMenu(sd);
    return;
  }
  int fd1 = open("customers.dat", O_CREAT | O_RDWR , 0666);
	int fd2 = open("accounts.dat", O_CREAT | O_RDWR, 0666);
  struct customer db[1000];
  struct account db1[1000];
	int i=0;
	lseek(fd1, 0, SEEK_SET);
	while(read(fd1, (char *)&db[i++], sizeof(struct customer)));
  close(fd1);
  remove("customers.dat");
  fd1 = open("customers.dat", O_CREAT | O_RDWR , 0666);
  lseek(fd1, 0, SEEK_SET);
	for(int j=0;j<i-1;j++){
		if(strcmp(db[j].userName, uname)==0){
      acc_no = db[j].account_number;
      continue;
    }
		else
			write(fd1,(char *)&db[j],sizeof(struct customer));
	}
  close(fd1);

    i=0;
  	lseek(fd2, 0, SEEK_SET);
  	while(read(fd2, (char *)&db1[i++], sizeof(struct account)));
    lseek(fd2, 0, SEEK_SET);
    for(int j=0;j<i-1;j++){
			if(db1[j].account_number==acc_no) continue;
			else
				write(fd2,(char *)&db1[j],sizeof(struct account));
		}

		write(sd,"Account deleted Successfully", sizeof("Account deleted Successfully"));

	close(fd2);
	adminMenu(sd);
}
void modifyAccount(int sd){

}
void search(int sd){
  char uname[1024];
  read(sd, uname, sizeof(uname));
  long long int acc_no;

  struct customer db;
  int fd1 = open("customers.dat", O_CREAT | O_RDWR , 0666);
  lseek(fd1, 0, SEEK_SET);
  while(read(fd1, (char *)&db, sizeof(struct customer))){
    if(strcmp(db.userName,uname)==0){
        acc_no = db.account_number;
        write(sd, &(db.account_number), sizeof(db.account_number));
        write(sd, &(db.userName), sizeof(db.userName));
        break;
      }
  }
  close(fd1);

  struct account db2;
  int fd2 = open("accounts.dat", O_CREAT | O_RDWR , 0666);
  lseek(fd2, 0, SEEK_SET);
  while(read(fd2, (char *)&db2, sizeof(struct account))){
    if(db2.account_number==acc_no){
        write(sd, &(db2.balance), sizeof(db2.balance));
        break;
      }
  }
  close(fd2);
  adminMenu(sd);
}
void printAccDetails(int sd){
  struct customer db;
  int flg = 1;
	int fd1 = open("customers.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
	while(read(fd1, (char *)&db, sizeof(struct customer))){
    write(sd, &flg, sizeof(flg));
		write(sd, &(db.userName), sizeof(db.userName));
    write(sd, &(db.password), sizeof(db.password));
    write(sd, &(db.account_number), sizeof(db.account_number));
    // printf("Uname: %s, password: %s \n", db.userName, getPassword(db.userName));
	}
  flg = 0;
  write(sd, &flg, sizeof(flg));
	close(fd1);
	return ;
}
void adminMenu(int sd){
  int choice;
  // printf("Select one of the below options:\n");
  read(sd, &choice, sizeof(choice));
  switch (choice) {
    case 1:
      addAccount(sd);
      break;
    case 2:
      deleteAccount(sd);
      break;
    case 3:
      modifyAccount(sd);
      break;
    case 4:
      search(sd);
      break;
    case 5:
      printAccDetails(sd);
      break;
    default:
      login(sd);
      break;
  }
}
void checkBalance(int sd, long long int acc_no){
  struct account db;
  int fd1 = open("accounts.dat", O_CREAT | O_RDWR , 0666);

  lseek(fd1, 0, SEEK_SET);
  while(read(fd1, (char *)&db, sizeof(struct account))){
    if(db.account_number==acc_no){
        write(sd, &(db.balance), sizeof(db.balance));
        break;
      }
  }
  close(fd1);
  userMenu(sd, acc_no);
}
void deposit(int sd, long long int acc_no){
  struct account db;
  int fd1 = open("accounts.dat", O_CREAT | O_RDWR , 0666);
  double amt;
  read(sd,&amt, sizeof(amt));

  lseek(fd1, 0, SEEK_SET);
  while(read(fd1, (char *)&db, sizeof(struct account))){

    if(db.account_number==acc_no){
        db.balance += amt;
        lseek(fd1, -1*sizeof(struct account), SEEK_CUR);
        write(fd1, (char *)&db, sizeof(struct account));
        break;
      }
  }
  close(fd1);
  userMenu(sd, acc_no);
}
void withdraw(int sd, long long int acc_no){
  struct account db;
  int fd1 = open("accounts.dat", O_CREAT | O_RDWR , 0666);
  double amt;
  read(sd,&amt, sizeof(amt));
  int flg = 1;
  lseek(fd1, 0, SEEK_SET);
  while(read(fd1, (char *)&db, sizeof(struct account))){

    if(db.account_number==acc_no){
        if(db.balance < amt){
          flg = 0;
          write(sd, &flg, sizeof(flg));
        }else{
          write(sd, &flg, sizeof(flg));
          db.balance -= amt;
          lseek(fd1, -1*sizeof(struct account), SEEK_CUR);
          write(fd1, (char *)&db, sizeof(struct account));
        }
        break;
      }
  }
  close(fd1);
  userMenu(sd, acc_no);
}
void changePassword(int sd, long long int acc_no){
  struct customer db;
  int fd1 = open("customers.dat", O_CREAT | O_RDWR , 0666);
  char pwd[1024];
  read(sd, pwd, sizeof(pwd));

  lseek(fd1, 0, SEEK_SET);
  while(read(fd1, (char *)&db, sizeof(struct customer))){
    if(db.account_number==acc_no){
        strcpy(db.password, pwd);
        lseek(fd1, -1*sizeof(struct customer), SEEK_CUR);
        write(fd1, (char *)&db, sizeof(struct customer));
        break;
      }
  }
  close(fd1);
  userMenu(sd, acc_no);
}
void viewAccountDetails(int sd, long long int acc_no){
  struct customer db;
  int fd1 = open("customers.dat", O_CREAT | O_RDWR , 0666);


  lseek(fd1, 0, SEEK_SET);
  while(read(fd1, (char *)&db, sizeof(struct customer))){
    if(db.account_number==acc_no){
        write(sd, &(db.account_number), sizeof(db.account_number));
        write(sd, &(db.userName), sizeof(db.userName));
        break;
      }
  }
  close(fd1);

  struct account db2;
  int fd2 = open("accounts.dat", O_CREAT | O_RDWR , 0666);

  lseek(fd2, 0, SEEK_SET);
  while(read(fd2, (char *)&db2, sizeof(struct account))){
    if(db2.account_number==acc_no){
        write(sd, &(db2.balance), sizeof(db2.balance));
        break;
      }
  }
  close(fd2);
  userMenu(sd, acc_no);
}
void userMenu(int sd, long long int acc_no){
  int choice;
  read(sd, &choice, sizeof(choice));
  switch (choice) {
    case 1:
      checkBalance(sd, acc_no);
      break;
    case 2:
      deposit(sd, acc_no);
      break;
    case 3:
      withdraw(sd, acc_no);
      break;
    case 4:
      changePassword(sd, acc_no);
      break;
    case 5:
      viewAccountDetails(sd, acc_no);
      break;
    default:
      login(sd);
      break;
  }
}
char* getPassword(char uname[]){
  struct customer db;
  static char newpwd[1024];
	int fd1 = open("customers.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
	while(read(fd1, (char *)&db, sizeof(struct customer))){
		if(strcmp(db.userName, uname)==0){
				close(fd1);
        strcpy(newpwd, db.password);
				return newpwd;
			}
	}
	close(fd1);
	return "";
}
long long int getAccNo(char uname[]){
  struct customer db;
  long long int new_accno;
	int fd1 = open("customers.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
	while(read(fd1, (char *)&db, sizeof(struct customer))){
		if(strcmp(db.userName, uname)==0){
				close(fd1);
        new_accno = db.account_number;
				return new_accno;
			}
	}
	close(fd1);
	return new_accno;
}
void login(int sd){
  char uname[1024], password[1024];
  printf("In login.....\n");
  read(sd, uname, sizeof(uname));
  read(sd, password, sizeof(password));
  int flg = 1;
  if(strcmp(uname, "admin")==0 && strcmp(password,"password")==0){
    write(sd, &flg, sizeof(flg));
    write(sd, "Welcome Admin!!!\n", sizeof("Welcome Admin!!!\n"));
    adminMenu(sd);
  }else if(isAccountExists(uname) && !strcmp(getPassword(uname),password)){
    flg = 0;
    write(sd, &flg, sizeof(flg));
    write(sd, "Login successful !!!\n", sizeof("Login successful !!!\n"));
    long long int acc_no=getAccNo(uname);
    userMenu(sd, acc_no);
  }else{
    flg = 2;
    write(sd, &flg, sizeof(flg));
    write(sd, "Invalid credentials !!!\nPlease try again!!!\n\n", sizeof("Invalid credentials !!!\nPlease try again!!!\n\n"));
    login(sd);
  }
  // printf("Received uname: %s\n", uname);
  // printf("Received password: %s\n", password);


}

// void mainMenu(int choice){
//   switch (choice) {
//     case 1: SignIn();
//           break;
//     case 2: login();
//           break;
//     default:
//         break;
//   }
// }

int main(){
  struct sockaddr_in server, client;

  int socket_desc, size_client, new_sd;
  char buff[1024];

  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(22222);

  bind(socket_desc, (void *)(&server), sizeof(server));
  listen(socket_desc, 1);

  while(1){
    size_client = sizeof(client);
    new_sd = accept(socket_desc, (void *)(&client), &size_client);
    if(!fork()){

      login(new_sd);


    }else{
      close(new_sd);
    }

  }

  close(socket_desc);
}
