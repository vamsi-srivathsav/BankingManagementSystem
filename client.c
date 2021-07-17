#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>


void login(int);
void adminMenu(int);
void userMenu(int);

void addAccount(int sd){
  char uname[1024], password[1024], msg[1024];
  int flg = 0;
  printf("\n\nEnter following details:\n");

  while(!flg){
    printf("Username (UNIQUE): ");
    scanf(" %s",uname);
    write(sd, uname, sizeof(uname));
    read(sd, &flg, sizeof(flg));
    if(!flg){
      printf("User name already exists !!!\nPlease try again.\n");
    }
  }
  printf("Password: ");
  scanf(" %s", password);
  write(sd, password, sizeof(password));
  read(sd, msg, sizeof(msg));
  printf("%s\n", msg);

  adminMenu(sd);

}
void deleteAccount(int sd){
  char uname[1024], msg[1020];
  printf("Enter the username of the account to delete: ");
  scanf(" %s", uname);
  write(sd, uname, sizeof(uname));
printf("After write... \n");
  read(sd, msg, sizeof(msg));
  printf("\n%s\n", msg);
  adminMenu(sd);
}
void modifyAccount(int sd){

}
void search(int sd){
  char uname[1024];
  printf("Enter the username to get details: ");
  scanf(" %s", uname);
  write(sd, uname, sizeof(uname));

  long long int acc_no;
  double balance;
  read(sd, &acc_no, sizeof(acc_no));
  read(sd, uname, sizeof(uname));
  read(sd, &balance, sizeof(balance));
  printf("\nAccount Number: %lld\n", acc_no);
  printf("Username: %s\n", uname);
  printf("Balance: %f\n\n",balance);
  adminMenu(sd);

}
void printAccDetails(int sd){
  int flg;
  read(sd, &flg, sizeof(flg));
  char uname[1024], password[1024];
  long long int acc_no;
  while(flg){
    read(sd, uname, sizeof(uname));
    read(sd, password, sizeof(password));
    read(sd, &acc_no, sizeof(acc_no));
    printf("Uname: %s, password: %s, acc_no:%lld \n", uname, password, acc_no);
    read(sd, &flg, sizeof(flg));
  }
  adminMenu(sd);
  return;
}
void adminMenu(int sd){
  int choice;
  printf("Select one of the below options:\n");
  printf("1. Add an account\n");
  printf("2. Delete an account\n");
  printf("3. Modify an account\n");
  printf("4. Search\n5. Print all accounts Details\n");
  printf("Press any other key to exit\n");
  printf("Enter your choice here: ");
  scanf("%d",&choice);
  write(sd, &choice, sizeof(choice));
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
void checkBalace(int sd){
  double bal;
  read(sd, &bal, sizeof(bal));
  printf("Account remaining balance: %f\n\n", bal);
  userMenu(sd);
}
void deposit(int sd){
  double amt;
  printf("Enter amount to deposit: ");
  scanf("%lf", &amt);
  write(sd, &amt,sizeof(amt));
  printf("\nSuccessfully deposited !!!\n");
  userMenu(sd);
}
void withdraw(int sd){
  double amt;
  int flg=1;
  printf("Enter amount to withdraw: ");
  scanf("%lf", &amt);
  write(sd, &amt,sizeof(amt));
  read(sd, &flg, sizeof(flg));
  if(flg) printf("\nSuccessfully deposited !!!\n");
  else printf("\nInsufficient funds !!!\n");
  userMenu(sd);
}
void changePassword(int sd){
  char pwd[1024];
  printf("\nPlease enter the new password: ");
  scanf(" %s",pwd);
  write(sd, pwd, sizeof(pwd));
  printf("\nPassword updated successfully!!!\n");
  userMenu(sd);
}
void viewAccountDetails(int sd){
  long long int acc_no;
  char uname[1024];
  double balance;

  read(sd, &acc_no, sizeof(acc_no));
  read(sd, uname, sizeof(uname));
  read(sd, &balance, sizeof(balance));
  printf("\nAccount Number: %lld\n", acc_no);
  printf("Username: %s\n", uname);
  printf("Balance: %f\n\n",balance);
  userMenu(sd);
}
void userMenu(int sd){
  int choice;
  printf("Select one of the below options:\n");
  printf("1. Check Balance\n");
  printf("2. Deposit\n");
  printf("3. Withdraw amount\n");
  printf("4. Change Password\n5. View Account Details\n");
  printf("Press any other key to exit\n");
  printf("Enter your choice here: ");
  scanf("%d",&choice);
  write(sd, &choice, sizeof(choice));
  switch (choice) {
    case 1:
      checkBalace(sd);
      break;
    case 2:
      deposit(sd);
      break;
    case 3:
      withdraw(sd);
      break;
    case 4:
      changePassword(sd);
      break;
    case 5:
      viewAccountDetails(sd);
      break;
    default:
      login(sd);
      break;
  }
}
void login(int sd){
  char uname[1024], password[1024];
  printf("Kindly Enter your credentials: \n\n");

  printf("Username: ");
  scanf(" %s", uname);
  printf("Password: ");
  scanf(" %s", password);

  write(sd, uname, sizeof(uname));
  write(sd, password, sizeof(password));
  int flg;
  char msg[1024];
  read(sd, &flg, sizeof(flg));
  read(sd, msg, sizeof(msg));
  printf("\n%s\n", msg);
  if(flg==1) adminMenu(sd);
  else if(flg==0) userMenu(sd);
  if(flg==2) login(sd);
}


int main(){

  struct sockaddr_in server;

  int sock_desc;

  sock_desc = socket(AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(22222);
  connect(sock_desc, (void *)(&server), sizeof(server));



	login(sock_desc);
	close(sock_desc);
	return 0;
}
